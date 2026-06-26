/*
 * reporter.h — 结果输出与报表模块
 *
 * 统一管理所有输出格式：统计摘要 + 成绩排名列表。
 */

#ifndef REPORTER_H
#define REPORTER_H

#include "common.h"
#include "calculator.h"

/* 打印统计摘要与成绩排名 */
void print_results(char names[][NAME_LEN], int scores[], int n,
                   const Stats *stats);

#endif /* REPORTER_H */
