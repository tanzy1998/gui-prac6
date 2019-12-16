#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <iostream>
#include <conio.h>
#include <string>

#pragma comment (lib, "OpenGL32.lib")
#pragma comment (lib, "GLU32.lib")

#define WINDOW_TITLE "Practical 6"

int rotate = 0;
bool pyramid = false;
bool light = true;

float diffuseColor[] = { 1,1,1 };
float diffusePosition[] = { 0.0f, 0.5f, 0.0f };
float objectColor[] = { 0,0,0 };

double w = 1920;
double h = 1080;
double ar = w / h; // aspect ratio

LRESULT WINAPI WindowProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CHAR:
		switch (wParam)
		{
		case 0x44:
			diffusePosition[0] += 0.3;
			break;
		case 0x41:
			diffusePosition[0] -= 0.3;
			break;
		case 0x57:
			diffusePosition[1] += 0.3;
			break;
		case 0x53:
			diffusePosition[1] -= 0.3;
			break;
		
		case 0x45:
			diffusePosition[2] += 0.3;
			break;
		case 0x51:
			diffusePosition[2] -= 0.3;
			break;
		case 0x4F:
			pyramid = false;
			break;
		case 0x50:
			pyramid = true;
			break;
		}
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:
			PostQuitMessage(0);
			break;
		case VK_SPACE:
			if (light)
				light = false;
			else
				light = true;
			break;
		case VK_UP:
			if (rotate == 0)
				rotate = 1;
			else
				rotate = 0;
			break;
		case VK_DOWN:
			if (rotate == 0)
				rotate = 2;
			else
				rotate = 0;
			break;
		}
		break;
	default:
		break;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}
//--------------------------------------------------------------------

bool initPixelFormat(HDC hdc)
{
	PIXELFORMATDESCRIPTOR pfd;
	ZeroMemory(&pfd, sizeof(PIXELFORMATDESCRIPTOR));

	pfd.cAlphaBits = 8;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 24;
	pfd.cStencilBits = 0;

	pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;

	pfd.iLayerType = PFD_MAIN_PLANE;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;

	// choose pixel format returns the number most similar pixel format available
	int n = ChoosePixelFormat(hdc, &pfd);

	// set pixel format returns whether it sucessfully set the pixel format
	if (SetPixelFormat(hdc, n, &pfd))
	{
		return true;
	}
	else
	{
		return false;
	}
}

void drawPyramid()
{
	glPushMatrix();
	glColor3ub(180, 255, 145);
	glTranslatef(0, -0.3f, 0);
	//glRotatef(rotateDeg, 1, 1, 1);
	glBegin(GL_POLYGON);

	glColor3f(0, 1, 0);
	glVertex3f(0, 0.5, -0.3);
	glVertex3f(-0.3, 0, 0);
	glVertex3f(0.3, 0, 0);

	glColor3f(0, 1, 0);
	glVertex3f(0, 0.5, -0.3);
	glVertex3f(-0.3, 0, 0);
	glVertex3f(-0.3, 0, -1.0);

	glColor3f(0, 1, 0);
	glVertex3f(0, 0.5, -0.3);
	glVertex3f(0.3, 0, 0);
	glVertex3f(0.3, 0, -1.0);

	glColor3f(0, 1, 0);
	glVertex3f(0, 0.5, -0.3);
	glVertex3f(0.3, 0, -1.0);
	glVertex3f(-0.3, 0, -1.0);

	glColor3f(0, 1, 0);
	glVertex3f(-0.3, 0, 0);
	glVertex3f(0.3, 0, 0);
	glVertex3f(0.3, 0, -1.0);
	glVertex3f(-0.3, 0, -1.0);

	glEnd();
	glPopMatrix();
}

void drawSphere() {
	GLUquadricObj* sphere = NULL;
	sphere = gluNewQuadric();
	glMaterialfv(GL_FRONT, GL_AMBIENT, objectColor);
	gluQuadricDrawStyle(sphere, GLU_LINE);
	gluSphere(sphere, 0.3, 30, 30);
	gluDeleteQuadric(sphere);
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glClearColor(1, 1, 1, 1);
	if (light)
	{
		glPushMatrix();
		{
			glLoadIdentity();
			glEnable(GL_LIGHTING);
			glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseColor);
			glLightfv(GL_LIGHT0, GL_POSITION, diffusePosition);
			glEnable(GL_LIGHT0);
		}
		glPopMatrix();
	}
	
	if (rotate == 1)
	{
		glRotatef(-0.5, 1, 1, 1);
	}
	else if (rotate == 2)
	{
		glRotatef(0.5, 1, 1, 1);
	}

	if (!pyramid)
	{
		drawSphere();
	}
	else
	{
		drawPyramid();
	}
	
	glPushMatrix();
	{
		glLoadIdentity();
		
		glDisable(GL_LIGHT0);
		GLUquadricObj* sphere = NULL;
		glMaterialfv(GL_FRONT, GL_AMBIENT, objectColor);
		sphere = gluNewQuadric();
		glTranslatef(diffusePosition[0], diffusePosition[1], diffusePosition[2]);
		gluQuadricDrawStyle(sphere, GLU_FILL);
		gluSphere(sphere, 0.05, 50, 50);
		gluDeleteQuadric(sphere);
	}
	glPopMatrix();
}


//--------------------------------------------------------------------

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE prevInstance,
	_In_ LPSTR cmdLine, _In_ int nCmdShow) // FIXED
{
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.hInstance = GetModuleHandle(NULL);
	wc.lpfnWndProc = WindowProcedure;
	wc.lpszClassName = WINDOW_TITLE;
	wc.style = CS_HREDRAW | CS_VREDRAW;

	if (!RegisterClassEx(&wc)) return false;

	HWND hWnd = CreateWindow(WINDOW_TITLE, WINDOW_TITLE, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 800, 800,
		NULL, NULL, wc.hInstance, NULL);

	//--------------------------------
	//	Initialize window for OpenGL
	//--------------------------------

	HDC hdc = GetDC(hWnd);

	//	initialize pixel format for the window
	initPixelFormat(hdc);

	//	get an openGL context
	HGLRC hglrc = wglCreateContext(hdc);

	//	make context current
	if (!wglMakeCurrent(hdc, hglrc)) return false;

	//--------------------------------
	//	End initialization
	//--------------------------------

	ShowWindow(hWnd, nCmdShow);

	MSG msg;
	ZeroMemory(&msg, sizeof(msg));

	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}


		display();

		SwapBuffers(hdc);
	}

	UnregisterClass(WINDOW_TITLE, wc.hInstance);

	return true;
}
//--------------------------------------------------------------------
