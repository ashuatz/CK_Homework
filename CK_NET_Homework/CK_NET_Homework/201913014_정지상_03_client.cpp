#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>

// 추가된 코드
// C4996 (Level 3) 컴파일러 경고를 무시처리
#pragma warning (disable:4996)

#define SERVERIP   "127.0.0.1"
#define SERVERPORT 3317
#define BUFSIZE    50

// 소켓 함수 오류 출력 후 종료
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

// 소켓 함수 오류 출력
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

	// 윈속 초기화
	WSADATA wsa;
	//2.2라는 버전을 전달하기 위해 MAKEWORD로 데이터처리 이후 던짐
	//MAKEWORD(bLow,bHigh) , bLow => 하위 8비트에 채워질 데이터, bHigh => 상위 8비트에 채워질 데이터
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	// socket()
	//소켓 생성 - AF : Address Family / INET IPv4 , TCP 통신 IPPROTO_TCP
	//SOCK_STREAM <> IPPROTO_TCP (TCP) / SOCK_DGRAM <> IPPROTO_UDP (UDP), 0을 넣을 경우 자동으로 맞춰서 인식
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) err_quit("socket()");

	// connect()
	SOCKADDR_IN serveraddr;
	//메모리 초기화
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	//주소체계 설정, Address Family / INET IPv4
	serveraddr.sin_family = AF_INET;

	//Host to network Long 을 사용하여 에디안 변환을 시행하며 주소정보를 서버 IP로 설정.

		//inet_addr 의 경우 c4996 오류를 발생합니다.
		//c4996의 자세한 주석은 서버의 주석 (line number : 148)를 참조해 주세요.
		//inet_addr의 경우에는 보안 이슈가 없지만, IPv6를 지원하지 않아 inet_ntoa와 함께 사용되지 않게 된 것 같습니다.
	serveraddr.sin_addr.s_addr = inet_addr(SERVERIP);
	//포트를 <호스트 to Network : 에디안 변경> - short 형으로 변환
	serveraddr.sin_port = htons(SERVERPORT);
	//연결수립시도
	//반환값 0 : 성공 / -1(SOCKET_ERROR) : 실패 
	retval = connect(sock, (SOCKADDR *)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("connect()");

	// 데이터 통신에 사용할 변수
	char buf[BUFSIZE];
	char *testdata[] = {
		"안녕하세요",
		"반가워요",
		"오늘따라 할 이야기가 많을 것 같네요",
		"저도 그렇네요",
	};
	int len;

	// 서버와 데이터 통신
	//총 4개의 데이터를 8회에 걸쳐 전송
	for (int i = 0; i < 4; i++) 
	{
		// 데이터 입력(시뮬레이션)
		// 입력받은 문자열의 길이를 잠시 메모리에 올려두고
		len = strlen(testdata[i]);
		//버퍼에 해당문자열 길이만큼 복사
			//strncpy 의 경우 c4996 오류를 발생합니다.
			//c4996의 자세한 주석은 서버의 주석 (line number : 148)를 참조해 주세요.
		strncpy(buf, testdata[i], len);

		// 데이터 보내기(고정 길이)
		// 해당 문자열의 길이를 전송하기 위해,len의 데이터를 char* 로 캐스팅하여 전송
		// 1byte크기인 char형식으로 직렬화 된 데이터의 시작 주소를 넘겨줌.C#에서는 byte로 직렬화하지만, C++에서는 char형식으로 하는 듯 합니다
		// 반환값은 보낸 데이터의 크기입니다.
		// flag
		//		MSG_DONTWAIT (전송을 위해 대기가 필요하다면 -1 반환 후 바로 복귀)
		//		MSG_NOSIGNAL (연결이 끊겼을때 SIGPIPE 시그널을 받지 않음)
		retval = send(sock, (char *)&len, sizeof(int), 0);
		if (retval == SOCKET_ERROR) {
			err_display("send()");
			break;
		}

		// 데이터 보내기(가변 길이)
		// 서버에게 자신이 보낼 데이터의 크기를 미리 알렸기때문에, 해당 문자열의 길이만큼 (=데이터의 크기만큼) 문자열을 전송합니다.
		// 반환값은 보낸 데이터의 크기입니다.
		// flag
		//		MSG_DONTWAIT (전송을 위해 대기가 필요하다면 -1 반환 후 바로 복귀)
		//		MSG_NOSIGNAL (연결이 끊겼을때 SIGPIPE 시그널을 받지 않음)
		retval = send(sock, buf, len, 0);
		if (retval == SOCKET_ERROR) {
			err_display("send()");
			break;
		}
		printf("[TCP 클라이언트] %d+%d바이트를 "
			"보냈습니다.\n", sizeof(int), retval);
	}

	// closesocket()
	closesocket(sock);

	// 윈속 종료
	WSACleanup();
	return 0;
}

