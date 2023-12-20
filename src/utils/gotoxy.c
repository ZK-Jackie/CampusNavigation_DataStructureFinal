#include <windows.h>

void gotoxy(int x, int y){	//移动dos中的光标，便于打印，减少卡顿和屏闪
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos;
	pos.X = (short)x;
	pos.Y = (short)y;
	SetConsoleCursorPosition(handle, pos);
}