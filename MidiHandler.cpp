#include "MidiHandler.h"

int MidiHandler::init(std::string midiDevice) {
	int status = snd_rawmidi_open(&midi_in, &midi_out, midiDevice.c_str(), SND_RAWMIDI_SYNC);

	if (status == 0) {

		currentNote = 0;

		for (int x = 0; x < 8; x++) {
			for (int y = 0; y < 8; y++) {
				ledBuffer[x][y] = 0x0;
			}
		}

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

	if (x < 0 || y < 0 || x > 8 || y > 8) {return;}

	//sendNote(true, (0x10 * y) + x, color);
	ledBuffer[x][y] = color;

}

//Refresh the 8x8 LED grid on launchpad
void MidiHandler::updateLEDs() {
	unsigned char buffer[65];

	buffer[0] = 0x92; //This midi message initiates the rapid update mode
	for (int y = 0; y < 8; y++) {
		for (int x = 0; x < 8; x++) {
			buffer[8*y + x + 1] = ledBuffer[x][y];
		}
	}

	snd_rawmidi_write(midi_out, buffer, 65);
}
void MidiHandler::resetLaunchpad(){

	sendEvent(0xB0, 0x00, 0x00);//Reset LEDs
	sendEvent(0xB0, 0x00, 0x01);//Select XY mapping mode
}

void MidiHandler::testMIDI() {

	if (!isInitialized) {return;}

	currentNote++;
	if (currentNote > 127) {currentNote = 0;}

	/*
	sendNote(true, (unsigned char) currentNote, 0b0010010);

	usleep(50000);

	sendNote(false, (unsigned char) currentNote, 0b0010010);

	//usleep(100000);
	*/

	for (int x = 2; x < 6; x++) {
		for (int y = 2; y < 6; y++) {
			setLED(x, y, 0b0110011 & (currentNote) );
		}
	}
	updateLEDs();
}


void MidiHandler::close() {
	if (isInitialized) {
		resetLaunchpad();

		snd_rawmidi_close(midi_in);
		snd_rawmidi_close(midi_out);
	}
}
