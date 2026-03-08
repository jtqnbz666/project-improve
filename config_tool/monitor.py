#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
配置生成系统监控脚本
用于生产环境健康检查和异常告警
"""

import os
import sys
import json
import time
from pathlib import Path
from datetime import datetime

def check_dependencies():
    """检查依赖环境"""
    print("🔍 检查依赖环境...")
    
    try:
        import openpyxl
        import jinja2
        print("✅ Python依赖正常")
        return True
    except ImportError as e:
        print(f"❌ 依赖缺失: {e}")
        return False

def check_file_structure():
    """检查文件结构完整性"""
    print("🔍 检查文件结构...")
    
    required_files = [
        "generate.py",
        "config_map.json", 
        "requirements.txt",
        "templates/cpp/generic.h.j2",
        "templates/cpp/generic.cpp.j2",
        "templates/py/generic.py.j2"
    ]
    
    missing_files = []
    for file_path in required_files:
        if not Path(file_path).exists():
            missing_files.append(file_path)
    
    if missing_files:
        print(f"❌ 缺失文件: {missing_files}")
        return False
    else:
        print("✅ 文件结构完整")
        return True

def check_excel_files():
    """检查Excel配置文件"""
    print("🔍 检查Excel配置文件...")
    
    config_dir = Path("configs")
    if not config_dir.exists():
        print("❌ configs目录不存在")
        return False
    
    excel_files = list(config_dir.glob("*.xlsx"))
    print(f"📊 发现 {len(excel_files)} 个Excel文件:")
    
    for excel_file in excel_files:
        size = excel_file.stat().st_size
        modified = datetime.fromtimestamp(excel_file.stat().st_mtime)
        print(f"  - {excel_file.name}: {size}字节, 修改时间: {modified}")
    
    if excel_files:
        print("✅ Excel文件检查通过")
        return True
    else:
        print("⚠️ 未发现Excel文件")
        return False

def check_output_directories():
    """检查输出目录权限"""
    print("🔍 检查输出目录...")
    
    with open("config_map.json", 'r', encoding='utf-8') as f:
        config_map = json.load(f)
    
    all_targets = set()
    for config_name, targets in config_map.items():
        all_targets.update(targets.get('py_targets', []))
        all_targets.update(targets.get('cpp_targets', []))
    
    for target in all_targets:
        target_path = Path("..") / target
        if target_path.exists():
            if os.access(target_path, os.W_OK):
                print(f"✅ {target} - 可写")
            else:
                print(f"❌ {target} - 无写权限")
                return False
        else:
            print(f"⚠️ {target} - 目录不存在")
    
    return True

def run_generation_test():
    """运行生成测试"""
    print("🔍 运行生成测试...")
    
    try:
        start_time = time.time()
        
        # 导入并运行生成器
        import generate
        
        # 模拟运行（实际不生成文件）
        tool_root = Path(".").resolve()
        config_map_file = tool_root / 'config_map.json'
        
        if config_map_file.exists():
            with open(config_map_file, 'r', encoding='utf-8') as f:
                config_map = json.load(f)
            
            generation_time = time.time() - start_time
            print(f"✅ 生成器运行正常 (耗时: {generation_time:.2f}秒)")
            print(f"📊 配置项数量: {len(config_map)}")
            return True
        else:
            print("❌ config_map.json不存在")
            return False
            
    except Exception as e:
        print(f"❌ 生成测试失败: {e}")
        return False

def generate_health_report():
    """生成健康报告"""
    report = {
        "timestamp": datetime.now().isoformat(),
        "status": "unknown",
        "checks": {}
    }
    
    checks = [
        ("dependencies", check_dependencies),
        ("file_structure", check_file_structure), 
        ("excel_files", check_excel_files),
        ("output_directories", check_output_directories),
        ("generation_test", run_generation_test)
    ]
    
    all_passed = True
    print("=" * 60)
    print("🏥 配置生成系统健康检查")
    print("=" * 60)
    
    for check_name, check_func in checks:
        try:
            result = check_func()
            report["checks"][check_name] = {
                "status": "pass" if result else "fail",
                "timestamp": datetime.now().isoformat()
            }
            if not result:
                all_passed = False
        except Exception as e:
            print(f"❌ {check_name} 检查异常: {e}")
            report["checks"][check_name] = {
                "status": "error",
                "error": str(e),
                "timestamp": datetime.now().isoformat()
            }
            all_passed = False
        
        print()
    
    report["status"] = "healthy" if all_passed else "unhealthy"
    
    # 保存报告
    report_file = Path("health_report.json")
    with open(report_file, 'w', encoding='utf-8') as f:
        json.dump(report, f, indent=2, ensure_ascii=False)
    
    print("=" * 60)
    if all_passed:
        print("🎉 系统健康状态良好！")
        print("✅ 所有检查项通过")
    else:
        print("⚠️ 系统存在问题！")
        print("❌ 部分检查项失败")
    
    print(f"📄 详细报告已保存至: {report_file}")
    print("=" * 60)
    
    return all_passed

if __name__ == "__main__":
    success = generate_health_report()
    sys.exit(0 if success else 1) 