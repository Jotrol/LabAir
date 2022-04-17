#include "GUI.hpp"
#include "LabAir.hpp"
#include "Graphic.hpp"

/* Данные, которые получили из окна */
VariantData varData = { 0 };

/* Точки, которые посчитали */
INT pointsLen = 0;
POINTFLOAT points[MAX_POINTS] = { 0 };

RECT graphRect = { 200, 10, 620, 400 };
POINT offset = { 0 };
SIZE innerSize = { 0 };

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	static HDC hdc = NULL;
	static PAINTSTRUCT ps = { 0 };

	if (message == WM_DESTROY) {
		PostQuitMessage(0);
	}
	else if (message == WM_PAINT) {
		hdc = BeginPaint(hWnd, &ps);

		if (pointsLen != 0) {
			DrawPoints(hdc, pointsLen);
		}

		EndPaint(hWnd, &ps);
		ReleaseDC(hWnd, hdc);
	}
	else if (message == WM_COMMAND) {
		INT wmId = LOWORD(wParam);
		if (wmId == IDC_BUTTON_COUNT) {
			if (!GetInputToVariantData(hWnd, varData, MAX_POINTS)) {
				return 0;
			}
			
			pointsLen = CountPoints(varData, points, varData.n);
			ScalePoints(points, pointsLen);

			InvalidateRect(hWnd, NULL, TRUE);
		}
		else if (wmId == IDC_BUTTON_ABOUT) {
			MessageBox(hWnd, L"Нажали \"О программе\"", L"Ошибка", MB_OK | MB_ICONERROR);
		}
	}
	else {
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

INT APIENTRY WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdParam, _In_ INT nCmdShow) {
	WNDCLASS wc = { 0 };
	wc.cbClsExtra = sizeof(WNDCLASS);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.hInstance = hInstance;
	wc.lpszClassName = className;
	wc.lpfnWndProc = WndProc;

	RECT desktopRect = { 0 };
	HWND desktop = GetDesktopWindow();
	GetWindowRect(desktop, &desktopRect);

	graphRect.right = desktopRect.right - 50;
	graphRect.bottom = desktopRect.bottom - 50;
	offset = { graphRect.left + 90, graphRect.bottom - 50 };
	innerSize = { graphRect.right - graphRect.left - 110, graphRect.bottom - graphRect.top - 75 };

	LONG wndSizeX = desktopRect.right - desktopRect.left;
	LONG wndSizeY = desktopRect.bottom - desktopRect.top;

	if (!RegisterClass(&wc)) {
		MessageBox(NULL, L"Класс окна не зарегистрировался!", L"Ошибка", MB_OK | MB_ICONERROR);
		return 1;
	}

	HWND hWnd = CreateWindow(className, L"Лабораторная работа \"Воздух\"", WND_STYLE, 0, 0, wndSizeX, wndSizeY, NULL, NULL, hInstance, NULL);
	if (!hWnd) {
		MessageBox(NULL, L"Окно не получилось создать!", L"Ошибка", MB_OK | MB_ICONERROR);
		return 1;
	}
	
	/* Инициализация полей ввода */
	InitInputField(hWnd, hInstance, inputBoxPos);

	/* Инициализация кнопок */
	InitButtons(hWnd, hInstance, buttonsPos);

	/* Инициализация графика */
	InitGraphic(hWnd, hInstance, graphRect);
	
	UpdateWindow(hWnd);
	ShowWindow(hWnd, nCmdShow);

	MSG msg = { 0 };
	while (GetMessage(&msg, NULL, NULL, NULL)) {
		if (!IsDialogMessage(hWnd, &msg)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	DeleteObject(blackPen);
	DeleteObject(blackDotPen);
	DeleteObject(redPen);
	DeleteObject(bluePen);

	return 0;
}