#include <Windows.h>
#include <sstream>
#include <iomanip>
#include <tchar.h>
#include <string>

#include <algorithm>

HWND hWnd;

TCHAR szWndAppName[] = TEXT("201913014������");
HINSTANCE g_Instance;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR    lpCmdLine, int       nCmdShow)
{
	// 1. ������ Ŭ���� ���� 
	WNDCLASS wc = { 0 };
	wc.style = 0;			   									// ������ Ŭ������ ��Ÿ�� ����
	wc.lpfnWndProc = WndProc;									// �޽��� ó�� �Լ��� ������ ���ν��� ������	
	wc.cbClsExtra = 0;											// ������ Ŭ���� ������ ���� ���� �ʱⰪ 0  
	wc.cbWndExtra = 0;											// ������ ������ ���� ����, �ʱⰪ 0  
	wc.hInstance = hInstance; 									// �ش� ������ Ŭ������ ����� �������α׷��� �ν��Ͻ�
	wc.hIcon = LoadIcon(0, IDI_APPLICATION);					// �����찡 �ּ�ȭ �ɶ� ������ ������ 	
	wc.hCursor = LoadCursor(0, IDC_CROSS);						// �ش� ������ Ŭ������ ������� �������� ���콺 Ŀ�� �ڵ�, IDC_ARROW, IDC_CROSS, IDC_IBEAM, IDC_NO, IDC_WAIT
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);		// ������ �۾� ������ ĥ�� ��� �귯�� �ڵ� (���)	
	wc.lpszMenuName = NULL;										// ����� �޴��� �����Ѵ�. 	
	wc.lpszClassName = szWndAppName;							// ����ϰ��� �ϴ� ������ Ŭ���� �̸� 

	if (!RegisterClass(&wc))
		return -1;

	// 2. ������ ���� 
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

	// 3. ������ ȭ�� ��� 
	ShowWindow(hWnd, nCmdShow);


	// 4. �޽��� ���� 
	MSG Message;
	while (GetMessage(&Message, 0, 0, 0)) //�޽��� �������� �޽��� ����(WM_QUIT�̸� FALSE ��ȯ)
	{
		TranslateMessage(&Message);       //WM_KEYDOWN�̰� Ű�� ���� Ű�� �� WM_CHAR �߻�
		DispatchMessage(&Message);        //�ݹ� ���ν����� ������ �� �ְ� ����ġ ��Ŵ
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
	{"����1.LPCSTR���� LPC�� ���� �ܾ��� �����ΰ�"},
	{"�亯1.Long Pointer Constant"}, //
	{"�亯2.Linear Predictive Coding"},
	{"�亯3.Local Procedure Call"},
	{"�亯4.Local Pointer Character"},

	{"����2. IEEE 802.11�� �ش��ϴ� ǥ�� ����� �����ΰ�?"},
	{"�亯1. LLC"},
	{"�亯2. WLAN"}, //
	{"�亯3. WMAN"},
	{"�亯4. WPAN"},

	{"����3. ��ǻ�Ͱ��п����� ���� (cognition)�� ���� �������� �������� �����ΰ�?"},
	{"�亯1. ��� (memory)"},
	{"�亯2. ���� (perception)"},
	{"�亯3. ��� (thinking)"},
	{"�亯4. �߸� (Reasoning)"}, //
};

const LPCSTR result[4]
{
	_T("���� 0 : �ٽ����ּ���"),
	_T("���� 1 : ������ּ���"),
	_T("���� 2 : �Ǹ��մϴ�"),
	_T("���� 3 : ����� ������ ���Դϴ�")
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

// 0.1. ������ ���ν��� 
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

			hSubmit = CreateWindow(_T("button"), _T("Ȯ��"),
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

					//�޽����ڽ� - ����� ���� ���. ok��ư ���� �� �������� ����
					if (CurrentAnswers[QuestionIdx] == Answers[QuestionIdx])
					{
						if (MessageBox(hWnd, _T("�Ǹ��մϴ�"), _T("����"), MB_OK) == IDOK)
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
						if (MessageBox(hWnd, _T("Ʋ�Ƚ��ϴ�"), _T("����"), MB_OK) == IDOK)
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

