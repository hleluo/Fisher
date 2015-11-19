//
//  SHDataAnalyzer.h
//  SHFisher
//
//  Created by two_monkey on 15/9/21.
//  Copyright (c) 2015年 twomonkeys. All rights reserved.
//

#ifndef __SHFisher__SHDataAnalyzer__
#define __SHFisher__SHDataAnalyzer__

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "SHFisher.h"

//状态标识区间
#define RUN_MAX 100
#define RUN_MIN 70
#define SLEEP_MAX 70
#define SLEEP_MIN 40
//状态标识
#define FLAG_DEFAULT 0
#define FLAG_RUN 1
#define FLAG_SLEEP 2

//传入数据结构体
struct SHData{
    long *time;     //时间，一维数组
    float *value;   //强度值，一维数组
    int length;     //数组长度
};

/**
 *@brief 数据分析
 *@param SHData 结构体数据
 */
void analyze(struct SHData shData);

#endif /* defined(__SHFisher__SHDataAnalyzer__) */
