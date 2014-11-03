#ifndef __MIDIHANDLER__
#define __MIDIHANDLER__

#include <stdlib.h>
#include <string>
#include <alsa/asoundlib.h>
#include <unistd.h>

#include "GameOfLife.h"

class MidiHandler{
	public:
		static bool ledBuffering;

		static int init(std::string midiDevice);

		static void sendEvent(unsigned char msg, unsigned char note, unsigned char velocity);
		static void sendNote(bool noteON, unsigned char note, unsigned char velocity);

		static void handleInput();
		static void testMIDI();

		static bool isButtonDown(int x, int y);
		static bool isButtonPressed(int x, int y);
		static bool isButtonReleased(int x, int y);

		static void setLED(int x, int y, unsigned char color);
		static void updateLEDs();

		static void resetLaunchpad();

		static void close();

	private:
		static bool isInitialized;

		static snd_rawmidi_t* midi_in;
		static snd_rawmidi_t* midi_out;

		static unsigned char lastCommand;

		static int currentNote;

		static bool noteBuffer[9][9];
		static bool noteBufferOld[9][9];
		static unsigned char ledBuffer[9][9];
		static unsigned char ledBufferOld[9][9];

		static bool bufferState;
};
#endif
