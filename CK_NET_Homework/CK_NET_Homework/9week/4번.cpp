#include <windows.h>
#include <stdio.h>
#define MAXCNT 100000000


int g_count = 0;

//임계구역객체 
//뮤텍스와 세마포어에 비해 상호배제 동기화를 위한 조금 더 빠르고, 효율적인 메커니즘 제공.
//한번에 하나의 스레드만 소유할 수 있으므로 동시 액세스로부터 보호하는데 유용
CRITICAL_SECTION cs;

// DWORD WINAPI <functionName> (LPVOID <argsName>)
// void* 형식을 통한 자료 넘기기용
DWORD WINAPI MyThread1(LPVOID arg)
{
	for (int i = 0; i < MAXCNT; i++) 
	{
		//임계구역객체의 소유권 대기. 소유권이 부여되면 함수가 리턴됨	
		//lpCriticalSection(&cs) = 임계구역 객체에 대한 롱포인터
		//대기작업시간 초과시 EXCEPTION_POSSIBLE_DEADLOCK 발생 가능.
		//이후 임계구역으로 들어갈때마다 LeaveCriticalSection를 한번 호출해야함.
		EnterCriticalSection(&cs);

		g_count += 2;

		// 임계구역객체의 소유권 해제. 
		//lpCriticalSection(&cs) = 임계구역 객체에 대한 롱포인터
		//스레드가 지정된 임계구역 오브젝트의 소유권이 없을 때, 
			//해당함수를 호출하면 EnterCriticalSection를 사용하는 다른 쓰레드가 무한 대기할 수 있는 오류 발생
		LeaveCriticalSection(&cs);
	}
	return 0;
}

// DWORD WINAPI <functionName> (LPVOID <argsName>)
// void* 형식을 통한 자료 넘기기용
DWORD WINAPI MyThread2(LPVOID arg)
{
	for (int i = 0; i < MAXCNT; i++) 
	{
		//임계구역객체의 소유권 대기. 소유권이 부여되면 함수가 리턴됨	
		//lpCriticalSection(&cs) = 임계구역 객체에 대한 롱포인터
		//대기작업시간 초과시 EXCEPTION_POSSIBLE_DEADLOCK 발생 가능.
		//이후 임계구역으로 들어갈때마다 LeaveCriticalSection를 한번 호출해야함.
		EnterCriticalSection(&cs);

		g_count -= 2;

		// 임계구역객체의 소유권 해제. 
		//lpCriticalSection(&cs) = 임계구역 객체에 대한 롱포인터
		//스레드가 지정된 임계구역 오브젝트의 소유권이 없을 때, 
			//해당함수를 호출하면 EnterCriticalSection를 사용하는 다른 쓰레드가 무한 대기할 수 있는 오류 발생
		LeaveCriticalSection(&cs);
	}
	return 0;
}

int main(int argc, char *argv[])
{
	//임계구역객체 초기화
	//lpCriticalSection(&cs) = 임계구역 객체에 대한 롱포인터
	//Win vista 이전에는 메모리 부족상황에서 예외를 발생하였지만, vista이후부터는 항상 성공합니다.
	InitializeCriticalSection(&cs);

	//스레드 핸들러 배열 생성
	HANDLE hThread[2];
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
	hThread[0] = CreateThread(NULL, 0, MyThread1, NULL, 0, NULL);
	hThread[1] = CreateThread(NULL, 0, MyThread2, NULL, 0, NULL);

	//지정된 오브젝트중 하나 또는 모두가 신호상태에 있거나 시간이 경과할때까지 대기
	//WaitForMultipleObjects
	//		DWORD        nCount,			//배열이 가리키는 핸들 수.0일 경우 입력이벤트만을 대기/
	//		const HANDLE *pHandles,			//객체핸들의 배열의 시작 주소.
	//		BOOL         fWaitAll,			//true 시 모든 오브젝트가 신호상태가 될 경우(&), false시 하나라도 신호상태가 될 경우(|)
	//		DWORD        dwMilliseconds,	//시간초과간격(ms).INFINITE의 경우 신호상태가 될 때 까지 대기.
	//		DWORD        dwWakeMask			//입력이벤트 객체관련 마스크. 해당 과제에서는 사용하지 않았으므로 생략
	WaitForMultipleObjects(2, hThread, TRUE, INFINITE);

	//임계구역객체에서 사용하는 리소스 해제.
	DeleteCriticalSection(&cs);

	printf("g_count = %d\n", g_count);
	
	return 0;
}