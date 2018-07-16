#include <iostream>
#include <chrono>
#include "SharedMemoryClient.h"
using namespace std;

#define MAPNAME "SharedMemoryTestMap"
#define MAPSIZE (512*1024)

int main()
{
	SharedMemoryClient client(MAPNAME, MAPSIZE);
	while (true)
	{
		string strText;
		cin >> strText;

		time_t time = chrono::system_clock::to_time_t(chrono::system_clock::now());
		string strTime = ctime(&time);
		strTime.resize(strTime.size() - 1);
		if (client.SendMessageToServer("[ send time : " + strTime + "]\tmsg: " + strText))
			cout << "<suc> send msg successfully." << endl;
		else
			cout << "<err> send msg unsuccessfully." << endl;
	}
}