/*
 * logger.h — 日志系统模块
 *
 * 提供 tee 输出能力：将 printf 内容同步写入控制台与日志文件。
 * 使用方式: 在程序入口调用 log_init() / log_close() 包裹主逻辑。
 */

#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>
#include <stdarg.h>

/* 初始化日志文件（output/run_result.txt） */
void log_init(void);

/* 关闭日志文件，释放资源 */
void log_close(void);

/* 同时输出到 stdout 和日志文件（va_list 可变参数） */
void tee_printf(const char *fmt, ...);

#endif /* LOGGER_H */
