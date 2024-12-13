// RoleAssignerGUI.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "RoleAssignerGUI.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_ROLEASSIGNERGUI, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_ROLEASSIGNERGUI));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ROLEASSIGNERGUI));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_ROLEASSIGNERGUI);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   // Create a static text control
   CreateWindowW(L"STATIC", L"Hello, this is some text!",
       WS_VISIBLE | WS_CHILD,
       50, 50, 200, 30,
       hWnd, nullptr, hInst, nullptr);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_NEW_FILE:
                OPENFILENAME ofn;       // Common dialog box structure
                WCHAR szFile[MAX_PATH]; // Buffer for file name
                WCHAR szInitialDir[MAX_PATH]; // Buffer for the initial directory

                ZeroMemory(&ofn, sizeof(ofn));
                ZeroMemory(szFile, sizeof(szFile));
                ZeroMemory(szInitialDir, sizeof(szInitialDir));

                // Get the path of the executable
                GetModuleFileNameW(NULL, szInitialDir, MAX_PATH);
                PathRemoveFileSpecW(szInitialDir); // Remove the file name, leaving only the directory

                ofn.lStructSize = sizeof(ofn);
                ofn.hwndOwner = hWnd;
                ofn.lpstrFile = szFile;
                ofn.nMaxFile = sizeof(szFile) / sizeof(WCHAR);
                ofn.lpstrFilter = L"CSV Files (*.csv)\0*.csv\0All Files (*.*)\0*.*\0"; // Filter for file types                ofn.nFilterIndex = 1;
                ofn.lpstrTitle = L"Create New File";
                ofn.nFilterIndex = 1;              // Default to "CSV Files"
                ofn.lpstrDefExt = L"csv";           // Default extension
                ofn.lpstrInitialDir = szInitialDir; // Set initial directory
                ofn.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT;

                // Display the Save As dialog box
                if (GetSaveFileName(&ofn) == TRUE) {
                    // Create the file in the selected path
                    HANDLE hFile = CreateFileW(
                        ofn.lpstrFile,         // File path
                        GENERIC_WRITE,         // Write access
                        0,                     // No sharing
                        NULL,                  // Default security attributes
                        CREATE_NEW,            // Create only if file does not exist
                        FILE_ATTRIBUTE_NORMAL, // Normal file attributes
                        NULL                   // No template file
                    );

                    if (hFile != INVALID_HANDLE_VALUE) {
                        CloseHandle(hFile);
                        MessageBox(hWnd, L"File created successfully!", L"Success", MB_OK);
                    }
                    else {
                        MessageBox(hWnd, L"Failed to create file.", L"Error", MB_OK | MB_ICONERROR);
                    }
                }
                break;
            case IDM_OPEN_FILE:
                OPENFILENAME ofnOpen;             // Common dialog box structure
                WCHAR szFileOpen[MAX_PATH];       // Buffer for file name
                WCHAR szInitialDirOpen[MAX_PATH]; // Buffer for the initial directory

                ZeroMemory(&ofnOpen, sizeof(ofnOpen));
                ZeroMemory(szFileOpen, sizeof(szFileOpen));
                ZeroMemory(szInitialDirOpen, sizeof(szInitialDirOpen));

                // Get the path of the executable
                GetModuleFileNameW(NULL, szInitialDirOpen, MAX_PATH);
                PathRemoveFileSpecW(szInitialDirOpen); // Remove the file name, leaving only the directory

                ofnOpen.lStructSize = sizeof(ofnOpen);
                ofnOpen.hwndOwner = hWnd;
                ofnOpen.lpstrFile = szFileOpen;
                ofnOpen.nMaxFile = sizeof(szFileOpen) / sizeof(WCHAR);
                ofnOpen.lpstrFilter = L"CSV Files (*.csv)\0*.csv\0All Files (*.*)\0*.*\0"; // Filter for file types
                ofnOpen.lpstrTitle = L"Open CSV File";
                ofnOpen.nFilterIndex = 1;              // Default to "CSV Files"
                ofnOpen.lpstrDefExt = L"csv";          // Default extension
                ofnOpen.lpstrInitialDir = szInitialDirOpen; // Set initial directory
                ofnOpen.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;
                
                //MessageBox(hWnd, ofnOpen.lpstrFile, L"Selected File Path", MB_OK);

                // Display the Open File dialog box
                if (GetOpenFileName(&ofnOpen) == TRUE) {
                    // The user has selected a file, process it
                    std::wifstream file(ofnOpen.lpstrFile); // Open the selected file

                    if (!file.is_open()) {
                        MessageBox(hWnd, L"Failed to open file.", L"Error", MB_OK | MB_ICONERROR);
                    }
                    else {
                        std::wstring line;
                        while (std::getline(file, line)) {
                            // Process each line here
                            // For example, you can display each line in a message box
                            MessageBox(hWnd, line.c_str(), L"CSV Line", MB_OK);
                        }
                        file.close(); // Close the file
                        MessageBox(hWnd, L"File opened and processed successfully.", L"Success", MB_OK);
                    }
                }
                break;
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            case IDM_TEST:
                MessageBox(hWnd, L"test text sussy", L"Test", MB_OK);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
