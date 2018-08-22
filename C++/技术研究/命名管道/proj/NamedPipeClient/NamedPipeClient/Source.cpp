#include <iostream>
#include <Windows.h>
#include <stdio.h>
#include <string>
using namespace std;

#define PIPE_NAME "\\\\.\\Pipe\\test"

void ClientFun()
{
	char buffer[1024];
	DWORD WriteNum;

	if (WaitNamedPipe(PIPE_NAME, NMPWAIT_WAIT_FOREVER) == FALSE) {
		cout << "Wait pipe failed." << endl;
		return;
	}

	HANDLE hPipe = CreateFile(PIPE_NAME, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hPipe == INVALID_HANDLE_VALUE) {
		cout << "Create pipe failed." << endl;
		CloseHandle(hPipe);
		return;
	}
	else
		cout << "Connect successfully." << endl;
	while (1) {
		string str;
		getline(cin, str);
		strcpy_s(buffer, 1024, str.c_str());
		if (WriteFile(hPipe, buffer, strlen(buffer), &WriteNum, NULL) == FALSE) {
			cout << "Write to pipe failed." << endl;
			break;
		}
	}
	
	cout << "Close pipe." << endl;
	CloseHandle(hPipe);
	return;
}

int main()
{
	ClientFun();

	system("pause");
}