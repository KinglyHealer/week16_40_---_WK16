# Linux命令与Git实操笔记 V3.0

> **版本**: V3.0 | **日期**: 2026-06-26
>
> 本文档记录学生成绩统计系统 V3.0 开发过程中实际使用的 Linux 与 Git 命令。

---

## 一、目录操作命令

### 1. pwd — 查看当前工作目录

```bash
# 确认自己所在的目录位置
$ pwd
/c/Users/KinglyHealer/Desktop/week16_40_郑礼铭_WK16
```

> `pwd` = **P**rint **W**orking **D**irectory，输出当前所在目录的绝对路径。

### 2. ls — 列出目录内容

```bash
# 列出当前目录下的文件和子目录
$ ls
README.md   include/    notebooks/  src/
git_log.txt notes/      output/     tools/

# 列出所有文件（含隐藏的 .gitignore 等）
$ ls -a
.git/        .gitignore   include/     notebooks/   src/
../          README.md    notes/       output/      tools/

# 以详细列表形式显示（权限、大小、修改时间）
$ ls -l
-rw-r--r-- 1 KinglyHealer 197609  9030 Jun 26 README.md
drwxr-xr-x 1 KinglyHealer 197609     0 Jun 26 include/
drwxr-xr-x 1 KinglyHealer 197609     0 Jun 26 notes/
drwxr-xr-x 1 KinglyHealer 197609     0 Jun 26 notebooks/
drwxr-xr-x 1 KinglyHealer 197609     0 Jun 26 output/
drwxr-xr-x 1 KinglyHealer 197609     0 Jun 26 src/
drwxr-xr-x 1 KinglyHealer 197609     0 Jun 26 tools/
```

### 3. mkdir — 创建目录

```bash
# 创建单个目录
$ mkdir src

# 同时创建多个目录
$ mkdir include output notebooks

# 递归创建多级目录（父目录不存在时自动创建）
$ mkdir -p src/subdir/deepdir

# 本项目创建标准化目录结构
$ mkdir -p include src output notebooks notes tools
```

> `-p` 参数确保父目录不存在时自动创建，不会报错。

### 4. cd — 切换目录

```bash
# 进入 src 目录
$ cd src

# 返回上一级目录
$ cd ..

# 直接回到项目根目录（从任意位置）
$ cd /c/Users/KinglyHealer/Desktop/week16_40_郑礼铭_WK16

# 进入用户主目录
$ cd ~
```

---

## 二、GCC 编译与程序运行

### 1. gcc 编译命令

V3.0 将代码拆分为 6 个源文件，使用通配符一次性编译所有 `.c` 文件：

```bash
# 进入项目根目录
$ cd /c/Users/KinglyHealer/Desktop/week16_40_郑礼铭_WK16

# 编译（开启严格警告，指定头文件路径）
$ gcc -Wall -Wextra -std=c99 src/*.c -I./include -o app.exe
```

> **参数含义**:
>
> | 参数 | 说明 |
> |:---|:---|
> | `-Wall` | 开启所有常用警告 |
> | `-Wextra` | 开启额外警告（如未使用变量） |
> | `-std=c99` | 使用 C99 标准 |
> | `src/*.c` | 编译 `src/` 下所有 C 源文件（6个） |
> | `-I./include` | 告诉编译器去 `include/` 目录查找 `.h` 头文件 |
> | `-o app.exe` | 指定输出文件名为 `app.exe` |

> **编译结果**: `-Wall -Wextra` 下 **0 警告, 0 错误**。

### 2. 运行程序

```bash
# 运行编译生成的可执行文件
$ ./app.exe
请输入学生人数: 3
请输入第1个学生的姓名: 张三
请输入第1个学生的成绩: 85
请输入第2个学生的姓名: 李四
请输入第2个学生的成绩: 92
请输入第3个学生的姓名: 王五
请输入第3个学生的成绩: 78

===== 成绩统计结果 =====
平均分: 85.00
最高分: 92
最低分: 78

成绩排名:
1. 李四: 92分
2. 张三: 85分
3. 王五: 78分
```

---

## 三、重定向输出

### 1. > — 输出重定向（覆盖写入）

```bash
# 将程序运行结果保存到文件（每次运行覆盖旧内容）
$ ./app.exe > output/run_result.txt

# 将 git 日志导出到文件
$ git log --oneline > git_log.txt

# 将 ls 结果保存到文件
$ ls -l > output/ls_result.txt
```

> `>` 会覆盖目标文件原有内容；若文件不存在则新建。

### 2. >> — 输出重定向（追加写入）

```bash
# 追加内容到文件末尾（不覆盖已有内容）
$ echo "===== 第二次测试 =====" >> output/run_result.txt
$ ./app.exe >> output/run_result.txt
```

> `>>` 将新内容追加到文件末尾，保留原有内容。

### 3. tee — 同时输出到屏幕和文件

本项目的 `logger.c` 模块通过 C 代码实现了 `tee_printf()` 函数（使用 `fprintf` + `vprintf` 双路输出），在程序内部同时输出到控制台和 `output/run_result.txt`，无需命令行重定向。

```c
// logger.c 核心逻辑（C 代码模拟 tee 命令效果）
void tee_printf(const char *fmt, ...) {
    va_list args1, args2;
    va_start(args1, fmt);
    va_copy(args2, args1);
    vprintf(fmt, args1);             // 输出到 stdout（屏幕）
    if (g_logfile)
        vfprintf(g_logfile, fmt, args2);  // 输出到文件
    va_end(args2);
    va_start(args1);
}
```

---

## 四、Git 版本控制命令

### 1. git init — 初始化仓库

```bash
$ cd /c/Users/KinglyHealer/Desktop/week16_40_郑礼铭_WK16
$ git init
Initialized empty Git repository in .../week16_40_郑礼铭_WK16/.git/
```

### 2. git add — 暂存文件

```bash
# 暂存单个文件
$ git add README.md

# 暂存整个目录
$ git add src/

# 暂存所有修改过的文件
$ git add .
```

### 3. git commit — 提交版本

```bash
# 提交并附带说明信息
$ git commit -m "feat: 新增 logger 模块（tee输出+独立日志系统）"
```

> V3.0 项目遵循 Conventional Commits 规范：
> - `feat:` 新功能
> - `fix:` 修复
> - `refactor:` 重构
> - `docs:` 文档
> - `chore:` 杂项

### 4. git log — 查看提交历史

```bash
# 简要单行显示
$ git log --oneline
98f346c docs: 重写 README.md 为 V3.0 版本说明
7bf68af docs: 添加 self_check.md 自我检查清单与 prompt_log.md AI审查记录
e62196c feat: 新增 tools/gen_tests.py 自动化测试脚本（7个测试用例）
7fe7620 docs: 添加 issue_or_pr.md 进阶组 GitHub 流程记录
f6c258c feat: 新增 reporter 模块（输出与报表分离）
c76f1b3 feat: 新增 sorter 模块（双算法支持+swap_students 消除重复代码）
3d7308b feat: 新增 calculator 模块（Stats 结构体替代长参数列表）
6f3f4c9 feat: 新增 input 模块（validate_score 消除校验重复代码+scanf安全加固）
2d86a53 feat: 新增 logger 模块（tee输出+独立日志系统）
46ebbd5 refactor: V3.0 公共宏提取到 include/common.h
b09fe65 refactor: 清理 V2.0 旧文件 (student.c, student.h)
ac6cbe7 chore: 移动 origin_main.c 到根目录
e47e5e3 init: 初始化 V3.0 项目目录结构与 .gitignore

# 带图形分支线显示
$ git log --oneline --graph

# 查看最近 5 条
$ git log --oneline -5

# 查看详细提交信息
$ git log
commit 98f346c...
Author: KinglyHealer
Date:   Thu Jun 26 2026

    docs: 重写 README.md 为 V3.0 版本说明
```

### 5. git log 重定向导出

```bash
# 导出提交日志到文件
$ git log --oneline > git_log.txt

# 查看导出的文件内容
$ cat git_log.txt
98f346c docs: 重写 README.md 为 V3.0 版本说明
7bf68af docs: 添加 self_check.md 自我检查清单与 prompt_log.md AI审查记录
...
```

### 6. git status — 查看工作区状态

```bash
$ git status
On branch main
nothing to commit, working tree clean
```

### 7. git diff — 查看修改内容

```bash
# 查看尚未暂存的修改
$ git diff

# 查看已暂存但未提交的修改
$ git diff --staged
```

### 8. git branch — 分支管理

```bash
# 查看所有分支
$ git branch

# 创建新分支
$ git branch v3.0-refactor

# 切换分支
$ git checkout v3.0-refactor

# 创建并切换（二合一）
$ git checkout -b v3.0-refactor

# 合并分支到当前分支
$ git merge v3.0-refactor

# 删除已合并的分支
$ git branch -d v3.0-refactor
```

---

## 五、其他常用命令

### 1. echo — 输出文本

```bash
$ echo "Hello World"
Hello World

# 追加到文件
$ echo "===== 测试分隔线 =====" >> output/run_result.txt
```

### 2. cat — 查看文件内容

```bash
# 查看 git_log.txt 内容
$ cat git_log.txt

# 查看程序输出结果
$ cat output/run_result.txt
```

### 3. rm — 删除文件

```bash
# 删除文件（谨慎使用）
$ rm app.exe

# 删除目录及其内容
$ rm -rf venv/
```

---

## 六、V3.0 开发完整命令流水线

以下是在项目根目录下从零到运行的完整命令序列：

```bash
# 1. 确认当前位置
pwd
# → /c/Users/KinglyHealer/Desktop/week16_40_郑礼铭_WK16

# 2. 查看项目文件
ls
# → README.md  git_log.txt  include/  notes/  notebooks/  output/  src/  tools/

# 3. 编译项目
gcc -Wall -Wextra -std=c99 src/*.c -I./include -o app.exe

# 4. 运行程序
./app.exe

# 5. 导出 git 日志
git log --oneline > git_log.txt

# 6. 查看 git 提交历史
git log --oneline

# 7. 查看工作区状态
git status
```

---

> **总结**: 以上为 V3.0 开发过程中实际用到的 Linux 与 Git 命令记录。重点掌握 `pwd`/`ls`/`mkdir`/`cd` 四个目录导航命令、`gcc` 多文件编译命令、`>` 输出重定向、以及 `git init`/`add`/`commit`/`log`/`status`/`branch` 等版本控制命令。
