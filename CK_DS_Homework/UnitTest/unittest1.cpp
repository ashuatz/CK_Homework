#include "stdafx.h"
#include "CppUnitTest.h"
#include "..\CK_DS_Homework\11week\object.h"
#include <iostream>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{		
	TEST_CLASS(UnitTest1)
	{
	public:
		
		TEST_METHOD(pingpong)
		{
			// TODO: �׽�Ʈ �ڵ带 ���⿡ �Է��մϴ�.
			for (int i = 0; i < 1000; ++i)
			{
				Enemy e;
				auto t = e.pingpong((float)i / 1000.f, 0.5f);
				std::cout << t << " ";
			}
		}

	};
}