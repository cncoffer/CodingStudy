#include "SharedMemoryServer.h"
#include <process.h>
#include <chrono>
#include <iostream>
using namespace std;

#define MAPNAME "SharedMemoryTestMap"
#define MAPSIZE (512*1024)

SharedMemoryServer * SharedMemoryServer::m_pInstance = nullptr;

SharedMemoryServer::SharedMemoryServer() :
	m_hFile(NULL), m_hMap(NULL), m_bIsShutDown(false), m_hThread(NULL)
{
}


SharedMemoryServer::~SharedMemoryServer()
{
}

void SharedMemoryServer::Init()
{
	char folderName[MAX_PATH];
	::GetTempPath(MAX_PATH, folderName);

	string strFileName(folderName);
	strFileName += MAPNAME;

	if ((m_hFile = CreateFile(strFileName.c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL,
		CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL)) == NULL)
		return;
	
	m_hMap = CreateFileMapping(m_hFile, NULL, PAGE_READWRITE, 0, MAPSIZE, MAPNAME);
	if (m_hMap != NULL && GetLastError() == ERROR_ALREADY_EXISTS)
	{
		CloseHandle(m_hFile);
		m_hFile = NULL;
		CloseHandle(m_hMap);
		m_hMap = NULL;
	}
}

void SharedMemoryServer::Start()
{
	Init();
	m_bIsShutDown = false;
	m_hThread = (HANDLE)_beginthreadex(NULL, 0, workThread, m_pInstance, 0, NULL);
}

void SharedMemoryServer::ShutDown()
{
	m_bIsShutDown = true;
}

SharedMemoryServer *SharedMemoryServer::GetInstance()
{
	if (!m_pInstance)
		m_pInstance = new SharedMemoryServer();
	return m_pInstance;
}

unsigned __stdcall SharedMemoryServer::workThread(PVOID p)
{
	SharedMemoryServer *pInst = (SharedMemoryServer*)p;

	if (pInst->m_hMap == NULL)
		return 0;

	while (!pInst->m_bIsShutDown)
	{
		char *pText = nullptr;
		pText = (char*)MapViewOfFile(pInst->m_hMap, FILE_MAP_ALL_ACCESS, 0, 0, MAPSIZE);
		string temp(pText);
		if (!temp.empty() && temp != "0")
		{
			time_t time = chrono::system_clock::to_time_t(chrono::system_clock::now());
			string strTime = ctime(&time);
			strTime.resize(strTime.size() - 1);

			cout << "[receive time : " + strTime + "]\n\t" << temp << endl;
		}

		sprintf(pText, "0");
		UnmapViewOfFile((LPCVOID)pText);
		Sleep(100);
	}

	return 0;
}
