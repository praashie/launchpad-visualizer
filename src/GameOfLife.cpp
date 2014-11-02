#include "../include/GameOfLife.h"

bool GameOfLife::cellsOld[8][8];
bool GameOfLife::cellsNew[8][8];

bool GameOfLife::isRunning;
bool GameOfLife::singleStep;
bool GameOfLife::colorMode;

int GameOfLife::init()
{
	for (int x = 0; x < 8; x++) {
		for (int y = 0; y < 8; y++) {
			cellsNew[x][y] = false;
			cellsOld[x][y] = false;
		}
	}
	isRunning = false;
	colorMode = false;

	return 0;
}

int wrap(int x, int a, int b) {
	while (x < a) {
		x += (b - a + 1);
	}
	while (x > b) {
		x -= (b - a + 1);
	}
	return x;
}

void GameOfLife::update() {

	if (MidiHandler::isButtonPressed(7, 8)) {
		isRunning = !isRunning;
	}

	if (MidiHandler::isButtonPressed(6, 8)) {
		singleStep = true;
	}

	if (MidiHandler::isButtonPressed(5, 8)) {
		colorMode = !colorMode;
	}

	if (singleStep) {isRunning = true;}

	if (isRunning) {
		for (int x = 0; x < 8; x++) {
			for (int y = 0; y < 8; y++) {
				cellsOld[x][y] = cellsNew[x][y];
			}
		}
	}

	int neighborCount;

	for (int x = 0; x < 8; x++) {
		for (int y = 0; y < 8; y++) {
			if (MidiHandler::isButtonPressed(4, 8)) { //Clear everything
				cellsNew[x][y] = false;
				cellsOld[x][y] = false;
				isRunning = false;
			}
			else if (!isRunning && MidiHandler::isButtonPressed(x, y)) {
					cellsNew[x][y] = !(cellsNew[x][y]);
			} else if (isRunning) {
				//count the number of neighboring cells
				neighborCount = 0;
				for (int dx = x-1; dx <= x+1; dx++) {
					for (int dy = y-1; dy <= y+1; dy++) {
						if ( ((dx != x) || (dy != y)) &&
						(cellsOld[wrap(dx, 0, 7)][wrap(dy, 0,7)])) {
							neighborCount++;
						}
					}
				}
				//if (neighborCount > 0) {fprintf(stderr, "Neighbors: %i\n", neighborCount);}
				if ((neighborCount < 2) || (neighborCount > 3)) {cellsNew[x][y] = false;} //Cell dies due to isolation or overpopulation
				else if (neighborCount == 3) {cellsNew[x][y] = true;}
				else {cellsNew[x][y] = cellsOld[x][y];}
			}

			bool newState = cellsNew[x][y];
			bool oldState = cellsOld[x][y];
			unsigned char color;

			if (colorMode) {
				if ((oldState && newState)) {color = 0x3E;}//surviving cell
				else if (oldState && !newState) {color = 0x0D;}//dying cell
				else if (!oldState && newState) {color = 0x38;}//newborn cell
				else {color = 0x00;}
			} else {
				if (newState) {color = 0x3E;}
				else {color = 0x00;}
			}

			MidiHandler::setLED(x, y, color);
		}
	}

	if (singleStep) {
		isRunning = false;
		singleStep = false;
	}


	if (isRunning) {MidiHandler::setLED(7, 8, 0x3C);}
	else {MidiHandler::setLED(7, 8, 0x0F);}

	if (colorMode) {MidiHandler::setLED(5, 8, 0x3C);}
	else {MidiHandler::setLED(5, 8, 0x0F);}

	MidiHandler::setLED(6, 8, 0x3E);
	MidiHandler::setLED(4, 8, 0x0F);

}
