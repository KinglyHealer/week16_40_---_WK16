# 学生成绩统计系统 V3.0 — 多模块工程化重构

> **版本**: V3.0 | **日期**: 2026-06-26
>
> 基于 V2.0 函数化版本的深度模块化重构，实现 6 头文件 + 6 源文件的高内聚低耦合架构。

---

## 一、项目简介

本项目经历了三个版本的持续迭代演进：

| 版本 | 架构特点 | 核心改进 |
|:---|:---|:---|
| **V1.0** | 单文件全部堆砌在 `main()` 中 | 原始功能实现 |
| **V2.0** | 单文件函数化拆分 + 日志系统 | 消除硬编码、函数职责分离、tee 输出 |
| **V3.0** | 多文件多模块工程化架构 | 6 模块拆分、Stats 结构体、双排序算法、scanf 安全加固 |

**核心功能**: 支持学生人数合法性校验、姓名及成绩录入、成绩统计（总分/平均分/最高分/最低分）、降序排序、结果展示与日志输出。在保持功能全覆盖的前提下，完成从"能跑"到"工程化"的实质性代码质量提升。

---

## 二、项目目录结构

```text
week16_40_郑礼铭_WK16/
├── README.md                    # 项目完整说明文档（本文件）
├── origin_main.c                # V2.0 参考代码（前后对照用）
├── prompt_log.md                # 2轮 AI 代码审查提示词与反馈记录
├── git_log.txt                  # Git 提交日志
├── self_check.md                # 自我检查清单（34项全部达标）
├── issue_or_pr.md               # 进阶组 GitHub Issue/PR 流程记录
├── .gitignore                   # Git 忽略配置
│
├── include/                     # 头文件目录（6个模块头文件）
│   ├── common.h                 #   公共宏定义（消除魔法数字）
│   ├── logger.h                 #   日志系统声明
│   ├── input.h                  #   数据输入与校验声明
│   ├── calculator.h             #   统计计算声明 + Stats 结构体
│   ├── sorter.h                 #   排序算法声明 + SortType 枚举
│   └── reporter.h               #   结果输出声明
│
├── src/                         # 源代码目录（6个模块源文件）
│   ├── main.c                   #   V3.0 程序入口
│   ├── logger.c                 #   日志系统实现（tee_printf 双路输出）
│   ├── input.c                  #   数据输入与校验实现（scanf 安全加固）
│   ├── calculator.c             #   统计计算实现（Stats 结构体）
│   ├── sorter.c                 #   排序实现（冒泡+选择，swap_students 去重）
│   └── reporter.c               #   报表输出实现
│
├── output/                      # 程序输出目录
│   └── run_result.txt           # 程序运行结果日志
│
├── tools/                       # 工具脚本目录
│   └── gen_tests.py             # Python 自动化测试脚本（7个用例）
│
├── notebooks/                   # 数据分析目录
│   └── analysis.ipynb           # Jupyter 数据分析与可视化（含交叉校验）
│
└── notes/                       # 开发笔记目录
    └── linux_git_notes.md       # Linux命令、Git实操笔记
```

---

## 三、V3.0 模块架构

### 3.1 模块依赖关系

```
                    ┌─────────────┐
                    │  common.h   │  ← 公共宏 (MAX_STU_NUM, NAME_LEN, ...)
                    └──────┬──────┘
                           │
        ┌──────────────────┼──────────────────┐
        │                  │                  │
   ┌────┴────┐      ┌──────┴──────┐    ┌──────┴──────┐
   │ logger  │      │   input     │    │  reporter   │
   │ .h/.c   │      │   .h/.c     │    │   .h/.c     │
   └────┬────┘      └──────┬──────┘    └──────┬──────┘
        │                  │                  │
        │           ┌──────┴──────┐           │
        │           │ calculator  │           │
        │           │   .h/.c     │           │
        │           └──────┬──────┘           │
        │                  │                  │
        │           ┌──────┴──────┐           │
        │           │   sorter    │           │
        │           │   .h/.c     │           │
        │           └──────┬──────┘           │
        │                  │                  │
        └──────────────────┼──────────────────┘
                           │
                    ┌──────┴──────┐
                    │   main.c    │  ← 组装所有模块
                    └─────────────┘
```

### 3.2 模块职责一览

| 模块 | 头文件 | 源文件 | 对外接口 | 内部辅助函数 |
|:---|:---|:---|:---|:---|
| **公共配置** | `common.h` | — | 4 个宏定义 | — |
| **日志系统** | `logger.h` | `logger.c` | `log_init()`, `log_close()`, `tee_printf()` | — |
| **数据输入** | `input.h` | `input.c` | `input_student_count()`, `input_student_info()` | `validate_score()` |
| **统计计算** | `calculator.h` | `calculator.c` | `calculate_stats()` + `Stats` 结构体 | — |
| **排序** | `sorter.h` | `sorter.c` | `sort_students()` + `SortType` 枚举 | `swap_students()`, `bubble_sort()`, `selection_sort()` |
| **报表输出** | `reporter.h` | `reporter.c` | `print_results()` | — |
| **程序入口** | — | `main.c` | `main()` | — |

---

## 四、环境依赖与运行方式

### 4.1 运行环境

| 工具 | 版本要求 | 用途 |
|:---|:---|:---|
| **GCC** | ≥ 5.0 (支持 C99) | C 程序编译 |
| **Python** | ≥ 3.7 | 自动化测试 |
| **Jupyter** | + pandas + matplotlib | 数据分析可视化 |
| **Git** | ≥ 2.0 | 版本控制 |

### 4.2 编译运行

```bash
# 一键编译（开启全部警告）
gcc -Wall -Wextra -std=c99 src/main.c src/logger.c src/input.c \
    src/calculator.c src/sorter.c src/reporter.c -I./include -o app.exe

# 运行程序
./app.exe
```

> 编译结果: ✅ **0 警告, 0 错误**

### 4.3 自动化测试

```bash
# 编译并运行全部测试用例
python tools/gen_tests.py

# 仅运行测试（跳过编译）
python tools/gen_tests.py --no-build
```

### 4.4 Jupyter 数据分析

```bash
# 安装依赖
pip install pandas matplotlib jupyter

# 启动 Notebook
jupyter notebook notebooks/analysis.ipynb
```

---

## 五、V3.0 核心重构优化点

### 重构点 1: 多模块拆分（业界标准工程架构）

**V2.0 缺陷**: 所有业务逻辑和日志系统混在 `student.c` 中，模块边界模糊，无法独立复用。

**V3.0 改进**: 按单一职责原则拆分为 5 个独立功能模块 + 1 个入口模块:

```
V2.0:  src/student.c  (120+ 行，混合5种职责)
        ↓
V3.0:  src/logger.c    (35 行，纯日志)
       src/input.c      (58 行，纯输入校验)
       src/calculator.c (22 行，纯计算)
       src/sorter.c     (68 行，纯排序)
       src/reporter.c   (18 行，纯输出)
       src/main.c       (27 行，纯组装)
```

**收益**: 每个模块可独立理解、独立测试、独立复用。

### 重构点 2: Stats 结构体消除长参数列表

**V2.0 缺陷**: `calculate_stats()` 需要 5 个指针参数，调用方代码冗长。

```c
// V2.0: 5个独立指针参数
void calculate_stats(int scores[], int n, int *sum, float *avg, int *max, int *min);

// V3.0: 1个结构体指针，语义更清晰
typedef struct { int sum; float avg; int max; int min; } Stats;
void calculate_stats(int scores[], int n, Stats *stats);
```

### 重构点 3: swap_students 消除排序重复代码

**V2.0 缺陷**: 姓名和分数的交换逻辑在 `bubble_sort` 中内联重复，新增排序算法时需重复编写。

```c
// V3.0: 提取为独立函数，两种排序算法共享
static void swap_students(char names[][NAME_LEN], int scores[], int i, int j) {
    /* 交换分数 + 交换姓名 — 一次编写，多处复用 */
}
```

### 重构点 4: validate_score 消除输入校验重复代码

**V2.0 缺陷**: 成绩合法性校验的 while 循环在多处重复。

**V3.0 改进**: 提取 `static int validate_score(void)` 静态函数，模块内部复用，外部不可见。

### 重构点 5: scanf 安全加固

**V2.0 缺陷**: `scanf` 未检查返回值，未限制字符串宽度，存在输入异常行为和缓冲区溢出隐患。

**V3.0 改进**:
- 所有 `scanf` 调用检查返回值 `!= 1` 并清空缓冲区
- `scanf("%s", names[i])` → `scanf("%19s", names[i])` (留 1 字节给 `'\0'`)

### 重构点 6: 双排序算法支持

**V2.0**: 仅硬编码冒泡排序，无法切换算法。

**V3.0 改进**: 通过 `SortType` 枚举支持冒泡排序和选择排序，`main.c` 一行代码即可切换:

```c
sort_students(names, scores, n, SORT_BUBBLE);    // 冒泡
sort_students(names, scores, n, SORT_SELECTION);  // 选择
```

---

## 六、版本演进对比

| 维度 | V1.0 | V2.0 | V3.0 |
|:---|:---|:---|:---|
| **源文件数** | 1 | 2 | 6 |
| **头文件数** | 0 | 1 | 6 |
| **函数数量** | 0（全在 main） | 6 | 9 个对外 + 4 个 static |
| **宏定义** | 无（硬编码） | 4 个 | 4 个（继承 V2.0） |
| **日志系统** | ❌ | ✅ tee_printf | ✅ 独立 logger 模块 |
| **结构体** | ❌ | ❌ | ✅ Stats |
| **排序算法** | 1 种（冒泡） | 1 种（冒泡） | 2 种（冒泡 + 选择） |
| **scanf 安全** | ❌ | ❌ | ✅ 返回值检查 + 宽度限制 |
| **重复代码** | 多处 | 少量 | ✅ 已消除 |
| **模块复用** | ❌ | ❌ | ✅ 任意模块可独立使用 |
| **测试自动化** | ❌ | ❌ | ✅ Python 7 用例 |
| **GCC -Wall -Wextra** | 未验证 | ✅ 零警告 | ✅ 零警告 |

---

## 七、测试覆盖

### 7.1 测试用例矩阵

| # | 用例名称 | 输入 | 预期输出 | 覆盖分支 |
|:---:|:---|:---|:---|:---:|
| 1 | 单人最低分 | n=1, 张三, 0 | avg=0, max=0, min=0 | 单人边界 |
| 2 | 单人最高分 | n=1, 李四, 100 | avg=100, max=100, min=100 | 单人边界 |
| 3 | 人数上限 | n=100, 100名学生 | 完整输出100人排名 | 数组上限 |
| 4 | 全员同分 | 3人, 70/70/70 | avg=max=min=70 | 稳定性 |
| 5 | 混合极值 | 4人, 0/100/55/33 | max=100, min=0, avg=47 | 极值 |
| 6 | 人数非法修正 | 0, -5 → 2 | 提示"不合法"后正常 | 异常路径 |
| 7 | 成绩非法修正 | -10, 105 → 修正 | 提示"不合法"后正常 | 异常路径 |

### 7.2 Jupyter 交叉校验

`notebooks/analysis.ipynb` 包含 Python 与 C 程序的统计结果交叉校验，图表输出:
- 学生成绩柱状分布图
- 成绩等级饼图（优秀/良好/及格/不及格）
- Python vs C 程序统计对比图

---

## 八、AI 代码审查记录

详见 [prompt_log.md](prompt_log.md)，包含两轮完整审查:

| 轮次 | 审查重点 | 发现 | 已修复 |
|:---|:---|:---:|:---:|
| 第 1 轮 | C 语言风险检查（内存泄漏、数组越界、类型转换） | 4 个问题 | 3 个 |
| 第 2 轮 | 测试覆盖检查（分支覆盖、边界条件、错误路径） | 5 条建议 | 全部采纳 |

---

## 九、项目反思与总结

本次 V3.0 重构是 V2.0 函数化重构之后的又一次实质性升级，核心目标是**从"写函数"进阶到"做架构"**。

**关键收获**:
1. **模块化思维**: 学会按职责拆分代码，让每个 `.c/.h` 文件对成为一个独立的功能单元
2. **接口设计意识**: Stats 结构体、SortType 枚举等设计让 API 更清晰、更易扩展
3. **代码去重习惯**: swap_students 和 validate_score 的提取帮助我认识到"复制粘贴是技术债"
4. **安全意识**: scanf 返回值检查和宽度限制是从 AI 审查中学到的实用防御性编程技巧
5. **工程全链路**: 编译→测试→分析→版本管理→文档，形成了完整的 C 语言项目开发闭环

---

## 十、仓库链接

GitHub 项目仓库: [https://github.com/KinglyHealer/WK16-40](https://github.com/KinglyHealer/WK16-40)
