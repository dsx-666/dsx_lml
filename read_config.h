#pragma once
#include<string.h>
#include<stdio.h>

const char FILE_NAME[] = "config.txt";
const char* result = "0";
// 读取配置参数 int
int read_config_int(const char* file_name,const char* var,int * ans) {
	FILE* f = fopen(file_name, "r");
	char str[128] = { 0 };
	if (f == NULL) {
		return -1;
	}
	else {
		while (fscanf(f, "%s", str) == 1) {
			if (strcmp(str, var)==0) {
				fscanf(f, "%d", &ans);
				fclose(f);
				return 1;
			}
		}
	}
	fclose(f);
	return -1;
}
// 读取配置参数 char*
int read_config_char(const char* file_name, const char* var,char* ans) {
	FILE* f = fopen(file_name, "r");
	char str[128] = { 0 };
	if (f == NULL) {
		return -1;
	}
	else {
		while (fscanf(f, "%s", str) == 1) {
			if (strcmp(str, var) == 0) {
				fscanf(f, "%s", ans);
				fclose(f);
				return 1;
			}
		}
	}
	fclose(f);
	return -1;
}
// 修改配置文件 整型
int change_config_int(const char* file_name, const char* var, int new_val)
{
	// 配置文件修改的核心逻辑：读原文件写入临时文件，修改对应行，再替换原文件
	FILE* f_read = fopen(file_name, "r");
	FILE* f_write = fopen("temp_config.txt", "w"); // 临时文件存修改后的内容
	char str[128] = { 0 };
	int find_flag = 0; // 标记是否找到要修改的变量

	// 文件打开失败判断
	if (f_read == NULL || f_write == NULL)
	{
		if (f_read) fclose(f_read);
		if (f_write) fclose(f_write);
		return -1;
	}

	// 逐行读取原文件，写入临时文件
	while (fscanf(f_read, "%s", str) == 1)
	{
		// 跳过注释行，原样写入临时文件，不修改注释
		if (str[0] == '#')
		{
			fprintf(f_write, "%s\n", str);
			// 读取注释行剩余内容，原样写入
			fscanf(f_read, "%*[^\n]");
			fgetc(f_read);
			continue;
		}

		// 匹配到要修改的变量名
		if (strcmp(str, var) == 0)
		{
			find_flag = 1; // 标记找到变量
			fprintf(f_write, "%s %d\n", var, new_val); // 写入【新值】
			fscanf(f_read, "%*s"); // 跳过原文件的旧值，不读取
		}
		else
		{
			// 不是目标变量，原样写入键+值
			char val_buf[128] = { 0 };
			fscanf(f_read, "%s", val_buf);
			fprintf(f_write, "%s %s\n", str, val_buf);
		}
	}

	// 关闭所有文件
	fclose(f_read);
	fclose(f_write);

	// 没找到变量，删除临时文件，返回-1
	if (find_flag == 0)
	{
		remove("temp_config.txt");
		return -1;
	}

	// 找到变量：删除原文件，把临时文件改名成原文件 → 完成替换
	remove(file_name);
	rename("temp_config.txt", file_name);

	return 1; // 修改成功返回1
}

// 修改配置文件 字符串
int change_config_char(const char* file_name, const char* var, const char* new_val)
{
	FILE* f_read = fopen(file_name, "r");
	FILE* f_write = fopen("temp_config.txt", "w");
	char str[128] = { 0 };
	int find_flag = 0;

	if (f_read == NULL || f_write == NULL)
	{
		if (f_read) fclose(f_read);
		if (f_write) fclose(f_write);
		return -1;
	}

	while (fscanf(f_read, "%s", str) == 1)
	{
		if (str[0] == '#')
		{
			fprintf(f_write, "%s\n", str);
			fscanf(f_read, "%*[^\n]");
			fgetc(f_read);
			continue;
		}

		if (strcmp(str, var) == 0)
		{
			find_flag = 1;
			fprintf(f_write, "%s %s\n", var, new_val); // 写入字符串新值
			fscanf(f_read, "%*s");
		}
		else
		{
			char val_buf[128] = { 0 };
			fscanf(f_read, "%s", val_buf);
			fprintf(f_write, "%s %s\n", str, val_buf);
		}
	}

	fclose(f_read);
	fclose(f_write);

	if (find_flag == 0)
	{
		remove("temp_config.txt");
		return -1;
	}

	remove(file_name);
	rename("temp_config.txt", file_name);

	return 1;
}




