#pragma comment(lib, "ws2_32")
#define _WINSOCK_DEPRECATED_NO_WARNINGS	
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>

#define SERVERIP   "::1"
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

// 사용자 정의 데이터 수신 함수
// len만큼 받을때까지 수신. 추가적으로 더 받게 될 경우, 더 받은 데이터를 포함한 총 데이터의 크기를 반환.
///<summary> len만큼 수신을 대기.최종적으로 받은 데이터의 크기를 반환</summary>
int recvn(SOCKET s, char *buf, int len, int flags)
{
	//받은 데이터 길이 저장용 함수
	int received;
	//문자열 시작주소 복사
	char *ptr = buf;
	//남은 데이터크기
	int left = len;

	//남은 데이터가 있다면
	while (left > 0)
	{
		//데이터 수신
		received = recv(s, ptr, left, flags);
		if (received == SOCKET_ERROR)
			return SOCKET_ERROR;
		else if (received == 0)
			break;
		//남은 데이터에서 받은량만큼 제외
		left -= received;
		//문자열 시작주소를 받은량만큼 넘김
		ptr += received;
	}

	//만약 len보다 더 많은 데이터를 수신하였다면, 해당분량만큼 더해진 '실제 받은 총합 사이즈' 전달
	return (len - left);
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

	// socket()
	//소켓 생성 - AF : Address Family / INET IPv6
	SOCKET sock = socket(AF_INET6, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) err_quit("socket()");

	// connect()
	SOCKADDR_IN6 serveraddr;
	//메모리 초기화
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	//주소체계 설정
	serveraddr.sin6_family = AF_INET6;

	int addrlen = sizeof(serveraddr);
	//서버 아이피를 문자열로 변환해, seerveraddr의 크기만큼 복사
	WSAStringToAddress((LPSTR)SERVERIP, AF_INET6, NULL, (SOCKADDR *)&serveraddr, &addrlen);
	//host to network short데이터형 변환. 에디안변환
	serveraddr.sin6_port = htons(SERVERPORT);

	//연결수립시도
	retval = connect(sock, (SOCKADDR *)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR)	err_quit("connect()");

	// 데이터 통신에 사용할 변수
	char buf[BUFSIZE + 1];
	int len;

	// 서버와 데이터 통신
	while (1) 
	{
		// 데이터 입력
		printf("\n[보낼 데이터] ");
		if (fgets(buf, BUFSIZE + 1, stdin) == NULL)
			break;

		// '\n' 문자 제거
		len = strlen(buf);
		if (buf[len - 1] == '\n')
			buf[len - 1] = '\0';
		if (strlen(buf) == 0)
			break;

		// 데이터 보내기
		// 버퍼사이즈만큼 버퍼에서 직렬화하여 전송
		// 반환값은 전송한 데이터의 크기
		retval = send(sock, buf, strlen(buf), 0);
		if (retval == SOCKET_ERROR) {
			err_display("send()");
			break;
		}
		printf("[TCP 클라이언트] %d바이트를 보냈습니다.\n", retval);

		// 데이터 받기
		//사용자 정의 함수. retval(보낸 데이터 크기)만큼 수신할 때 까지 대기.
		//반환은 총 수신한 데이터의 크기
		retval = recvn(sock, buf, retval, 0);
		if (retval == SOCKET_ERROR) 
		{
			err_display("recv()");
			break;
		}
		else if (retval == 0)
			break;

		// 받은 데이터 출력
		buf[retval] = '\0';
		printf("[TCP 클라이언트] %d바이트를 받았습니다.\n", retval);
		printf("[받은 데이터] %s\n", buf);
	}

	// closesocket()
	// 서버와 연결이 수립된 소켓 종료
	closesocket(sock);

	// 윈속 종료
	WSACleanup();
	return 0;
}