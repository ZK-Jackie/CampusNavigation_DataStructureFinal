#include <windows.h>
#include <stdbool.h>

void setCursorVisible(bool isVisible){	//隐藏光标
	CONSOLE_CURSOR_INFO cursor_Info = {1,isVisible};
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_Info);
}
