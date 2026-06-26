/*
 * sorter.c — 排序模块实现
 *
 * V3.0 改进:
 * - 提取 swap_students() 消除姓名/分数交换的重复代码
 * - 支持两种排序算法: 冒泡排序 / 选择排序
 * - 通过 SortType 枚举选择算法，方便扩展新算法
 */

#include "sorter.h"
#include <string.h>

/* ---------- 内部辅助函数 ---------- */

/*
 * 交换两个学生的姓名和成绩 — 消除 V2.0 中内联交换的代码重复
 */
static void swap_students(char names[][NAME_LEN], int scores[], int i, int j)
{
    /* 交换分数 */
    int tmp_sc = scores[i];
    scores[i] = scores[j];
    scores[j] = tmp_sc;

    /* 交换姓名 */
    char tmp_name[NAME_LEN];
    strcpy(tmp_name, names[i]);
    strcpy(names[i], names[j]);
    strcpy(names[j], tmp_name);
}

/*
 * 冒泡排序 — 稳定排序，相邻元素比较交换
 */
static void bubble_sort(char names[][NAME_LEN], int scores[], int n)
{
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - 1 - i; j++) {
            if (scores[j] < scores[j + 1]) {
                swap_students(names, scores, j, j + 1);
            }
        }
    }
}

/*
 * 选择排序 — 每轮选出最大值放到前面
 */
static void selection_sort(char names[][NAME_LEN], int scores[], int n)
{
    for (int i = 0; i < n - 1; i++) {
        int max_idx = i;
        for (int j = i + 1; j < n; j++) {
            if (scores[j] > scores[max_idx]) {
                max_idx = j;
            }
        }
        if (max_idx != i) {
            swap_students(names, scores, i, max_idx);
        }
    }
}

/* ---------- 对外接口 ---------- */

void sort_students(char names[][NAME_LEN], int scores[], int n, SortType type)
{
    switch (type) {
    case SORT_SELECTION:
        selection_sort(names, scores, n);
        break;
    case SORT_BUBBLE:
    default:
        bubble_sort(names, scores, n);
        break;
    }
}
