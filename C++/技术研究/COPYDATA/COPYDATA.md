# 8-22

发送消息
	void CclientDlg::OnBnClickedButton1()
	{
		// TODO: Add your control notification handler code here
		HWND receiveWindow = ::FindWindow(NULL, L"Server");
		if (!receiveWindow)
			return;
	
		CString sendData;
		GetDlgItemText(IDC_EDIT1, sendData);
		COPYDATASTRUCT cds;
		cds.dwData = 0;
		cds.lpData = sendData.GetBuffer();
		cds.cbData = (sendData.GetLength()+1)*sizeof(TCHAR);
	
		::SendMessage(receiveWindow, WM_COPYDATA, (WPARAM)GetSafeHwnd(), (LPARAM)&cds);
		sendData.ReleaseBuffer();
	}

接收消息
	BOOL CclientDlg::OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct)
	{
		m_Listbox.InsertString(0, (LPCTSTR)(pCopyDataStruct->lpData));
		return true;
	}

建立了两个可以互相通信的窗口, 名为Server和Client, 其实并没有主客之分.
需要注意的是必须用SendMessage来发送, 否则在另一个进程接收到消息之前, 就有可能将内存释放掉, 导致那边读取不到.
以及, 对于款字符的长度计算`cds.cbData = (sendData.GetLength()+1)*sizeof(TCHAR);`要准确, 否则会出现乱码.
