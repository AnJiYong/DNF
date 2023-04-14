#pragma once
#include <Windows.h>
#include <process.h>

class GameEngineIocp
{
public:
	GameEngineIocp()
		: IocpHandle(nullptr)
	{
	}

	~GameEngineIocp() 
	{
	}

public:
	bool Start(int ThreadCount = 0)
	{
		IocpHandle = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, ThreadCount);

		if (nullptr == IocpHandle)
		{
			return false;
		}

		return true;
	}

	BOOL WaitforWork(DWORD& lpNumberOfBytesTransferred, ULONG_PTR& lpCompletionKey, LPOVERLAPPED& lpOverlapped, DWORD dwMilliseconds = INFINITE)
	{
		return GetQueuedCompletionStatus(IocpHandle, &lpNumberOfBytesTransferred, &lpCompletionKey, &lpOverlapped, dwMilliseconds);
	}

	BOOL Makework(DWORD _WorkParameter, void* _Ptr)
	{
		return PostQueuedCompletionStatus(IocpHandle, _WorkParameter, reinterpret_cast<ULONG_PTR>(_Ptr), nullptr);
	}

private:
	HANDLE IocpHandle;
};

