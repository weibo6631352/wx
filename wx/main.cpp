#include "wx.h"
#include <QtWidgets/QApplication>
#include <QDate>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include "QWxLogin.h"
#include "QwxSetting.h"
#include <windows.h>
#include <SHA1.h>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
 
 	int len = 256;
 	char lpszCpu[256] = { 0 };
 	using namespace std;
 	const long MAX_COMMAND_SIZE = 10000; // 命令行输出缓冲大小	
 	WCHAR szFetCmd[] = L"wmic cpu get processorid"; // 获取CPU序列号命令行	
 	const string strEnSearch = "ProcessorId"; // CPU序列号的前导信息
 
 	BOOL   bret = FALSE;
 	HANDLE hReadPipe = NULL; //读取管道
 	HANDLE hWritePipe = NULL; //写入管道	
 	PROCESS_INFORMATION pi;   //进程信息	
 	STARTUPINFO			si;	  //控制命令行窗口信息
 	SECURITY_ATTRIBUTES sa;   //安全属性
 
 	char			szBuffer[MAX_COMMAND_SIZE + 1] = { 0 }; // 放置命令行结果的输出缓冲区
 	string			strBuffer;
 	unsigned long	count = 0;
 	long			ipos = 0;
 
 	memset(&pi, 0, sizeof(pi));
 	memset(&si, 0, sizeof(si));
 	memset(&sa, 0, sizeof(sa));
 
 	pi.hProcess = NULL;
 	pi.hThread = NULL;
 	si.cb = sizeof(STARTUPINFO);
 	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
 	sa.lpSecurityDescriptor = NULL;
 	sa.bInheritHandle = TRUE;
 
 	//1.0 创建管道
 	bret = CreatePipe(&hReadPipe, &hWritePipe, &sa, 0);
 	if (!bret)
 	{
 		goto END;
 	}
 
 	//2.0 设置命令行窗口的信息为指定的读写管道
 	GetStartupInfo(&si);
 	si.hStdError = hWritePipe;
 	si.hStdOutput = hWritePipe;
 	si.wShowWindow = SW_HIDE; //隐藏命令行窗口
 	si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
 
 	//3.0 创建获取命令行的进程
 	bret = CreateProcess(NULL, szFetCmd, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi);
 	if (!bret)
 	{
 		goto END;
 	}
 
 	//4.0 读取返回的数据
 	WaitForSingleObject(pi.hProcess, 500/*INFINITE*/);
 	bret = ReadFile(hReadPipe, szBuffer, MAX_COMMAND_SIZE, &count, 0);
 	if (!bret)
 	{
 		goto END;
 	}
 
 	//5.0 查找CPU序列号
 	bret = FALSE;
 	strBuffer = szBuffer;
 	ipos = strBuffer.find(strEnSearch);
 
 	if (ipos < 0) // 没有找到
 	{
 		goto END;
 	}
 	else
 	{
 		strBuffer = strBuffer.substr(ipos + strEnSearch.length());
 	}
 
 	memset(szBuffer, 0x00, sizeof(szBuffer));
 	strcpy_s(szBuffer, strBuffer.c_str());
 
 	//去掉中间的空格 
 
 
 	int j = 0;
 	for (int i = 0; i < strlen(szBuffer); i++)
 	{
 		if (szBuffer[i] == '\t' || szBuffer[i] == ' ' || szBuffer[i] == '\r' || szBuffer[i] == '\n')
 		{
 			lpszCpu[j] = '0';
 		}
 		else
 		{
 			lpszCpu[j] = szBuffer[i];
 		}
 		j++;
 	}
 
 	bret = TRUE;
 
 END:
 	//关闭所有的句柄
 	CloseHandle(hWritePipe);
 	CloseHandle(hReadPipe);
 	CloseHandle(pi.hProcess);
 	CloseHandle(pi.hThread);
 
 
 	CSHA1 sha1;
 	sha1.Update((unsigned char*)lpszCpu, strlen(lpszCpu));
 	sha1.Final();
 	unsigned char chSha1[20] = "";
 	sha1.GetHash(chSha1);
 	
 	QFile f(QApplication::applicationDirPath() + "/wx.key");
 	if (!f.open(QIODevice::ReadOnly))
 	{
		QMessageBox::information(0, QStringLiteral("没有找到授权文件"), "wx.key", 0);
		return 0;
 	}

	char *chSha2 = new char [20];
	uint sha2len = 20;
	


	char  buffer = 0;
	int i = 0;
	while (f.read(&buffer, 1) != 0)
	{
		if ((unsigned char)buffer != chSha1[i] || i >= 20)
		{
			QMessageBox::information(0, QStringLiteral("授权失败，请联系管理员"), "wx.key", 0);
			f.close();
			return 0;
		}
		++i;
	}
	f.close();
	

	QWxLogin login;
	if (login.exec() == QDialog::Accepted)
	{
		QwxSetting * wx_setting = QwxSetting::ins();
		if (wx_setting->ReadIni())
		{
			wx_setting->date_ = login.date_;
			wx_setting->session_ = login.session_;
			wx w;
			w.show();
			return a.exec();
		}
	}

	return 0;
}
