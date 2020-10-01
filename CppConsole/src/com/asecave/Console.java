package com.asecave;

import java.awt.AWTException;
import java.awt.Robot;
import java.awt.event.KeyEvent;
import java.io.IOException;

public class Console {

	private int fgColor;
	private int bgColor;

	private boolean fullscreen = false;

	static {
		try {
			NativeUtils.loadLibraryFromJar("/Console.dll");
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

	public Console() {
		open();
		start(80, 40);
		fgColor = 0;
		bgColor = 0;
	}

	public static class Color {
		public static final int BLACK = 0x0;
		public static final int RED = 0x1;
		public static final int YELLOW = 0x2;
		public static final int GREEN = 0x3;
		public static final int CYAN = 0x4;
		public static final int BLUE = 0x5;
		public static final int PURPLE = 0x6;
		public static final int DARK_RED = 0x7;
		public static final int GOLD = 0x8;
		public static final int DARK_GREEN = 0x9;
		public static final int DARK_CYAN = 0xA;
		public static final int DARK_BLUE = 0xB;
		public static final int DARK_PURPLE = 0xC;
		public static final int DARK_GRAY = 0xD;
		public static final int GRAY = 0xE;
		public static final int WHITE = 0xF;
	}

	public static class FontSize {
		public static final int SMALL = 0;
		public static final int NORMAL = 1;
		public static final int LARGE = 2;
	}

	public void setFGColor(int color) {
		fgColor = color;
		setColorCode(makeColorCode());
	}

	public void setBGColor(int color) {
		bgColor = color;
		setColorCode(makeColorCode());
	}

	private int makeColorCode() {
		int code = bgColor * 16;
		return code | fgColor;
	}

	public void setCursor(int x, int y) {
		setCursor((short) x, (short) y);
	}

	public void setFullscreen(boolean fullscreen) {
		if (this.fullscreen != fullscreen) {
			this.fullscreen = fullscreen;
			try {
				Robot robot = new Robot();
				robot.keyPress(KeyEvent.VK_ALT);
				robot.keyPress(KeyEvent.VK_ENTER);
				robot.keyRelease(KeyEvent.VK_ENTER);
				robot.keyRelease(KeyEvent.VK_ALT);
			} catch (AWTException e) {
				e.printStackTrace();
			}
		}
	}

	public boolean isFullscreen() {
		return fullscreen;
	}

	private native void open();

	private native boolean start(int width, int height);

	private native void setCursor(short x, short y);

	public native void fill(char filler);

	public native void fill(char filler, int x1, int y1, int width, int height);

	public native void print(int i);

	public native void print(char c);

	public native void print(String s);

	private native void setColorCode(int color);

	public native int getConsoleWidth();

	public native int getConsoleHeight();

	public native void setCursorVisible(boolean visible);

	public native void setTitle(String title);

	public native char getInputChar();

	public native void setFontSize(int size);
}
