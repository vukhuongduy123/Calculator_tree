#if defined(UNICODE) && !defined(_UNICODE)
    #define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
    #define UNICODE
#endif

#include <tchar.h>
#include <windows.h>
#include "Tree.h"
#include <iostream>

using namespace std;
/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

/*  Make the class name into a global variable  */
TCHAR szClassName[ ] = _T("Calculator");
struct Text
{
    TCHAR* text[24]={"1","2","3","+","4","5",
                    "6","-","7","8","9","*","0","=",
                    "/","clr","(",")",".","abs","log","!","^","root"};

}Text;

HINSTANCE hInst;

int WINAPI WinMain (HINSTANCE hThisInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpszArgument,
                     int nCmdShow)
{
    HWND hwnd;               /* This is the handle for our window */
    MSG messages;            /* Here messages to the application are saved */
    WNDCLASSEX wincl;        /* Data structure for the windowclass */
    hInst=hThisInstance;
    /* The Window structure */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
    wincl.style = CS_DBLCLKS;                 /* Catch double-clicks */
    wincl.cbSize = sizeof (WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default colour as the background of the window */
    wincl.hbrBackground = (HBRUSH) COLOR_BACKGROUND;

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl))
        return 0;

    /* The class is registered, let's create the program*/
    hwnd = CreateWindowEx (
           0,                   /* Extended possibilites for variation */
           szClassName,         /* Classname */
           _T("Calculator"),       /* Title Text */
           WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU  | WS_MINIMIZEBOX | WS_MAXIMIZEBOX, /* default window */
           CW_USEDEFAULT,       /* Windows decides the position */
           CW_USEDEFAULT,       /* where the window ends up on the screen */
           500,                 /* The programs width */
           600,                 /* and height in pixels */
           NULL,        /* The window is a child-window to desktop */
           NULL,                /* No menu */
           hThisInstance,       /* Program Instance handler */
           NULL                 /* No Window Creation data */
           );

    /* Make the window visible on the screen */
    ShowWindow (hwnd, nCmdShow);

    /* Run the message loop. It will run until GetMessage() returns 0 */
    while (GetMessage (&messages, NULL, 0, 0))
    {
        /* Translate virtual-key messages into character messages */
        TranslateMessage(&messages);
        /* Send message to WindowProcedure */
        DispatchMessage(&messages);
    }

    /* The program return-value is 0 - The value that PostQuitMessage() gave */
    return messages.wParam;
}

HFONT Font(HDC hdc)
{
    LOGFONT logFont = {0};
    logFont.lfHeight = -MulDiv(30, GetDeviceCaps(hdc, LOGPIXELSY), 72);
    logFont.lfWeight = FW_SEMIBOLD;
    strcpy(logFont.lfFaceName, TEXT("arial"));
    return CreateFontIndirect(&logFont);
}

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static Tree tree;
    static int cxClient,cyClient;
    static HWND hwndButton[25],hwndStatic;
    static HDC hdcStatic;
    static HDC hdc;
    static HFONT hFont,hOldFont;
    switch (message)                  /* handle the messages */
    {
        case WM_CREATE:
            for(int i=0;i<25;i++)
                hwndButton[i]=CreateWindowEx(WS_EX_CLIENTEDGE,TEXT("BUTTON"),Text.text[i],WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
                                           0,0,0,0, hwnd,(HMENU)i, hInst,NULL);


            hwndStatic=CreateWindowEx(WS_EX_CLIENTEDGE,TEXT("static"),0,WS_CHILD|WS_VISIBLE|SS_RIGHT,0,0,0,0,hwnd,(HMENU)100,hInst,0);
            hdc=GetDC(hwnd);
            hFont=Font(hdc);
            ReleaseDC(hwnd,hdc);
            for(int i=0;i<25;i++)
                SendMessage(hwndButton[i],WM_SETFONT,(WPARAM)hFont,MAKELPARAM(TRUE, 0));

            SendMessage(hwndStatic,WM_SETFONT,(WPARAM)hFont,MAKELPARAM(TRUE, 0));

            return 0;
        case WM_SIZE:
            cxClient=LOWORD(lParam);
            cyClient=HIWORD(lParam);
            for(int i=0;i<24;i+=4)
                for(int j=0;j<4;j++)
                    MoveWindow(hwndButton[j+i],cxClient/4*j,cyClient/5+cyClient/30*i,cxClient/4,cyClient/7,TRUE);

            MoveWindow(hwndStatic,0,0,cxClient,cyClient/5,TRUE);
            return 0;
        case WM_COMMAND:
            {
                string temp;
                TCHAR *text;
                int Maxcount;
                switch(LOWORD(wParam))
                {
                case 15:
                    tree.SetInfix("");
                    SetWindowText(hwndStatic,NULL);
                    break;
                case 13:
                    if(tree.GetInfix().empty())
                        return 0;
                    SetWindowText(hwndStatic,to_string(ProcTree(tree)).c_str());
                    break;
                default:
                    Maxcount=GetWindowTextLength(hwndButton[LOWORD(wParam)]);
                    text=new TCHAR[Maxcount+1];
                    GetWindowText(hwndButton[LOWORD(wParam)],text,Maxcount+1);
                    temp=text;
                    if(((text[0]<='9'&&text[0]>='0')||text[0]=='.')&&(!tree.GetInfix().empty()&&
                        (('0'<=tree.GetInfix().back()&&tree.GetInfix().back()<='9')||tree.GetInfix().back()=='.')))
                        tree.insertStr(temp);
                    else
                        tree.insertStr(" "+temp);
                    SetWindowText(hwndStatic,tree.GetInfix().c_str());

                    delete[] text;
                }
                return 0;
            }

        case WM_DESTROY:
            PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
            break;
        default:                      /* for messages that we don't deal with */
            return DefWindowProc (hwnd, message, wParam, lParam);
    }
    return 0;
}
