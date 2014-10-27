#include "MidiHandler.h"

int MidiHandler::init(std::string midiDevice) {
	int status = snd_rawmidi_open(&midi_in, &midi_out, midiDevice.c_str(), SND_RAWMIDI_SYNC);

	if (status == 0) {

		currentNote = 0;
		resetLaunchpad();

		isInitialized = true;

	}

	return status;
}

void MidiHandler::sendEvent(unsigned char msg, unsigned char data1, unsigned char data2) {
	if (!isInitialized) {return;}

	unsigned char buffer[3];

	buffer[0] = msg;
	buffer[1] = data1;
	buffer[2] = data2;

	snd_rawmidi_write(midi_out, buffer, 3);
}

void MidiHandler::sendNote(bool noteOn, unsigned char note, unsigned char velocity) {

	unsigned char msg;

	if (noteOn) {msg = 0x90;}
	else {msg = 0x80;}

	sendEvent(msg, note, velocity);
}

void MidiHandler::setLED(int x, int y, unsigned char color) {

	if (x < 0 || y < 0) {return;}

	sendNote(true, (0x10 * y) + x, color);

}
void MidiHandler::resetLaunchpad(){

	sendEvent(0xB0, 0x00, 0x00);//Reset LEDs
	sendEvent(0xB0, 0x00, 0x01);//Select XY mapping mode
}

void MidiHandler::testMIDI() {

	if (!isInitialized) {return;}

	unsigned char buffer[8];

	currentNote++;
	if (currentNote > 127) {currentNote = 0;}

	sendNote(true, (unsigned char) currentNote, 0b0010010);

	usleep(50000);

	sendNote(false, (unsigned char) currentNote, 0b0010010);

	snd_rawmidi_write(midi_out, buffer, 3);

	//usleep(100000);
}


void MidiHandler::close() {
	if (isInitialized) {
		snd_rawmidi_close(midi_in);
		snd_rawmidi_close(midi_out);
	}
}
