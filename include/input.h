/*
 * input.h — 数据输入与校验模块
 *
 * 负责学生人数校验、姓名与成绩录入、成绩合法性校验。
 * 所有校验逻辑内聚在本模块，不对外暴露实现细节。
 */

#ifndef INPUT_H
#define INPUT_H

#include "common.h"

/* 输入并校验学生人数 (1..MAX_STU_NUM) */
int  input_student_count(void);

/* 批量录入 n 位学生的姓名与成绩，含成绩合法性校验 */
void input_student_info(char names[][NAME_LEN], int scores[], int n);

#endif /* INPUT_H */
