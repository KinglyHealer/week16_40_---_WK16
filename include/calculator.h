/*
 * calculator.h — 成绩统计计算模块
 *
 * 提供总分/平均分/最高分/最低分的一站式计算。
 * V3.0 改进: 使用 Stats 结构体替代冗长的指针参数列表。
 */

#ifndef CALCULATOR_H
#define CALCULATOR_H

/* 统计结果结构体 — 一次返回全部统计量 */
typedef struct {
    int   sum;   /* 总分   */
    float avg;   /* 平均分 */
    int   max;   /* 最高分 */
    int   min;   /* 最低分 */
} Stats;

/* 遍历成绩数组，计算并填充 Stats 结构体 */
void calculate_stats(int scores[], int n, Stats *stats);

#endif /* CALCULATOR_H */
