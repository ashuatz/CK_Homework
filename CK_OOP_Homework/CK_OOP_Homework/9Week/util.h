#pragma once
#include <vector>

#define ENTER	13
#define LEFT    75
#define RIGHT   77
#define UP		72
#define DOWN	80
#define SPACE	32
#define VK 224

#define PointToInt(point) point.x,point.y

void gotoxy(const int& x, const int& y);

void ScreenInit();
void ScreenFlipping();
void ScreenClear();
void ScreenRelease();

void SetCursorReset();
void ScreenPrint(const char *string);
void ScreenPrint(const int& x, const int& y, const char* string);

void AppendScreenPrint(const char *string);
void SetColor(unsigned short color);

template <typename T>
int CustomRemove(std::vector<T> target, const T& rhs);