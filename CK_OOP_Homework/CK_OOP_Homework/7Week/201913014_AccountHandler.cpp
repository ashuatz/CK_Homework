#include "201913014_AccountHandler.h"
#include <iostream>

void AccountHandler::Resize()
{
	Capacity *= 2;
	auto* temp = RegisteredAccounts;
	RegisteredAccounts = new Account*[Capacity];
	for (int i = 0; i < Count; ++i)
	{
		RegisteredAccounts[i] = temp[i];
	}
	delete[] temp;
}

Account * AccountHandler::Find(const int& accountID) const
{
	for (int i = 0; i < Count; ++i)
	{
		if (RegisteredAccounts[i]->GetAccountID() == accountID)
		{
			return RegisteredAccounts[i];
		}
	}

	return nullptr;
}

Account * AccountHandler::Find(const char* name) const
{
	for (int i = 0; i < Count; ++i)
	{
		if (strcmp(RegisteredAccounts[i]->GetName(), name) == 0)
		{
			return RegisteredAccounts[i];
		}
	}

	return nullptr;
}

void AccountHandler::RegisterAccount(InputModule& input)
{
	if (Count >= Capacity)
	{
		Resize();
	}

	auto&& accountID = input.MakeInt("AccountID");
	auto&& name = input.MakeString("Name");
	auto&& balance = input.MakeInt("Balance");

	RegisteredAccounts[Count] = new Account(accountID, name, balance);
	Count++;
}

void AccountHandler::WithDraw(InputModule& input) const
{
	auto&& accountID = input.MakeInt("AccountID");
	auto&& balance = input.MakeInt("Balance");

	auto* targetAccount = Find(accountID);
	if (targetAccount == nullptr)
	{
		cout << "계좌를 찾지 못했습니다." << endl;
		return;
	}

	if (!targetAccount->TryWithDraw(balance))
	{
		cout << "출금 실패. 현재 잔액 : " << targetAccount->GetBalance() << endl;
		return;
	}

	cout << "출금 성공. 남은 잔액 : " << targetAccount->GetBalance() << endl;
}

void AccountHandler::Deposit(InputModule& input) const
{
	auto&& accountID = input.MakeInt("AccountID");
	auto&& balance = input.MakeInt("Balance");

	auto* targetAccount = Find(accountID);
	if (targetAccount == nullptr)
	{
		cout << "계좌를 찾지 못했습니다." << endl;
		return;
	}

	targetAccount->Deposit(balance);
	cout << "입금 성공. 현재 잔액 : " << targetAccount->GetBalance() << endl;
}

void AccountHandler::ShowAccountInfoByAccountID(InputModule& input) const
{
	auto&& accountID = input.MakeInt("AccountID");

	auto* targetAccount = Find(accountID);
	if (targetAccount == nullptr)
	{
		cout << "계좌를 찾지 못했습니다." << endl;
		return;
	}

	cout << endl;
	cout << "[계좌정보]" << endl;
	cout << "예금주 : " << targetAccount->GetName() << endl;
	cout << "계좌번호 : " << targetAccount->GetAccountID() << endl;
	cout << "계좌잔고 : " << targetAccount->GetBalance() << endl << endl;
}

void AccountHandler::ShowAccountInfoByName(InputModule& input) const
{
	auto&& name = input.MakeString("Name");

	auto* targetAccount = Find(name);
	if (targetAccount == nullptr)
	{
		cout << "계좌를 찾지 못했습니다." << endl;
		return;
	}

	cout << endl;
	cout << "[계좌정보]" << endl;
	cout << "예금주 : " << targetAccount->GetName() << endl;
	cout << "계좌번호 : " << targetAccount->GetAccountID() << endl;
	cout << "계좌잔고 : " << targetAccount->GetBalance() << endl << endl;
}

void AccountHandler::ShowAllAccountInfo() const
{
	for (int i = 0; i < Count; ++i)
	{
		auto* targetAccount = RegisteredAccounts[i];

		cout << endl;
		cout << "[계좌정보]" << endl;
		cout << "예금주 : " << targetAccount->GetName() << endl;
		cout << "계좌번호 : " << targetAccount->GetAccountID() << endl;
		cout << "계좌잔고 : " << targetAccount->GetBalance() << endl << endl;
	}
}