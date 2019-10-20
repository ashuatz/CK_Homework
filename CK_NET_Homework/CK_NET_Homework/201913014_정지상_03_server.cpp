#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>

// 추가된 코드
// C4996 (Level 3) 컴파일러 경고를 무시처리
#pragma warning (disable:4996)

#define SERVERPORT 3317
#define BUFSIZE    512

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

/**
 * 사용자 정의 수신 함수
 * 
 *
 * @param s recv를 수행할 소켓.
 * @param buf 데이터를 정의할 버퍼.
 * @param len 먼저 정의된 받을 사이즈.
 * @param flags recv에 사용되는 flag. 
 *		MSG_DONTWAIT (수신을 위해 대기가 필요하지 않다면 -1 반환 후 바로 복귀)
 *		MSG_NOSIGNAL (연결이 끊겼을때 SIGPIPE 시그널을 받지 않음)
 * @return 최종적으로 받은 데이터의 총합크기.
 */
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
		//버퍼중 사용되지 않는 주소(변경된 ptr)부터 값 할당
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
	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sock == INVALID_SOCKET) err_quit("socket()");

	// bind()
	SOCKADDR_IN serveraddr;
	//메모리 초기화
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	//주소체계 설정, Address Family / INET IPv4
	serveraddr.sin_family = AF_INET;

	//Host to network Long 을 사용하여 에디안 변환을 시행하며 주소정보를 들어오는 모든 주소로 설정.
	//자신의 아이피를 적어도 정상작동하고, any로 한다면 IP주소를 자동으로 찾아서 대입해줌.
	//서버는 여러 아이피를 동시할당받을 수 있을 가능성이 있어 INADDR_ANY로 작성해주는것이 좋음.
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);

	//포트를 <호스트 to Network : 에디안 변경> - short 형으로 변환
	serveraddr.sin_port = htons(SERVERPORT);

	//적은 SOCKADDR_IN를 SOCKADD 형으로 캐스팅하여 소켓 바인딩
	retval = bind(listen_sock, (SOCKADDR *)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("bind()");

	// listen()
	//타 클라이언트에서 접속이 올때까지 대기. Listening
	retval = listen(listen_sock, SOMAXCONN);
	if (retval == SOCKET_ERROR) err_quit("listen()");

	// 데이터 통신에 사용할 변수
	//클라이언트 소켓 데이터를 담을 빈 구조체
	SOCKET client_sock;
	//클라이언트 주소 데이터를 담을 빈 구조체
	SOCKADDR_IN clientaddr;
	//클라이언트 주소 데이터를 담을 빈 구조체 의 크기(size_t)를 저장할 변수
	int addrlen;
	
	char buf[BUFSIZE + 1];
	int len;

	while (1) {
		// accept()
		//메모리 사이즈를 미리 받아둠
		addrlen = sizeof(clientaddr);
		//클라이언트 접속요청을 허용하며, 두 단말간 통신을 위하여 새로운 소켓을 생성(위의 정의한 client_sock에 addrlen 만큼 담음)
		client_sock = accept(listen_sock, (SOCKADDR *)&clientaddr, &addrlen);
		if (client_sock == INVALID_SOCKET) {
			err_display("accept()");
			break;
		}

		// 접속한 클라이언트 정보 출력
		// hton의 반대로 Network to host 에디안 변환 실행 후 출력

			// inet_ntoa 의 경우,C4996 보안 경고(Level 3)를 발생시킵니다.
			// 해당 컴파일러 경고는 strcpy, memcpy 등 buffer overread / buffer overflow 등의 메모리 보안을 침해하는 경우 등에서 자주 확인할 수 있습니다.

			// inet_ntoa 는 원래 구현상 정적으로 할당된 char 버퍼를 사용하므로, Thread Safe 하지 않습니다.
			// ms와 glibc는 버퍼쓰레드를 로컬로 만들어 해결했지만,
			// macOS 등 일부 os에서는 그렇지 않기 때문에 해당 경고를 출력하는것으로 생각됩니다.

			// 따라서 이를 현재 작성단계에서 해소하기 위해 7번 줄의 전처리문이 추가되었습니다 
			// 다른 방법으로는, 프로젝트의 SDL 검사를 해제하면 됩니다. 다만 권장하는 방법은 inet_ntop을 사용하는 방법입니다.
			// 이후 이 inet_ntoa의 C4996관련 주석은 추가하지 않습니다.
		printf("\n[TCP 서버] 클라이언트 접속: IP 주소=%s, 포트 번호=%d\n",
			inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

		// 클라이언트와 데이터 통신
		while (1) {
			// 데이터 받기(고정 길이)
			// 데이터 사이즈를 int형 만큼 받아, 버퍼에서 받은 길이 정보를 정수형 메모리(char *형으로 캐스팅된)에 삽입
			retval = recvn(client_sock, (char *)&len, sizeof(int), 0);
			if (retval == SOCKET_ERROR) {
				err_display("recv()");
				break;
			}
			else if (retval == 0)
				break;

			// 데이터 받기(가변 길이)
			//사용자정의 수신함수 - 길이를 받은 만큼 수신, 메모리 대입 실행
			retval = recvn(client_sock, buf, len, 0);
			if (retval == SOCKET_ERROR) {
				err_display("recv()");
				break;
			}
			else if (retval == 0)
				break;

			// 받은 데이터 출력
			buf[retval] = '\0';
			printf("[TCP/%s:%d] %s\n", inet_ntoa(clientaddr.sin_addr),
				ntohs(clientaddr.sin_port), buf);
		}

		// closesocket()
		// accept에서 새로 생성된 클라이언트-서버간 소켓 종료
		closesocket(client_sock);
		printf("[TCP 서버] 클라이언트 종료: IP 주소=%s, 포트 번호=%d\n",
			inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
	}

	// closesocket()
	// accept을 위해 생성된 listening 소켓 종료
	closesocket(listen_sock);

	// 윈속 종료
	WSACleanup();
	return 0;
}