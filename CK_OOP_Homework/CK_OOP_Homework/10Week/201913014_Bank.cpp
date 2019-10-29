#include "201913014_Bank.h"
#include <iostream>

void Bank::Resize()
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

Account * Bank::Find(const int& accountID) const
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

Account * Bank::Find(const char* name) const
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

void Bank::ShowInfo(Account * const target) const
{
	HighCreditAccount* account = dynamic_cast<HighCreditAccount*>(target);
	if (account != nullptr)
	{
		cout << endl;
		cout << "[계좌정보]" << endl;
		cout << "계좌종류 : 신용계좌" << endl;
		cout << "신용등급 : " << account->GetCreditGrade() << endl;
		cout << "예금주 : " << account->GetName() << endl;
		cout << "계좌번호 : " << account->GetAccountID() << endl;
		cout << "계좌잔고 : " << account->GetBalance() << endl << endl;
	}
	else
	{
		cout << endl;
		cout << "[계좌정보]" << endl;
		cout << "계좌종류 : 일반계좌" << endl;
		cout << "예금주 : " << target->GetName() << endl;
		cout << "계좌번호 : " << target->GetAccountID() << endl;
		cout << "계좌잔고 : " << target->GetBalance() << endl << endl;
	}
}

//public

void Bank::RegisterAccount(const int& balance, const char*&& name, const int& accountID)
{
	if (Count >= Capacity)
	{
		Resize();
	}

	RegisteredAccounts[Count] = new Account(accountID, name, balance, defaultInterestRate);
	Count++;
}

void Bank::RegisterAccount(const char& grade, const int& balance, const char*&& name, const int& accountID)
{
	if (Count >= Capacity)
	{
		Resize();
	}

	RegisteredAccounts[Count] = new HighCreditAccount(accountID, name, balance, grade, defaultInterestRate);
	Count++;
}

void Bank::WithDraw(const int& balance, const int& accountID) const
{
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

void Bank::Deposit(const int& balance, const int& accountID) const
{
	auto* targetAccount = Find(accountID);
	if (targetAccount == nullptr)
	{
		cout << "계좌를 찾지 못했습니다." << endl;
		return;
	}

	targetAccount->Deposit(balance);
	cout << "입금 성공. 현재 잔액 : " << targetAccount->GetBalance() << endl;
}

void Bank::ShowAccountInfoByAccountID(const int& accountID) const
{
	auto* targetAccount = Find(accountID);
	if (targetAccount == nullptr)
	{
		cout << "계좌를 찾지 못했습니다." << endl;
		return;
	}

	ShowInfo(targetAccount);
}

void Bank::ShowAccountInfoByName(const char* name) const
{
	auto* targetAccount = Find(name);
	if (targetAccount == nullptr)
	{
		cout << "계좌를 찾지 못했습니다." << endl;
		return;
	}

	ShowInfo(targetAccount);
	delete[] name;
}

void Bank::ShowAllAccountInfo() const
{
	for (int i = 0; i < Count; ++i)
	{
		auto* targetAccount = RegisteredAccounts[i];

		ShowInfo(targetAccount);
	}
}