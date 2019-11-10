#pragma once
#include "pch.h"

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
const T pingpong(const T t, const T length)
{
	//Rounding
	const T k = t - ((int)(t / (2 * length)) * (2 * length));

	//clamp
	return k < length ? k : 2 * length - k;
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