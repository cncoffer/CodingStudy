#include <iostream>
#include <Windows.h>
using namespace std;

#define PIPE_NAME "\\\\.\\Pipe\\test"

void ServerFun()
{
	char buffer[1024];
	DWORD ReadNum;
	HANDLE hPipe = CreateNamedPipe(PIPE_NAME, PIPE_ACCESS_DUPLEX, PIPE_TYPE_BYTE | PIPE_READMODE_BYTE, 1, 0, 0, 1000, NULL);
	if (hPipe == INVALID_HANDLE_VALUE) {
		cout << "Create pipe failed." << endl;
		CloseHandle(hPipe);
		return;
	}

	if (ConnectNamedPipe(hPipe, NULL) == FALSE) {
		cout << "Connect failed." << endl;
		CloseHandle(hPipe);
		return;
	}
	else
		cout << "Connect successfully." << endl;

	while (1) {
		if (ReadFile(hPipe, buffer, 1024, &ReadNum, NULL) == FALSE) {
			cout << "Read buffer failed." << endl;
			break;
		}

		buffer[ReadNum] = '\0';
		cout << "buffer: " << buffer << endl;
	}

	cout << "Close pipe." << endl;
	CloseHandle(hPipe);
}

int main()
{
	ServerFun();

	system("pause");
}