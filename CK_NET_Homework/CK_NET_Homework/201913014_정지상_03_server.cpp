#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>

// �߰��� �ڵ�
// C4996 (Level 3) �����Ϸ� ��� ����ó��
#pragma warning (disable:4996)

#define SERVERPORT 3317
#define BUFSIZE    512

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

/**
 * ����� ���� ���� �Լ�
 * 
 *
 * @param s recv�� ������ ����.
 * @param buf �����͸� ������ ����.
 * @param len ���� ���ǵ� ���� ������.
 * @param flags recv�� ���Ǵ� flag. 
 *		MSG_DONTWAIT (������ ���� ��Ⱑ �ʿ����� �ʴٸ� -1 ��ȯ �� �ٷ� ����)
 *		MSG_NOSIGNAL (������ �������� SIGPIPE �ñ׳��� ���� ����)
 * @return ���������� ���� �������� ����ũ��.
 */
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
		//������ ������ �ʴ� �ּ�(����� ptr)���� �� �Ҵ�
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
	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sock == INVALID_SOCKET) err_quit("socket()");

	// bind()
	SOCKADDR_IN serveraddr;
	//�޸� �ʱ�ȭ
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	//�ּ�ü�� ����, Address Family / INET IPv4
	serveraddr.sin_family = AF_INET;

	//Host to network Long �� ����Ͽ� ����� ��ȯ�� �����ϸ� �ּ������� ������ ��� �ּҷ� ����.
	//�ڽ��� �����Ǹ� ��� �����۵��ϰ�, any�� �Ѵٸ� IP�ּҸ� �ڵ����� ã�Ƽ� ��������.
	//������ ���� �����Ǹ� �����Ҵ���� �� ���� ���ɼ��� �־� INADDR_ANY�� �ۼ����ִ°��� ����.
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);

	//��Ʈ�� <ȣ��Ʈ to Network : ����� ����> - short ������ ��ȯ
	serveraddr.sin_port = htons(SERVERPORT);

	//���� SOCKADDR_IN�� SOCKADD ������ ĳ�����Ͽ� ���� ���ε�
	retval = bind(listen_sock, (SOCKADDR *)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("bind()");

	// listen()
	//Ÿ Ŭ���̾�Ʈ���� ������ �ö����� ���. Listening
	retval = listen(listen_sock, SOMAXCONN);
	if (retval == SOCKET_ERROR) err_quit("listen()");

	// ������ ��ſ� ����� ����
	//Ŭ���̾�Ʈ ���� �����͸� ���� �� ����ü
	SOCKET client_sock;
	//Ŭ���̾�Ʈ �ּ� �����͸� ���� �� ����ü
	SOCKADDR_IN clientaddr;
	//Ŭ���̾�Ʈ �ּ� �����͸� ���� �� ����ü �� ũ��(size_t)�� ������ ����
	int addrlen;
	
	char buf[BUFSIZE + 1];
	int len;

	while (1) {
		// accept()
		//�޸� ����� �̸� �޾Ƶ�
		addrlen = sizeof(clientaddr);
		//Ŭ���̾�Ʈ ���ӿ�û�� ����ϸ�, �� �ܸ��� ����� ���Ͽ� ���ο� ������ ����(���� ������ client_sock�� addrlen ��ŭ ����)
		client_sock = accept(listen_sock, (SOCKADDR *)&clientaddr, &addrlen);
		if (client_sock == INVALID_SOCKET) {
			err_display("accept()");
			break;
		}

		// ������ Ŭ���̾�Ʈ ���� ���
		// hton�� �ݴ�� Network to host ����� ��ȯ ���� �� ���

			// inet_ntoa �� ���,C4996 ���� ���(Level 3)�� �߻���ŵ�ϴ�.
			// �ش� �����Ϸ� ���� strcpy, memcpy �� buffer overread / buffer overflow ���� �޸� ������ ħ���ϴ� ��� ��� ���� Ȯ���� �� �ֽ��ϴ�.

			// inet_ntoa �� ���� ������ �������� �Ҵ�� char ���۸� ����ϹǷ�, Thread Safe ���� �ʽ��ϴ�.
			// ms�� glibc�� ���۾����带 ���÷� ����� �ذ�������,
			// macOS �� �Ϻ� os������ �׷��� �ʱ� ������ �ش� ��� ����ϴ°����� �����˴ϴ�.

			// ���� �̸� ���� �ۼ��ܰ迡�� �ؼ��ϱ� ���� 7�� ���� ��ó������ �߰��Ǿ����ϴ� 
			// �ٸ� ������δ�, ������Ʈ�� SDL �˻縦 �����ϸ� �˴ϴ�. �ٸ� �����ϴ� ����� inet_ntop�� ����ϴ� ����Դϴ�.
			// ���� �� inet_ntoa�� C4996���� �ּ��� �߰����� �ʽ��ϴ�.
		printf("\n[TCP ����] Ŭ���̾�Ʈ ����: IP �ּ�=%s, ��Ʈ ��ȣ=%d\n",
			inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

		// Ŭ���̾�Ʈ�� ������ ���
		while (1) {
			// ������ �ޱ�(���� ����)
			// ������ ����� int�� ��ŭ �޾�, ���ۿ��� ���� ���� ������ ������ �޸�(char *������ ĳ���õ�)�� ����
			retval = recvn(client_sock, (char *)&len, sizeof(int), 0);
			if (retval == SOCKET_ERROR) {
				err_display("recv()");
				break;
			}
			else if (retval == 0)
				break;

			// ������ �ޱ�(���� ����)
			//��������� �����Լ� - ���̸� ���� ��ŭ ����, �޸� ���� ����
			retval = recvn(client_sock, buf, len, 0);
			if (retval == SOCKET_ERROR) {
				err_display("recv()");
				break;
			}
			else if (retval == 0)
				break;

			// ���� ������ ���
			buf[retval] = '\0';
			printf("[TCP/%s:%d] %s\n", inet_ntoa(clientaddr.sin_addr),
				ntohs(clientaddr.sin_port), buf);
		}

		// closesocket()
		// accept���� ���� ������ Ŭ���̾�Ʈ-������ ���� ����
		closesocket(client_sock);
		printf("[TCP ����] Ŭ���̾�Ʈ ����: IP �ּ�=%s, ��Ʈ ��ȣ=%d\n",
			inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
	}

	// closesocket()
	// accept�� ���� ������ listening ���� ����
	closesocket(listen_sock);

	// ���� ����
	WSACleanup();
	return 0;
}