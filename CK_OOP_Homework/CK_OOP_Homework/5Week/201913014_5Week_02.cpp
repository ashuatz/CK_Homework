#include <iostream>
#include "201913014_Account.h"
#include "201913014_Bank.h"

using namespace std;
/*

2) Banking System 0.1 구현
*4주차 강의자료에 문제 설명이 있습니다.

은행계좌 관리 프로그램을 구현하려고 한다. 구현할 기능은 다음과 같다.
• 기능1. 계좌 개설
• 기능2. 입금
• 기능3. 출금
• 기능4. 전체고객 잔액조회

단, 아래의 가정을 포함한다.
• 고객정보는 계좌번호, 이름, 잔액이다.
• 계좌번호는 중복되지 않는다.(중복검사는 하지 않는다.)
• 입금 및 출금액은 무조건 0보다 크다.(입력값의 오류검사는 필요없다.)
• 고객의 계좌정보는 계좌번호, 이름, 잔액만 저장/관리한다.
• 고객관리를 위해 배열을 사용한다.(고객은 100명을 넘지 않는다.)
• 계좌번호는 정수이다.
• 전체고객의 잔액조회를 선택하면 고객 전체의 계좌번호, 이름, 잔액이 출력된다.
Account 클래스를 정의하시오.
• 고객이름은 문자열포인터로 선언하고 동적할당의 형태로 구현하라.

*/

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
			case 1:	myBank.RegisterAccount(InputBalance(), InputAndMakeName(), InputAccountID());	break;
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