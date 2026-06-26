/*
 * main.c — 学生成绩统计系统 V3.0 入口
 *
 * V3.0 重构亮点:
 * - 6 个功能模块各司其职 (logger / input / calculator / sorter / reporter)
 * - Stats 结构体替代长参数列表，API 更清晰
 * - SortType 枚举支持多算法切换
 * - swap_students / validate_score 消除重复代码
 */

#include <stdlib.h>
#include "common.h"
#include "logger.h"
#include "input.h"
#include "calculator.h"
#include "sorter.h"
#include "reporter.h"

int main(void)
{
    system("chcp 65001 >nul");
    log_init();

    char names[MAX_STU_NUM][NAME_LEN];
    int  scores[MAX_STU_NUM];
    Stats stats;

    /* 1. 输入 */
    int n = input_student_count();
    input_student_info(names, scores, n);

    /* 2. 统计 */
    calculate_stats(scores, n, &stats);

    /* 3. 排序（可切换 SORT_SELECTION） */
    sort_students(names, scores, n, SORT_BUBBLE);

    /* 4. 输出 */
    print_results(names, scores, n, &stats);

    log_close();
    return 0;
}
