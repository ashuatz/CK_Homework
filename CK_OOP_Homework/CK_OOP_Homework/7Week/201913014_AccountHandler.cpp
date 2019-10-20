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

void AccountHandler::Deposit(InputModule& input) const
{
	auto&& accountID = input.MakeInt("AccountID");
	auto&& balance = input.MakeInt("Balance");

	auto* targetAccount = Find(accountID);
	if (targetAccount == nullptr)
	{
		cout << "���¸� ã�� ���߽��ϴ�." << endl;
		return;
	}

	targetAccount->Deposit(balance);
	cout << "�Ա� ����. ���� �ܾ� : " << targetAccount->GetBalance() << endl;
}

void AccountHandler::ShowAccountInfoByAccountID(InputModule& input) const
{
	auto&& accountID = input.MakeInt("AccountID");

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

void AccountHandler::ShowAccountInfoByName(InputModule& input) const
{
	auto&& name = input.MakeString("Name");

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
}

void AccountHandler::ShowAllAccountInfo() const
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