#include <windows.h>
#include <stdio.h>
#define BUFSIZE 10

//핸들 포인터 선언
HANDLE hReadEvent;
HANDLE hWriteEvent;
int buf[BUFSIZE];

// DWORD WINAPI <functionName> (LPVOID <argsName>)
// void* 형식을 통한 자료 넘기기용
DWORD WINAPI WriteThread(LPVOID arg)
{
	DWORD retval;

	for (int k = 1; k <= 500; k++) 
	{
		//ReadEvent가 신호상태가 될때까지 대기. (= 읽기가 끝난 시점에서 신호상태 변경 까지 대기.)
		//대기시간이 있지만 INFINITE이므로 신호상태가 될때까지 대기.
		//ReadEvent가 maunal이 아니므로 자동으로 Reset.
		//반환값 : 
		//	WAIT_OBJECT_0 : 지정객체의 상태가 신호화됨.
		//  WAIT_TIMEOUT : 시간종료간격이 경과. 오브젝트는 신호화 되지 않음.
		//	WAIT_FAILED : 실패
		retval = WaitForSingleObject(hReadEvent, INFINITE);

		// 지정한 오브젝트가 신호 상태가 되지 않았을 경우 함수를 종료.
		if (retval != WAIT_OBJECT_0) 
		{
			break;
		}

		// 전역버퍼에 k값으로 채움
		for (int i = 0; i < BUFSIZE; i++)
			buf[i] = k;

		// hWriteEvent 이벤트를 신호 상태로 설정
		SetEvent(hWriteEvent);
	}
	return 0;
}

// DWORD WINAPI <functionName> (LPVOID <argsName>)
// void* 형식을 통한 자료 넘기기용
DWORD WINAPI ReadThread(LPVOID arg)
{
	DWORD retval;

	while (1) 
	{
		//WriteEvent가 신호상태가 될때까지 대기. (= 읽기가 끝난 시점에서 신호상태 변경 까지 대기.)
		//대기시간이 있지만 INFINITE이므로 신호상태가 될때까지 대기.
		//WriteEvent가 maunal이 아니므로 자동으로 Reset.
		//반환값 : 
		//	WAIT_OBJECT_0 : 지정객체의 상태가 신호화됨.
		//  WAIT_TIMEOUT : 시간종료간격이 경과. 오브젝트는 신호화 되지 않음.
		//	WAIT_FAILED : 실패
		retval = WaitForSingleObject(hWriteEvent, INFINITE);

		// 지정한 오브젝트가 신호 상태가 되지 않았을 경우 함수를 종료.
		if (retval != WAIT_OBJECT_0) 
		{
			break;
		}
		
		//출력
		printf("Thread %4d: ", GetCurrentThreadId());
		for (int i = 0; i < BUFSIZE; i++)
		{
			printf("%3d ", buf[i]);
		}
		printf("\n");

		// 버퍼 초기화
		ZeroMemory(buf, sizeof(buf));

		// hReadEvent 이벤트를 신호 상태로 설정
		SetEvent(hReadEvent);
	}
	return 0;
}

int main(int argc, char *argv[])
{
	// 이벤트. 
	// CreateEvent:
	// 		LPSECURITY_ATTRIBUTES lpEventAttributes, 	// 핸들의 상속성 어트리뷰트 포인터. NULL일 경우 하위 프로세스에 의해 상속될 수 없음.
	// 		BOOL                  bManualReset,			// 이벤트 초기화를 수동으로 할것인지(true, manual) 자동으로 할것인지(false,auto)에 대한 값
	// 		BOOL                  bInitialState,		// 초기상태. 신호상태(true), 비신호상태(false)를 설정할 값.
	// 		LPCSTR                lpName				// 이벤트 객체의 이름.
	// 명명되지 않은 이벤트 객체를 생성하거나, 명명된 이벤트 객체를 여는함수.
	// 반환값은 이벤트 오브젝트에 대한 핸들. 이미 지정된 이름(lpName에 해당하는) 이 있다면,기존 핸들을 반환하며 GetLastError에서 ErrorAlready_Exists를 반환합니다.
	hWriteEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	if (hWriteEvent == NULL) return 1;
	hReadEvent = CreateEvent(NULL, FALSE, TRUE, NULL);
	if (hReadEvent == NULL) return 1;

	//"호출 프로세스"의 가상 주소 공간 내에서 실행할 스레드 작성
	//CreateThread
	// 		LPSECURITY_ATTRIBUTES   lpThreadAttributes, // 어트리뷰트. 반환된 핸들이 자식 프로세스에 의해 상속될 수 있는지 여부를 결정. NULL시 상속불가
	// 		SIZE_T                  dwStackSize,		// 스택크기. 0일 경우 
	// 		LPTHREAD_START_ROUTINE  lpStartAddress,		// 스레드가 실행할 함수에 대한 포인터. 스레드의 시작주소
	// 		__drv_aliasesMem LPVOID lpParameter,		// 스레드에 전달될 변수에 대한 포인터. void* 형이기때문에 여러개의 인자는 struct로 묶어 해당 struct*형식으로 전달
	// 		DWORD                   dwCreationFlags,	// 스레드 생성을 제어하는 플래그. CREATE_SUSPENDED - 일시 중단상태로 시작, ResumeThread 호출시까지 실행안됨. 
	// 		LPDWORD                 lpThreadId			// 스레드 식별자를 받는 변수에 대한 포인터. NULL이면 ID가 반환되지 않음.
	//함수가 성공하면 해당 스레드에 대한 핸들을 반환. 실패시 NULL반환, 오류에 대해서는 GetLastError를 호출해서 확인 가능.
	//생성이 성공해도 시작주소가 유효하지 않다면, 예외발생이후 스레드 종료됨. 즉 잘못된 주소로도 스레드 생성자체는 가능
	HANDLE hThread[3];
	hThread[0] = CreateThread(NULL, 0, WriteThread, NULL, 0, NULL);
	hThread[1] = CreateThread(NULL, 0, ReadThread, NULL, 0, NULL);
	hThread[2] = CreateThread(NULL, 0, ReadThread, NULL, 0, NULL);

	//지정된 오브젝트중 하나 또는 모두가 신호상태에 있거나 시간이 경과할때까지 대기
	//WaitForMultipleObjects
	//		DWORD        nCount,			//배열이 가리키는 핸들 수.0일 경우 입력이벤트만을 대기/
	//		const HANDLE *pHandles,			//객체핸들의 배열의 시작 주소.
	//		BOOL         fWaitAll,			//true 시 모든 오브젝트가 신호상태가 될 경우(&), false시 하나라도 신호상태가 될 경우(|)
	//		DWORD        dwMilliseconds,	//시간초과간격(ms).INFINITE의 경우 신호상태가 될 때 까지 대기.
	//		DWORD        dwWakeMask			//입력이벤트 객체관련 마스크. 해당 과제에서는 사용하지 않았으므로 생략
	WaitForMultipleObjects(3, hThread, TRUE, INFINITE);

	// 이벤트 제거
	CloseHandle(hWriteEvent);
	CloseHandle(hReadEvent);
	return 0;
}