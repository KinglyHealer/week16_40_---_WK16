# 学生成绩统计系统 V2.0 - 开发与运行指南

## 一、Python 数据分析环境配置

本项目的 Python 数据分析部分（`notebooks/analysis.ipynb`）建议使用虚拟环境进行隔离开发，避免污染全局依赖。

### 1. 创建并激活虚拟环境
```bash
# 进入项目根目录，创建名为 venv 的虚拟环境
python3 -m venv venv

# 激活虚拟环境（Linux / macOS）
source venv/bin/activate

# Windows 用户请使用以下命令激活：
# .\venv\Scripts\activate
```

### 2. 安装依赖库
在虚拟环境中安装数据处理与可视化所需的第三方库：
```bash
pip install pandas matplotlib jupyter
```

### 3. 启动 Jupyter Notebook
```bash
jupyter notebook notebooks/analysis.ipynb
```

---

## 二、项目工程化操作命令

### 1. 项目目录创建
一键生成标准化的 C 语言工程目录结构：
```bash
mkdir -p include src output notebooks notes
```

### 2. GCC 编译命令
开启严格警告模式进行编译，排查代码潜在隐患：
```bash
gcc -Wall -Wextra src/main.c src/student.c -I./include -o app.out
```

> **参数说明**：
> - `-Wall -Wextra`：开启全面警告提示，提升代码健壮性。
> - `-I./include`：指定头文件搜索路径为 `include` 目录。
> - `-o app.out`：指定生成的可执行文件名称为 `app.out`。

---

## 三、Git 分步提交参考

以下为项目规范化版本管理的 Git 提交流程示例，建议遵循 Conventional Commits 规范：

```bash
# 1. 初始化仓库并提交忽略配置
git init
git add .gitignore && git commit -m "init: 初始化项目目录与忽略文件"

# 2. 提交核心重构代码
git add include/ src/ && git commit -m "feat: v2.0分文件模块化重构代码"

# 3. 提交自动化脚本
git add run.sh && git commit -m "feat: 新增一键运行shell脚本"

# 4. 提交项目文档
git add notes/ && git commit -m "docs: 补充linux&git操作笔记"