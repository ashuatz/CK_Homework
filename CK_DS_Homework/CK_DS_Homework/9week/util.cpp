#include "pch.h"
#include "util.h"

static int g_nScreenIndex;
static HANDLE g_hScreen[2];

void gotoxy(const int& x, const int& y)
{
	COORD CursorPosition = {x, y};
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), CursorPosition);
}

void ScreenInit()
{
     CONSOLE_CURSOR_INFO cci;
        
     // 화면 버퍼 2개를 만든다.
     g_hScreen[0] = CreateConsoleScreenBuffer( GENERIC_READ | GENERIC_WRITE, 0,
                                            NULL, CONSOLE_TEXTMODE_BUFFER, NULL );
     g_hScreen[1] = CreateConsoleScreenBuffer( GENERIC_READ | GENERIC_WRITE, 0,
                                            NULL, CONSOLE_TEXTMODE_BUFFER, NULL );
 
     // 커서 숨기기
     cci.dwSize = 1;
     cci.bVisible = FALSE;
     SetConsoleCursorInfo( g_hScreen[0], &cci );
     SetConsoleCursorInfo( g_hScreen[1], &cci );
}
 
void ScreenFlipping()
{               
     SetConsoleActiveScreenBuffer( g_hScreen[g_nScreenIndex] );    
     g_nScreenIndex = !g_nScreenIndex; 
}
 
void ScreenClear()
{               
     COORD Coor = { 0, 0 };
     DWORD dw;
     FillConsoleOutputCharacter( g_hScreen[g_nScreenIndex], ' ', 80*25, Coor, &dw );
}
 
void ScreenRelease()
{
     CloseHandle( g_hScreen[0] );
     CloseHandle( g_hScreen[1] );
}

void SetCursorReset()
{
	COORD CursorPosition{ 0,0 };
	SetConsoleCursorPosition(g_hScreen[g_nScreenIndex], CursorPosition);
}

void ScreenPrint(const char *string)
{
	DWORD dw;
	WriteFile(g_hScreen[g_nScreenIndex], string, strlen(string), &dw, NULL);
}

void ScreenPrint(const int& x, const int& y, const char *string )
{
     DWORD dw;
     COORD CursorPosition = { x, y };
     SetConsoleCursorPosition( g_hScreen[g_nScreenIndex], CursorPosition );    
     WriteFile( g_hScreen[g_nScreenIndex], string, strlen( string ), &dw, NULL );
}

void AppendScreenPrint(const char *string)
{
	DWORD dw;
	WriteFile(g_hScreen[!g_nScreenIndex], string, strlen(string), &dw, NULL);
}

// 1 ~ 15 까지 색상 설정 가능
void SetColor( unsigned short color )
{               
     SetConsoleTextAttribute( g_hScreen[g_nScreenIndex], color );
}

//DO NOT use in List-Homework

//template <typename T>
//int CustomRemove(std::vector<T> target, const T& rhs)
//{
//	auto lastSize = target.size();
//	auto end = target.erase(std::remove_if(target.begin(), target.end(),
//		[=](const T& t)
//	{
//		return t == rhs;
//	}), target.end());
//
//	return (lastSize - std::distance(target.begin(), end)) / sizeof(T);
//}