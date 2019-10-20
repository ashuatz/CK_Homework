#pragma once
/*
201913014 정지상 객체지향 과제
2주차 3번 - 계산기

*덧셈 뺄셈 곱셉 나눗셈

*연산자 우선순위는 구현하지 못하였습니다.
*BIC 행사를 끝내고 올라가면 과제 마감시간을 넘긴 시각이라....
*다음에 시간이 된다면 후위연산 변환 및 스택을 이용하여 연산자 우선순위 및 괄호연산 처리를 할 수 있을 것 같습니다.
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