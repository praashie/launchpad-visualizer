#include "../include/TextScroller.h"

int TextScroller::cells[SCROLL_LENGTH][8];
int TextScroller::viewX;

bool TextScroller::isRunning;
int TextScroller::currentColor;

char lightColors[LIGHT_COLOR_NUM] = {LIGHT_RED, LIGHT_YELLOW, LIGHT_GREEN};


void TextScroller::init() {

	for (int x = 0; x < SCROLL_LENGTH; x++) {
		for (int y = 0; y < 8; y++) {
			cells[x][y] = -1;
		}
	}
	viewX = 0;
	isRunning = false;
	currentColor = 0;
}

int wrap_(int x, int a, int b) {
	while (x < a) {
		x += (b - a + 1);
	}
	while (x > b) {
		x -= (b - a + 1);
	}
	return x;
}

void TextScroller::update() {

	if (MidiHandler::isButtonPressed(7, 8)) {
		isRunning = !isRunning;
	}

	if (MidiHandler::isButtonPressed(5, 8)) {
		currentColor += 1;
		if (currentColor >= LIGHT_COLOR_NUM) {currentColor = 0;}
	}

	if (MidiHandler::isButtonPressed(2, 8)) {
		viewX -= 1;
	}
	if (MidiHandler::isButtonPressed(3, 8)) {
		viewX += 1;
	}

	int x;

	for (int xa = 0; xa < 8; xa++) {
		x = wrap_(xa + viewX, 0, SCROLL_LENGTH - 1);
		for (int y = 0; y < 8; y++) {
			if (!isRunning && MidiHandler::isButtonPressed(xa, y)) {
					if (cells[x][y] == currentColor) {
						cells[x][y] = -1;
					} else {
						cells[x][y] = currentColor;
					}
			}
			//unsigned char color;

			if (cells[x][y] >= 0) {
				MidiHandler::setLED(xa, y, lightColors[cells[x][y]]);
			} else {
				MidiHandler::setLED(xa, y, 0x00);
			}
		}
	}

	if (isRunning) {
		viewX += 1;
	}


	if (isRunning) {MidiHandler::setLED(7, 8, LIGHT_GREEN);}
	else {MidiHandler::setLED(7, 8, LIGHT_RED);}

	MidiHandler::setLED(5, 8, lightColors[currentColor]);

	MidiHandler::setLED(4, 8, LIGHT_RED);
	MidiHandler::setLED(2, 8, LIGHT_YELLOW);
	MidiHandler::setLED(3, 8, LIGHT_YELLOW);

}
