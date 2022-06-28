#include "framework.h"
#include "IlkoAndr_FOG.h"

#include "gl/gl.h"  
#include "gl/glu.h"
#include "glut.h"


//using namespace  std;

#define MAX_LOADSTRING 100
// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

HGLRC hRC = NULL;
HDC hdc;
HWND hWnd;
float r = 0.0;
GLint fogMode = GL_EXP;

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

void SetDCPixelFormat(HDC hDC)

{

    int nPixelFormat;

    static PIXELFORMATDESCRIPTOR pfd = {

        sizeof(PIXELFORMATDESCRIPTOR), // Size of this structure
        1,                             // Version of this structure
        PFD_DRAW_TO_WINDOW |           // Draw to window (not to bitmap)
        PFD_SUPPORT_OPENGL |           // Support OpenGL calls in window
        PFD_DOUBLEBUFFER,              // Double-buffered mode
        PFD_TYPE_RGBA,                 // RGBA color mode
        32,                            // Want 32-bit color
        0, 0, 0, 0, 0, 0,                   // Not used to select mode

        0, 0,                           // Not used to select mode
        0, 0, 0, 0, 0,                     // Not used to select mode
        16,                            // Size of depth buffer
        0,                             // Not used here
        0,                             // Not used here
        0,                             // Not used here
        0,                             // Not used here
        0, 0, 0 };                       // Not used here


    // Choose a pixel format that best matches that described in pfd

    nPixelFormat = ChoosePixelFormat(hDC, &pfd);

    // Set the pixel format for the device context
    SetPixelFormat(hDC, nPixelFormat, &pfd);
}
void Init(void);
void Reshape(GLsizei w, GLsizei h);
void DrawRedTeapot(GLfloat x, GLfloat y, GLfloat z);
void Render(); // прототип на рендиращата функция

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
    LoadStringW(hInstance, IDC_ILKOANDRFOG, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_ILKOANDRFOG));

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

    return (int)msg.wParam;
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

    wcex.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ILKOANDRFOG));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_ILKOANDRFOG);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

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
    case WM_RBUTTONDOWN:
    {
        if (fogMode == GL_EXP) {
            fogMode = GL_EXP2;
        }
        else
            if (fogMode == GL_EXP2) {
                fogMode = GL_LINEAR;
                glFogf(GL_FOG_START, 1.0);
                glFogf(GL_FOG_END, 5.0);
            }
            else if (fogMode == GL_LINEAR) {
                fogMode = GL_EXP;
            }

        glFogi(GL_FOG_MODE, fogMode);


    }
    break;

    case WM_CREATE:
        // съхраняване на контекста на устройството
        hdc = GetDC(hWnd);
        // селектиране на пикселния формат
        SetDCPixelFormat(hdc);
        // създаване на контекст за рендиране и насочване като текущ
        hRC = wglCreateContext(hdc);
        wglMakeCurrent(hdc, hRC);

        break;

        // Window is being destroyed, clean up

    case WM_DESTROY:

        // деселектиране на текущия контекст за рендиране и изтриване
        hdc = GetDC(hWnd);
        wglMakeCurrent(hdc, NULL);
        wglDeleteContext(hRC);

        //приложението спира след затваряне на прозореца  

        PostQuitMessage(0);
        break;

        // Промяна на размера на прозореца.

    case WM_SIZE:

        // Извикване на функцията, която задава размер на прозореца и изгледа

    { GLsizei height = HIWORD(lParam);
    GLsizei width = LOWORD(lParam);
    Reshape(width, height);
    }
    break;



    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);

        // Parse the menu selections:
        switch (wmId)
        {
        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;
    case WM_PAINT:
    {
        /*   PAINTSTRUCT ps;
           HDC hdc = BeginPaint(hWnd, &ps);
           // TODO: Add any drawing code that uses hdc here...
           EndPaint(hWnd, &ps);*/
        Render();
    }
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

void Init(void)
{
    GLfloat fogColor[4] = { 0.8, 0.8, 0.95, 1.0 };
    GLfloat position[] = { 0.0, 3.0, 3.0, 0.0 };
    GLfloat local_view[] = { 0.0 };

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glLightfv(GL_LIGHT0, GL_POSITION, position);
    glLightModelfv(GL_LIGHT_MODEL_LOCAL_VIEWER, local_view);

    glFrontFace(GL_CW);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_AUTO_NORMAL);
    glEnable(GL_NORMALIZE);
    glEnable(GL_FOG);

    //  fogMode = GL_EXP;
    glFogi(GL_FOG_MODE, fogMode);
    glFogfv(GL_FOG_COLOR, fogColor);
    glFogf(GL_FOG_DENSITY, 0.35);
    glClearColor(1.0, 0.96, 0.866, 1.0);
}

void Reshape(GLsizei w, GLsizei h)
{
    if (h == 0) h = 1;

    // задаване на изглед върху целия екран 
    glViewport(0, 0, w, h);
    // зареждане на матрицата на проекцията
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (w <= (h * 3))
        glOrtho(-6.0, 6.0, -2.0 * ((GLfloat)h * 3.0) / (GLfloat)w,
            2.0 * ((GLfloat)h * 3.0) / (GLfloat)w, -10.0, 10.0);
    else
        glOrtho(-6.0 * (GLfloat)w / ((GLfloat)h * 3.0),
            6.0 * (GLfloat)w / ((GLfloat)h * 3.0), -2.0, 2.0, -10.0, 10.0);

    // зареждане на моделната матрица         
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    Init();

}

void DrawRedTeapot(GLfloat x, GLfloat y, GLfloat z)
{
    GLfloat mat[3];

    glPushMatrix();
    glTranslatef(x, y, z);
    mat[0] = 0.1745; mat[1] = 0.01175; mat[2] = 0.01175;
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat);
    mat[0] = 0.61424; mat[1] = 0.04136; mat[2] = 0.04136;
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat);
    mat[0] = 0.727811; mat[1] = 0.626959; mat[2] = 0.626959;
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat);
    glMaterialf(GL_FRONT, GL_SHININESS, 0.6 * 128.0);
    glutSolidTeapot(1.0);
    glPopMatrix();
}

void Render(void)
{

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    glFogi(GL_FOG_MODE, fogMode);
    r += 0.001; if (r >= 10.0)r = 0.0;
    gluLookAt(r, 0.0, -3.0, 450.0, 150.0, 0.0, 0.0, 1.0, 0.0);

   // DrawRedTeapot(-4.0, -0.5, -1.0);
    DrawRedTeapot(0.0, -0.5, -6.0);
   // DrawRedTeapot(4.0, -0.5, -5.0);

    // смяна на буферите
    SwapBuffers(hdc);
    ReleaseDC(hWnd, hdc);
}
