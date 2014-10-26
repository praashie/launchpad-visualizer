#ifndef __MIDIHANDLER__
#define __MIDIHANDLER__

#include <stdlib.h>
#include <alsa/asoundlib.h>
#include <tclap/CmdLine.h>
#include <unistd.h>

class MidiHandler{
	public:
		int init(std::string midiDevice);
		void testMIDI();
		void close();
	private:
		bool isInitialized;

		snd_rawmidi_t* midi_in;
		snd_rawmidi_t* midi_out;

		int currentNote;


};
#endif
