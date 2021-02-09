#include "windows.h"
#include <iostream>
#include <fstream>
#include <map>
using namespace std;

//#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" )

map<CHAR,CHAR>g_map;

int g_cx = 0;
int g_cy = 0;
CHAR g_pwd[MAX_PATH] = { 0 };

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
        ::SendMessage(hwnd, SW_SHOW, NULL, 0);
         SetForegroundWindow(hwnd);

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
        if (width == 656 && hight == 519) {
            ::SendMessage(hwnd, WM_CLOSE, 0, 0);
            return TRUE;
        }
        cout << hwnd << "  width:" << width << "   hight:" << hight <<"  "<< r1.left + width / 2 << "  "<< r1.top + hight / 2 << endl;

        DWORD dwThread = GetWindowThreadProcessId(hwnd, NULL);
        AttachThreadInput(dwThread, GetCurrentThreadId(), TRUE);
        for (int i = 0; i < 12; i++) {
            inputKey(8);
        }
        int len = strlen(g_pwd);
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
    ifstream in("txt.dat", fstream::in);
    if (in.is_open()) {
       in.getline(g_pwd, MAX_PATH);
    }
    else {
        ofstream out("txt.dat");
        if (out.is_open()) {
            cout << "ÇëÊäÈëÃÜÂë" << endl;
            cin >> g_pwd;
            out << g_pwd;
        }
        out.close();
    }
    in.close();

    ::EnumWindows(EnumWindowsProc, NULL);

    return 0;
}
