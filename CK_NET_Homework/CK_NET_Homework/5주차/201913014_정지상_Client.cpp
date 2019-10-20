#pragma comment(lib, "ws2_32")
#define _WINSOCK_DEPRECATED_NO_WARNINGS	
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>

#define SERVERIP   "::1"
#define SERVERPORT 5565
#define BUFSIZE    512

// ���� �Լ� ���� ��� �� ����
void err_quit(const char *msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	MessageBox(NULL, (LPCTSTR)lpMsgBuf, msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(1);
}

// ���� �Լ� ���� ���
void err_display(const char *msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	printf("[%s] %s", msg, (char *)lpMsgBuf);
	LocalFree(lpMsgBuf);
}

// ����� ���� ������ ���� �Լ�
// len��ŭ ���������� ����. �߰������� �� �ް� �� ���, �� ���� �����͸� ������ �� �������� ũ�⸦ ��ȯ.
///<summary> len��ŭ ������ ���.���������� ���� �������� ũ�⸦ ��ȯ</summary>
int recvn(SOCKET s, char *buf, int len, int flags)
{
	//���� ������ ���� ����� �Լ�
	int received;
	//���ڿ� �����ּ� ����
	char *ptr = buf;
	//���� ������ũ��
	int left = len;

	//���� �����Ͱ� �ִٸ�
	while (left > 0)
	{
		//������ ����
		received = recv(s, ptr, left, flags);
		if (received == SOCKET_ERROR)
			return SOCKET_ERROR;
		else if (received == 0)
			break;
		//���� �����Ϳ��� ��������ŭ ����
		left -= received;
		//���ڿ� �����ּҸ� ��������ŭ �ѱ�
		ptr += received;
	}

	//���� len���� �� ���� �����͸� �����Ͽ��ٸ�, �ش�з���ŭ ������ '���� ���� ���� ������' ����
	return (len - left);
}

int main(int argc, char *argv[])
{
	//���� �˻�� ��κ��� ������ ����� ���Ϲ���� �����ϴ� ��(RETurnVALue)
	int retval;

	// ���� �ʱ�ȭ
	WSADATA wsa;
	//2.2��� ������ �����ϱ� ���� MAKEWORD�� ������ó�� ���� ����
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	// socket()
	//���� ���� - AF : Address Family / INET IPv6
	SOCKET sock = socket(AF_INET6, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) err_quit("socket()");

	// connect()
	SOCKADDR_IN6 serveraddr;
	//�޸� �ʱ�ȭ
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	//�ּ�ü�� ����
	serveraddr.sin6_family = AF_INET6;

	int addrlen = sizeof(serveraddr);
	//���� �����Ǹ� ���ڿ��� ��ȯ��, seerveraddr�� ũ�⸸ŭ ����
	WSAStringToAddress((LPSTR)SERVERIP, AF_INET6, NULL, (SOCKADDR *)&serveraddr, &addrlen);
	//host to network short�������� ��ȯ. ����Ⱥ�ȯ
	serveraddr.sin6_port = htons(SERVERPORT);

	//��������õ�
	retval = connect(sock, (SOCKADDR *)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR)	err_quit("connect()");

	// ������ ��ſ� ����� ����
	char buf[BUFSIZE + 1];
	int len;

	// ������ ������ ���
	while (1) 
	{
		// ������ �Է�
		printf("\n[���� ������] ");
		if (fgets(buf, BUFSIZE + 1, stdin) == NULL)
			break;

		// '\n' ���� ����
		len = strlen(buf);
		if (buf[len - 1] == '\n')
			buf[len - 1] = '\0';
		if (strlen(buf) == 0)
			break;

		// ������ ������
		// ���ۻ����ŭ ���ۿ��� ����ȭ�Ͽ� ����
		// ��ȯ���� ������ �������� ũ��
		retval = send(sock, buf, strlen(buf), 0);
		if (retval == SOCKET_ERROR) {
			err_display("send()");
			break;
		}
		printf("[TCP Ŭ���̾�Ʈ] %d����Ʈ�� ���½��ϴ�.\n", retval);

		// ������ �ޱ�
		//����� ���� �Լ�. retval(���� ������ ũ��)��ŭ ������ �� ���� ���.
		//��ȯ�� �� ������ �������� ũ��
		retval = recvn(sock, buf, retval, 0);
		if (retval == SOCKET_ERROR) 
		{
			err_display("recv()");
			break;
		}
		else if (retval == 0)
			break;

		// ���� ������ ���
		buf[retval] = '\0';
		printf("[TCP Ŭ���̾�Ʈ] %d����Ʈ�� �޾ҽ��ϴ�.\n", retval);
		printf("[���� ������] %s\n", buf);
	}

	// closesocket()
	// ������ ������ ������ ���� ����
	closesocket(sock);

	// ���� ����
	WSACleanup();
	return 0;
}