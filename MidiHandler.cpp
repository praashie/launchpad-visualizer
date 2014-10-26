#include "MidiHandler.h"

int MidiHandler::init(std::string midiDevice) {
	int status = snd_rawmidi_open(&midi_in, &midi_out, midiDevice.c_str(), SND_RAWMIDI_SYNC);
	if (status == 0) {
		currentNote = 0;
		isInitialized = true;
	}

	return status;
}

void MidiHandler::testMIDI() {

    if (!isInitialized) {return;}

	unsigned char buffer[8];

	currentNote++;
	if (currentNote > 127) {currentNote = 0;}

	buffer[0] = 0x90;
	buffer[1] = (unsigned char) currentNote;
	buffer[2] = 0x1C;

	snd_rawmidi_write(midi_out, buffer, 3);

	usleep(50000);

	buffer[0] = 0x80;
	buffer[1] = (unsigned char) currentNote;
	buffer[2] = 0x1C;

	snd_rawmidi_write(midi_out, buffer, 3);

	//usleep(100000);
}

void MidiHandler::close() {
	if (isInitialized) {
		snd_rawmidi_close(midi_in);
		snd_rawmidi_close(midi_out);
	}
}
