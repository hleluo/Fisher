//
//  SHDataAnalyzer.c
//  SHFisher
//
//  Created by two_monkey on 15/9/21.
//  Copyright (c) 2015年 twomonkeys. All rights reserved.
//

#include "SHDataAnalyzer.h"
#define DISTANCE_MIN 10  //各段最小数据量长度
#define RESULT_COL_LEN 3   //结果二维数组列长度：[起始索引,结束索引,标识]

/**
 *@brief 分析数据源
 *@param SHData 数据信息结构体
 */
void analyze(struct SHData shData){
    int *indexs;    //分段点索引序列
    int count;      //总段数
    //利用Fisher最优分类算法分段处理
    fisherBest(shData.value, shData.length, &indexs, &count);
    
    //根据各段的平均值设置标识
    int result[count][RESULT_COL_LEN];   //结果,[起始索引,结束索引,标识]
    int countMerge = count; //合并后的总段数
    int i = 0;
    for (i = 0; i < count; i++) {
        int end = (i == count - 1 ? shData.length: indexs[i + 1]) - 1;
        //设置起始索引点
        result[i][0] = indexs[i];
        result[i][1] = end;
        //计算平均值
        float avg = 0.0f;
        int j = 0;
        for (j = indexs[i]; j <= end; j++) {
            avg += shData.value[j];
        }
        avg = avg / (end - indexs[i] + 1);
        //设置类型标识
        if(avg > RUN_MIN && avg <= RUN_MAX){
            result[i][2] = FLAG_RUN;
        }else if (avg > SLEEP_MIN && avg <= SLEEP_MAX){
            result[i][2] = FLAG_SLEEP;
        }else{
            result[i][2] = FLAG_DEFAULT;
        }
        //相邻之间的标识相同的数据合并
        if(i > 0){
            //前一条数据向当前数据合并
            if(result[i - 1][2] == result[i][2]){
                result[i][0] = result[i - 1][0];
                result[i - 1][2] = -1;
                countMerge--;
            }
        }
    }
    //合并结果
    int resultMerge[countMerge][RESULT_COL_LEN];
    int k = 0;
    //计算合并结果
    for (i = 0; i < count; i ++) {
        if(result[i][2] != -1){
            resultMerge[k][0] = result[i][0];
            resultMerge[k][1] = result[i][1];
            resultMerge[k][2] = result[i][2];
            k++;
        }
    }
    printf("\n-------------------------- 标识合并后总段数：%d --------------------------\n",countMerge);
    for (i = 0; i < countMerge; i++) {
        for (int j = 0; j < RESULT_COL_LEN; j++) {
            printf("%d\t",resultMerge[i][j]);
        }
        printf("\n");
    }
    printf("\n************************** 结束标识合并后总段数 ***************************\n");
    
    //各段数据量合并
    merge(&resultMerge[0][0], &countMerge, shData.value);
    
    printf("\n-------------------------- 数据量合并后总段数：%d --------------------------\n",countMerge);
    for (i = 0; i < countMerge; i++) {
        for (int j = 0; j < RESULT_COL_LEN; j++) {
            printf("%d\t",resultMerge[i][j]);
        }
        printf("\n");
    }
    printf("\n************************** 结束数据量合并后总段数 ***************************\n");
}

/**
 *@brief 计算方差
 *@param data[] 数据源
 *@param start 起始索引
 *@param end 结束索引
 *@return 方差
 */
float calculateVariance(float data[],int start,int end){
    return start >= end ? 0 : calculateDiameter(data, start, end) / (end - start);
}

/**
 *@brief 数据合并
 *@param *result 待合并数据指针
 *@param *length 待合并数据长度
 *@param value[] 原始强度值
 */
int merge(int *result,int *length,float value[]){
    int i;
//    //计算平均值
//    float averages[*length];
//    for (i = 0; i < *length; i++) {
//        averages[i] = 0.0f;
//        int start = *(result + i * RESULT_COL_LEN);
//        int end = *(result + i * RESULT_COL_LEN + 1);
//        for (int j = start; j <= end; j++) {
//            averages[i] += value[j];
//        }
//        averages[i] = averages[i] / (end - start + 1);
//    }
    //合并操作
    int lengthMerge = *length;
    for (i = 1; i < *length; i++) {
        int start = *(result + i * RESULT_COL_LEN);
        int end = *(result + i * RESULT_COL_LEN + 1);
        //段数据量小于指定值
        if(end - start + 1 < DISTANCE_MIN){
            //合并最后一段
            if(i == *length - 1){
                //向前合并
                if(*(result + (i - 1) * RESULT_COL_LEN + 2) != -1){
                    *(result + i * RESULT_COL_LEN + 2) = -1;
                    //前一段结束点置为当前结束
                    *(result + (i - 1) * RESULT_COL_LEN + 1) = *(result + i * RESULT_COL_LEN + 1);
                    lengthMerge--;
                }
            }else{
                //合并第1段到倒数第二段
//                float disPrevious = averages[i] - averages[i - 1];  //与前一段平均值差
//                float disNext = averages[i] - averages[i + 1];  //与后一段平均值差
                
                //通过方差比较合并
                float previousVariance = calculateVariance(value, *(result + (i - 1) * RESULT_COL_LEN), *(result + i * RESULT_COL_LEN + 1));  //与前一段方差
                float nextVariance = calculateVariance(value, *(result + i * RESULT_COL_LEN), *(result + (i + 1) * RESULT_COL_LEN + 1));  //与后一段方差
                //向后合并
                if(nextVariance < previousVariance){
                    if(*(result + (i + 1) * RESULT_COL_LEN + 2) != -1){
                        *(result + i * RESULT_COL_LEN + 2) = -1;
                        //后一段起始点置为当前起始点
                        *(result + (i + 1) * RESULT_COL_LEN) = *(result + i * RESULT_COL_LEN);
                        lengthMerge--;
                    }
                }else{
                    //向前合并
                    if(*(result + (i - 1) * RESULT_COL_LEN + 2) != -1){
                        *(result + i * RESULT_COL_LEN + 2) = -1;
                        //前一段结束点置为当前结束
                        *(result + (i - 1) * RESULT_COL_LEN + 1) = *(result + i * RESULT_COL_LEN + 1);
                        lengthMerge--;
                    }
                }
            }
        }
    }
    //合并第一段数据，向后合并
    if(*length > 1 && *(result + 1) - *result + 1 < DISTANCE_MIN && *(result + RESULT_COL_LEN + 2) != -1){
        *(result + 2) = -1;
        //后一段起始点置为当前起始点
        *(result + RESULT_COL_LEN) = *result;
        lengthMerge--;
    }
    //提炼数组
    if(lengthMerge < *length){
        int temp[lengthMerge][RESULT_COL_LEN];  //合并结果
        int k = 0;
        for (int j = 0; j < *length; j++) {
            if(*(result + j * RESULT_COL_LEN + 2) != -1){
                temp[k][0] = *(result + j * RESULT_COL_LEN);
                temp[k][1] = *(result + j * RESULT_COL_LEN + 1);
                temp[k][2] = *(result + j * RESULT_COL_LEN + 2);
                k++;
            }
        }
        k = 0;
        for (; k < lengthMerge; k++) {
            *(result + k * RESULT_COL_LEN) = temp[k][0];
            *(result + k * RESULT_COL_LEN + 1) = temp[k][1];
            *(result + k * RESULT_COL_LEN + 2) = temp[k][2];
        }
        *length = lengthMerge;
        //递归判断
        merge(result, length, value);
        return 0;
    }
    return 1;
}