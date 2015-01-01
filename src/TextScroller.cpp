#include "../include/TextScroller.h"

char TextScroller::cells[SCROLL_LENGTH][8];
int TextScroller::viewX;

bool TextScroller::isRunning;
int TextScroller::currentColor;

char lightColors[LIGHT_COLOR_NUM] = {
	LIGHT_OFF,
	LIGHT_RED,
	LIGHT_DAWN,
	LIGHT_ORANGE,
	LIGHT_AMBER,
	LIGHT_YELLOW,
	LIGHT_LIME,
	LIGHT_GREEN};


void TextScroller::init() {

	for (int x = 0; x < SCROLL_LENGTH; x++) {
		for (int y = 0; y < 8; y++) {
			cells[x][y] = 0;
		}
	}
	viewX = 0;
	isRunning = false;
	currentColor = 1;
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

	//Mixer button - Start and stop the scrolling animation
	if (MidiHandler::isButtonPressed(7, 8))
		{isRunning = !isRunning;}

	if (isRunning) {
		MidiHandler::setLED(7, 8, LIGHT_GREEN);
	} else {
		MidiHandler::setLED(7, 8, LIGHT_RED);
	}

	/*
	if (MidiHandler::isButtonPressed(5, 8)) {
		currentColor += 1;
		if (currentColor >= LIGHT_COLOR_NUM)
			{currentColor = 1;}
	}
	*/

	//Scene launch buttons - Color palette
	for (int i = 1; i < 7; i++) {
			MidiHandler::setLED(8, i, lightColors[i]);
			if (MidiHandler::isButtonPressed(8, i))
				{currentColor = i;}
	}

	//User1 - Only indicates current color selection
	MidiHandler::setLED(5, 8, lightColors[currentColor]);

	//Session button - Reset everything
	if (MidiHandler::isButtonPressed(4, 8)) {
		for (int x = 0; x < SCROLL_LENGTH; x++) {
			for (int y = 0; y < 8; y++) {
				cells[x][y] = 0;
			}
		}
		isRunning = false;
		viewX = 0;
	}
	MidiHandler::setLED(4, 8, LIGHT_RED);

	//Left, Right - Manual scrolling
	if (MidiHandler::isButtonDown(2, 8))
		{viewX -= 1;}
	if (MidiHandler::isButtonDown(3, 8))
		{viewX += 1;}
	MidiHandler::setLED(2, 8, LIGHT_YELLOW);
	MidiHandler::setLED(3, 8, LIGHT_YELLOW);

	//xa points to light grid location, x points to scroller data location
	int x;
	for (int xa = 0; xa < 8; xa++) {
		x = wrap_(xa + viewX, 0, SCROLL_LENGTH - 1); //

		for (int y = 0; y < 8; y++) {

			//Interactive editing
			if (MidiHandler::isButtonPressed(xa, y)) {
					if (cells[x][y] == currentColor) { //If the cell is already set with the selected color, toggle it off
						cells[x][y] = 0;
					} else {
						cells[x][y] = currentColor;
					}
			}

			//Draw visible cells
			if (cells[x][y] >= 0) {
				MidiHandler::setLED(xa, y, lightColors[cells[x][y]]);
			}
		}
	}

	//Scroll automatically
	if (isRunning) {
		viewX += 1;
	}
}
