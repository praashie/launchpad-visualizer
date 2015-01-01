#ifndef TEXTSCROLLER_H
#define TEXTSCROLLER_H

#define SCROLL_LENGTH 128

#define LIGHT_OFF 0x00
#define LIGHT_RED 0x0F
#define LIGHT_DAWN 0x1F
#define LIGHT_ORANGE 0x2F
#define LIGHT_AMBER 0x3F
#define LIGHT_YELLOW 0x3E
#define LIGHT_LIME 0x3D
#define LIGHT_GREEN 0x3C

#define LIGHT_COLOR_NUM 8

#include "MidiHandler.h"

class TextScroller
{
	public:
		static void init();
		static void update();
	protected:
	private:
		static char cells[SCROLL_LENGTH][8];

		static int viewX;
		static bool isRunning;
		static int currentColor;
};

#endif // TEXTSCROLLER_H
