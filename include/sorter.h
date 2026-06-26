/*
 * sorter.h — 排序模块
 *
 * 支持多种排序算法，通过 SortType 枚举选择。
 * V3.0 改进: 提取 swap_students 消除交换重复代码，新增选择排序算法。
 */

#ifndef SORTER_H
#define SORTER_H

#include "common.h"

/* 排序算法类型 */
typedef enum {
    SORT_BUBBLE,      /* 冒泡排序（默认） */
    SORT_SELECTION    /* 选择排序          */
} SortType;

/* 按成绩降序排列学生，同步移动姓名 */
void sort_students(char names[][NAME_LEN], int scores[], int n, SortType type);

#endif /* SORTER_H */
