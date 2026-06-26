/*
 * ============================================================================
 *  V2.0 参考代码 — 函数化重构版本（单文件留存）
 *
 *  此文件为 V3.0 多模块拆分前的版本存档，用于新旧代码前后对照。
 *
 *  V2.0 重构特点:
 *  - 函数化拆分: 输入/计算/排序/输出 各自独立
 *  - 宏定义消除魔法数字: MAX_STU_NUM, NAME_LEN, SCORE_MIN, SCORE_MAX
 *  - 日志系统: tee_printf() 同步输出到控制台与 output/run_result.txt
 *  - 单文件架构: 所有代码集中在 main.c + student.h
 *
 *  V2.0 局限性（V3.0 针对性改进）:
 *  - 业务逻辑混杂在单一 student.c 中，模块边界模糊
 *  - 统计函数参数列表冗长（5个指针参数）
 *  - 排序算法单一，交换逻辑重复
 *  - 缺少测试自动化与 CI 意识
 * ============================================================================
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

/* ---------- 宏定义 ---------- */
#define MAX_STU_NUM    100
#define NAME_LEN       20
#define SCORE_MIN      0
#define SCORE_MAX      100

/* ---------- 日志系统 ---------- */
static FILE *g_logfile = NULL;

void log_init(void)
{
    g_logfile = fopen("output/run_result.txt", "w");
}

void log_close(void)
{
    if (g_logfile) { fclose(g_logfile); g_logfile = NULL; }
}

void tee_printf(const char *fmt, ...)
{
    va_list args1, args2;
    va_start(args1, fmt);
    va_copy(args2, args1);
    vprintf(fmt, args1);
    if (g_logfile) { vfprintf(g_logfile, fmt, args2); fflush(g_logfile); }
    va_end(args2);
    va_end(args1);
}

/* ---------- 函数声明 ---------- */
int  input_student_count(void);
void input_student_info(char names[][NAME_LEN], int scores[], int n);
void calculate_stats(int scores[], int n, int *sum, float *avg, int *max, int *min);
void sort_students(char names[][NAME_LEN], int scores[], int n);
void print_results(char names[][NAME_LEN], int scores[], int n, float avg, int max, int min);

/* ---------- 主函数 ---------- */
int main(void)
{
    system("chcp 65001 >nul");
    log_init();

    char names[MAX_STU_NUM][NAME_LEN];
    int  scores[MAX_STU_NUM];
    int  sum = 0, max, min;
    float avg;

    int n = input_student_count();
    input_student_info(names, scores, n);
    calculate_stats(scores, n, &sum, &avg, &max, &min);
    sort_students(names, scores, n);
    print_results(names, scores, n, avg, max, min);

    log_close();
    return 0;
}

/* ---------- 函数实现 ---------- */
int input_student_count(void)
{
    int n;
    tee_printf("请输入学生人数: ");
    scanf("%d", &n);
    while (n <= 0 || n > MAX_STU_NUM) {
        tee_printf("人数不合法，请重新输入(1-%d): ", MAX_STU_NUM);
        scanf("%d", &n);
    }
    return n;
}

void input_student_info(char names[][NAME_LEN], int scores[], int n)
{
    for (int i = 0; i < n; i++) {
        tee_printf("请输入第%d个学生的姓名: ", i + 1);
        scanf("%s", names[i]);
        tee_printf("请输入第%d个学生的成绩: ", i + 1);
        scanf("%d", &scores[i]);
        while (scores[i] < SCORE_MIN || scores[i] > SCORE_MAX) {
            tee_printf("成绩不合法，请重新输入(%d-%d): ", SCORE_MIN, SCORE_MAX);
            scanf("%d", &scores[i]);
        }
    }
}

void calculate_stats(int scores[], int n,
                     int *sum, float *avg, int *max, int *min)
{
    *sum = 0;
    *max = SCORE_MIN;
    *min = SCORE_MAX;
    for (int i = 0; i < n; i++) {
        *sum += scores[i];
        if (scores[i] > *max) *max = scores[i];
        if (scores[i] < *min) *min = scores[i];
    }
    *avg = (float)(*sum) / n;
}

void sort_students(char names[][NAME_LEN], int scores[], int n)
{
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - 1 - i; j++) {
            if (scores[j] < scores[j + 1]) {
                int tmp_sc = scores[j];
                scores[j] = scores[j + 1];
                scores[j + 1] = tmp_sc;
                char tmp_name[NAME_LEN];
                strcpy(tmp_name, names[j]);
                strcpy(names[j], names[j + 1]);
                strcpy(names[j + 1], tmp_name);
            }
        }
    }
}

void print_results(char names[][NAME_LEN], int scores[], int n,
                   float avg, int max, int min)
{
    tee_printf("\n===== 成绩统计结果 =====\n");
    tee_printf("平均分: %.2f\n", avg);
    tee_printf("最高分: %d\n", max);
    tee_printf("最低分: %d\n", min);
    tee_printf("\n成绩排名:\n");
    for (int i = 0; i < n; i++) {
        tee_printf("%d. %s: %d分\n", i + 1, names[i], scores[i]);
    }
}
