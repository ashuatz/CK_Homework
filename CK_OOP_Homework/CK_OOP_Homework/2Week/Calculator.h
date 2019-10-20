#pragma once
/*
201913014 ������ ��ü���� ����
2���� 3�� - ����

*���� ���� ���� ������

*������ �켱������ �������� ���Ͽ����ϴ�.
*BIC ��縦 ������ �ö󰡸� ���� �����ð��� �ѱ� �ð��̶�....
*������ �ð��� �ȴٸ� �������� ��ȯ �� ������ �̿��Ͽ� ������ �켱���� �� ��ȣ���� ó���� �� �� ���� �� �����ϴ�.
*/
#include <cstdlib>

class Calculator
{
private:
	float Add(const float& rhs, const float& lhs);
	float Remove(const float& rhs, const float& lhs);
	float Multi(const float& rhs, const float& lhs);
	float Divide(const float& rhs, const float& lhs);

	float (Calculator::*current)(const float& rhs, const float& lhs);


	int GetLength(float value) const;

public:
	float Calculate(char* expression);
};