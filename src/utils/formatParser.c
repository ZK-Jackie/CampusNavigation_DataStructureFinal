#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include "iconv.h"

char *GbkToUtf8(const char* gbkStr) {
	// 创建iconv句柄
	iconv_t conv = iconv_open("UTF-8", "GBK");
	if (conv == (iconv_t)-1) {
		perror("iconv_open failed");
		return NULL;
	}
	size_t in_len = strlen(gbkStr);
	size_t out_len = in_len * 4;  // UTF-8最多占用4个字节
	// 分配输出缓冲区
	char *utf8_str = (char *)malloc(out_len);
	memset(utf8_str, 0, out_len);
	char *inBuf = (char *)gbkStr;
	char *outBuf = utf8_str;
	// 进行转换
	if (iconv(conv, &inBuf, &in_len, &outBuf, &out_len) == -1) {
		perror("iconv conversion failed");
		iconv_close(conv);
		free(utf8_str);
		return NULL;
	}
	// 关闭iconv句柄
	iconv_close(conv);
	return utf8_str;
}
char *Utf8ToGbk(const char *utf8Str) {
	size_t inLen = strlen(utf8Str);
	size_t outLen = inLen * 2;  // 预留足够的空间
	char* gbkStr = (char *)malloc(outLen + 1);  // 分配存储空间
	memset(gbkStr, 0, outLen + 1);  // 初始化为零
	iconv_t cd = iconv_open("GBK", "UTF-8");  // 创建转换句柄
	if (cd == (iconv_t)-1) {
		perror("iconv_open");
		free(gbkStr);
		return NULL;
	}
	char *inPtr = (char *)utf8Str;
	char *outPtr = gbkStr;
	if (iconv(cd, &inPtr, &inLen, &outPtr, &outLen) == (size_t)-1) {
		perror("iconv");
		free(gbkStr);
		iconv_close(cd);
		return NULL;
	}
	iconv_close(cd);
	return gbkStr;
}
char toLowerDir(char ch){	//仅对方向字符开放
	if(ch == 's' || ch == 'n'  || ch == 'w'  || ch == 'e'){
		return ch;
	}
	if(ch == 'S' || ch == 'N'  || ch == 'W'  || ch == 'E'){
		return (char)(ch+32);
	}
	return 0;
}
int parseInt(char *str){
	if(str == NULL){
		return -1;
	}
	int ret = 0;
	int len = (int)strlen(str);
	for (int i = 0; i < len; ++i) {
		if(str[i] < '0' || str[i] > '9'){
			return -1;
		}
		ret *= 10;
		ret += str[i] - '0';
	}
	return ret;
}
double parseDouble(char *str){
	if(str == NULL){
		return -1.0;
	}
	double ret = 0.0;
	int fracCnt = 0;
	bool isFrac = false;
	for (int i = 0; i < strlen(str); ++i) {
		if(str[i] == '.'){
			i++;
			isFrac = true;
		} else if(str[i] > '9' || str[i] < '0'){
			return -1.0;
		}
		ret *= 10;
		ret += str[i] - '0';
		if(isFrac){
			fracCnt++;	//数小数个数
		}
	}
	return ret/pow(10.0,(double)fracCnt);
}
