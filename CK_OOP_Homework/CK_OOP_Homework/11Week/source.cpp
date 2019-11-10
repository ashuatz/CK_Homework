#include <iostream>
#include "police.h"
using namespace std;

int main()
{
	Police* pman1 = new Police(5, 3);
	Police pman2 = *pman1;
	pman2.PutHandcuff();
	pman2.Shot();

	Police pman3(2, 4);
	pman3 = *pman1;

	delete pman1;

	pman3.PutHandcuff();
	pman3.Shot();

	return 0;
}