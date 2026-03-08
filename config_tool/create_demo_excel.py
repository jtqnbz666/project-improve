import openpyxl
from pathlib import Path
import json

def create_ultimate_demo_excel():
    """创建一个包含所有支持的数据类型的终极演示Excel文件。"""
    
    file_path = Path(__file__).parent / 'configs' / 'demo_config.xlsx'
    workbook = openpyxl.Workbook()
    sheet = workbook.active
    sheet.title = "UltimateDemo"

    json_example = {
        "level": 10,
        "vip_only": True,
        "unlock_condition": "FINISH_QUEST_101"
    }

    headers = {
        'id':                 ('int', '主键ID'),
        'name':               ('string', '配置名称'),
        'value_float':        ('float', '浮点数值'),
        'keywords':           ('repeated string', '关键字列表'),
        'str_to_int_map':     ('dict<string,int>', 'string到int的字典'),
        'str_pair_array':     ('array<pair<string,int>>', 'string-int的键值对数组'),
        'complex_logic':      ('json', '复杂的JSON逻辑'),
        '#notes':             ('string', '这是一个注释列，它的列名以#开头') 
    }
    header_names = list(headers.keys())
    
    sheet.append(header_names) 
    sheet.append([headers[h][0] for h in header_names])
    sheet.append([headers[h][1] for h in header_names])

    data_rows = [
        [
            1, '新手礼包', 9.9, 'newbie;gift',
            'gold:100;diamond:10', 'exp:500;stamina:10',
            json.dumps(json_example), '这是给新手的备注，不会被解析'
        ],
        [
            2, '大师宝箱', 198.0, 'master;rare',
            'wood:999;iron:50', 'boss_ticket:1;gem:5',
            '', '这是给老玩家的备注，不会被解析'
        ]
    ]
    for row_data in data_rows:
        sheet.append(row_data)

    workbook.save(file_path)
    print(f"✅ Ultimate demo config file created at: {file_path}")

if __name__ == "__main__":
    create_ultimate_demo_excel() 