#pragma comment(lib, "ws2_32") 
#include <winsock2.h>
#include <stdlib.h> 
#include <stdio.h>
#include "resource.h" 

#define SERVERIP   "127.0.0.1"
#define SERVERPORT 3317
#define BUFSIZE    512 

// 대화상자 프로시저
BOOL CALLBACK DlgProc(HWND, UINT, WPARAM, LPARAM);

// 편집 컨트롤 출력 함수
void DisplayText(char *fmt, ...);
// 오류 출력 함수 
void err_quit(char *msg);
void err_display(char *msg);

// 사용자 정의 데이터 수신 함수 
int recvn(SOCKET s, char *buf, int len, int flags);

// 소켓 통신 스레드 함수
DWORD WINAPI ClientMain(LPVOID arg);

// 소켓 
SOCKET sock; 
// 데이터 송수신 버퍼 
char buf[BUFSIZE + 1]; 
// 이벤트 
HANDLE hReadEvent, hWriteEvent; 

// 보내기 버튼
HWND hSendButton; 
// 편집 컨트롤 
HWND hEdit1, hEdit2; 

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// 이벤트 생성 
	// 읽기 완료 이벤트와 쓰기 완료 이벤트는 자동으로 생성
	//이는 Wait절 스레드 대기 이후 자동으로 false로 변경되도록
	hReadEvent = CreateEvent(NULL, FALSE, TRUE, NULL);
	if (hReadEvent == NULL)
	{
		return 1;
	}

	hWriteEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	if (hWriteEvent == NULL) 
	{
		return 1;
	}

	// 소켓 통신 스레드 생성  
	CreateThread(NULL, 0, ClientMain, NULL, 0, NULL);

	// 대화상자 생성 
	DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, DlgProc);

	// 이벤트 제거 
	CloseHandle(hReadEvent); 
	CloseHandle(hWriteEvent);

	// closesocket()  
	closesocket(sock);

	// 윈속 종료 
	WSACleanup();
	return 0;
}

// 대화상자 프로시저 
BOOL CALLBACK DlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		//다이얼로그 초기화 이벤트
		case WM_INITDIALOG:
		{
			//연결안된 상황에서 전송을 1차적으로 못하도록 
			//UI단에서 EnableWindow(hSendButton,FALSE)를 해야한다고 생각합니다.

			hEdit1 = GetDlgItem(hDlg, IDC_EDIT1);
			hEdit2 = GetDlgItem(hDlg, IDC_EDIT2);
			hSendButton = GetDlgItem(hDlg, IDOK);
			SendMessage(hEdit1, EM_SETLIMITTEXT, BUFSIZE, 0);
			return TRUE;
		}
		case WM_COMMAND:
		{
			switch (LOWORD(wParam))
			{
				//UI단. 사용자가 입력해서 전송을 누르는것을 담당한다.
				//해당 전송버튼을 누를 때, 데이터를 버퍼에 옮긴 후 수동적 처리요청(호출이 아닌 이벤트 설정)를 한다.
				case IDOK:
					// 보내기 버튼 비활성화  
					EnableWindow(hSendButton, FALSE);

					// 읽기 완료 대기
					// 이전 전송요청한 문자열이 처리완료될때까지 대기한다
					WaitForSingleObject(hReadEvent, INFINITE);

					GetDlgItemText(hDlg, IDC_EDIT1, buf, BUFSIZE + 1);

					//모든 구문이 끝나지 않아도 이벤트를 설정하는 이유는
					//동시접근이 일어날 수 있는 부분이 buf에 접근하는 경우이기 때문이다.
					//기능적으로 나뉘어져있기 때문에 이후 문구는 동시에 작업할 수 있어 이벤트를 열어준다.
					SetEvent(hWriteEvent); // 쓰기 완료 알리기
					SetFocus(hEdit1);
					SendMessage(hEdit1, EM_SETSEL, 0, -1);
					return TRUE;

				case IDCANCEL:
					EndDialog(hDlg, IDCANCEL);
					return TRUE;
			}
			return FALSE;
		}
	}

	return FALSE;
}

// WINAPI상에 텍스트를 렌더링하기 위한 편집 컨트롤 출력 함수
// ...은 stdarg에 존재하는 가변인자 함수(printf등에서 자주 볼 수 있는)
//format을 받고 이후 format내부에 있는 포맷형식문자열(%d 등)의 갯수만큼 arg에 넣어줘야함
//Event를 사용하여 스레드안전을 사용하고 있기에 함수 블록에 스레드관련 사항이 없음
void DisplayText(char *fmt, ...)
{
	va_list arg;
	va_start(arg, fmt);

	char cbuf[BUFSIZE + 256];
	vsprintf(cbuf, fmt, arg);

	int nLength = GetWindowTextLength(hEdit2);
	SendMessage(hEdit2, EM_SETSEL, nLength, nLength);
	SendMessage(hEdit2, EM_REPLACESEL, FALSE, (LPARAM)cbuf);

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

// 사용자 정의 데이터 수신 함수
int recvn(SOCKET s, char *buf, int len, int flags)
{
	int received;
	char *ptr = buf;
	int left = len;

	while (left > 0)
	{
		received = recv(s, ptr, left, flags);

		if (received == SOCKET_ERROR)
			return SOCKET_ERROR;

		else if (received == 0)
			break;

		left -= received;
		ptr += received;
	}

	return (len - left);
}


// TCP 클라이언트 시작 부분 
DWORD WINAPI ClientMain(LPVOID arg)
{
	int retval;

	// 윈속 초기화 
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		return 1;
	}

	// socket() 
	// 소켓 생성 TCP,INET,IPv4
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET)
	{
		err_quit("socket()");
	}

	// connect()
	// 서버에 연결할 정보를 작성
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr(SERVERIP);
	serveraddr.sin_port = htons(SERVERPORT);

	//작성한 정보를 토대로 서버연결요청 후 대기
	retval = connect(sock, (SOCKADDR *)&serveraddr, sizeof(serveraddr));

	if (retval == SOCKET_ERROR)
	{
		err_quit("connect()");
	}

	// 서버와 데이터 통신 
	// 실질적으로 서버와 연결되어 통신되는 부분
	while (1)
	{
		//.대기단계
		// 쓰기 완료 기다리기 
		// 스레드안전을 위한 기법중 하나. 
		//hWriteEvent는 매뉴얼로 작성되지 않았기 때문에 자동형식을 사용한다

		//UI단에서 전송버튼을 누를때까지 대기한다.
		WaitForSingleObject(hWriteEvent, INFINITE);

		// 문자열 길이가 0이면 보내지 않음 
		if (strlen(buf) == 0) 
		{
			// 다시 초기화 단계
			// 보내기 버튼 활성화
			// 읽기 완료 알리기  
			EnableWindow(hSendButton, TRUE);
			SetEvent(hReadEvent);
			continue;
		}

		//.통신로직
		// 데이터 보내기 
		retval = send(sock, buf, strlen(buf), 0);
		if (retval == SOCKET_ERROR)
		{
			err_display("send()");
			break;
		}

		DisplayText("[TCP 클라이언트] %d바이트를 보냈습니다.\r\n", retval);

		// 데이터 받기 
		retval = recvn(sock, buf, retval, 0);
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
		buf[retval] = '\0';
		DisplayText("[TCP 클라이언트] %d바이트를 받았습니다.\r\n", retval);
		DisplayText("[받은 데이터] %s\r\n", buf);


		//. 다시 초기화 단계
		// 보내기 버튼 활성화   
		// 읽기 완료 알리기  
		EnableWindow(hSendButton, TRUE);
		SetEvent(hReadEvent);
	}

	return 0;
}