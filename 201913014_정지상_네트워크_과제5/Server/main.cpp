//일부 문자열 문제로 인해 해당 파일은 UTF8 with BOM으로 작성되어있습니다.
//인코딩에 유의해주세요

#include <stdio.h> 
#include <stdlib.h>
#include <winsock2.h> 
#pragma comment(lib, "ws2_32")

#define SERVERPORT 3317
#define BUFSIZE    512 

// 윈도우 프로시저 
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// WINAPI상에 텍스트를 렌더링하기 위한 편집 컨트롤 출력 함수
// ...은 stdarg에 존재하는 가변인자(printf등에서 자주 볼 수 있는)
//format을 받고 이후 format내부에 있는 포맷형식문자열(%d 등)의 갯수만큼 arg에 넣어줘야함
void DisplayText(char *fmt, ...);

// 오류 출력 함수 
void err_quit(char *msg);
void err_display(char *msg);

// 소켓 통신 스레드 함수
DWORD WINAPI ServerMain(LPVOID arg);
DWORD WINAPI ProcessClient(LPVOID arg);

// 인스턴스 핸들
//CreateWindow를 위한 인스턴트 정보 저장용 전역 변수
HINSTANCE hInst;

// 편집 컨트롤
// 메시지를 전달하여 텍스트를 렌더링 할 윈도우 핸들
HWND hEdit;


// 임계 영역 
// 다중스레드 작업에서의 문제점을 회피하기 위한 임계영역
CRITICAL_SECTION cs;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	hInst = hInstance;
	InitializeCriticalSection(&cs);

	// 윈도우 클래스 등록
	WNDCLASS wndclass;
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = WndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;

	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = "MyWndClass";

	if (!RegisterClass(&wndclass))
		return 1;


	// 윈도우 생성  
	HWND hWnd = CreateWindow(
		"MyWndClass", "TCP 서버", WS_OVERLAPPEDWINDOW,
		0, 0, 600, 200,
		NULL, NULL, hInstance, NULL);

	if (hWnd == NULL)
		return 1;

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	// 소켓 통신 스레드 생성  
	CreateThread(NULL, 0, ServerMain, NULL, 0, NULL);

	// 메시지 루프 
	MSG msg;
	while (GetMessage(&msg, 0, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	DeleteCriticalSection(&cs);
	return msg.wParam;

}

// 윈도우 프로시저 
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		//생성될 때 실제 메시지가 렌더링될 외부수정불가 Edit contol 로 이루어진 윈도우 생성
		case WM_CREATE:
			hEdit = CreateWindow("edit", NULL,
				//주요 특성은 자동스크롤(상하,좌우), 채팅이 쌓이기 위한 멀티라인, 외부에서 건드릴 수 없도록 읽기전용 속성을 사용
				WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL | ES_AUTOHSCROLL | ES_AUTOVSCROLL | ES_MULTILINE | ES_READONLY,
				0, 0, 0, 0, hWnd, (HMENU)100, hInst, NULL);
			return 0;

		case WM_SIZE:
			MoveWindow(hEdit, 0, 0, LOWORD(lParam), HIWORD(lParam), TRUE);
			return 0;

		case WM_SETFOCUS:
			SetFocus(hEdit);
			return 0;

		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

// WINAPI상에 텍스트를 렌더링하기 위한 편집 컨트롤 출력 함수
// ...은 stdarg에 존재하는 가변인자 함수(printf등에서 자주 볼 수 있는)
//format을 받고 이후 format내부에 있는 포맷형식문자열(%d 등)의 갯수만큼 arg에 넣어줘야함
void DisplayText(char *fmt, ...)
{
	va_list arg;
	va_start(arg, fmt);
	char cbuf[BUFSIZE + 256];
	vsprintf(cbuf, fmt, arg);

	//공유자원에 액세스 할 때, 겹쳐서 문제를 발생하는것을 막기 위한 임계영역 설정.
	EnterCriticalSection(&cs);

	int nLength = GetWindowTextLength(hEdit);
	SendMessage(hEdit, EM_SETSEL, nLength, nLength);
	SendMessage(hEdit, EM_REPLACESEL, FALSE, (LPARAM)cbuf);

	LeaveCriticalSection(&cs);
	va_end(arg);
}


// 소켓 함수 오류 출력 후 종료
void err_quit(char *msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL,
		WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);

	MessageBox(NULL, (LPCTSTR)lpMsgBuf, msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(1);
}

// 소켓 함수 오류 출력
void err_display(char *msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL,
		WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);

	DisplayText("[%s] %s", msg, (char *)lpMsgBuf);
	LocalFree(lpMsgBuf);
}

// TCP 서버 시작 부분
DWORD WINAPI ServerMain(LPVOID arg)
{
	int retval;

	// 윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		return 1;
	}

	// socket() 
	// 클라이언트의 접속연결을 듣기 위한 소켓 생성
	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sock == INVALID_SOCKET)
	{
		err_quit("socket()");
	}

	// bind()  
	// 서버주소를 설정(INET,IPv4)해서 해당 주소로 바인딩.
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = bind(listen_sock, (SOCKADDR *)&serveraddr, sizeof(serveraddr));

	if (retval == SOCKET_ERROR) err_quit("bind()");

	// listen() 
	// 클라이언트 접속 요청이 올때까지 해당 쓰레드 대기(비동기가 아니므로)
	retval = listen(listen_sock, SOMAXCONN);
	if (retval == SOCKET_ERROR)
	{
		err_quit("listen()");
	}

	// 데이터 통신에 사용할 변수
	// 클라이언트 소켓,
	SOCKET client_sock;
	//클라이언트 주소,
	SOCKADDR_IN clientaddr;
	//클라이언트 주소객체의 사이즈,
	int addrlen;
	//해당클라이언트와 통신할 쓰레드의 핸들
	//다중접속을 위해서라면, 핸들을 하나만 소지하면 안됨. 연결된 클라이언트 수만큼 필요.
	//또한 매번 생성하는것이 아닌 쓰레드풀링을 이용해 성능향상을 꾀할 수 있음
	HANDLE hThread;

	while (1)
	{
		// accept()  
		// 연결요청한 클라이언트의 정보를 토대로 소켓 생성.
		addrlen = sizeof(clientaddr);
		client_sock = accept(listen_sock, (SOCKADDR *)&clientaddr, &addrlen);
		if (client_sock == INVALID_SOCKET)
		{
			err_display("accept()");
			break;
		}

		// 접속한 클라이언트 정보 출력   
		DisplayText("\r\n[TCP 서버] 클라이언트 접속: IP 주소=%s, 포트 번호=%d\r\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

		// 스레드 생성  
		// ProcessClient 함수를 실행할 때, client_sock를 넘기며 쓰레드 생성
		hThread = CreateThread(NULL, 0, ProcessClient, (LPVOID)client_sock, 0, NULL);

		if (hThread == NULL) { closesocket(client_sock); }
		else { CloseHandle(hThread); }
	}

	// closesocket()  
	closesocket(listen_sock);

	// 윈속 종료  
	WSACleanup();

	return 0;
}

// 클라이언트와 데이터 통신 
// 메인쓰레드가 아닌 타 쓰레드에서 동작
DWORD WINAPI ProcessClient(LPVOID arg)
{
	SOCKET client_sock = (SOCKET)arg;
	int retval;
	SOCKADDR_IN clientaddr;
	int addrlen;
	char buf[BUFSIZE + 1];

	// 클라이언트 정보 얻기  
	addrlen = sizeof(clientaddr);

	//socket descriptor에서 연결된 상대의 주소를 획득
	getpeername(client_sock, (SOCKADDR *)&clientaddr, &addrlen);

	while (1)
	{
		// 데이터 받기   
		retval = recv(client_sock, buf, BUFSIZE, 0);
		if (retval == SOCKET_ERROR)
		{
			err_display("recv()");
			break;
		}
		else if (retval == 0)
		{
			break;
		}

		// 받은 데이터 출력  
		// 버퍼의 마지막위치에 종료 문자열 추가(0)
		buf[retval] = '\0';

		//edit으로 출력
		DisplayText("[TCP/%s:%d] %s\r\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port), buf);

		// 데이터 보내기   
		retval = send(client_sock, buf, retval, 0);
		if (retval == SOCKET_ERROR)
		{
			err_display("send()");
			break;
		}
	}

	// closesocket() 

	closesocket(client_sock);

	DisplayText("[TCP 서버] 클라이언트 종료: IP 주소=%s, 포트 번호=%d\r\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

	return 0;
}