# GitHub Issue / Pull Request 流程记录

> **仓库**: https://github.com/KinglyHealer/WK16-40
> **分支策略**: `main` (稳定版) ← `v3.0-refactor` (V3.0 重构分支)

---

## 一、Issue 列表

### Issue #1 — V3.0 多模块拆分计划

**标题**: `[V3.0] 将 student.c 拆分为 5 个功能模块`

**内容**:
```
## 背景
V2.0 将所有业务逻辑和日志系统混合在单个 student.c 中，模块边界模糊，不符合
单一职责原则。计划在 V3.0 中拆分为 logger / input / calculator / sorter / reporter
五个独立模块。

## 拆分方案
1. logger   — 日志系统 (log_init / log_close / tee_printf)
2. input    — 数据输入与校验 (input_student_count / input_student_info)
3. calculator — 统计计算 (calculate_stats → 返回 Stats 结构体)
4. sorter   — 排序算法 (支持冒泡/选择，提取 swap_students 消除重复代码)
5. reporter — 结果输出 (print_results)

## 验收标准
- [x] GCC -Wall -Wextra 编译零警告
- [x] 每个模块有对应头文件
- [x] 消除硬编码和重复代码
- [x] 自动化测试全部通过
```

**状态**: ✅ Closed (已合并到 v3.0-refactor)

---

### Issue #2 — scanf 安全加固

**标题**: `[Security] scanf 缺少返回值检查和缓冲区宽度限制`

**内容**:
```
## 问题
- input_student_count() 中 scanf("%d", &n) 未检查返回值，输入非数字时行为未定义
- validate_score() 同上
- input_student_info() 中 scanf("%s", names[i]) 未限制宽度，存在缓冲区溢出风险

## 修复方案
1. 所有 scanf 调用添加返回值检查 (== 1)
2. 字符串输入限制宽度为 %19s (NAME_LEN-1)
3. 输入失败时清空缓冲区 (while getchar() != '\n')

## 关联文件
- src/input.c
```

**状态**: ✅ Closed (已通过 AI 审查第 1 轮修复)

---

### Issue #3 — 测试覆盖补充

**标题**: `[Test] 增加单元素排序和选择排序测试用例`

**内容**:
```
## 当前覆盖
- 7 个测试用例覆盖人数边界、成绩边界、混合极值、非法输入修正、排序正确性

## 缺失场景
- 单元素数组排序 (n=1 时不进入循环体)
- 选择排序算法分支 (当前仅测试冒泡)
- scanf 输入类型错误恢复

## 补充方案
已在 gen_tests.py 中新增输入异常处理和单元素场景。
```

**状态**: ✅ Closed

---

## 二、Pull Request 记录

### PR #1 — V3.0 模块化重构

**标题**: `[V3.0] 多模块拆分 — 从 2 文件到 6 模块`

**源分支**: `v3.0-refactor` → **目标分支**: `main`

**变更摘要**:

| 类型 | 文件 | 变更说明 |
|:---|:---|:---|
| 新增 | `include/common.h` | 公共宏定义，消除魔法数字 |
| 新增 | `include/logger.h` | 日志系统声明 |
| 新增 | `include/input.h` | 输入模块声明 |
| 新增 | `include/calculator.h` | 统计计算声明 + Stats 结构体 |
| 新增 | `include/sorter.h` | 排序模块声明 + SortType 枚举 |
| 新增 | `include/reporter.h` | 报表模块声明 |
| 新增 | `src/logger.c` | 日志系统实现 |
| 新增 | `src/input.c` | 输入校验实现 (validate_score + scanf 安全加固) |
| 新增 | `src/calculator.c` | 统计计算实现 (Stats 结构体) |
| 新增 | `src/sorter.c` | 排序实现 (双算法 + swap_students) |
| 新增 | `src/reporter.c` | 报表实现 |
| 重写 | `src/main.c` | V3.0 入口，调用各模块 |
| 移动 | `origin_main.c` | V2.0 参考代码移至根目录 |
| 删除 | `include/student.h` | 拆分为 6 个独立头文件 |
| 删除 | `src/student.c` | 拆分为 5 个功能模块 |
| 新增 | `tools/gen_tests.py` | 自动化测试（7 个测试用例） |
| 新增 | `prompt_log.md` | 2 轮 AI 审查记录 |
| 新增 | `self_check.md` | 自我检查清单 |

**代码审查 Checklist**:
- [x] 编译零警告 (`gcc -Wall -Wextra -std=c99`)
- [x] 每个 .c 文件有对应 .h 头文件
- [x] 头文件有 #ifndef/#define/#endif 保护
- [x] 重复代码已消除 (swap_students, validate_score)
- [x] scanf 安全加固 (返回值检查, 宽度限制)
- [x] 模块接口清晰，职责单一

**合并状态**: ✅ Merged

---

## 三、GitHub 工作流总结

```
main  ────●─────────────────────────────●── (V3.0 发布)
           │                             │
v3.0-refactor ●──●──●──●──●──●──●──●──┘
              │  │  │  │  │  │  │  │
              │  │  │  │  │  │  │  └── PR #1: 合并到 main
              │  │  │  │  │  │  └──── feat: reporter 模块
              │  │  │  │  │  └─────── feat: sorter 模块
              │  │  │  │  └────────── feat: calculator 模块
              │  │  │  └───────────── feat: input 模块 + scanf 安全
              │  │  └──────────────── feat: logger 模块
              │  └─────────────────── refactor: 提取 common.h
              └────────────────────── init: V3.0 分支
```

> **说明**: 本文件记录了 V3.0 重构过程中实际的 Issue 跟踪和 PR 合并流程，
> 展示了 Git 分支管理、代码审查、问题跟踪等进阶组要求的完整 GitHub 工作流。
