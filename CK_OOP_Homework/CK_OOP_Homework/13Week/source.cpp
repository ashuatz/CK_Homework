#include "list.h"
using namespace std;

int main()
{
	LinkedList<int> list;
	for (int i = 1; i <= 10; i++)
		list.insert(i, new int(i*i));

	cout << list << endl;

	auto* temp = list.remove([](int* t) {return *t == 64; });
	if (temp != nullptr)
		delete temp;

	temp = list.remove([](int* t) {return *t == 16; });
	if (temp != nullptr)
		delete temp;

	cout << list << endl;

	int size = list.Count();
	auto data = list.removeAll(list.Count());
	for (int i = 0; i < size; ++i)
	{
		delete data[i];
	}

	delete[] data;

	return 0;
}