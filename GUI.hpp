#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <WTypesbase.h>

#define IDC_BUTTON_COUNT (1000U)
#define IDC_BUTTON_ABOUT (1001U)

#define WND_STYLE (WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX)
#define GROUP_STYLE (WS_VISIBLE | WS_CHILD | BS_GROUPBOX | BS_CENTER)
#define BUTTON_STYLE (WS_VISIBLE | WS_CHILD | WS_BORDER | WS_TABSTOP)

#define EDIT_COUNT (8U)
#define EDIT_H (20U)
#define EDIT_MARGIN (10U)
#define EDIT_NAMES_STYLE (WS_VISIBLE | WS_CHILD)
#define EDIT_STYLE (WS_VISIBLE | WS_CHILD | WS_BORDER | WS_TABSTOP)

#define FONT_SIZE (8U)

/* Функция инициализации кнопок */
void InitButtons(HWND hWnd, HINSTANCE hInst, POINT pos) {
	CreateWindow(L"BUTTON", L"Управление", GROUP_STYLE, pos.x, pos.y, 150, 125, hWnd, NULL, hInst, NULL);
	CreateWindow(L"BUTTON", L"Рассчитать", BUTTON_STYLE, pos.x + 15, pos.y + 30, 120, 30, hWnd, (HMENU)IDC_BUTTON_COUNT, hInst, NULL);
	CreateWindow(L"BUTTON", L"О программе", BUTTON_STYLE, pos.x + 15, pos.y + 80, 120, 30, hWnd, (HMENU)IDC_BUTTON_ABOUT, hInst, NULL);
}

/* Функция инициализации графика */
void InitGraphic(HWND hWnd, HINSTANCE hInst, RECT graphRect) {
	CreateWindow(L"BUTTON", L"Графики", GROUP_STYLE, graphRect.left, graphRect.top, graphRect.right - graphRect.left, graphRect.bottom - graphRect.top, hWnd, NULL, hInst, NULL);
}