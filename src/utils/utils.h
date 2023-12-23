#ifndef CAMPUSNAVIGATION_UTILS_H
#define CAMPUSNAVIGATION_UTILS_H

char *GbkToUtf8(const char* gbkStr);
char *Utf8ToGbk(const char *utf8Str);
void gotoxy(int x, int y);
void setCursorVisible(bool isVisible);
int parseInt(char *str);
double parseDouble(char *str);
char toLowerDir(char ch);
bool PrintPageHead();
#endif //CAMPUSNAVIGATION_UTILS_H
