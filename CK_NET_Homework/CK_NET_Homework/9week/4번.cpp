#include <windows.h>
#include <stdio.h>
#define MAXCNT 100000000


int g_count = 0;

//�Ӱ豸����ü 
//���ؽ��� ������� ���� ��ȣ���� ����ȭ�� ���� ���� �� ������, ȿ������ ��Ŀ���� ����.
//�ѹ��� �ϳ��� �����常 ������ �� �����Ƿ� ���� �׼����κ��� ��ȣ�ϴµ� ����
CRITICAL_SECTION cs;

// DWORD WINAPI <functionName> (LPVOID <argsName>)
// void* ������ ���� �ڷ� �ѱ���
DWORD WINAPI MyThread1(LPVOID arg)
{
	for (int i = 0; i < MAXCNT; i++) 
	{
		//�Ӱ豸����ü�� ������ ���. �������� �ο��Ǹ� �Լ��� ���ϵ�	
		//lpCriticalSection(&cs) = �Ӱ豸�� ��ü�� ���� ��������
		//����۾��ð� �ʰ��� EXCEPTION_POSSIBLE_DEADLOCK �߻� ����.
		//���� �Ӱ豸������ �������� LeaveCriticalSection�� �ѹ� ȣ���ؾ���.
		EnterCriticalSection(&cs);

		g_count += 2;

		// �Ӱ豸����ü�� ������ ����. 
		//lpCriticalSection(&cs) = �Ӱ豸�� ��ü�� ���� ��������
		//�����尡 ������ �Ӱ豸�� ������Ʈ�� �������� ���� ��, 
			//�ش��Լ��� ȣ���ϸ� EnterCriticalSection�� ����ϴ� �ٸ� �����尡 ���� ����� �� �ִ� ���� �߻�
		LeaveCriticalSection(&cs);
	}
	return 0;
}

// DWORD WINAPI <functionName> (LPVOID <argsName>)
// void* ������ ���� �ڷ� �ѱ���
DWORD WINAPI MyThread2(LPVOID arg)
{
	for (int i = 0; i < MAXCNT; i++) 
	{
		//�Ӱ豸����ü�� ������ ���. �������� �ο��Ǹ� �Լ��� ���ϵ�	
		//lpCriticalSection(&cs) = �Ӱ豸�� ��ü�� ���� ��������
		//����۾��ð� �ʰ��� EXCEPTION_POSSIBLE_DEADLOCK �߻� ����.
		//���� �Ӱ豸������ �������� LeaveCriticalSection�� �ѹ� ȣ���ؾ���.
		EnterCriticalSection(&cs);

		g_count -= 2;

		// �Ӱ豸����ü�� ������ ����. 
		//lpCriticalSection(&cs) = �Ӱ豸�� ��ü�� ���� ��������
		//�����尡 ������ �Ӱ豸�� ������Ʈ�� �������� ���� ��, 
			//�ش��Լ��� ȣ���ϸ� EnterCriticalSection�� ����ϴ� �ٸ� �����尡 ���� ����� �� �ִ� ���� �߻�
		LeaveCriticalSection(&cs);
	}
	return 0;
}

int main(int argc, char *argv[])
{
	//�Ӱ豸����ü �ʱ�ȭ
	//lpCriticalSection(&cs) = �Ӱ豸�� ��ü�� ���� ��������
	//Win vista �������� �޸� ������Ȳ���� ���ܸ� �߻��Ͽ�����, vista���ĺ��ʹ� �׻� �����մϴ�.
	InitializeCriticalSection(&cs);

	//������ �ڵ鷯 �迭 ����
	HANDLE hThread[2];
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
	hThread[0] = CreateThread(NULL, 0, MyThread1, NULL, 0, NULL);
	hThread[1] = CreateThread(NULL, 0, MyThread2, NULL, 0, NULL);

	//������ ������Ʈ�� �ϳ� �Ǵ� ��ΰ� ��ȣ���¿� �ְų� �ð��� ����Ҷ����� ���
	//WaitForMultipleObjects
	//		DWORD        nCount,			//�迭�� ����Ű�� �ڵ� ��.0�� ��� �Է��̺�Ʈ���� ���/
	//		const HANDLE *pHandles,			//��ü�ڵ��� �迭�� ���� �ּ�.
	//		BOOL         fWaitAll,			//true �� ��� ������Ʈ�� ��ȣ���°� �� ���(&), false�� �ϳ��� ��ȣ���°� �� ���(|)
	//		DWORD        dwMilliseconds,	//�ð��ʰ�����(ms).INFINITE�� ��� ��ȣ���°� �� �� ���� ���.
	//		DWORD        dwWakeMask			//�Է��̺�Ʈ ��ü���� ����ũ. �ش� ���������� ������� �ʾ����Ƿ� ����
	WaitForMultipleObjects(2, hThread, TRUE, INFINITE);

	//�Ӱ豸����ü���� ����ϴ� ���ҽ� ����.
	DeleteCriticalSection(&cs);

	printf("g_count = %d\n", g_count);
	
	return 0;
}