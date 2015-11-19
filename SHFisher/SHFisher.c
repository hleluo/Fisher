//
//  Test.c
//  SHFisher
//
//  Created by two_monkey on 15/9/23.
//  Copyright (c) 2015年 twomonkeys. All rights reserved.
//

#include "SHFisher.h"

/**
 *@brief 计算直径
 *@param data[] 源数据
 *@param start 起始索引
 *@param end 结束索引
 *@return 方差
 */
float calculateDiameter(float data[],int start,int end){
    if (start == end) {
        return 0.0f;
    }
    int i;
    //计算平均值
    float average = 0.0f;
    for (i = start; i <= end; i++) {
        average += data[i];
    }
    average = average / (end - start + 1);
    //计算方差
    float diameter = 0.0f;
    for (i = start; i <= end; i++) {
        diameter += powf(data[i] - average, 2.0f);
    }
    return diameter;
}

/**
 *@brief 计算最小损失函数
 *@param *diameter 直径，二维数组（注：上三角）
 *@param *loss 损失函数，二维数组（注：下三角）
 *@param *point 与损失函数对应的索引序列，二维数组（注：下三角）
 *@param length 数据长度
 *@param l 损失函数行号
 *@param k 损失函数列号
 */
void calcluateMinLoss(float *diameter,float *loss,int *point,int length,int l,int k){
    float data[l - k + 1];
    int index[l - k + 1];
    int diameterLength = (length * (length + 1)) / 2;
    //计算不同情况下的损失函数
    if (k == 2) {
        for (int i = k; i <= l; i++) {
            data[i - k] = *(diameter + i - 2) + *(diameter + diameterLength - (length - i + 1) *(length - i + 2) / 2 + l - i);
            index[i - k] = i;
        }
    }else{
        for (int i = k; i <= l; i++) {
            data[i - k] = *(loss + (i - 1) * i / 2 + k - 1) + *(diameter + diameterLength - (length - i + 1) * (length - i + 2) / 2 + l - i);
            index[i - k] = i;
        }
    }
    //选取最小的损失函数
    *(loss + l * (l + 1) / 2 + k) = data[0];
    *(point + l * (l + 1) / 2 + k) = index[0];
    for (int i = 1; i < l - k + 1; i ++) {
        if(data[i] < *(loss + l * (l + 1) / 2 + k)){
            *(loss + l * (l + 1) / 2 + k) = data[i];
            *(point + l * (l + 1) / 2 + k) = index[i];
        }
    }
}

/**
 *@brief 计算最优分段，根据斜率绝对值大小比对结果
 *@param *data 待计算斜率的原始数据，此处*data为*loss，计算过程中取*loss的最后一行数据
 *@param length 数据长度
 */
int calculateBestCount(float *data,int length){
    //3段及以下不作处理
    if(length <= 3){
        return length;
    }
    int count = 3;
    //数据起始索引
    int startIndex = length * (length + 1) / 2;
    //选择最大斜率绝对值
    float difference = fabsf(*(data + startIndex + 2) - *(data + startIndex + 3));
    for (int i = 3; i < length - 1; i++) {
        if(difference < fabsf(*(data + startIndex + i) - *(data + startIndex + i + 1))){
            difference = fabsf(*(data + startIndex + i) - *(data + startIndex + i + 1));
            count = i + 1;
        }
    }
    return count;
}

/**
 *@brief Fisher最优分段算法
 *@param data[] 源数据
 *@param length 源数据长度
 *@param **result 分段索引结果，一维数组
 *@param *count 总段数
 */
void fisherBest(float data[],int length,int **result,int *count){
    int i;
    //初始化直径
    int diameterLength = (length * (length + 1)) / 2;
    float *diameter = (float *)malloc(sizeof(float) * diameterLength);
    for (i = 0; i < diameterLength; i++) {
        *(diameter + i) = 0;
    }
    //计算直径
    for (i = 0; i < length; i++) {
        for (int j = i; j < length; j++) {
            *(diameter + diameterLength - ((length - i) * (length - i + 1)) / 2 + j - i) = calculateDiameter(data, i, j);
        }
    }
    printf("\n-------------------------- 打印直径序列 -----------------------------\n");
    for (i = 0; i < length; i++) {
        for (int j = 0; j < length; j++) {
            if(i > j){
                printf("\t\t\t");
            }else{
                printf("%.3f\t\t",*(diameter + diameterLength - ((length - i) * (length - i + 1)) / 2 + j - i));
            }
        }
        printf("\n");
    }
    printf("\n************************** 结束打印直径序列 ***************************\n");
    //初始化损失函数和分割点
    int lossLength = ((length + 1) * (length + 2)) / 2;
    float *loss = (float *)malloc(sizeof(float) * lossLength);
    int *point = (int *)malloc(sizeof(int) * lossLength);
    for (i = 0; i < length + 1; i++) {
        for (int j = 0; j <= i; j++) {
            *(loss + (i * (i + 1)) / 2 + j) = 0;
            *(point + (i * (i + 1)) / 2 + j) = i == j ? i : 0;
        }
    }
    printf("\n-------------------------- 初始化损失函数 -----------------------------\n");
    for (i = 0; i < length + 1; i++) {
        for (int j = 0; j <= i; j++) {
            printf("%d\t",*(point + (i * (i + 1)) / 2 + j));
        }
        printf("\n");
    }
    printf("\n************************ 结束初始化损失函数 ***************************\n");
    //计算最小损失函数
    for (i = 3; i < length + 1; i++) {
        for (int j = 2; j <= i; j++) {
            calcluateMinLoss(diameter, loss, point, length, i, j);
        }
    }
    //释放指针
    free(diameter);
    diameter = NULL;
    printf("\n-------------------------- 打印损失函数 -----------------------------\n");
    for (i = 0; i < length + 1; i++) {
        for (int j = 0; j <= i; j++) {
            printf("%.3f\t",*(loss + (i * (i + 1)) / 2 + j));
        }
        printf("\n");
    }
    printf("\n************************* 结束打印损失函数 **************************\n");
    printf("\n-------------------------- 打印位置函数 -----------------------------\n");
    for (i = 0; i < length + 1; i++) {
        for (int j = 0; j <= i; j++) {
            printf("%d\t",*(point + (i * (i + 1)) / 2 + j));
        }
        printf("\n");
    }
    printf("\n************************ 结束打印位置函数 ***************************\n");
    int row = length;
    *count = calculateBestCount(loss, length); //最优总段数
    free(loss);
    loss = NULL;
    int column = *count;
    //初始分段化索引序列
    int *indexs = (int *)malloc(sizeof(int) * column);  //分段索引，各段起始索引
    for (i = 0; i < column; i++) {
        *(indexs + i) = 0;
    }
    //查找分段索引序列
    while (column > 1) {
        int currentIndex = *(point + row * (row + 1) / 2 + column) - 1;
        *(indexs + column - 1) = currentIndex;
        row = currentIndex;
        column--;
    }
    //释放指针
    free(point);
    point = NULL;
    printf("\n------------------------ 打印分段索引总段数：%d -------------------------\n",*count);
    for (i = 0; i < *count; i++) {
        printf("%d\t",*(indexs + i));
    }
    printf("\n********************** 结束打印分段索引总段数 ***************************\n");
    //结果赋值
    *result = indexs;
    //释放指针
    free(indexs);
    indexs = NULL;
}