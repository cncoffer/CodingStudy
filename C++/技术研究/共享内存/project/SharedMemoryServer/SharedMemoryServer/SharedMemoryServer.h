#pragma once
#include <string>
#include <Windows.h>

class SharedMemoryServer
{
public:
	static SharedMemoryServer *GetInstance();
	void Start();
	void ShutDown();

private:
	SharedMemoryServer();
	~SharedMemoryServer();

	static unsigned __stdcall workThread(PVOID p);
	void Init();

	HANDLE m_hFile;
	HANDLE m_hMap;
	bool m_bIsShutDown;
	HANDLE m_hThread;
	static SharedMemoryServer *m_pInstance;
};

