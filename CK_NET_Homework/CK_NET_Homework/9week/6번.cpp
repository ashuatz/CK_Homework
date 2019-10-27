#include <windows.h>
#include <stdio.h>
#define BUFSIZE 10

//�ڵ� ������ ����
HANDLE hReadEvent;
HANDLE hWriteEvent;
int buf[BUFSIZE];

// DWORD WINAPI <functionName> (LPVOID <argsName>)
// void* ������ ���� �ڷ� �ѱ���
DWORD WINAPI WriteThread(LPVOID arg)
{
	DWORD retval;

	for (int k = 1; k <= 500; k++) 
	{
		//ReadEvent�� ��ȣ���°� �ɶ����� ���. (= �бⰡ ���� �������� ��ȣ���� ���� ���� ���.)
		//���ð��� ������ INFINITE�̹Ƿ� ��ȣ���°� �ɶ����� ���.
		//ReadEvent�� maunal�� �ƴϹǷ� �ڵ����� Reset.
		//��ȯ�� : 
		//	WAIT_OBJECT_0 : ������ü�� ���°� ��ȣȭ��.
		//  WAIT_TIMEOUT : �ð����ᰣ���� ���. ������Ʈ�� ��ȣȭ ���� ����.
		//	WAIT_FAILED : ����
		retval = WaitForSingleObject(hReadEvent, INFINITE);

		// ������ ������Ʈ�� ��ȣ ���°� ���� �ʾ��� ��� �Լ��� ����.
		if (retval != WAIT_OBJECT_0) 
		{
			break;
		}

		// �������ۿ� k������ ä��
		for (int i = 0; i < BUFSIZE; i++)
			buf[i] = k;

		// hWriteEvent �̺�Ʈ�� ��ȣ ���·� ����
		SetEvent(hWriteEvent);
	}
	return 0;
}

// DWORD WINAPI <functionName> (LPVOID <argsName>)
// void* ������ ���� �ڷ� �ѱ���
DWORD WINAPI ReadThread(LPVOID arg)
{
	DWORD retval;

	while (1) 
	{
		//WriteEvent�� ��ȣ���°� �ɶ����� ���. (= �бⰡ ���� �������� ��ȣ���� ���� ���� ���.)
		//���ð��� ������ INFINITE�̹Ƿ� ��ȣ���°� �ɶ����� ���.
		//WriteEvent�� maunal�� �ƴϹǷ� �ڵ����� Reset.
		//��ȯ�� : 
		//	WAIT_OBJECT_0 : ������ü�� ���°� ��ȣȭ��.
		//  WAIT_TIMEOUT : �ð����ᰣ���� ���. ������Ʈ�� ��ȣȭ ���� ����.
		//	WAIT_FAILED : ����
		retval = WaitForSingleObject(hWriteEvent, INFINITE);

		// ������ ������Ʈ�� ��ȣ ���°� ���� �ʾ��� ��� �Լ��� ����.
		if (retval != WAIT_OBJECT_0) 
		{
			break;
		}
		
		//���
		printf("Thread %4d: ", GetCurrentThreadId());
		for (int i = 0; i < BUFSIZE; i++)
		{
			printf("%3d ", buf[i]);
		}
		printf("\n");

		// ���� �ʱ�ȭ
		ZeroMemory(buf, sizeof(buf));

		// hReadEvent �̺�Ʈ�� ��ȣ ���·� ����
		SetEvent(hReadEvent);
	}
	return 0;
}

int main(int argc, char *argv[])
{
	// �̺�Ʈ. 
	// CreateEvent:
	// 		LPSECURITY_ATTRIBUTES lpEventAttributes, 	// �ڵ��� ��Ӽ� ��Ʈ����Ʈ ������. NULL�� ��� ���� ���μ����� ���� ��ӵ� �� ����.
	// 		BOOL                  bManualReset,			// �̺�Ʈ �ʱ�ȭ�� �������� �Ұ�����(true, manual) �ڵ����� �Ұ�����(false,auto)�� ���� ��
	// 		BOOL                  bInitialState,		// �ʱ����. ��ȣ����(true), ���ȣ����(false)�� ������ ��.
	// 		LPCSTR                lpName				// �̺�Ʈ ��ü�� �̸�.
	// ������ ���� �̺�Ʈ ��ü�� �����ϰų�, ���� �̺�Ʈ ��ü�� �����Լ�.
	// ��ȯ���� �̺�Ʈ ������Ʈ�� ���� �ڵ�. �̹� ������ �̸�(lpName�� �ش��ϴ�) �� �ִٸ�,���� �ڵ��� ��ȯ�ϸ� GetLastError���� ErrorAlready_Exists�� ��ȯ�մϴ�.
	hWriteEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	if (hWriteEvent == NULL) return 1;
	hReadEvent = CreateEvent(NULL, FALSE, TRUE, NULL);
	if (hReadEvent == NULL) return 1;

	//"ȣ�� ���μ���"�� ���� �ּ� ���� ������ ������ ������ �ۼ�
	//CreateThread
	// 		LPSECURITY_ATTRIBUTES   lpThreadAttributes, // ��Ʈ����Ʈ. ��ȯ�� �ڵ��� �ڽ� ���μ����� ���� ��ӵ� �� �ִ��� ���θ� ����. NULL�� ��ӺҰ�
	// 		SIZE_T                  dwStackSize,		// ����ũ��. 0�� ��� 
	// 		LPTHREAD_START_ROUTINE  lpStartAddress,		// �����尡 ������ �Լ��� ���� ������. �������� �����ּ�
	// 		__drv_aliasesMem LPVOID lpParameter,		// �����忡 ���޵� ������ ���� ������. void* ���̱⶧���� �������� ���ڴ� struct�� ���� �ش� struct*�������� ����
	// 		DWORD                   dwCreationFlags,	// ������ ������ �����ϴ� �÷���. CREATE_SUSPENDED - �Ͻ� �ߴܻ��·� ����, ResumeThread ȣ��ñ��� ����ȵ�. 
	// 		LPDWORD                 lpThreadId			// ������ �ĺ��ڸ� �޴� ������ ���� ������. NULL�̸� ID�� ��ȯ���� ����.
	//�Լ��� �����ϸ� �ش� �����忡 ���� �ڵ��� ��ȯ. ���н� NULL��ȯ, ������ ���ؼ��� GetLastError�� ȣ���ؼ� Ȯ�� ����.
	//������ �����ص� �����ּҰ� ��ȿ���� �ʴٸ�, ���ܹ߻����� ������ �����. �� �߸��� �ּҷε� ������ ������ü�� ����
	HANDLE hThread[3];
	hThread[0] = CreateThread(NULL, 0, WriteThread, NULL, 0, NULL);
	hThread[1] = CreateThread(NULL, 0, ReadThread, NULL, 0, NULL);
	hThread[2] = CreateThread(NULL, 0, ReadThread, NULL, 0, NULL);

	//������ ������Ʈ�� �ϳ� �Ǵ� ��ΰ� ��ȣ���¿� �ְų� �ð��� ����Ҷ����� ���
	//WaitForMultipleObjects
	//		DWORD        nCount,			//�迭�� ����Ű�� �ڵ� ��.0�� ��� �Է��̺�Ʈ���� ���/
	//		const HANDLE *pHandles,			//��ü�ڵ��� �迭�� ���� �ּ�.
	//		BOOL         fWaitAll,			//true �� ��� ������Ʈ�� ��ȣ���°� �� ���(&), false�� �ϳ��� ��ȣ���°� �� ���(|)
	//		DWORD        dwMilliseconds,	//�ð��ʰ�����(ms).INFINITE�� ��� ��ȣ���°� �� �� ���� ���.
	//		DWORD        dwWakeMask			//�Է��̺�Ʈ ��ü���� ����ũ. �ش� ���������� ������� �ʾ����Ƿ� ����
	WaitForMultipleObjects(3, hThread, TRUE, INFINITE);

	// �̺�Ʈ ����
	CloseHandle(hWriteEvent);
	CloseHandle(hReadEvent);
	return 0;
}