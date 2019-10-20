#pragma once
#include <iostream>
using namespace std;

class InputModule
{
	int Capacity;
	int Count;
	char** caches;

	void Resize();

public:
	InputModule() : Capacity(2), Count(0), caches(nullptr)
	{
		caches = new char*[Capacity];
	};

	const char* MakeString(const char* inputDesc);

	const int MakeInt(const char * inputDesc);

	void Clear();

	~InputModule()
	{
		Clear();
		delete[] caches;
	}
};

