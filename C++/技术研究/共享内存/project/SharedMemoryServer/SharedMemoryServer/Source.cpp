#include <iostream>
#include <string>
#include <chrono>
#include "SharedMemoryServer.h"
using namespace std;

int main()
{
	SharedMemoryServer *pserver = SharedMemoryServer::GetInstance();
	pserver->Start();

	while (true)
	{
		char c;
		cin >> c;
		if (c == 'q')
			break;
	}

	pserver->ShutDown();
}