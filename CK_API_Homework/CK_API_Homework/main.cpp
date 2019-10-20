#include <Windows.h>
#include <sstream>
#include <iomanip>
#include <tchar.h>
#include <string>

#include <algorithm>

COLORREF CurrentColor;

HWND hWnd;

TCHAR szWndAppName[] = TEXT("201913014정지상");

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

void SetTitle(std::wstring str);

void DrawScanline(const POINT& start, const POINT& end, const HDC& hdc);
void DrawTriangle(const POINT& _v1, const POINT& _v2, const POINT& _v3, const HDC& hdc);
void DrawFlatSideTriangle(const POINT& v1, const POINT& v2, const POINT& v3, const HDC& hdc);

#define RECT_TO_INT(rect) rect.left,rect.top,rect.right,rect.bottom

template <typename T> int sign(const T& val) {
	return (T(0) < val) - (val < T(0));
}

template<typename T>
void customSwap(T& a, T& b)
{
	T Temp = a;
	a = b;
	b = Temp;
}


// 0. 윈도우 메인 함수 
// 멀티바이트와 유니코드의 차이는 wWinMain과 WinMain.
// nCmdShow = WinUse.h 371

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
		(1920 / 2) - (520 / 2),				//x
		(1080 / 2) - (660 / 2), 			//y
		520, 					            //Width
		660 + 35, 								//Height
		NULL,							    //hWndParent
		NULL,							    //hMenu
		hInstance,						    //hInstance
		NULL);	     					    //lpParam

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

// 0.1. 윈도우 프로시저 
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;

	static bool isLButtonDown;
	static POINT LastPoint;
	switch (message)
	{
		case WM_PAINT:
		{
			hdc = BeginPaint(hWnd, &ps);

			#pragma region 열지마세요
			//all black setup
			CurrentColor = RGB(33, 33, 33);
			DrawTriangle(POINT{ 0,0 }, POINT{ 515,658 }, POINT{ 515,0}, hdc);
			DrawTriangle(POINT{ 0,0 }, POINT{ 515,658 }, POINT{ 0,658 }, hdc);

			//foot orange setup
			CurrentColor = RGB(200, 155, 0);
			DrawTriangle(POINT{ 337,658 }, POINT{ 329,649 }, POINT{ 459,623 }, hdc);
			DrawTriangle(POINT{ 337,658 }, POINT{ 450,658 }, POINT{ 459,623 }, hdc);


			//little gray setup
			CurrentColor = RGB(167, 167, 167);
			DrawTriangle(POINT{ 335,648 }, POINT{ 361,653 }, POINT{ 400,635 }, hdc);
			DrawTriangle(POINT{ 415,639 }, POINT{ 454,622 }, POINT{ 400,635 }, hdc);
			DrawTriangle(POINT{ 355,648 }, POINT{ 253,537 }, POINT{ 417,410 }, hdc);
			DrawTriangle(POINT{ 355,648 }, POINT{ 515,605 }, POINT{ 461,418 }, hdc);

			DrawTriangle(POINT{ 335,648 }, POINT{ 515,605 }, POINT{ 461,418 }, hdc);
			DrawTriangle(POINT{ 335,648 }, POINT{ 417,411 }, POINT{ 461,418 }, hdc);
			DrawTriangle(POINT{ 335,648 }, POINT{ 310,624 }, POINT{ 253,537 }, hdc);
			DrawTriangle(POINT{ 335,648 }, POINT{ 400,580 }, POINT{ 253,537 }, hdc);

			//Back - BG
			CurrentColor = RGB(165, 221, 220);
			DrawTriangle(POINT{ 0,0 }, POINT{ 273,0 }, POINT{ 0,173 }, hdc);
			DrawTriangle(POINT{ 273,0 }, POINT{ 333,0 }, POINT{ 223,32 }, hdc);
			DrawTriangle(POINT{ 0,173 }, POINT{ 190,53 }, POINT{ 0,305 }, hdc);
			DrawTriangle(POINT{ 129,134 }, POINT{ 112,210 }, POINT{ 112,156 }, hdc);
			DrawTriangle(POINT{ 110,369 }, POINT{ 113,156 }, POINT{ 0,305 }, hdc);
			DrawTriangle(POINT{ 110,369 }, POINT{ 120,539 }, POINT{ 0,305 }, hdc);
			DrawTriangle(POINT{ 0,600 }, POINT{ 120,539 }, POINT{ 0,305 }, hdc);
			DrawTriangle(POINT{ 0,600 }, POINT{ 120,539 }, POINT{ 44,595 }, hdc);

			//head - BG
			DrawTriangle(POINT{ 271,11 }, POINT{ 333,0 }, POINT{ 411,12 }, hdc);
			DrawTriangle(POINT{ 353,0 }, POINT{ 515,53 }, POINT{ 515,0 }, hdc);
			DrawTriangle(POINT{ 271,11 }, POINT{ 333,0 }, POINT{ 515,0 }, hdc);
			DrawTriangle(POINT{ 515,92 }, POINT{ 515,0 }, POINT{ 411,12 }, hdc);
			DrawTriangle(POINT{ 471,54 }, POINT{ 515,136 }, POINT{ 515,92 }, hdc);

			DrawTriangle(POINT{ 425,213 }, POINT{ 421,256 }, POINT{ 431,340 }, hdc);
			DrawTriangle(POINT{ 425,213 }, POINT{ 456,300 }, POINT{ 431,340 }, hdc);
			DrawTriangle(POINT{ 450,347 }, POINT{ 456,300 }, POINT{ 431,340 }, hdc);

			DrawTriangle(POINT{ 515,183 }, POINT{ 481,258 }, POINT{ 515,261 }, hdc);
			DrawTriangle(POINT{ 472,319 }, POINT{ 481,258 }, POINT{ 515,261 }, hdc);
			DrawTriangle(POINT{ 450,347 }, POINT{ 515,367 }, POINT{ 515,261 }, hdc);
			DrawTriangle(POINT{ 515,355 }, POINT{ 468,383 }, POINT{ 515,367 }, hdc);

			//little head - BG
			DrawTriangle(POINT{ 515,345 }, POINT{ 468,383 }, POINT{ 515,419 }, hdc);


			DrawTriangle(POINT{ 461,418 }, POINT{ 468,383 }, POINT{ 515,419 }, hdc);
			DrawTriangle(POINT{ 461,418 }, POINT{ 477,486 }, POINT{ 515,419 }, hdc);
			DrawTriangle(POINT{ 515,660 }, POINT{ 477,486 }, POINT{ 515,419 }, hdc);

			DrawTriangle(POINT{ 515,660 }, POINT{ 477,486 }, POINT{ 469,513 }, hdc);
			DrawTriangle(POINT{ 515,660 }, POINT{ 450,660 }, POINT{ 469,513 }, hdc);

			//little leg - BG
			DrawTriangle(POINT{ 367,650 }, POINT{ 391,634 }, POINT{ 399,637 }, hdc);
			DrawTriangle(POINT{ 367,650 }, POINT{ 392,650 }, POINT{ 399,637 }, hdc);
			DrawTriangle(POINT{ 392,650 }, POINT{ 404,646 }, POINT{ 399,637 }, hdc);
			DrawTriangle(POINT{ 411,656 }, POINT{ 404,646 }, POINT{ 399,648 }, hdc);

			DrawTriangle(POINT{ 457,641 }, POINT{ 449,627 }, POINT{ 428,641 }, hdc);
			DrawTriangle(POINT{ 424,636 }, POINT{ 449,627 }, POINT{ 428,641 }, hdc);


			//tail - BG
			DrawTriangle(POINT{ 0,613 }, POINT{ 30,612 }, POINT{ 0,591 }, hdc);
			DrawTriangle(POINT{ 0,613 }, POINT{ 23,613 }, POINT{ 59,635 }, hdc);
			DrawTriangle(POINT{ 0,634 }, POINT{ 34,658 }, POINT{ 75,622 }, hdc);
			DrawTriangle(POINT{ 0,600 }, POINT{ 85,658 }, POINT{ 0,658 }, hdc);
			DrawTriangle(POINT{ 44,636 }, POINT{ 103,636 }, POINT{ 85,658 }, hdc);

			//Down - BG
			DrawTriangle(POINT{ 0,660 }, POINT{ 173,610 }, POINT{ 289,658 }, hdc);
			DrawTriangle(POINT{ 254,646 }, POINT{ 289,658 }, POINT{ 294,644 }, hdc);
			DrawTriangle(POINT{ 326,660 }, POINT{ 289,658 }, POINT{ 294,644 }, hdc);

			DrawTriangle(POINT{ 457,641 }, POINT{ 449,627 }, POINT{ 469,593 }, hdc);


			//white 
			CurrentColor = RGB(233, 233, 233);
			
			//head
			DrawTriangle(POINT{ 336,77 }, POINT{ 369,71 }, POINT{ 389,108 }, hdc);
			DrawTriangle(POINT{ 409,78 }, POINT{ 369,71 }, POINT{ 389,108 }, hdc);
			DrawTriangle(POINT{ 409,78 }, POINT{ 439,98 }, POINT{ 389,108 }, hdc);
			DrawTriangle(POINT{ 447,116 }, POINT{ 439,98 }, POINT{ 389,108 }, hdc);
			DrawTriangle(POINT{ 447,116 }, POINT{ 422,125 }, POINT{ 389,108 }, hdc);
			DrawTriangle(POINT{ 407,149 }, POINT{ 422,125 }, POINT{ 389,108 }, hdc);

			//body
			DrawTriangle(POINT{ 326,131 }, POINT{ 346,113 }, POINT{ 368,113 }, hdc);
			DrawTriangle(POINT{ 326,131 }, POINT{ 315,190 }, POINT{ 368,113 }, hdc);

			DrawTriangle(POINT{ 405,152 }, POINT{ 315,190 }, POINT{ 368,113 }, hdc);
			DrawTriangle(POINT{ 405,152 }, POINT{ 315,190 }, POINT{ 425,212 }, hdc);

			DrawTriangle(POINT{ 280,271 }, POINT{ 315,190 }, POINT{ 425,212 }, hdc);
			DrawTriangle(POINT{ 280,271 }, POINT{ 414,347 }, POINT{ 425,212 }, hdc);
			DrawTriangle(POINT{ 431,340 }, POINT{ 414,347 }, POINT{ 421,256 }, hdc);

			DrawTriangle(POINT{ 280,271 }, POINT{ 414,347 }, POINT{ 281,401 }, hdc);
			DrawTriangle(POINT{ 382,441 }, POINT{ 392,352 }, POINT{ 281,401 }, hdc);
			DrawTriangle(POINT{ 382,441 }, POINT{ 260,468 }, POINT{ 281,401 }, hdc);

			DrawTriangle(POINT{ 382,441 }, POINT{ 260,468 }, POINT{ 329,504 }, hdc);
			DrawTriangle(POINT{ 213,508 }, POINT{ 260,468 }, POINT{ 329,504 }, hdc);

			DrawTriangle(POINT{ 213,508 }, POINT{ 261,637 }, POINT{ 321,500 }, hdc);
			DrawTriangle(POINT{ 311,622 }, POINT{ 261,637 }, POINT{ 292,543 }, hdc);
			DrawTriangle(POINT{ 311,622 }, POINT{ 261,637 }, POINT{ 291,636 }, hdc);

			DrawTriangle(POINT{ 297,581 }, POINT{ 292,543 }, POINT{ 320,524 }, hdc);
			DrawTriangle(POINT{ 315,502 }, POINT{ 292,543 }, POINT{ 320,524 }, hdc);

			//hand
			DrawTriangle(POINT{ 220,381 }, POINT{ 205,431 }, POINT{ 214,493 }, hdc);
			DrawTriangle(POINT{ 220,381 }, POINT{ 230,429 }, POINT{ 214,493 }, hdc);

			//little-head
			DrawTriangle(POINT{ 417,410 }, POINT{ 461,418 }, POINT{ 468,383 }, hdc);
			DrawTriangle(POINT{ 417,410 }, POINT{ 454,375 }, POINT{ 468,383 }, hdc);
			DrawTriangle(POINT{ 417,410 }, POINT{ 454,375 }, POINT{ 405,386 }, hdc);
			DrawTriangle(POINT{ 412,364 }, POINT{ 454,375 }, POINT{ 405,386 }, hdc);
			DrawTriangle(POINT{ 412,364 }, POINT{ 454,375 }, POINT{ 428,354 }, hdc);
			DrawTriangle(POINT{ 450,358 }, POINT{ 454,375 }, POINT{ 428,354 }, hdc);

			//Orange Bu--ri
			CurrentColor = RGB(255, 127, 39);
			DrawTriangle(POINT{ 422,125 }, POINT{ 447,116 }, POINT{ 459,139 }, hdc);
			DrawTriangle(POINT{ 422,125 }, POINT{ 435,162 }, POINT{ 459,139 }, hdc);
			DrawTriangle(POINT{ 422,125 }, POINT{ 435,162 }, POINT{ 407,149 }, hdc);

			DrawTriangle(POINT{ 430,223 }, POINT{ 461,275 }, POINT{ 455,299 }, hdc);
			DrawTriangle(POINT{ 463,316 }, POINT{ 461,275 }, POINT{ 455,299 }, hdc);
			DrawTriangle(POINT{ 463,316 }, POINT{ 451,345 }, POINT{ 455,299 }, hdc);

#pragma endregion

			EndPaint(hWnd, &ps);
			return 0;
		}

		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}

void SetTitle(std::wstring str)
{
	SetWindowText(hWnd, str.c_str());
}

void DrawScanline(const POINT& start, const POINT& end, const HDC& hdc)
{
	int dx = abs(ceil(end.x - start.x));
	const int signx = sign(end.x - start.x);

	POINT current = start;
	for (int i = 0; i <= dx; ++i)
	{
		//putpixel here
		SetPixel(hdc, current.x, current.y, CurrentColor);
		current.x += signx;
	}
}

void DrawTriangle(const POINT& _v1, const POINT& _v2, const POINT& _v3, const HDC& hdc)
{
	//sort by y axis
	POINT vts[3]{ _v1,_v2,_v3 };
	std::sort(vts, vts + 3, [](const POINT& a, const POINT& b) {return a.y > b.y; });

	if (vts[1].y == vts[2].y)
	{
		DrawFlatSideTriangle(vts[0], vts[1], vts[2], hdc);
	}
	else if (vts[0].y == vts[1].y)
	{
		DrawFlatSideTriangle(vts[2], vts[0], vts[1], hdc);
	}
	else
	{
		float x = vts[0].x - (vts[0].y - vts[1].y) * (vts[0].x - vts[2].x) / (vts[0].y - vts[2].y);
		POINT v4{ round(x),vts[1].y };
		DrawFlatSideTriangle(vts[0], vts[1], v4, hdc); // up-side
		DrawFlatSideTriangle(vts[2], vts[1], v4, hdc); // down-side
	}
}

void DrawFlatSideTriangle(const POINT& v1, const POINT& v2, const POINT& v3, const HDC& hdc)
{
	POINT vt1 = v1;
	POINT vt2 = v1;

	int dx1 = abs(v2.x - v1.x);
	int dy1 = abs(v2.y - v1.y);

	int dx2 = abs(v3.x - v1.x);
	int dy2 = abs(v3.y - v1.y);

	const int signx1 = sign(v2.x - v1.x);
	const int signx2 = sign(v3.x - v1.x);

	const int signy1 = sign(v2.y - v1.y);
	const int signy2 = sign(v3.y - v1.y);

	const bool isChanged1 = dy1 > dx1;
	const bool isChanged2 = dy2 > dx2;

	if (isChanged1)
	{
		customSwap(dx1, dy1);
	}

	if (isChanged2)
	{
		customSwap(dx2, dy2);
	}

	int e1 = 2 * dy1 - dx1;
	int e2 = 2 * dy2 - dx2;

	for (int i = 0; i <= dx1; ++i)
	{
		DrawScanline(vt1, vt2, hdc);
		//edge 1
		while (e1 >= 0)
		{
			if (isChanged1)
				vt1.x += signx1;
			else
				vt1.y += signy1;
			e1 -= 2 * dx1;
		}

		if (isChanged1)
			vt1.y += signy1;
		else
			vt1.x += signx1;

		e1 += 2 * dy1;

		//edge 2
		while (vt2.y != vt1.y)
		{
			while (e2 >= 0)
			{
				if (isChanged2)
					vt2.x += signx2;
				else
					vt2.y += signy2;
				e2 -= 2 * dx2;
			}

			if (isChanged2)
				vt2.y += signy2;
			else
				vt2.x += signx2;

			e2 += 2 * dy2;
		}
	}
}
