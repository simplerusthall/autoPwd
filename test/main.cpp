#include "windows.h"
#include <iostream>
#include <fstream>
#include <tchar.h>
#include <map>
using namespace std;

#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" )

map<CHAR,CHAR>g_map;

int g_cx = 0;
int g_cy = 0;
CHAR g_pwd[MAX_PATH] = { 0 };
HANDLE g_hMutex;

void inputKey(int key) {
    keybd_event(key, 0, 0, 0);
    keybd_event(key, 0, KEYEVENTF_KEYUP, 0);
}

void inputComboKey(int contorl, int key) {
    keybd_event(contorl, 0, 0, 0);
    keybd_event(key, 0, 0, 0);
    keybd_event(key, 0, KEYEVENTF_KEYUP, 0);
    keybd_event(contorl, 0, KEYEVENTF_KEYUP, 0);
}

void input(CHAR ch) {
    if (ch >= 'a' && ch <= 'z') {
        if (::GetKeyState(VK_CAPITAL) > 0) {
            inputKey(VK_CAPITAL);
        }
        inputKey(ch-32);
    }
    else if (ch >= 'A' && ch <= 'Z') {
        if (::GetKeyState(VK_CAPITAL) < 0) {
            inputKey(VK_CAPITAL);
        }
        inputKey(ch);
    }
    else if ( g_map.find(ch) != g_map.end()) {
        auto iter = g_map.find(ch);
        inputComboKey(16, iter->second);
    }
    else if (ch >= '0' && ch <= '9') {
        inputKey(ch);
    }
}

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam)
{
    TCHAR szTitle[MAX_PATH] = { 0 };
    TCHAR szClassName[MAX_PATH] = { 0 };
    ::GetWindowText(hwnd, szTitle, MAX_PATH);
    ::RealGetWindowClass(hwnd, szClassName, MAX_PATH);

    if (wcscmp(szTitle, L"Mango3") == 0 && wcscmp(szClassName, L"Qt5QWindowIcon") == 0)
    {
        if (hwnd == NULL) {
            return TRUE;
        }   
     
        SwitchToThisWindow(hwnd, TRUE);
        RECT r1;
        ::GetWindowRect(hwnd, &r1);
        int width = r1.right - r1.left;
        int hight = r1.bottom - r1.top;

        if (width == g_cx && hight == g_cy) {
            return TRUE;
        }
    
        if (width < 380) {
            return TRUE;
        }
        
       
       // cout << hwnd << "  width:" << width << "   hight:" << hight <<"  "<< r1.left + width / 2 << "  "<< r1.top + hight / 2 << endl;
        
       // DWORD dwThread = GetWindowThreadProcessId(hwnd, NULL);
       // cout <<hwnd <<" " << dwThread << endl;
       // AttachThreadInput(dwThread, GetCurrentThrea
      //  mouse_event(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE, r1.left + width / 2, r1.top + hight / 2, 0, 0);
 //       mouse_event(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, r1.left + width / 2, r1.top + hight / 2, 0, 0);
  //      mouse_event(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, r1.left + width / 2, r1.top + hight / 2, 0, 0);
        Sleep(200);
     
        ::SendMessage(hwnd, SW_SHOW, NULL, 0);
        SetForegroundWindow(hwnd);
       
        for (int i = 0; i < 12; i++) {
            inputKey(8);
        }
       
        int len = strlen(g_pwd);
        //cout << "输入密码为：" << g_pwd << " 长度："<< len << endl;
        for (int i = 0; i < len; i++) {
            input(g_pwd[i]);
        }
   
        inputKey(13);
 
        return FALSE;
       
    }
    return TRUE;
}

int main()
{ 
    g_hMutex = CreateMutex(NULL, FALSE, L"AUTOPWD_Mutex");

    if (g_hMutex && GetLastError() == ERROR_ALREADY_EXISTS)
    {
        MessageBox(NULL,L"大佬，你已经开了，还开，这么想我吗", NULL, MB_OK);
        return 0;
    }

    
    g_map['!'] = '1';
    g_map['@'] = '2';
    g_map['#'] = '3';
    g_map['$'] = '4';
    g_map['%'] = '5';
    g_map['^'] = '6';
    g_map['&'] = '7';
    g_map['*'] = '8';
    g_map['('] = '9';
    g_map[')'] = '0';
    g_map['_'] = '-';
    g_map['+'] = '=';

    g_cx = ::GetSystemMetrics(SM_CXSCREEN);
    g_cy = ::GetSystemMetrics(SM_CYSCREEN);

    //::EnumWindows(EnumWindowsProc, NULL);
  //  system("pause");
     //添加以下代码
    HKEY   hKey;
    TCHAR pFileName[MAX_PATH] = { 0 };
    DWORD dwRet = ::GetModuleFileName(NULL, pFileName, MAX_PATH);
    LPCTSTR lpRun = L"Software\\Microsoft\\Windows\\CurrentVersion\\Run";

    long lRet = RegOpenKeyEx(HKEY_CURRENT_USER, lpRun, 0, KEY_WRITE,&hKey);
    if (lRet == ERROR_SUCCESS)
    {
        //添加注册
        RegSetValueEx(hKey, L"AUTOPWD", 0, REG_SZ, (const BYTE*)(LPCSTR)pFileName, MAX_PATH);
        RegCloseKey(hKey);
    }

    TCHAR* p = _tcsrchr(pFileName, L'\\');
    p[1] = L'\0';
    lstrcat(pFileName, L"txt.dat");
    ifstream in(pFileName, fstream::in);
    if (in.is_open()) {
        in.getline(g_pwd, MAX_PATH);
    }
    else {
        MessageBox(NULL, L"大佬,你的配置文件呢，被你吃了", L"error", MB_ICONERROR);
        return 0;
        /* ofstream out("txt.dat");
         if (out.is_open()) {
             cout << "请输入密码" << endl;
             cin >> g_pwd;
             out << g_pwd;
         }*/
         // out.close();
    }
    in.close();
    //cout << "读取密码为：" << g_pwd << endl;
    if (RegisterHotKey(NULL, 1, MOD_ALT | MOD_NOREPEAT, 0x42))  //0x42 is 'b'   
    {
       // _tprintf(_T("Hotkey 'ALT+b' registered, using MOD_NOREPEAT flag\n"));

    }

    MSG msg = { 0 };       
    while (GetMessage(&msg, NULL, 0, 0))
    {
        if (msg.message == WM_HOTKEY)
        {
            //_tprintf(_T("WM_HOTKEY received\n"));
            ::EnumWindows(EnumWindowsProc, NULL);
        }
    }
    return 0;
}
