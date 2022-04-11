#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <stdlib.h>

/* Название класса окна */
LPCWSTR className = L"LabAirWindow";

/* Буфер для получения ввода из полей */
#define BUFFER_LEN (60U)
WCHAR inputBuffer[BUFFER_LEN] = { 0 };

/* Дескрипторы полей ввода и названия параметров */
HWND editHwnd[EDIT_COUNT] = { 0 };
LPCWSTR editNames[EDIT_COUNT] = { L"h: ", L"S: ", L"n: ", L"Pуст: ", L"Pосв: ", L"k: ", L"So: ", L"m: " };
LPCWSTR editInitVals[EDIT_COUNT] = { L"", L"", L"", L"", L"", L"", L"", L"" };

/* Структура для набора данных по варианту */
struct VariantData {
	DOUBLE h;		/* Высота помещения */
	INT S;			/* Площадь помещения */
	INT n;			/* Количество человек */
	FLOAT Pust;		/* Мощность электроустановок */
	FLOAT Posv;		/* Мощность освещения */
	FLOAT k;		/* Коэф-ент характера остекления */
	FLOAT So;		/* Площадь окна */
	INT m;			/* Количество окон */
};

/* Константы для расчета */
FLOAT C = 1.03f * 1000;
FLOAT p = 1.3f;
FLOAT d = 1.5f;
FLOAT tpr = 17.5f;
FLOAT trz = 24.f;
FLOAT eta = 0.15f;
FLOAT q = 90.0f;
FLOAT kL = 0.4f;
FLOAT A = 127.f;

/* Функция вычисления одной точки */
inline POINTFLOAT CountPointByData(const VariantData& data) {
	FLOAT Qrad = A * data.k * data.So * data.m;
	FLOAT Qosv = kL * data.Posv * 1000;
	FLOAT QL = q * data.n;
	FLOAT Qobor = eta * data.Pust * 1000;
	FLOAT Qprih = Qobor + QL + Qosv + Qrad;
	FLOAT Qizb = Qprih - 0.1f * Qprih;

	FLOAT tyh = trz + d * (data.h - 2.0f);
	FLOAT Vvent = (3600.0f * Qizb) / (C * p * (tyh - tpr));
	FLOAT K = Vvent / (data.S * data.h);

	return { Vvent, K };
}

/* Функция инициализации полей ввода */
void InitInputField(HWND hWnd, HINSTANCE hInst, POINT pos) {
	CreateWindow(L"BUTTON", L"Ввод", GROUP_STYLE, pos.x, pos.y, 150, 260, hWnd, NULL, hInst, NULL);
	pos.x += 10; pos.y += 20;
	for (INT i = 0; i < EDIT_COUNT; i += 1) {
		CreateWindow(L"STATIC", editNames[i], EDIT_NAMES_STYLE, pos.x, pos.y + i * (EDIT_H + EDIT_MARGIN), 35, EDIT_H, hWnd, NULL, hInst, NULL);
		editHwnd[i] = CreateWindow(L"EDIT", L"", EDIT_STYLE, pos.x + 50, pos.y + i * (EDIT_H + EDIT_MARGIN), 70, EDIT_H, hWnd, NULL, hInst, NULL);
		SetWindowText(editHwnd[i], editInitVals[i]);
	}
}

INT CountPoints(VariantData varData, POINTFLOAT* points, INT nMax) {
	for (INT i = 0; i <= nMax; i += 1) {
		varData.n = i + 1;
		points[i] = CountPointByData(varData);
	}
	return nMax;
}

/* Функция получения данных из полей ввода */
BOOL GetInputToVariantData(HWND hWnd, VariantData& varData, INT maxPoints) {
	UINT charsGet = 0;
	BOOL isOk = TRUE;
	WCHAR error_msg[325] = { 0 };

	/* Получаем строку с высотой помещения, переводим в число */
	/* Проверяем кол-во символов, которое получили */
	charsGet = GetWindowText(editHwnd[0], inputBuffer, BUFFER_LEN);
	varData.h = _wtof(inputBuffer);
	isOk &= (charsGet > 0);
	if (charsGet == 0) {
		lstrcat(error_msg, L"Вы не ввели h - высоту помещения!\n");
	}

	/* Получаем площадь помещения */
	charsGet = GetWindowText(editHwnd[1], inputBuffer, BUFFER_LEN);
	varData.S = _wtoi(inputBuffer);
	isOk &= (charsGet > 0);
	if (charsGet == 0) {
		lstrcat(error_msg, L"Вы не ввели S - площадь помещения\n");
	}

	/* Получаем количество человек */
	charsGet = GetWindowText(editHwnd[2], inputBuffer, BUFFER_LEN);
	varData.n = _wtoi(inputBuffer);
	isOk &= (charsGet > 0) && ((varData.n >= 5 && varData.n < maxPoints));
	if (charsGet == 0) {
		lstrcat(error_msg, L"Вы не ввели n - кол-во человек\n");
	}

	/* Если человек слишком мало или слишком много - не рисуем */
	if (varData.n < 5 && charsGet > 0) {
		lstrcat(error_msg, L"Слишком мало человек, минимум 5!\n");
	}
	if (varData.n > maxPoints && charsGet > 0) {
		wsprintf(inputBuffer, L"Слишком много человек, максимум %d!\n", maxPoints);
		lstrcat(error_msg, inputBuffer);
	}

	/* Получаем мощность установки */
	charsGet = GetWindowText(editHwnd[3], inputBuffer, BUFFER_LEN);
	varData.Pust = _wtof(inputBuffer);
	isOk &= (charsGet > 0);
	if (charsGet == 0) {
		lstrcat(error_msg, L"Вы не ввели Pуст - мощность установки\n");
	}

	/* Получаем мощность освещения */
	charsGet = GetWindowText(editHwnd[4], inputBuffer, BUFFER_LEN);
	varData.Posv = _wtof(inputBuffer);
	isOk &= (charsGet > 0);
	if (charsGet == 0) {
		lstrcat(error_msg, L"Вы не ввели Pосв - мощность освещения\n");
	}

	/* Получаем коэффициент характера остекления */
	charsGet = GetWindowText(editHwnd[5], inputBuffer, BUFFER_LEN);
	varData.k = _wtof(inputBuffer);
	isOk &= (charsGet > 0);
	if (charsGet == 0) {
		lstrcat(error_msg, L"Вы не ввели k - коэфф. характера остекления\n");
	}

	/* Получаем площадь окна */
	charsGet = GetWindowText(editHwnd[6], inputBuffer, BUFFER_LEN);
	varData.So = _wtof(inputBuffer);
	isOk &= (charsGet > 0);
	if (charsGet == 0) {
		lstrcat(error_msg, L"Вы не ввели So - площадь окна\n");
	}

	/* Получаем количество окон */
	charsGet = GetWindowText(editHwnd[7], inputBuffer, BUFFER_LEN);
	varData.m = _wtoi(inputBuffer);
	isOk &= (charsGet > 0);
	if (charsGet == 0) {
		lstrcat(error_msg, L"Вы не ввели m - кол-во окон\n");
	}

	/* Если где-то была ошибка ввода, указать ошибку */
	if (!isOk) {
		MessageBox(hWnd, error_msg, L"Ошибка", MB_OK | MB_ICONERROR);
		return FALSE;
	}
	return TRUE;
}