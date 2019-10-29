#pragma once
#include <string>
using namespace std;

class Account
{
private:
	int accountID;
	char* name;
	int balance;
	float interestRate;

protected:
	virtual const float GetInterestRate() const;

public:
	Account(const int& id, const char* name, const int&  balance, const float& interestRate);
	Account(const Account& copy);

	const char* GetName() const { return name; }
	const int GetAccountID() const { return accountID; }
	const int GetBalance() const { return balance; }

	bool TryWithDraw(const int&  value);
	void Deposit(const int&  value);

	~Account()
	{
		delete[] name;
	}
};

class HighCreditAccount : public Account
{
private:
	char credit;
protected:

	const float GetInterestRate() const override;
public:
	HighCreditAccount(const int & id, const char * name, const int & balance, const char & credit, const float & interestRate);
	const char GetCreditGrade() const { return credit; }
};