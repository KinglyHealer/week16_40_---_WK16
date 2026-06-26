/*
 * calculator.c — 成绩统计计算模块实现
 *
 * V3.0 改进:
 * - 使用 Stats 结构体作为输出容器，替代 V2.0 的 5 个独立指针参数
 * - 调用方只需传入一个 Stats 指针，API 更清晰
 */

#include "calculator.h"
#include "common.h"

void calculate_stats(int scores[], int n, Stats *stats)
{
    stats->sum = 0;
    stats->max = SCORE_MIN;   /* 用宏替代魔法数字 0  */
    stats->min = SCORE_MAX;   /* 用宏替代魔法数字 100 */

    for (int i = 0; i < n; i++) {
        stats->sum += scores[i];
        if (scores[i] > stats->max) stats->max = scores[i];
        if (scores[i] < stats->min) stats->min = scores[i];
    }

    stats->avg = (float)(stats->sum) / n;
}
