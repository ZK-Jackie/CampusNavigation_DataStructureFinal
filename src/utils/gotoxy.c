#include <windows.h>

void gotoxy(int x, int y){	//移动cmd中的光标，复现linux中conic.h中的函数
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos;
	pos.X = (short)x;
	pos.Y = (short)y;
	SetConsoleCursorPosition(handle, pos);
}