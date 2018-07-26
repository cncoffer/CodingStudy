#include "SharedMemoryClient.h"

SharedMemoryClient::SharedMemoryClient(std::string strMapName, int FileSize) :
	m_hMap(nullptr), m_lpMap(nullptr), m_strMapName(strMapName), m_nFileSize(FileSize)
{
}


SharedMemoryClient::~SharedMemoryClient()
{
}

bool SharedMemoryClient::SendMessageToServer(std::string strMsg)
{
	if (!OpenMapFile() || !m_lpMap)
		return false;

	sprintf((char*)m_lpMap, "%s", strMsg.c_str());

	CloseMapFile();

	return true;
}

bool SharedMemoryClient::OpenMapFile()
{
	HANDLE hMap = nullptr;
	if (hMap = OpenFileMapping(FILE_MAP_WRITE, FALSE, m_strMapName.c_str()))
	{
		LPBYTE lpMap;
		if (lpMap = (LPBYTE)MapViewOfFile(hMap, FILE_MAP_WRITE, 0, 0, m_nFileSize))
		{
			m_hMap = hMap;
			m_lpMap = lpMap;
		}
		else
		{
			CloseHandle(hMap);
		}
	}

	return m_lpMap != nullptr;
}

void SharedMemoryClient::CloseMapFile()
{
	if (m_lpMap)
	{
		UnmapViewOfFile(m_lpMap);
		CloseHandle(m_hMap);
	}
	m_lpMap = nullptr;
	m_hMap = nullptr;
}
