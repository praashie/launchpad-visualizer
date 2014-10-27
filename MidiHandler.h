#ifndef __MIDIHANDLER__
#define __MIDIHANDLER__

#include <stdlib.h>
#include <alsa/asoundlib.h>
#include <tclap/CmdLine.h>
#include <unistd.h>

class MidiHandler{
	public:
		int init(std::string midiDevice);

		void sendEvent(unsigned char msg, unsigned char note, unsigned char velocity);
		void sendNote(bool noteON, unsigned char note, unsigned char velocity);

		void testMIDI();

		void setLED(int x, int y, unsigned char color);
		void updateLEDs();

		void resetLaunchpad();

		void close();

	private:
		bool isInitialized;

		snd_rawmidi_t* midi_in;
		snd_rawmidi_t* midi_out;

		int currentNote;
		unsigned char ledBuffer[8][8];

};
#endif
