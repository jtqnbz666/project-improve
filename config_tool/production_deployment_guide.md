# 配置生成系统 - 生产环境部署指南

### 🎯 部署前检查清单

### 1. 依赖环境
- [ ] Python 3.7+ (推荐3.9+)
- [ ] pip包管理器
- [ ] GitLab Runner (已配置)
- [ ] C++编译环境 (clang++/g++ with C++11)

### 2. 权限配置
- [ ] GitLab项目的Developer/Maintainer权限
- [ ] CI/CD Pipeline执行权限
- [ ] 目标目录的写入权限

### 3. 文件结构验证
```
项目根目录/
├── config_tool/
│   ├── configs/           # Excel配置文件
│   ├── templates/         # Jinja2模板
│   ├── generate.py        # 核心生成器
│   ├── config_map.json    # 配置映射
│   └── requirements.txt   # Python依赖
├── webservice/
│   └── GeneratedConfigs/  # Python配置输出
└── service/sjgame/sjgame/
    └── configs/           # C++配置输出
```

## 🔧 生产环境优化建议

### 1. 性能优化
- 使用缓存机制，避免重复生成相同配置
- 大型Excel文件考虑分批处理
- 配置文件版本控制和增量更新

### 2. 安全措施
- Excel文件访问权限控制
- 生成的代码进行语法检查
- 敏感配置数据加密存储

### 3. 监控告警
- CI/CD Pipeline失败告警
- 配置文件格式错误监控
- 生成文件大小异常检测

### 4. 备份策略
- 定期备份Excel配置文件
- 生成的代码文件版本记录
- 配置变更历史追踪

## 📋 部署步骤

### Step 1: 环境初始化
```bash
# 安装Python依赖
cd config_tool
pip3 install -r requirements.txt

# 验证生成器功能
python3 generate.py
```

### Step 2: GitLab CI/CD配置
- 确认`.gitlab-ci.yml`配置正确
- 测试Pipeline运行
- 配置自动触发条件

### Step 3: 团队培训
- 策划人员：Excel配置规范
- 开发人员：生成代码使用方法
- 运维人员：Pipeline监控和故障处理

### Step 4: 渐进式上线
1. 测试环境验证
2. 预发布环境测试
3. 生产环境小规模试点
4. 全量上线

## ⚠️ 注意事项

### 1. 配置变更流程
- 所有配置变更必须经过Code Review
- 重要配置变更需要团队确认
- 生产环境部署需要手动审批

### 2. 回滚预案
- 保留上一版本的配置文件
- 快速回滚机制
- 紧急情况处理流程

### 3. 兼容性考虑
- 新增字段保持向后兼容
- 删除字段提前通知相关团队
- 数据类型变更需要迁移脚本 
