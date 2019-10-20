#pragma comment(lib, "ws2_32")
#define _WINSOCK_DEPRECATED_NO_WARNINGS	
#include <winsock2.h>
#include <ws2tcpip.h>
#include <tchar.h>
#include <stdlib.h>
#include <stdio.h>

#define SERVERPORT 5565
#define BUFSIZE    512

// 소켓 함수 오류 출력 후 종료
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

// 소켓 함수 오류 출력
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
	//오류 검사용 대부분의 곳에서 사용할 리턴밸류를 저장하는 값(RETurnVALue)
	int retval;

	// 윈속 초기화
	WSADATA wsa;

	//2.2라는 버전을 전달하기 위해 MAKEWORD로 데이터처리 이후 던짐
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	// 소켓 생성 - AF : Address Family / INET IPv6
	SOCKET listen_sock = socket(AF_INET6, SOCK_STREAM, 0);
	if (listen_sock == INVALID_SOCKET) err_quit("socket()");

	// 소켓바인딩 IPv6을 사용하므로 IN6
	SOCKADDR_IN6 serveraddr;
	// 메모리 초기화
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	// 주소체계 INET IPv6을 사용
	serveraddr.sin6_family = AF_INET6;
	//모든 IPv6 클라이언트가 연결을 설정할 수 있도록. 
	//자신의 아이피를 적어도 정상작동하고, any로 한다면 IP주소를 자동으로 찾아서 대입해줌.
	//서버는 여러 아이피를 동시할당받을 수 있을 가능성이 있어 inAddr any로 작성해주는것이 좋음
	serveraddr.sin6_addr = in6addr_any;
	//포트를 <호스트 to Network : 에디안 변경> - short 형으로 변환
	serveraddr.sin6_port = htons(SERVERPORT);
	//적은 SOCKADD정보를 기반으로 소켓 바인딩
	retval = bind(listen_sock, (SOCKADDR *)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("bind()");

	//타 클라이언트에서 접속이 올때까지 대기. Listening
	retval = listen(listen_sock, SOMAXCONN);
	if (retval == SOCKET_ERROR) err_quit("listen()");

	// 데이터 통신에 사용할 변수
	//클라이언트 소켓 데이터를 담을 빈 구조체
	SOCKET client_sock;
	//클라이언트 주소 데이터를 담을 빈 구조체
	SOCKADDR_IN6 clientaddr;
	//클라이언트 주소 데이터를 담을 빈 구조체 의 크기(size_t)를 저장할 변수
	int addrlen;
	//받은 데이터를 저장할 버퍼
	char buf[BUFSIZE + 1];

	while (1) 
	{
		// accept()
		//메모리 사이즈를 미리 받아둠
		addrlen = sizeof(clientaddr);
		//클라이언트 접속요청을 허락하며, 두 단말간 통신을 위하여 새로운 소켓을 생성(위의 정의한 client_sock에 담음)
		client_sock = accept(listen_sock, (SOCKADDR *)&clientaddr, &addrlen);
		if (client_sock == INVALID_SOCKET) 
		{
			err_display("accept()");
			break;
		}

		// 접속한 클라이언트 정보 출력
		char ipaddr[50];
		DWORD ipaddrlen = sizeof(ipaddr);
		//주소정보를 문자열로 변환 - ipaddr에 ipaddrlen 만큼 변환 데이터 복사
		WSAAddressToString((SOCKADDR *)&clientaddr, sizeof(clientaddr),
			NULL, ipaddr, &ipaddrlen);

		printf("\n[TCP 서버] 클라이언트 접속: %s\n", ipaddr);

		// 클라이언트와 데이터 통신
		while (1) 
		{
			// 데이터 받기
			//반환값 : 실제 수신한 바이트 수
			//해당소켓에서 데이터를 buf로 BUFSIZE 만큼 복사
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
			buf[retval] = '\0';
			printf("[TCP/%s] %s\n", ipaddr, buf);

			// 데이터 보내기
			//반환값 : 실제 송신한 바이트 수
			retval = send(client_sock, buf, retval, 0);
			if (retval == SOCKET_ERROR) 
			{
				err_display("send()");
				break;
			}
		}

		// closesocket()
		// accept에서 새로 생성된 클라이언트-서버간 소켓 종료
		closesocket(client_sock);
		printf("[TCP 서버] 클라이언트 종료: %s\n", ipaddr);
	}

	// closesocket()
	// accept을 위해 생성된 listening 소켓 종료
	closesocket(listen_sock);

	// 윈속 종료
	WSACleanup();

	return 0;
}