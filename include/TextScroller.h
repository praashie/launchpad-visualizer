#ifndef TEXTSCROLLER_H
#define TEXTSCROLLER_H

#define SCROLL_LENGTH 128

#define LIGHT_RED 0x0D
#define LIGHT_GREEN 0x38
#define LIGHT_YELLOW 0x3E
#define LIGHT_COLOR_NUM 3

#include "MidiHandler.h"

class TextScroller
{
	public:
		static void init();
		static void update();
	protected:
	private:
		static int cells[SCROLL_LENGTH][8];

		static int viewX;
		static bool isRunning;
		static int currentColor;
};

#endif // TEXTSCROLLER_H
