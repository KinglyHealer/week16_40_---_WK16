#!/usr/bin/env python3
"""
学生成绩统计系统 — 自动化测试脚本
=====================================
功能:
  1. 编译 C 程序 (gcc -Wall -Wextra)
  2. 批量执行边界测试用例
  3. 验证输出结果与预期匹配
  4. 生成测试报告到 output/run_result.txt

用法:
  python tools/gen_tests.py              # 编译并运行全部测试
  python tools/gen_tests.py --no-build   # 跳过编译，直接运行已有程序
"""

import subprocess
import os
import sys
import re
import argparse
from pathlib import Path

# ---------------------------------------------------------------------------
# 配置
# ---------------------------------------------------------------------------
PROJECT_ROOT = Path(__file__).resolve().parent.parent
SRC_DIR      = PROJECT_ROOT / "src"
INCLUDE_DIR  = PROJECT_ROOT / "include"
OUTPUT_DIR   = PROJECT_ROOT / "output"
EXE_PATH     = PROJECT_ROOT / "app_test.exe"

SRC_FILES = [
    "main.c", "logger.c", "input.c",
    "calculator.c", "sorter.c", "reporter.c"
]

# ---------------------------------------------------------------------------
# 测试用例定义
#  每个用例: (名称, 输入行列表, 预期输出正则列表)
#  输入行按顺序对应 scanf 读取的内容
# ---------------------------------------------------------------------------
TEST_CASES = [
    # ── 用例1: 单人边界 ──
    {
        "name":   "单人最低分边界",
        "inputs": ["1", "张三", "0"],
        "expect": [
            r"平均分:\s*0\.00",
            r"最高分:\s*0",
            r"最低分:\s*0",
            r"1\.\s+张三:\s*0分",
        ],
    },
    {
        "name":   "单人最高分边界",
        "inputs": ["1", "李四", "100"],
        "expect": [
            r"平均分:\s*100\.00",
            r"最高分:\s*100",
            r"最低分:\s*100",
            r"1\.\s+李四:\s*100分",
        ],
    },

    # ── 用例2: 全员同分 ──
    {
        "name":   "全员同分70",
        "inputs": ["3", "A", "70", "B", "70", "C", "70"],
        "expect": [
            r"平均分:\s*70\.00",
            r"最高分:\s*70",
            r"最低分:\s*70",
        ],
    },

    # ── 用例4: 混合极值 ──
    {
        "name":   "混合极值0-100-55-33",
        "inputs": ["4", "P1", "0", "P2", "100", "P3", "55", "P4", "33"],
        "expect": [
            r"平均分:\s*47\.00",
            r"最高分:\s*100",
            r"最低分:\s*0",
            r"1\.\s+P2:\s*100分",
            r"4\.\s+P1:\s*0分",
        ],
    },

    # ── 用例5: 人数非法输入 → 合法 ──
    {
        "name":   "人数非法0和-5修正为2",
        "inputs": ["0", "-5", "2", "X", "85", "Y", "90"],
        "expect": [
            r"人数不合法",
            r"平均分:\s*87\.50",
        ],
    },

    # ── 用例6: 成绩非法输入 → 合法 ──
    {
        "name":   "成绩非法-10和105修正",
        "inputs": ["2", "Tom", "-10", "60", "Jerry", "105", "80"],
        "expect": [
            r"成绩不合法",
            r"平均分:\s*70\.00",
        ],
    },

    # ── 用例7: 降序排序验证 ──
    {
        "name":   "降序排序正确性",
        "inputs": ["5",
                   "S1", "55",
                   "S2", "90",
                   "S3", "70",
                   "S4", "85",
                   "S5", "60"],
        "expect": [
            r"1\.\s+S2:\s*90分",
            r"2\.\s+S4:\s*85分",
            r"3\.\s+S3:\s*70分",
            r"4\.\s+S5:\s*60分",
            r"5\.\s+S1:\s*55分",
        ],
    },
]

# ---------------------------------------------------------------------------
# 编译
# ---------------------------------------------------------------------------
def build_project() -> bool:
    """编译 V3.0 项目，返回是否成功"""
    OUTPUT_DIR.mkdir(parents=True, exist_ok=True)

    src_paths = [str(SRC_DIR / f) for f in SRC_FILES]
    cmd = [
        "gcc", "-Wall", "-Wextra", "-std=c99",
        *src_paths,
        f"-I{INCLUDE_DIR}",
        "-o", str(EXE_PATH),
    ]
    print(f"[BUILD] {' '.join(cmd)}")
    result = subprocess.run(cmd, capture_output=True, text=True,
                            encoding='utf-8', errors='replace',
                            cwd=str(PROJECT_ROOT))

    if result.returncode != 0:
        print(f"[BUILD] FAIL: Compile error:\n{result.stderr}")
        return False

    if result.stderr:
        warnings = [l for l in result.stderr.splitlines() if l.strip()]
        if warnings:
            print(f"[BUILD] WARN: {len(warnings)} warnings:")
            for w in warnings:
                print(f"       {w}")
            return False

    print("[BUILD] OK: Build successful (0 warnings)")
    return True


# ---------------------------------------------------------------------------
# Run a single test
# ---------------------------------------------------------------------------
def run_test(name: str, inputs: list[str], expects: list[str]) -> bool:
    """Run a single test case, return True if all expectations matched"""
    input_bytes = ("\n".join(inputs) + "\n").encode('utf-8')
    try:
        proc = subprocess.Popen(
            [str(EXE_PATH)],
            stdin=subprocess.PIPE,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            cwd=str(PROJECT_ROOT),
        )
        out_bytes, _ = proc.communicate(input=input_bytes, timeout=30)
    except subprocess.TimeoutExpired:
        proc.kill()
        print(f"  [{name}] FAIL: Timeout")
        return False

    output = out_bytes.decode('utf-8', errors='replace') if out_bytes else ""
    all_ok = True
    for pattern in expects:
        if not re.search(pattern, output):
            print(f"  [{name}] FAIL: Not matched: {pattern}")
            all_ok = False

    if all_ok:
        print(f"  [{name}] PASS")
    return all_ok


# ---------------------------------------------------------------------------
# 主流程
# ---------------------------------------------------------------------------
def main():
    parser = argparse.ArgumentParser(description="学生成绩统计系统自动化测试")
    parser.add_argument("--no-build", action="store_true", help="跳过编译")
    args = parser.parse_args()

    os.chdir(str(PROJECT_ROOT))

    # 1. 编译
    if not args.no_build:
        if not build_project():
            sys.exit(1)
    elif not EXE_PATH.exists():
        print("[ERROR] 可执行文件不存在，请先编译")
        sys.exit(1)

    # Test run header
    print(f"\n{'='*50}")
    print(f"Total: {len(TEST_CASES)} test cases")
    print(f"{'='*50}\n")

    passed = 0
    for tc in TEST_CASES:
        if run_test(tc["name"], tc["inputs"], tc["expect"]):
            passed += 1

    # Summary
    print(f"\n{'='*50}")
    print(f"Result: {passed}/{len(TEST_CASES)} passed")
    if passed == len(TEST_CASES):
        print("All tests passed!")
    else:
        print(f"{len(TEST_CASES) - passed} tests failed")
    print(f"{'='*50}")

    # Append test report to run_result.txt
    report_path = OUTPUT_DIR / "run_result.txt"
    if report_path.exists():
        with open(report_path, "a", encoding="utf-8") as f:
            f.write(f"\n\n===== Automated Test Report =====\n")
            f.write(f"Total test cases: {len(TEST_CASES)}\n")
            f.write(f"Passed: {passed}\n")
            f.write(f"Failed: {len(TEST_CASES) - passed}\n")

    return 0 if passed == len(TEST_CASES) else 1


if __name__ == "__main__":
    sys.exit(main())
