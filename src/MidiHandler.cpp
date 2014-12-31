#include "../include/MidiHandler.h"
#include "../include/TextScroller.h"

bool MidiHandler::isInitialized;
bool MidiHandler::ledBuffering;
bool MidiHandler::bufferState;

snd_rawmidi_t* MidiHandler::midi_in;
snd_rawmidi_t* MidiHandler::midi_out;

unsigned char MidiHandler::lastCommand;

int MidiHandler::currentNote;

bool MidiHandler::noteBuffer[9][9];
bool MidiHandler::noteBufferOld[9][9];
unsigned char MidiHandler::ledBuffer[9][9];
unsigned char MidiHandler::ledBufferOld[9][9];

int MidiHandler::init(std::string midiDevice) {

	int status = snd_rawmidi_open(&midi_in, &midi_out, midiDevice.c_str(), SND_RAWMIDI_NONBLOCK);
	if (status == 0) {

		currentNote = 0;

		for (int x = 0; x < 9; x++) {
			for (int y = 0; y < 9; y++) {
				ledBuffer[x][y] = 0x0;
				ledBufferOld[x][y] = 0x0;

				noteBuffer[x][y] = false;
				noteBufferOld[x][y] = false;
			}
		}

		lastCommand = 0x00;

		resetLaunchpad();

		isInitialized = true;

		if (ledBuffering) {sendEvent(0xB0, 0x00, 0x31);}

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

//Go through the MIDI Input buffer
void MidiHandler::handleInput() {

	if (!isInitialized) {return;}

	fprintf(stderr, "Reading...");


	unsigned char tempBuffer[1];
	unsigned char readBuffer[1024];
	int byteCount = 0;

	int x, y;
	unsigned char note, velocity;

	//Empty the pending buffer of MIDI input messages
	int status = 0;
	while (status != -EAGAIN && byteCount < 1024) {
		status = snd_rawmidi_read(midi_in, &tempBuffer, 1);
		if (status >= 0) {
			readBuffer[byteCount] = tempBuffer[0];
			byteCount++;
			fprintf(stderr, "%x ", tempBuffer[0]);
		}
	}

	for (int x = 0; x < 9; x++) {
		for (int y = 0; y < 9; y++) {
			noteBufferOld[x][y] = noteBuffer[x][y];
		}
	}


	for (int i = 0; i < byteCount; i++) {

		if (readBuffer[i] > 0x7F) {
			lastCommand = readBuffer[i];
			i++;
		}

		if (lastCommand == 0x90 || lastCommand == 0xB0) {
			note = readBuffer[i];
			velocity = readBuffer[++i];

			if (lastCommand == 0x90) {
				y = (note >> 4); //0x10 = 16 = 2^4
				x = (note & 0x0F);
			} else if (lastCommand == 0xB0) { //Control buttons
                x = note - 0x68;
                y = 8;
			}

			if (x > 8) {x = 8;}

			if (velocity > 0x10) {noteBuffer[x][y] = true;}// fprintf(stderr, "LOLVELOCITY IS: %x", velocity);}
			else {noteBuffer[x][y] = false;}// fprintf(stderr, "velocity is actually %x", velocity);}
		}
	}
	fprintf(stderr, "\n");
}

bool MidiHandler::isButtonDown(int x, int y) {
	if (x < 0 || y < 0 || x > 8 || y > 8) {return false;}

    return noteBuffer[x][y];
}

bool MidiHandler::isButtonPressed(int x, int y) {
	if (x < 0 || y < 0 || x > 8 || y > 8) {return false;}

    return (!noteBufferOld[x][y]) && noteBuffer[x][y];
}

bool MidiHandler::isButtonReleased(int x, int y) {
	if (x < 0 || y < 0 || x > 8 || y > 8) {return false;}

    return noteBufferOld[x][y] && (!noteBuffer[x][y]);
}


void MidiHandler::setLED(int x, int y, unsigned char color) {

	if (x < 0 || y < 0 || x > 8 || y > 8) {return;}

	//sendNote(true, (0x10 * y) + x, color);
	ledBuffer[x][y] = color;
}

//Refresh the 8x8 LED grid on launchpad
void MidiHandler::updateLEDs() {

	if (!ledBuffering) {
		unsigned char buffer[81];

		int i = 0;

		buffer[i] = 0x92; //This midi message initiates the rapid update mode
		i++;

		for (int y = 0; y < 8; y++) {
			for (int x = 0; x < 8; x++) {
				buffer[i] = ledBuffer[x][y];
				i++;
			}
		}

		for (int y = 0; y < 8; y++) {
			buffer[i] = ledBuffer[8][y];
			i++;
		}

		for (int x = 0; x < 8; x++) {
			buffer[i] = ledBuffer[x][8];
			i++;
		}


		snd_rawmidi_write(midi_out, buffer, 81);

	} else { //Launchpad's internal double buffering

		unsigned char color;

		for (int x = 0; x < 9; x++) {
			for (int y = 0; y < 9; y++) {

				if (ledBuffer[x][y] != ledBufferOld[x][y]) {
					color = ledBuffer[x][y] & 0b00110011;

					if (y < 8) {
						sendEvent(0x90, (y * 0x10) + x, color);
					} else {
						sendEvent(0xB0, 0x68 + x, color);
					}
				}
				ledBufferOld[x][y] = ledBuffer[x][y];
			}
		}

		if (bufferState) {
			sendEvent(0xB0, 0x00, 0x31);
		} else {
			sendEvent(0xB0, 0x00, 0x34);
		}

		bufferState = !bufferState;

	}
	snd_rawmidi_drain(midi_out);
}

void MidiHandler::resetLaunchpad() {
	sendEvent(0xB0, 0x00, 0x00);//Reset LEDs
	sendEvent(0xB0, 0x00, 0x01);//Select XY mapping mode
}

void MidiHandler::testMIDI() {

	if (!isInitialized) {return;}

	handleInput();

	//GameOfLife::update();
	TextScroller::update();

	updateLEDs();
	usleep(10000);
}


void MidiHandler::close() {
	if (isInitialized) {
		resetLaunchpad();

		snd_rawmidi_close(midi_in);
		snd_rawmidi_close(midi_out);
	}
}
