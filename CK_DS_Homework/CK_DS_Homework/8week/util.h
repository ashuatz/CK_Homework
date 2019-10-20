#pragma once

#define ENTER	13
#define LEFT    75
#define RIGHT   77
#define UP		72
#define DOWN	80
#define SPACE	32

void gotoxy(int x, int y);

void ScreenInit();
void ScreenFlipping();
void ScreenClear();
void ScreenRelease();

void SetCursorReset();
void ScreenPrint(const char *string);
void ScreenPrint(int x, int y, const char* string);
void AppendScreenPrint(const char *string);
void SetColor(unsigned short color);
