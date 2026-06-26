/*
 * common.h — 项目通用宏定义
 *
 * 所有模块共享的常量统一管理于此文件，消除硬编码魔法数字。
 * 修改参数时仅需改动此处，全局生效。
 */

#ifndef COMMON_H
#define COMMON_H

#define MAX_STU_NUM    100    /* 最大学生人数        */
#define NAME_LEN       20     /* 姓名缓冲区长度      */
#define SCORE_MIN      0      /* 成绩有效下限        */
#define SCORE_MAX      100    /* 成绩有效上限        */

#endif /* COMMON_H */
