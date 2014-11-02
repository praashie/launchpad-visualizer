#ifndef GAMEOFLIFE_H
#define GAMEOFLIFE_H

#include "MidiHandler.h"
class GameOfLife
{
	public:
		GameOfLife();
		static int init();
		static void update();
	protected:
	private:
		static bool cellsOld[8][8];
		static bool cellsNew[8][8];

		static bool isRunning;
		static bool singleStep;
};

#endif // GAMEOFLIFE_H
