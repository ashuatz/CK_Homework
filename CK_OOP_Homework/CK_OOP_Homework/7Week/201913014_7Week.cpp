#include <iostream>
#include "201913014_Account.h"
#include "201913014_AccountHandler.h"

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

int main()
{
	AccountHandler handler;
	InputModule module;
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
			case 1:	handler.RegisterAccount(module);	break;
			case 2:	handler.Deposit(module);break;
			case 3:	handler.WithDraw(module);break;
			case 4:	handler.ShowAllAccountInfo();break;
			case 5:	handler.ShowAccountInfoByAccountID(module);break;
			case 6:	handler.ShowAccountInfoByName(module);break;
			default:
				break;
		}
		module.Clear();
	}
}