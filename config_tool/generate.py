# -*- coding: utf-8 -*-
import openpyxl
from jinja2 import Environment, FileSystemLoader
import os
import re
import json
import shutil
from pathlib import Path

# --- 1. 类型解析模块 ---
# 使用正则表达式进行强大且灵活的类型解析

class TypeParser:
    """负责解析Excel中的类型字符串和单元格数据"""

    @staticmethod
    def _parse_single_value(value_str, type_str):
        if not isinstance(value_str, str): value_str = str(value_str)
        if type_str in ['int', 'int32', 'int64']: return int(value_str)
        if type_str == 'float': return float(value_str)
        return value_str

    @classmethod
    def parse_value(cls, value, type_str):
        if value is None:
            if 'dict' in type_str: return {}
            if 'array' in type_str or 'repeated' in type_str or type_str == 'json': return [] if type_str != 'json' else None
            return None
        
        value = str(value).strip()
        if not value:
            if 'dict' in type_str: return {}
            if 'array' in type_str or 'repeated' in type_str: return []
            if type_str == 'json': return None

        # 新增：JSON类型
        if type_str == 'json':
            try:
                return json.loads(value)
            except json.JSONDecodeError:
                print(f"⚠️ Warning: Invalid JSON string found in a 'json' type cell: {value[:100]}...")
                return None

        # 字典: dict<key_type,val_type>
        dict_match = re.search(r'dict<(\w+),(\w+)>', type_str)
        if dict_match:
            key_type, val_type = dict_match.groups()
            return {
                cls._parse_single_value(k.strip(), key_type): cls._parse_single_value(v.strip(), val_type)
                for k, v in (pair.split(':', 1) for pair in value.split(';') if ':' in pair)
            }

        # 结构体数组: array<pair<type1,type2>>
        pair_match = re.search(r'array<pair<(\w+),(\w+)>>', type_str)
        if pair_match:
            type1, type2 = pair_match.groups()
            return [
                [cls._parse_single_value(v1.strip(), type1), cls._parse_single_value(v2.strip(), type2)]
                for v1, v2 in (item.split(':', 1) for item in value.split(';') if ':' in item)
            ]

        # 简单类型数组: repeated type
        repeated_match = re.search(r'(?:repeated|array)\s+(\w+)', type_str)
        if repeated_match:
            item_type = repeated_match.group(1)
            return [cls._parse_single_value(item.strip(), item_type) for item in value.split(';') if item.strip()]

        return cls._parse_single_value(value, type_str)

# --- 2. Jinja2 过滤器模块 ---
# 将所有模板相关的转换逻辑集中于此

class JinjaFilters:
    @staticmethod
    def to_py_value(value):
        # 使用indent=4进行格式化，并处理布尔值和None
        json_string = json.dumps(value, ensure_ascii=False, indent=4)
        return json_string.replace('null', 'None').replace('true', 'True').replace('false', 'False')
    
    @staticmethod
    def to_py_variable_name(field_name):
        """转换为Python小写无下划线变量名"""
        # 移除下划线，转为小写
        return field_name.replace('_', '').lower()
    
    @staticmethod
    def to_cpp_variable_name(field_name):
        """转换为C++匈牙利命名法变量名"""
        # 移除下划线，分割单词
        words = field_name.split('_')
        if len(words) == 1:
            words = [field_name]
        
        # 首个单词小写，后续单词首字母大写
        camel_case = words[0].lower() + ''.join(word.capitalize() for word in words[1:])
        return camel_case
    
    @staticmethod
    def to_cpp_variable_name_with_type(field_name, field_type):
        """转换为C++匈牙利命名法变量名，带类型前缀"""
        # 获取类型前缀
        type_prefix = ""
        if field_type == "int":
            type_prefix = "n"
        elif field_type == "float":
            type_prefix = "f"
        elif field_type == "string":
            type_prefix = "s"
        elif "vector" in field_type or "array" in field_type or "repeated" in field_type:
            type_prefix = "v"
        elif "map" in field_type or "dict" in field_type:
            type_prefix = "m"
        elif field_type == "json":
            type_prefix = "s"  # JSON作为字符串存储
        else:
            # 默认情况，尝试从字段名推断
            type_prefix = ""
        
        # 移除下划线，分割单词
        words = field_name.split('_')
        if len(words) == 1:
            words = [field_name]
        
        # 首个单词首字母大写，后续单词首字母大写
        pascal_case = ''.join(word.capitalize() for word in words)
        
        return type_prefix + pascal_case

    @classmethod
    def to_cpp_type(cls, type_str):
        if type_str == 'json':
            return 'std::string'

        dict_match = re.search(r'dict<(\w+),(\w+)>', type_str)
        if dict_match:
            k, v = (cls.to_cpp_type(t) for t in dict_match.groups())
            return f'std::unordered_map<{k}, {v}>'

        pair_match = re.search(r'array<pair<(\w+),(\w+)>>', type_str)
        if pair_match:
            t1, t2 = (cls.to_cpp_type(t) for t in pair_match.groups())
            return f'std::vector<std::pair<{t1}, {t2}> >'

        repeated_match = re.search(r'(?:repeated|array)\s+(\w+)', type_str)
        if repeated_match:
            item_type = cls.to_cpp_type(repeated_match.group(1))
            return f'std::vector<{item_type}>'

        if type_str in ['int', 'int32', 'int64']: return 'int'
        if type_str == 'float': return 'float'
        if type_str == 'string': return 'std::string'
        return type_str

    @classmethod
    def to_cpp_value(cls, value, type_str):
        if type_str == 'json':
            if value is None:
                return '""'
            json_str = json.dumps(value, ensure_ascii=False)
            # 转义引号以避免 C++ 字符串问题
            escaped_json = json_str.replace('\\', '\\\\').replace('"', '\\"')
            return f'"{escaped_json}"'

        if value is None: return '{}'

        dict_match = re.search(r'dict<(\w+),(\w+)>', type_str)
        if dict_match:
            if not isinstance(value, dict): return '{}'
            k_type, v_type = dict_match.groups()
            pairs = [f'{{{cls.to_cpp_value(k, k_type)}, {cls.to_cpp_value(v, v_type)}}}' for k, v in value.items()]
            return f'{{{", ".join(pairs)}}}'

        pair_match = re.search(r'array<pair<(\w+),(\w+)>>', type_str)
        if pair_match:
            if not isinstance(value, list): return '{}'
            t1, t2 = pair_match.groups()
            pairs = [f'{{{cls.to_cpp_value(p[0], t1)}, {cls.to_cpp_value(p[1], t2)}}}' for p in value]
            return f'{{{", ".join(pairs)}}}'

        repeated_match = re.search(r'(?:repeated|array)\s+(\w+)', type_str)
        if repeated_match:
            if not isinstance(value, list): return '{}'
            item_type = repeated_match.group(1)
            items = [cls.to_cpp_value(item, item_type) for item in value]
            return f'{{{", ".join(items)}}}'

        if type_str == 'string': 
            # 使用普通字符串字面量而不是 u8 前缀，避免编译器兼容性问题
            escaped_value = str(value).replace('\\', '\\\\').replace('"', '\\"')
            return f'"{escaped_value}"'
        return str(value)

# --- 3. 核心逻辑 ---

def process_excel(file_path):
    """读取并解析单个Excel文件，返回结构化数据"""
    workbook = openpyxl.load_workbook(file_path)
    sheet = workbook.active
    rows = list(sheet.iter_rows(values_only=True))

    header, types, comments = rows[0], rows[1], rows[2]
    
    # 核心修改：基于第一行（列名）是否以#开头来过滤
    valid_fields = []
    header_map = {} # 用于快速查找原始索引
    for i, h in enumerate(header):
        if h and not str(h).strip().startswith('#'):
            valid_fields.append({'name': h, 'type': types[i]})
            header_map[h] = i
            
    data_list = []
    for row_idx, row in enumerate(rows[3:], 3):
        row_data = {}
        for field in valid_fields:
            field_name = field['name']
            original_index = header_map[field_name]
            raw_value = row[original_index]
            try:
                row_data[field_name] = TypeParser.parse_value(raw_value, field['type'])
            except (ValueError, TypeError) as e:
                print(f"⚠️ Error parsing row {row_idx+1}, column '{field_name}' in {Path(file_path).name}: {e}")
                row_data[field_name] = None
        data_list.append(row_data)
        
    return valid_fields, data_list

def generate_and_distribute(config_name, targets, tool_root, repo_root):
    """完整的生成与分发流程"""
    # 优先查找驼峰命名的文件，如果不存在则回退到下划线命名
    camel_case_file = tool_root / 'configs' / f"{config_name}.xlsx"
    snake_case_name = ''.join(['_' + c.lower() if c.isupper() and i > 0 else c.lower() for i, c in enumerate(config_name)])
    snake_case_file = tool_root / 'configs' / f"{snake_case_name}.xlsx"
    
    if camel_case_file.exists():
        config_file = camel_case_file
        print(f"--- Processing: {config_name} (using {config_name}.xlsx) ---")
    elif snake_case_file.exists():
        config_file = snake_case_file
        print(f"--- Processing: {config_name} (using {snake_case_name}.xlsx) ---")
    else:
        print(f"⚠️ Warning: Config file not found for '{config_name}' (tried {config_name}.xlsx and {snake_case_name}.xlsx), skipping.")
        return
    
    # 1. 解析Excel
    fields, data = process_excel(config_file)
    if not data:
        print(f"  -> No data found. Skipping code generation.")
        return

    # 2. 准备模板
    py_env = Environment(loader=FileSystemLoader(tool_root / 'templates' / 'py'), trim_blocks=False, lstrip_blocks=False)
    py_env.filters['to_py_value'] = JinjaFilters.to_py_value
    py_env.filters['to_py_variable_name'] = JinjaFilters.to_py_variable_name
    
    cpp_env = Environment(
        loader=FileSystemLoader(tool_root / 'templates' / 'cpp'),
        trim_blocks=False,  # 关闭所有自动空白控制
        lstrip_blocks=False # 以实现完全的手动格式化
    )
    cpp_env.filters['to_cpp_type'] = JinjaFilters.to_cpp_type
    cpp_env.filters['to_cpp_value'] = JinjaFilters.to_cpp_value
    cpp_env.filters['to_cpp_variable_name'] = JinjaFilters.to_cpp_variable_name
    cpp_env.filters['to_cpp_variable_name_with_type'] = JinjaFilters.to_cpp_variable_name_with_type
    
    # config_name 已经是驼峰命名了
    class_name = config_name
    # 生成文件名使用驼峰命名
    py_file_name = config_name
    context = {
        'class_name': class_name,
        'config_name': py_file_name,
        'key_field': fields[0]['name'],
        'fields': fields,
        'data': data
    }

    # 3. 生成代码
    output_py_dir = tool_root / 'output' / 'py'
    output_cpp_dir = tool_root / 'output' / 'cpp'

    py_content = py_env.get_template("generic.py.j2").render(context)
    py_path = output_py_dir / f"{py_file_name}.py"
    py_path.write_text(py_content, encoding='utf-8')
    print(f"  -> Generated Python: {Path.relative_to(py_path, tool_root)}")

    h_content = cpp_env.get_template("generic.h.j2").render(context)
    h_path = output_cpp_dir / f"{class_name}.h"
    h_path.write_text(h_content, encoding='utf-8')
    print(f"  -> Generated C++ Header: {Path.relative_to(h_path, tool_root)}")

    cpp_content = cpp_env.get_template("generic.cpp.j2").render(context)
    cpp_path = output_cpp_dir / f"{class_name}.cpp"
    cpp_path.write_text(cpp_content, encoding='utf-8')
    print(f"  -> Generated C++ Source: {Path.relative_to(cpp_path, tool_root)}")

    # 4. 分发
    for target in targets.get('py_targets', []):
        dest = repo_root / target
        dest.mkdir(parents=True, exist_ok=True)
        shutil.copy(py_path, dest)
        print(f"  => Copied Python to: {target}")
    
    for target in targets.get('cpp_targets', []):
        dest = repo_root / target
        dest.mkdir(parents=True, exist_ok=True)
        shutil.copy(h_path, dest)
        shutil.copy(cpp_path, dest)
        print(f"  => Copied C++ to: {target}")

def main():
    """主函数"""
    tool_root = Path(__file__).parent.resolve()
    repo_root = tool_root.parent
    
    output_dir = tool_root / 'output'
    if output_dir.exists():
        shutil.rmtree(output_dir)
    (output_dir / 'py').mkdir(parents=True)
    (output_dir / 'cpp').mkdir(parents=True)
    
    map_file = tool_root / 'config_map.json'
    with open(map_file, 'r', encoding='utf-8') as f:
        config_map = json.load(f)
        
    print("🚀 Starting config generation and distribution...")
    for name, targets in config_map.items():
        generate_and_distribute(name, targets, tool_root, repo_root)
    print("\n✅ All configs generated and distributed successfully!")

if __name__ == "__main__":
    main() ## Modified for auto-commit test
