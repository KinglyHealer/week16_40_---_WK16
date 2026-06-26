/*
 * reporter.c — 结果输出与报表模块实现
 *
 * 统一输出格式: 统计摘要（平均/最高/最低） + 成绩排名列表。
 * V3.0 改进: 接收 const Stats * 结构体指针，接口语义更明确。
 */

#include "reporter.h"
#include "logger.h"

void print_results(char names[][NAME_LEN], int scores[], int n,
                   const Stats *stats)
{
    tee_printf("\n===== 成绩统计结果 =====\n");
    tee_printf("平均分: %.2f\n", stats->avg);
    tee_printf("最高分: %d\n", stats->max);
    tee_printf("最低分: %d\n", stats->min);

    tee_printf("\n成绩排名:\n");
    for (int i = 0; i < n; i++) {
        tee_printf("%d. %s: %d分\n", i + 1, names[i], scores[i]);
    }
}
