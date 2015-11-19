//
//  main.c
//  SHFisher
//
//  Created by two_monkey on 15/9/18.
//  Copyright (c) 2015年 twomonkeys. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "SHDataAnalyzer.h"

int main(int argc, const char * argv[]) {
    float start,finish; //程序开始执行时间和结束这行时间
    struct SHData shData;
    int length = 7000;
    float data[length];
    //生成随机数
    srand((unsigned)time(NULL));
    for (int i = 0; i < length; i++) {
        data[i] = rand() / (double)(RAND_MAX / 100);
    }
    float testData[] = {9.3,1.8,1.9,1.7,1.5,1.3,1.4,2.0,1.9,2.3,2.1};
    shData.value = testData;
    shData.length = 11;
    printf("\n############################# 开始数据分析程序 #############################\n");
    start = (float)clock();
    analyze(shData);
    finish = (float)clock();
    printf("\n############################# 结束数据分析程序 #############################\n");
    printf("\n程序执行时间：%.3f s\n",(finish - start) / CLOCKS_PER_SEC);
    return 0;
}

