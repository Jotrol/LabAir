#pragma once

#include <Windows.h>
#include "GUI.hpp"

#include <stdio.h>

/* Максимальное кол-во точек */
#define MAX_POINTS (100U)

/* Позиция и размеры графика */
extern RECT graphRect;
extern POINT offset;
extern SIZE innerSize;

const HPEN blackPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
const HPEN redPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
const HPEN blackDotPen = CreatePen(PS_DOT, 1, RGB(0, 0, 0));
const HPEN bluePen = CreatePen(PS_SOLID, 1, RGB(0, 0, 255));

/* Буфер для отображения числового диапозона */
WCHAR numBuffer[20] = { 0 };

POINT vPoints[MAX_POINTS] = { 0 };
POINT kPoints[MAX_POINTS] = { 0 };

FLOAT MAX_V = 2000.0f;
FLOAT MAX_K = 20.0f;

/* Переводим из значения функции в абсолютные точки */
void ScalePoints(POINTFLOAT points[], INT pointsLen) {
	FLOAT maxV = points[pointsLen].x - points[0].x;
	FLOAT maxK = points[pointsLen].y - points[0].y;

	MAX_V = points[pointsLen].x;
	MAX_K = points[pointsLen].y;

	FLOAT offsetV = points[0].x * (FLOAT)innerSize.cy / maxV;
	FLOAT offsetK = points[0].y * (FLOAT)innerSize.cy / (2 * maxK);

	FLOAT scaleV = (FLOAT)innerSize.cy / maxV;
	FLOAT scaleK = (FLOAT)innerSize.cy / (2 * maxK);

	for (INT i = 0; i <= pointsLen; i += 1) {
		INT x = offset.x + i * ((FLOAT)innerSize.cx / (FLOAT)pointsLen);
		vPoints[i].x = x;
		kPoints[i].x = x;

		vPoints[i].y = offset.y + (INT)(offsetV - points[i].x * scaleV);
		kPoints[i].y = offset.y + (INT)(offsetK - points[i].y * scaleK);
	}
}
void DrawPoints(HDC hdc, INT pointsLen) {
	/*  */
	FLOAT scaleX = (FLOAT)innerSize.cx / (FLOAT)pointsLen;
	FLOAT scaleY = (FLOAT)innerSize.cy / (FLOAT)10;

	/* Рисуем ось Y */
	SelectObject(hdc, blackPen);
	MoveToEx(hdc, offset.x, offset.y, NULL);
	LineTo(hdc, offset.x, offset.y - innerSize.cy);

	/* Рисуем ось X */
	MoveToEx(hdc, offset.x, offset.y, NULL);
	LineTo(hdc, offset.x + innerSize.cx, offset.y);

	/* Рисуем плоскость по оси Y */
	SelectObject(hdc, blackDotPen);
	for (INT i = 1; i <= pointsLen; i += 1) {
		INT x = offset.x + (INT)(i * scaleX);
		MoveToEx(hdc, x, offset.y, NULL);
		LineTo(hdc, x, offset.y - innerSize.cy);
	}

	/* Рисуем плоскость по оси X */
	SelectObject(hdc, blackDotPen);
	for (INT i = 1; i <= pointsLen; i += 1) {
		INT y = offset.y - (INT)(i * scaleY);
		MoveToEx(hdc, offset.x, y, NULL);
		LineTo(hdc, offset.x + innerSize.cx, y);
	}

	/* Рисуем график Vвент */
	SelectObject(hdc, redPen);
	MoveToEx(hdc, vPoints[0].x, vPoints[0].y, NULL);
	for (int i = 1; i <= pointsLen; i += 1) {
		LineTo(hdc, vPoints[i].x, vPoints[i].y);
		MoveToEx(hdc, vPoints[i].x, vPoints[i].y, NULL);
	}

	/* Рисуем график K */
	SelectObject(hdc, bluePen);
	MoveToEx(hdc, kPoints[0].x, kPoints[0].y, NULL);
	for (int i = 1; i <= pointsLen; i += 1) {
		LineTo(hdc, kPoints[i].x, kPoints[i].y);
		MoveToEx(hdc, kPoints[i].x, kPoints[i].y, NULL);
	}
	
	/* Рисуем черточки обозначений по оси X */
	SelectObject(hdc, blackPen);
	for (INT i = 0; i <= pointsLen; i += 1) {
		INT x = offset.x + (INT)(i * scaleX);
		MoveToEx(hdc, x, offset.y - 5, NULL);
		LineTo(hdc, x, offset.y + 5);

		/* Отображаем значения по оси X */
		wsprintf(numBuffer, L"%d", i % 10);
		TextOut(hdc, x - FONT_SIZE / 2, offset.y + FONT_SIZE, numBuffer, 1);
	}

	FLOAT stepV = MAX_V / 10;
	FLOAT stepK = MAX_K / 10;

	/* Рисуем черточки обозначений по оси Y */
	for (INT i = 0; i <= 10; i += 1) {
		INT y = offset.y - (INT)(i * scaleY);
		MoveToEx(hdc, offset.x - 5, y, NULL);
		LineTo(hdc, offset.x + 5, y);

		/* Отображаем значения */
		swprintf_s(numBuffer, 20, L"%.0f", i * stepV);
		TextOut(hdc, offset.x - 5 * FONT_SIZE, y - 2 * FONT_SIZE, numBuffer, lstrlen(numBuffer));

		swprintf_s(numBuffer, 20, L"%.2f", i * stepK);
		TextOut(hdc, offset.x - 5 * FONT_SIZE, y, numBuffer, lstrlen(numBuffer));
	}
}