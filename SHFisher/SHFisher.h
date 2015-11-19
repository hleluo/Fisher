//
//  Test.h
//  SHFisher
//
//  Created by two_monkey on 15/9/23.
//  Copyright (c) 2015年 twomonkeys. All rights reserved.
//

#ifndef __SHFisher__Test__
#define __SHFisher__Test__

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/**
 *@brief 计算直径
 *@param data[] 源数据
 *@param start 起始索引
 *@param end 结束索引
 *@return 方差
 */
float calculateDiameter(float data[],int start,int end);

/**
 *@brief Fisher最优分段算法
 *@param data[] 源数据
 *@param length 源数据长度
 *@param **result 分段索引结果，一维数组
 *@param *count 总段数
 */
void fisherBest(float data[],int length,int **result,int *count);

#endif /* defined(__SHFisher__Test__) */
