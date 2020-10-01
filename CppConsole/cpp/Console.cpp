#include <windows.h>
#include <Wincon.h>
#include <iostream>
#include <synchapi.h>
#include <chrono>
#include <thread>
#include <string>
#include <stdio.h>
#include <conio.h>
#include "header/main_Console.h"
#include "header/main_Console_FontSize.h"
using namespace std;

HANDLE Handle;

COORD getConsoleSize() {
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	int columns, rows;
	GetConsoleScreenBufferInfo(Handle, &csbi);
	columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
	rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
	return { (short) columns, (short) rows };
}

void open() {
	AllocConsole();
	freopen("CONOUT$", "w", stdout);
}

boolean start(int Width, int Height) {

	_SMALL_RECT Rect;
	Rect.Top = 0;
	Rect.Left = 0;
	Rect.Bottom = Height - 1;
	Rect.Right = Width - 1;

	Handle = GetStdHandle(STD_OUTPUT_HANDLE);
	if (SetConsoleWindowInfo(Handle, TRUE, &Rect) == 0) {
		cout << "Error while opening Console: " << GetLastError() << endl;
		return false;
	}
	SetConsoleScreenBufferSize(Handle, getConsoleSize());
	SetConsoleWindowInfo(Handle, TRUE, &Rect);

	SetConsoleTextAttribute(Handle, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY | BACKGROUND_BLUE);

	CONSOLE_SCREEN_BUFFER_INFOEX info;
	info.cbSize = sizeof(CONSOLE_SCREEN_BUFFER_INFOEX);
	GetConsoleScreenBufferInfoEx(Handle, &info);

	info.ColorTable[0x0] = RGB(0, 0, 0); // Black
	info.ColorTable[0x1] = RGB(255, 0, 0); // Red
	info.ColorTable[0x2] = RGB(255, 255, 0); // Yellow
	info.ColorTable[0x3] = RGB(0, 255, 0); // Green
	info.ColorTable[0x4] = RGB(0, 255, 255); // Cyan
	info.ColorTable[0x5] = RGB(0, 0, 255); // Blue
	info.ColorTable[0x6] = RGB(255, 0, 255); // Purple
	info.ColorTable[0x7] = RGB(127, 0, 0); // Dark Red
	info.ColorTable[0x8] = RGB(255, 148, 0); // Gold
	info.ColorTable[0x9] = RGB(0, 127, 0); // Dark Green
	info.ColorTable[0xA] = RGB(0, 127, 127); // Dark Cyan
	info.ColorTable[0xB] = RGB(0, 0, 127); // Dark Blue
	info.ColorTable[0xC] = RGB(127, 0, 127); // Dark Purple
	info.ColorTable[0xD] = RGB(25, 25, 25); // Dark Gray
	info.ColorTable[0xE] = RGB(127, 127, 127); // Gray
	info.ColorTable[0xF] = RGB(255, 255, 255); // White

	SetConsoleScreenBufferInfoEx(Handle, &info);

	return true;
}

void setCursor(short x, short y) {
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(Handle, coord);
}

void fill(char filler) {
	setCursor(0, 0);
	COORD csize = getConsoleSize();
	string line = "";
	for (int x = 0; x < csize.X; x++) {
		line += filler;
	}
	for (int y = 0; y < csize.Y; y++) {
		cout << line;
	}
	setCursor(0, 0);
}

void fillRect(char filler, int x1, int y1, int width, int height) {
	string line = "";
	for (int x = 0; x < width; x++) {
		line += filler;
	}
	for (int y = y1; y < y1 + height; y++) {
		setCursor(x1, y);
		cout << line;
	}
}

void printInt(int i) {
	cout << i;
}

void printChar(char c) {
	if (isprint(c)) {
		cout << c;
	}
}

void printString(string s) {
	cout << s;
}

void setCursorVisible(bool visible) {
	CONSOLE_CURSOR_INFO cursorInfo;
	GetConsoleCursorInfo(Handle, &cursorInfo);
	cursorInfo.bVisible = visible;
	SetConsoleCursorInfo(Handle, &cursorInfo);
}

void setTitle(LPCSTR title) {
	SetConsoleTitleA(title);
}

void setColorCode(int color) {
	SetConsoleTextAttribute(Handle, color);
}

jchar getInputChar() {
	if (kbhit()) {
		return getch();
	}
	return 0;
}

void setFontSize(int size) {
	int wh;
	switch (size) {
	case main_Console_FontSize_LARGE:
		wh = 32;
		break;
	case main_Console_FontSize_SMALL:
		wh = 8;
		break;
	case main_Console_FontSize_NORMAL:
	default:
		wh = 16;
		break;
	}
	CONSOLE_FONT_INFOEX cfi;
	cfi.cbSize = sizeof cfi;
	cfi.nFont = 0;
	cfi.dwFontSize.X = wh;
	cfi.dwFontSize.Y = wh;
	cfi.FontFamily = FF_DONTCARE;
	cfi.FontWeight = FW_NORMAL;
	wcscpy(cfi.FaceName, L"Terminal");
	SetCurrentConsoleFontEx(Handle, FALSE, &cfi);
	SetConsoleScreenBufferSize(Handle, getConsoleSize());
}

JNIEXPORT void JNICALL Java_com_asecave_Console_open(JNIEnv*, jobject) {
	open();
}

JNIEXPORT jboolean JNICALL Java_com_asecave_Console_start(JNIEnv*, jobject, jint Width, jint Height) {
	return start(Width, Height);
}

JNIEXPORT void JNICALL Java_com_asecave_Console_setCursor(JNIEnv*, jobject, jshort x, jshort y) {
	setCursor(x, y);
}

JNIEXPORT void JNICALL Java_com_asecave_Console_fill__C(JNIEnv*, jobject, jchar filler) {
	fill(filler);
}

JNIEXPORT void JNICALL Java_com_asecave_Console_fill__CIIII(JNIEnv*, jobject, jchar filler, jint x1, jint y1, jint width, jint height) {
	fillRect(filler, x1, y1, width, height);
}

JNIEXPORT void JNICALL Java_com_asecave_Console_print__I(JNIEnv*, jobject, jint i) {
	printInt(i);
}

JNIEXPORT void JNICALL Java_com_asecave_Console_print__C(JNIEnv*, jobject, jchar c) {
	printChar(c);
}

JNIEXPORT void JNICALL Java_com_asecave_Console_print__Ljava_lang_String_2(JNIEnv* env, jobject, jstring s) {
	jboolean isCopy;
	const char* convertedValue = env->GetStringUTFChars(s, &isCopy);
	std::string string = std::string(convertedValue, env->GetStringUTFLength(s));
	printString(string);
	env->ReleaseStringUTFChars(s, convertedValue);
}

JNIEXPORT void JNICALL Java_com_asecave_Console_setColorCode(JNIEnv*, jobject, jint color) {
	setColorCode(color);
}

JNIEXPORT jint JNICALL Java_com_asecave_Console_getConsoleWidth(JNIEnv*, jobject) {
	return getConsoleSize().X;
}

JNIEXPORT jint JNICALL Java_com_asecave_Console_getConsoleHeight(JNIEnv*, jobject) {
	return getConsoleSize().Y;
}

JNIEXPORT void JNICALL Java_com_asecave_Console_setCursorVisible(JNIEnv*, jobject, jboolean visible) {
	setCursorVisible(visible);
}

JNIEXPORT void JNICALL Java_com_asecave_Console_setTitle(JNIEnv* env, jobject, jstring title) {
	jboolean isCopy;
	const char* convertedValue = env->GetStringUTFChars(title, &isCopy);
	setTitle((LPCSTR) convertedValue);
	env->ReleaseStringUTFChars(title, convertedValue);
}

JNIEXPORT jchar JNICALL Java_com_asecave_Console_getInputChar(JNIEnv*, jobject) {
	return getInputChar();
}

JNIEXPORT void JNICALL Java_com_asecave_Console_setFontSize(JNIEnv*, jobject, jint size) {
	setFontSize(size);
}