#define _CRT_SECURE_NO_WARNINGS  1 
#include <stdio.h>
#include <stdlib.h>
#include "kmp.h"
#include <time.h>
#include<string.h>
// 暴力统计
int brute_find(const char* string, const char* substr) {
    if (!string || !substr) return -1;

    int str = strlen(string);
    int sub = strlen(substr);

    if (str < sub || sub == 0) return -1;

    int count = 0;
    for (int i = 0; i <= str - sub; i++) {
        int j = 0;
        while (j < sub && string[i + j] == substr[j]) {
            j++;
        }
        if (j == sub) {
            return 1;
        }
    }
    return -1;
}

// 随机字符串
void random_string(char* s, int len) {
    static const char charset[] = "abcdefghijklmnopqrstxyz";
    for (int i = 0; i < len; i++) {
        s[i] = charset[rand() % 2];
    }
    s[len] = '\0';
}



int main() {
    srand((unsigned)time(NULL));

    const int STR_LEN = 10000000;   // 主串长度
    const int SUB_LEN = 100;       // 模式串长度

    char* text = (char*)malloc(STR_LEN + 1);
    char* pattern = (char*)malloc(SUB_LEN + 1);

    random_string(text, STR_LEN);
    random_string(pattern, SUB_LEN);

    printf("pattern = %s\n", pattern);

    clock_t start, end;

    // 暴力
    start = clock();
    int brute_ans = brute_find(text, pattern);
    end = clock();
    double brute_time = (double)(end - start) / CLOCKS_PER_SEC;

    // KMP
    start = clock();
    int kmp_ans = kmp_find(text, pattern);
    end = clock();
    double kmp_time = (double)(end - start) / CLOCKS_PER_SEC;

    // 结果
    printf("暴力 Count = %d\n", brute_ans);
    printf("KMP   Count = %d\n", kmp_ans);

    if (brute_ans == kmp_ans) {
        printf("结果一致\n");
    }
    else {
        printf("结果不一致\n");
    }

    printf("暴力 Time = %.4f s\n", brute_time);
    printf("KMP  Time = %.4f s\n", kmp_time);

    free(text);
    free(pattern);
    return 0;
}
