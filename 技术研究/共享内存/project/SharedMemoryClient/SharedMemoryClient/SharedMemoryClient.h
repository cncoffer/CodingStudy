#pragma once
#include <Windows.h>
#include <string>

class SharedMemoryClient
{
public:
	SharedMemoryClient(std::string strMapName, int FileSize);
	~SharedMemoryClient();
	bool SendMessageToServer(std::string strMsg);

private:
	HANDLE m_hMap;
	LPBYTE m_lpMap;
	std::string m_strMapName;
	int m_nFileSize;

	bool OpenMapFile();
	void CloseMapFile();
};

