#include "windows.h"
#include <iostream>
#include <vector>
using namespace std;

#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" )

vector<HWND>g_vec;

int g_cx = 0;
int g_cy = 0;

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

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam)
{
    TCHAR szTitle[MAX_PATH] = { 0 };
    TCHAR szClassName[MAX_PATH] = { 0 };
    ::GetWindowText(hwnd, szTitle, MAX_PATH);
    ::RealGetWindowClass(hwnd, szClassName, MAX_PATH);
   // wprintf(L"%s\n", szTitle);
    //wprintf(L"%s\n", szClassName);
    
    if (wcscmp(szTitle, L"Mango3") == 0 && wcscmp(szClassName, L"Qt5QWindowIcon") == 0)
    {
        if (hwnd == NULL) {
            return TRUE;
        }   
        cout << hwnd << endl;
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

        cout << hwnd << "  width:"<< width <<"   hight:"<<hight << endl;

        
        
        for (int i = 0; i < 12; i++) {
            inputKey(8);
        }
        if (::GetKeyState(VK_CAPITAL) > 0) {
            inputKey(VK_CAPITAL);
        }
        // 
        // inputKey    ‰»Î√‹¬Î
        inputKey('M');

        inputComboKey(16, 51);
        inputKey('1');
        inputKey('2');
        inputKey(51);
        inputKey(52);
        inputKey(53);
        inputKey(54);
        inputKey(13);

     //   mouse_event(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE, r1.left + width / 2, r1.top + hight / 2, 0, 0);
      //  mouse_event(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, r1.left + width / 2, r1.top + hight / 2, 0, 0);

        return FALSE;
       
    }
    return TRUE;
}

int main()
{ 
    g_cx = ::GetSystemMetrics(SM_CXSCREEN);
    g_cy = ::GetSystemMetrics(SM_CYSCREEN);

    ::EnumWindows(EnumWindowsProc, NULL);

    return 0;
}
