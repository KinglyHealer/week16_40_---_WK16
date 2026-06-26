/*
 * logger.c — 日志系统实现
 *
 * 内部维护一个静态文件指针 g_logfile，通过 log_init/log_close 管理生命周期。
 * tee_printf 使用 va_copy 安全地复制可变参数列表，分别输出到 stdout 和文件。
 */

#include "logger.h"

static FILE *g_logfile = NULL;

void log_init(void)
{
    g_logfile = fopen("output/run_result.txt", "w");
    /* 打开失败不阻断程序，仅跳过文件写入 */
}

void log_close(void)
{
    if (g_logfile) {
        fclose(g_logfile);
        g_logfile = NULL;
    }
}

void tee_printf(const char *fmt, ...)
{
    va_list args1, args2;
    va_start(args1, fmt);
    va_copy(args2, args1);

    vprintf(fmt, args1);

    if (g_logfile) {
        vfprintf(g_logfile, fmt, args2);
        fflush(g_logfile);   /* 实时刷新，防止崩溃丢失日志 */
    }

    va_end(args2);
    va_end(args1);
}
