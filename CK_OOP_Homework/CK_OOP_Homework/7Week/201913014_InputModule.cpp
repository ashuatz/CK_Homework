#include "201913014_InputModule.h"

void InputModule::Resize()
{
	Capacity *= 2;
	auto temp = caches;
	caches = new char*[Capacity];
	for (int i = 0; i < Count; ++i)
	{
		caches[i] = temp[i];
	}
	delete[] temp;
}

const char * InputModule::MakeString(const char * inputDesc)
{
	char temp[100];
	cout << inputDesc << " : ";
	cin >> temp;

	if (Count >= Capacity)
		Resize();

	caches[Count] = new char[strlen(temp) + 1];
	strcpy_s(caches[Count], strlen(temp) + 1, temp);

	return caches[Count++];
}

const int InputModule::MakeInt(const char * inputDesc)
{
	int value;
	cout << inputDesc << " : ";
	cin >> value;
	return value;
}

void InputModule::Clear()
{
	for (int i = 0; i < Count; ++i)
	{
		if (caches[i] != nullptr)
			delete[] caches[i];

		caches[i] = nullptr;
	}

	Count = 0;
}
