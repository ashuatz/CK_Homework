#pragma comment(lib, "ws2_32") 
#include <winsock2.h>
#include <stdlib.h> 
#include <stdio.h>
#include "resource.h" 

#define SERVERIP   "127.0.0.1"
#define SERVERPORT 3317
#define BUFSIZE    512 

// ��ȭ���� ���ν���
BOOL CALLBACK DlgProc(HWND, UINT, WPARAM, LPARAM);

// ���� ��Ʈ�� ��� �Լ�
void DisplayText(char *fmt, ...);
// ���� ��� �Լ� 
void err_quit(char *msg);
void err_display(char *msg);

// ����� ���� ������ ���� �Լ� 
int recvn(SOCKET s, char *buf, int len, int flags);

// ���� ��� ������ �Լ�
DWORD WINAPI ClientMain(LPVOID arg);

// ���� 
SOCKET sock; 
// ������ �ۼ��� ���� 
char buf[BUFSIZE + 1]; 
// �̺�Ʈ 
HANDLE hReadEvent, hWriteEvent; 

// ������ ��ư
HWND hSendButton; 
// ���� ��Ʈ�� 
HWND hEdit1, hEdit2; 

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// �̺�Ʈ ���� 
	// �б� �Ϸ� �̺�Ʈ�� ���� �Ϸ� �̺�Ʈ�� �ڵ����� ����
	//�̴� Wait�� ������ ��� ���� �ڵ����� false�� ����ǵ���
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

	// ���� ��� ������ ����  
	CreateThread(NULL, 0, ClientMain, NULL, 0, NULL);

	// ��ȭ���� ���� 
	DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, DlgProc);

	// �̺�Ʈ ���� 
	CloseHandle(hReadEvent); 
	CloseHandle(hWriteEvent);

	// closesocket()  
	closesocket(sock);

	// ���� ���� 
	WSACleanup();
	return 0;
}

// ��ȭ���� ���ν��� 
BOOL CALLBACK DlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		//���̾�α� �ʱ�ȭ �̺�Ʈ
		case WM_INITDIALOG:
		{
			//����ȵ� ��Ȳ���� ������ 1�������� ���ϵ��� 
			//UI�ܿ��� EnableWindow(hSendButton,FALSE)�� �ؾ��Ѵٰ� �����մϴ�.

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
				//UI��. ����ڰ� �Է��ؼ� ������ �����°��� ����Ѵ�.
				//�ش� ���۹�ư�� ���� ��, �����͸� ���ۿ� �ű� �� ������ ó����û(ȣ���� �ƴ� �̺�Ʈ ����)�� �Ѵ�.
				case IDOK:
					// ������ ��ư ��Ȱ��ȭ  
					EnableWindow(hSendButton, FALSE);

					// �б� �Ϸ� ���
					// ���� ���ۿ�û�� ���ڿ��� ó���Ϸ�ɶ����� ����Ѵ�
					WaitForSingleObject(hReadEvent, INFINITE);

					GetDlgItemText(hDlg, IDC_EDIT1, buf, BUFSIZE + 1);

					//��� ������ ������ �ʾƵ� �̺�Ʈ�� �����ϴ� ������
					//���������� �Ͼ �� �ִ� �κ��� buf�� �����ϴ� ����̱� �����̴�.
					//��������� ���������ֱ� ������ ���� ������ ���ÿ� �۾��� �� �־� �̺�Ʈ�� �����ش�.
					SetEvent(hWriteEvent); // ���� �Ϸ� �˸���
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

// WINAPI�� �ؽ�Ʈ�� �������ϱ� ���� ���� ��Ʈ�� ��� �Լ�
// ...�� stdarg�� �����ϴ� �������� �Լ�(printf��� ���� �� �� �ִ�)
//format�� �ް� ���� format���ο� �ִ� �������Ĺ��ڿ�(%d ��)�� ������ŭ arg�� �־������
//Event�� ����Ͽ� ����������� ����ϰ� �ֱ⿡ �Լ� ��Ͽ� ��������� ������ ����
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


// ���� �Լ� ���� ��� �� ����
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

// ���� �Լ� ���� ���
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

// ����� ���� ������ ���� �Լ�
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


// TCP Ŭ���̾�Ʈ ���� �κ� 
DWORD WINAPI ClientMain(LPVOID arg)
{
	int retval;

	// ���� �ʱ�ȭ 
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		return 1;
	}

	// socket() 
	// ���� ���� TCP,INET,IPv4
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET)
	{
		err_quit("socket()");
	}

	// connect()
	// ������ ������ ������ �ۼ�
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr(SERVERIP);
	serveraddr.sin_port = htons(SERVERPORT);

	//�ۼ��� ������ ���� ���������û �� ���
	retval = connect(sock, (SOCKADDR *)&serveraddr, sizeof(serveraddr));

	if (retval == SOCKET_ERROR)
	{
		err_quit("connect()");
	}

	// ������ ������ ��� 
	// ���������� ������ ����Ǿ� ��ŵǴ� �κ�
	while (1)
	{
		//.���ܰ�
		// ���� �Ϸ� ��ٸ��� 
		// ����������� ���� ����� �ϳ�. 
		//hWriteEvent�� �Ŵ���� �ۼ����� �ʾұ� ������ �ڵ������� ����Ѵ�

		//UI�ܿ��� ���۹�ư�� ���������� ����Ѵ�.
		WaitForSingleObject(hWriteEvent, INFINITE);

		// ���ڿ� ���̰� 0�̸� ������ ���� 
		if (strlen(buf) == 0) 
		{
			// �ٽ� �ʱ�ȭ �ܰ�
			// ������ ��ư Ȱ��ȭ
			// �б� �Ϸ� �˸���  
			EnableWindow(hSendButton, TRUE);
			SetEvent(hReadEvent);
			continue;
		}

		//.��ŷ���
		// ������ ������ 
		retval = send(sock, buf, strlen(buf), 0);
		if (retval == SOCKET_ERROR)
		{
			err_display("send()");
			break;
		}

		DisplayText("[TCP Ŭ���̾�Ʈ] %d����Ʈ�� ���½��ϴ�.\r\n", retval);

		// ������ �ޱ� 
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

		// ���� ������ ��� 
		buf[retval] = '\0';
		DisplayText("[TCP Ŭ���̾�Ʈ] %d����Ʈ�� �޾ҽ��ϴ�.\r\n", retval);
		DisplayText("[���� ������] %s\r\n", buf);


		//. �ٽ� �ʱ�ȭ �ܰ�
		// ������ ��ư Ȱ��ȭ   
		// �б� �Ϸ� �˸���  
		EnableWindow(hSendButton, TRUE);
		SetEvent(hReadEvent);
	}

	return 0;
}