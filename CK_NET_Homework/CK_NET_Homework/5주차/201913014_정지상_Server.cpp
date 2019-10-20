#pragma comment(lib, "ws2_32")
#define _WINSOCK_DEPRECATED_NO_WARNINGS	
#include <winsock2.h>
#include <ws2tcpip.h>
#include <tchar.h>
#include <stdlib.h>
#include <stdio.h>

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

int main(int argc, char *argv[])
{
	//���� �˻�� ��κ��� ������ ����� ���Ϲ���� �����ϴ� ��(RETurnVALue)
	int retval;

	// ���� �ʱ�ȭ
	WSADATA wsa;

	//2.2��� ������ �����ϱ� ���� MAKEWORD�� ������ó�� ���� ����
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	// ���� ���� - AF : Address Family / INET IPv6
	SOCKET listen_sock = socket(AF_INET6, SOCK_STREAM, 0);
	if (listen_sock == INVALID_SOCKET) err_quit("socket()");

	// ���Ϲ��ε� IPv6�� ����ϹǷ� IN6
	SOCKADDR_IN6 serveraddr;
	// �޸� �ʱ�ȭ
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	// �ּ�ü�� INET IPv6�� ���
	serveraddr.sin6_family = AF_INET6;
	//��� IPv6 Ŭ���̾�Ʈ�� ������ ������ �� �ֵ���. 
	//�ڽ��� �����Ǹ� ��� �����۵��ϰ�, any�� �Ѵٸ� IP�ּҸ� �ڵ����� ã�Ƽ� ��������.
	//������ ���� �����Ǹ� �����Ҵ���� �� ���� ���ɼ��� �־� inAddr any�� �ۼ����ִ°��� ����
	serveraddr.sin6_addr = in6addr_any;
	//��Ʈ�� <ȣ��Ʈ to Network : ����� ����> - short ������ ��ȯ
	serveraddr.sin6_port = htons(SERVERPORT);
	//���� SOCKADD������ ������� ���� ���ε�
	retval = bind(listen_sock, (SOCKADDR *)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("bind()");

	//Ÿ Ŭ���̾�Ʈ���� ������ �ö����� ���. Listening
	retval = listen(listen_sock, SOMAXCONN);
	if (retval == SOCKET_ERROR) err_quit("listen()");

	// ������ ��ſ� ����� ����
	//Ŭ���̾�Ʈ ���� �����͸� ���� �� ����ü
	SOCKET client_sock;
	//Ŭ���̾�Ʈ �ּ� �����͸� ���� �� ����ü
	SOCKADDR_IN6 clientaddr;
	//Ŭ���̾�Ʈ �ּ� �����͸� ���� �� ����ü �� ũ��(size_t)�� ������ ����
	int addrlen;
	//���� �����͸� ������ ����
	char buf[BUFSIZE + 1];

	while (1) 
	{
		// accept()
		//�޸� ����� �̸� �޾Ƶ�
		addrlen = sizeof(clientaddr);
		//Ŭ���̾�Ʈ ���ӿ�û�� ����ϸ�, �� �ܸ��� ����� ���Ͽ� ���ο� ������ ����(���� ������ client_sock�� ����)
		client_sock = accept(listen_sock, (SOCKADDR *)&clientaddr, &addrlen);
		if (client_sock == INVALID_SOCKET) 
		{
			err_display("accept()");
			break;
		}

		// ������ Ŭ���̾�Ʈ ���� ���
		char ipaddr[50];
		DWORD ipaddrlen = sizeof(ipaddr);
		//�ּ������� ���ڿ��� ��ȯ - ipaddr�� ipaddrlen ��ŭ ��ȯ ������ ����
		WSAAddressToString((SOCKADDR *)&clientaddr, sizeof(clientaddr),
			NULL, ipaddr, &ipaddrlen);

		printf("\n[TCP ����] Ŭ���̾�Ʈ ����: %s\n", ipaddr);

		// Ŭ���̾�Ʈ�� ������ ���
		while (1) 
		{
			// ������ �ޱ�
			//��ȯ�� : ���� ������ ����Ʈ ��
			//�ش���Ͽ��� �����͸� buf�� BUFSIZE ��ŭ ����
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

			// ���� ������ ���
			buf[retval] = '\0';
			printf("[TCP/%s] %s\n", ipaddr, buf);

			// ������ ������
			//��ȯ�� : ���� �۽��� ����Ʈ ��
			retval = send(client_sock, buf, retval, 0);
			if (retval == SOCKET_ERROR) 
			{
				err_display("send()");
				break;
			}
		}

		// closesocket()
		// accept���� ���� ������ Ŭ���̾�Ʈ-������ ���� ����
		closesocket(client_sock);
		printf("[TCP ����] Ŭ���̾�Ʈ ����: %s\n", ipaddr);
	}

	// closesocket()
	// accept�� ���� ������ listening ���� ����
	closesocket(listen_sock);

	// ���� ����
	WSACleanup();

	return 0;
}