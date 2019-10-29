#include <Windows.h>
#include <sstream>
#include <iomanip>
#include <tchar.h>
#include <string>

#include <algorithm>

HWND hWnd;

TCHAR szWndAppName[] = TEXT("201913014정지상");
HINSTANCE g_Instance;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR    lpCmdLine, int       nCmdShow)
{
	// 1. 윈도우 클래스 정의 
	WNDCLASS wc = { 0 };
	wc.style = 0;			   									// 윈도우 클래스의 스타일 종류
	wc.lpfnWndProc = WndProc;									// 메시지 처리 함수인 윈도우 프로시저 포인터	
	wc.cbClsExtra = 0;											// 윈도우 클래스 데이터 예약 영역 초기값 0  
	wc.cbWndExtra = 0;											// 윈도우 데이터 예약 영역, 초기값 0  
	wc.hInstance = hInstance; 									// 해당 윈도우 클래스를 등록한 응용프로그램의 인스턴스
	wc.hIcon = LoadIcon(0, IDI_APPLICATION);					// 윈도우가 최소화 될때 보여줄 아이콘 	
	wc.hCursor = LoadCursor(0, IDC_CROSS);						// 해당 윈도우 클래스로 만들어진 윈도우의 마우스 커서 핸들, IDC_ARROW, IDC_CROSS, IDC_IBEAM, IDC_NO, IDC_WAIT
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);		// 윈도우 작업 영역을 칠할 배경 브러쉬 핸들 (흰색)	
	wc.lpszMenuName = NULL;										// 사용할 메뉴를 지정한다. 	
	wc.lpszClassName = szWndAppName;							// 등록하고자 하는 윈도우 클래스 이름 

	if (!RegisterClass(&wc))
		return -1;

	// 2. 윈도우 생성 
	hWnd = CreateWindow(
		szWndAppName,					    //lpClassName
		szWndAppName, 					    //lpWindowName
		WS_OVERLAPPEDWINDOW ^ (WS_MINIMIZEBOX | WS_MAXIMIZEBOX),			    //dwStyle
		//(WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU  | WS_MINIMIZEBOX | WS_MAXIMIZEBOX ),
		(1920 / 2) - (800 / 2),				//x
		(1080 / 2) - (600 / 2), 			//y
		800, 					            //Width
		600 + 35, 								//Height
		NULL,							    //hWndParent
		NULL,							    //hMenu
		hInstance,						    //hInstance
		NULL);	     					    //lpParam

	g_Instance = hInstance;

	// 3. 윈도우 화면 출력 
	ShowWindow(hWnd, nCmdShow);


	// 4. 메시지 루프 
	MSG Message;
	while (GetMessage(&Message, 0, 0, 0)) //메시지 루프에서 메시지 꺼냄(WM_QUIT이면 FALSE 반환)
	{
		TranslateMessage(&Message);       //WM_KEYDOWN이고 키가 문자 키일 때 WM_CHAR 발생
		DispatchMessage(&Message);        //콜백 프로시저가 수행할 수 있게 디스패치 시킴
	}

	return 0;
}

enum Type
{
	Answer = 1,
	Question = 5,
};

const LPCSTR strings[15]
{
	{"문제1.LPCSTR에서 LPC는 무슨 단어의 약자인가"},
	{"답변1.Long Pointer Constant"}, //
	{"답변2.Linear Predictive Coding"},
	{"답변3.Local Procedure Call"},
	{"답변4.Local Pointer Character"},

	{"문제2. IEEE 802.11에 해당하는 표준 기술은 무엇인가?"},
	{"답변1. LLC"},
	{"답변2. WLAN"}, //
	{"답변3. WMAN"},
	{"답변4. WPAN"},

	{"문제3. 컴퓨터과학에서의 인지 (cognition)에 가장 적합하지 않은것은 무엇인가?"},
	{"답변1. 기억 (memory)"},
	{"답변2. 지각 (perception)"},
	{"답변3. 사고 (thinking)"},
	{"답변4. 추리 (Reasoning)"}, //
};

const LPCSTR result[4]
{
	_T("정답 0 : 다시해주세요"),
	_T("정답 1 : 노력해주세요"),
	_T("정답 2 : 훌륭합니다"),
	_T("정답 3 : 당신은 퀴즈의 신입니다")
};

RECT butPos[4]
{
	RECT{50,400,300,30},
	RECT{400,400,300,30},
	RECT{50,450,300,30},
	RECT{400,450,300,30},
};

#define ID_SUBMIT 120
#define ID_QUESTION 100

HWND hQuestion;
HWND hSubmit;
HWND hAnswer[4];

int QuestionIdx = 0;
int correctCount = 0;
int Answers[3];
int CurrentAnswers[3]{ 1,2,4 };

// 0.1. 윈도우 프로시저 
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_CREATE:
		{
			for (int i = 0; i < 4; ++i)
			{
				int idx = Type::Question*QuestionIdx + i + 1;
				hAnswer[i] = CreateWindow(_T("button"), strings[idx],
					WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
					butPos[i].left, butPos[i].top, butPos[i].right, butPos[i].bottom,
					hWnd, (HMENU)(ID_QUESTION + i + 1), g_Instance, 0);
			}

			hSubmit = CreateWindow(_T("button"), _T("확인"),
				WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
				50, 500, 650, 30,
				hWnd, (HMENU)ID_SUBMIT, g_Instance, 0);

			hQuestion = CreateWindow(_T("static"), strings[Type::Question * QuestionIdx],
				WS_CHILD | WS_VISIBLE | SS_LEFT,
				50, 100, 650, 90,
				hWnd, (HMENU)ID_QUESTION, g_Instance, 0);

			return 0;
		}

		case WM_COMMAND:
		{
			if (HIWORD(wParam) == BN_CLICKED)
			{
				if (LOWORD(wParam) == ID_SUBMIT)
				{
					if (QuestionIdx >= 3)
						return 0;

					//메시지박스 - 정답과 오답 출력. ok버튼 누를 시 다음문제 설정
					if (CurrentAnswers[QuestionIdx] == Answers[QuestionIdx])
					{
						if (MessageBox(hWnd, _T("훌륭합니다"), _T("정답"), MB_OK) == IDOK)
						{
							correctCount++;
							QuestionIdx++;

							if (QuestionIdx < 3)
							{
								SendMessage(hQuestion, WM_SETTEXT, 0, (LPARAM)strings[Type::Question * QuestionIdx]);
								for (int i = 0; i < 4; ++i)
								{
									SendMessage(hAnswer[i], WM_SETTEXT, 0, (LPARAM)strings[Type::Question * QuestionIdx + i + 1]);
								}
							}
							else
							{
								SendMessage(hQuestion, WM_SETTEXT, 0, (LPARAM)result[correctCount]);
								for (int i = 0; i < 4; ++i)
								{
									SendMessage(hAnswer[i], WM_SETTEXT, 0, (LPARAM)("---"));
								}
							}
						}
					}
					else
					{
						if (MessageBox(hWnd, _T("틀렸습니다"), _T("오답"), MB_OK) == IDOK)
						{
							QuestionIdx++;

							if (QuestionIdx < 3)
							{
								SendMessage(hQuestion, WM_SETTEXT, 0, (LPARAM)strings[Type::Question * QuestionIdx]);
								for (int i = 0; i < 4; ++i)
								{
									SendMessage(hAnswer[i], WM_SETTEXT, 0, (LPARAM)strings[Type::Question * QuestionIdx + i + 1]);
								}
							}
							else
							{
								SendMessage(hQuestion, WM_SETTEXT, 0, (LPARAM)result[correctCount]);
								for (int i = 0; i < 4; ++i)
								{
									SendMessage(hAnswer[i], WM_SETTEXT, 0, (LPARAM)("---"));
								}
							}
						}
					}

					return 0;
				}

				Answers[QuestionIdx] = LOWORD(wParam) % 5;
			}
			return 0;
		}
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}

