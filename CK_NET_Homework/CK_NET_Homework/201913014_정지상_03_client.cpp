#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>

// �߰��� �ڵ�
// C4996 (Level 3) �����Ϸ� ��� ����ó��
#pragma warning (disable:4996)

#define SERVERIP   "127.0.0.1"
#define SERVERPORT 3317
#define BUFSIZE    50

// ���� �Լ� ���� ��� �� ����
void err_quit(char *msg)
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
void err_display(char *msg)
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

int main(int argc, char *argv[])
{
	int retval;

	// ���� �ʱ�ȭ
	WSADATA wsa;
	//2.2��� ������ �����ϱ� ���� MAKEWORD�� ������ó�� ���� ����
	//MAKEWORD(bLow,bHigh) , bLow => ���� 8��Ʈ�� ä���� ������, bHigh => ���� 8��Ʈ�� ä���� ������
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	// socket()
	//���� ���� - AF : Address Family / INET IPv4 , TCP ��� IPPROTO_TCP
	//SOCK_STREAM <> IPPROTO_TCP (TCP) / SOCK_DGRAM <> IPPROTO_UDP (UDP), 0�� ���� ��� �ڵ����� ���缭 �ν�
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) err_quit("socket()");

	// connect()
	SOCKADDR_IN serveraddr;
	//�޸� �ʱ�ȭ
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	//�ּ�ü�� ����, Address Family / INET IPv4
	serveraddr.sin_family = AF_INET;

	//Host to network Long �� ����Ͽ� ����� ��ȯ�� �����ϸ� �ּ������� ���� IP�� ����.

		//inet_addr �� ��� c4996 ������ �߻��մϴ�.
		//c4996�� �ڼ��� �ּ��� ������ �ּ� (line number : 148)�� ������ �ּ���.
		//inet_addr�� ��쿡�� ���� �̽��� ������, IPv6�� �������� �ʾ� inet_ntoa�� �Բ� ������ �ʰ� �� �� �����ϴ�.
	serveraddr.sin_addr.s_addr = inet_addr(SERVERIP);
	//��Ʈ�� <ȣ��Ʈ to Network : ����� ����> - short ������ ��ȯ
	serveraddr.sin_port = htons(SERVERPORT);
	//��������õ�
	//��ȯ�� 0 : ���� / -1(SOCKET_ERROR) : ���� 
	retval = connect(sock, (SOCKADDR *)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("connect()");

	// ������ ��ſ� ����� ����
	char buf[BUFSIZE];
	char *testdata[] = {
		"�ȳ��ϼ���",
		"�ݰ�����",
		"���õ��� �� �̾߱Ⱑ ���� �� ���׿�",
		"���� �׷��׿�",
	};
	int len;

	// ������ ������ ���
	//�� 4���� �����͸� 8ȸ�� ���� ����
	for (int i = 0; i < 4; i++) 
	{
		// ������ �Է�(�ùķ��̼�)
		// �Է¹��� ���ڿ��� ���̸� ��� �޸𸮿� �÷��ΰ�
		len = strlen(testdata[i]);
		//���ۿ� �ش繮�ڿ� ���̸�ŭ ����
			//strncpy �� ��� c4996 ������ �߻��մϴ�.
			//c4996�� �ڼ��� �ּ��� ������ �ּ� (line number : 148)�� ������ �ּ���.
		strncpy(buf, testdata[i], len);

		// ������ ������(���� ����)
		// �ش� ���ڿ��� ���̸� �����ϱ� ����,len�� �����͸� char* �� ĳ�����Ͽ� ����
		// 1byteũ���� char�������� ����ȭ �� �������� ���� �ּҸ� �Ѱ���.C#������ byte�� ����ȭ������, C++������ char�������� �ϴ� �� �մϴ�
		// ��ȯ���� ���� �������� ũ���Դϴ�.
		// flag
		//		MSG_DONTWAIT (������ ���� ��Ⱑ �ʿ��ϴٸ� -1 ��ȯ �� �ٷ� ����)
		//		MSG_NOSIGNAL (������ �������� SIGPIPE �ñ׳��� ���� ����)
		retval = send(sock, (char *)&len, sizeof(int), 0);
		if (retval == SOCKET_ERROR) {
			err_display("send()");
			break;
		}

		// ������ ������(���� ����)
		// �������� �ڽ��� ���� �������� ũ�⸦ �̸� �˷ȱ⶧����, �ش� ���ڿ��� ���̸�ŭ (=�������� ũ�⸸ŭ) ���ڿ��� �����մϴ�.
		// ��ȯ���� ���� �������� ũ���Դϴ�.
		// flag
		//		MSG_DONTWAIT (������ ���� ��Ⱑ �ʿ��ϴٸ� -1 ��ȯ �� �ٷ� ����)
		//		MSG_NOSIGNAL (������ �������� SIGPIPE �ñ׳��� ���� ����)
		retval = send(sock, buf, len, 0);
		if (retval == SOCKET_ERROR) {
			err_display("send()");
			break;
		}
		printf("[TCP Ŭ���̾�Ʈ] %d+%d����Ʈ�� "
			"���½��ϴ�.\n", sizeof(int), retval);
	}

	// closesocket()
	closesocket(sock);

	// ���� ����
	WSACleanup();
	return 0;
}

