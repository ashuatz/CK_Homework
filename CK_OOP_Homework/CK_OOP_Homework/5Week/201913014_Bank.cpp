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

void Bank::RegisterAccount(const int& balance, const char*&& name, const int& accountID)
{
	if (Count >= Capacity)
	{
		Resize();
	}

	RegisteredAccounts[Count] = new Account(accountID, name, balance);
	Count++;
}

void Bank::WithDraw(const int& balance, const int& accountID) const
{
	auto* targetAccount = Find(accountID);
	if (targetAccount == nullptr)
	{
		cout << "���¸� ã�� ���߽��ϴ�." << endl;
		return;
	}

	if (!targetAccount->TryWithDraw(balance))
	{
		cout << "��� ����. ���� �ܾ� : " << targetAccount->GetBalance() << endl;
		return;
	}

	cout << "��� ����. ���� �ܾ� : " << targetAccount->GetBalance() << endl;
}

void Bank::Deposit(const int& balance, const int& accountID) const
{
	auto* targetAccount = Find(accountID);
	if (targetAccount == nullptr)
	{
		cout << "���¸� ã�� ���߽��ϴ�." << endl;
		return;
	}

	targetAccount->Deposit(balance);
	cout << "�Ա� ����. ���� �ܾ� : " << targetAccount->GetBalance() << endl;
}

void Bank::ShowAccountInfoByAccountID(const int& accountID) const
{
	auto* targetAccount = Find(accountID);
	if (targetAccount == nullptr)
	{
		cout << "���¸� ã�� ���߽��ϴ�." << endl;
		return;
	}

	cout << endl;
	cout << "[��������]" << endl;
	cout << "������ : " << targetAccount->GetName() << endl;
	cout << "���¹�ȣ : " << targetAccount->GetAccountID() << endl;
	cout << "�����ܰ� : " << targetAccount->GetBalance() << endl << endl;
}

void Bank::ShowAccountInfoByName(const char* name) const
{
	auto* targetAccount = Find(name);
	if (targetAccount == nullptr)
	{
		cout << "���¸� ã�� ���߽��ϴ�." << endl;
		return;
	}

	cout << endl;
	cout << "[��������]" << endl;
	cout << "������ : " << targetAccount->GetName() << endl;
	cout << "���¹�ȣ : " << targetAccount->GetAccountID() << endl;
	cout << "�����ܰ� : " << targetAccount->GetBalance() << endl << endl;
	delete[] name;
}

void Bank::ShowAllAccountInfo() const
{
	for (int i = 0; i < Count; ++i)
	{
		auto* targetAccount = RegisteredAccounts[i];

		cout << endl;
		cout << "[��������]" << endl;
		cout << "������ : " << targetAccount->GetName() << endl;
		cout << "���¹�ȣ : " << targetAccount->GetAccountID() << endl;
		cout << "�����ܰ� : " << targetAccount->GetBalance() << endl << endl;
	}
}