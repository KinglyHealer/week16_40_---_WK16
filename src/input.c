/*
 * input.c — 数据输入与校验模块实现
 *
 * V3.0 改进:
 * - 提取 validate_score() 静态函数，消除成绩校验重复代码
 * - 使用 common.h 宏定义替代硬编码边界值
 */

#include "input.h"
#include "logger.h"

/* ---------- 内部辅助函数 ---------- */

/*
 * 读取并校验单个成绩，循环提示直到输入合法值。
 * 作为 static 函数，仅本模块可见 — 隐藏实现细节。
 */
static int validate_score(void)
{
    int score;
    while (scanf("%d", &score) != 1) {
        tee_printf("输入无效，请输入数字(%d-%d): ", SCORE_MIN, SCORE_MAX);
        while (getchar() != '\n');  /* 清空输入缓冲区 */
    }
    while (score < SCORE_MIN || score > SCORE_MAX) {
        tee_printf("成绩不合法，请重新输入(%d-%d): ", SCORE_MIN, SCORE_MAX);
        while (scanf("%d", &score) != 1) {
            tee_printf("输入无效，请输入数字(%d-%d): ", SCORE_MIN, SCORE_MAX);
            while (getchar() != '\n');
        }
    }
    return score;
}

/* ---------- 对外接口 ---------- */

int input_student_count(void)
{
    int n;
    tee_printf("请输入学生人数: ");
    while (scanf("%d", &n) != 1) {
        tee_printf("输入无效，请输入数字: ");
        while (getchar() != '\n');  /* 清空输入缓冲区 */
    }
    while (n <= 0 || n > MAX_STU_NUM) {
        tee_printf("人数不合法，请重新输入(1-%d): ", MAX_STU_NUM);
        while (scanf("%d", &n) != 1) {
            tee_printf("输入无效，请输入数字: ");
            while (getchar() != '\n');
        }
    }
    return n;
}

void input_student_info(char names[][NAME_LEN], int scores[], int n)
{
    for (int i = 0; i < n; i++) {
        tee_printf("请输入第%d个学生的姓名: ", i + 1);
        scanf("%19s", names[i]);  /* NAME_LEN=20, 留1字节给'\0' */
        tee_printf("请输入第%d个学生的成绩: ", i + 1);
        scores[i] = validate_score();
    }
}
