#include<iostream>
#include "201913014_Account.h"
#include "201913014_Bank.h"

using namespace std;

void PrintMenu()
{
	cout << "-----Menu----			" << endl;
	cout << "1.계좌개설				" << endl;
	cout << "2.입금					" << endl;
	cout << "3.출금					" << endl;
	cout << "4.계좌정보전체출력		" << endl;
	cout << "5.특정계좌정보출력 : 계좌번호" << endl;
	cout << "6.특정계좌정보출력 : 이름" << endl;
	cout << endl;
	cout << "0.프로그램종료			" << endl;
	cout << "-------------			" << endl;
}

void PrintInnerMenu()
{
	cout << "[계좌개설]			" << endl;
	cout << "1.보통예금 계좌	2.신용계좌 : " << endl;
}

const char* InputAndMakeName()
{
	char temp[100];
	cout << "Input Name : ";
	cin >> temp;

	char* ret = new char[strlen(temp) + 1];
	strcpy_s(ret, strlen(temp) + 1, temp);
	return ret;
}

const int InputAccountID()
{
	int value;
	cout << "Input AccountID : ";
	cin >> value;
	return value;
}

const int InputBalance()
{
	int value;
	cout << "Input Balance : ";
	cin >> value;
	return value;
}

const char InputGrade()
{
	char value;
	cout << "input Credit Grade : ";
	cin >> value;
	return value;
}

int main()
{
	Bank myBank;
	int selector = -1;
	while (selector != 0)
	{
		PrintMenu();
		cout << "input :";
		cin >> selector;
		cin.clear();
		//system("cls");

		switch (selector)
		{
			case 1:	
			{
				int selector_inner;
				PrintInnerMenu();
				cin >> selector_inner;

				switch (selector_inner)
				{
					case 1:myBank.RegisterAccount(InputBalance(), InputAndMakeName(), InputAccountID()); break;
					case 2:myBank.RegisterAccount(InputGrade(), InputBalance(), InputAndMakeName(), InputAccountID()); break;
					default:
						break;
				}
				break;
			}
			case 2:	myBank.Deposit(InputBalance(), InputAccountID());								break;
			case 3:	myBank.WithDraw(InputBalance(), InputAccountID());								break;
			case 4:	myBank.ShowAllAccountInfo();													break;
			case 5:	myBank.ShowAccountInfoByAccountID(InputAccountID());							break;
			case 6:	myBank.ShowAccountInfoByName(InputAndMakeName());								break;
			default:
				break;
		}
	}
}