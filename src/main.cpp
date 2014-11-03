#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

#include <tclap/CmdLine.h>

#include "../include/GameOfLife.h"


//====================================================
//Signal handling
//====================================================

sig_atomic_t interruptionReceived = 0;
sig_atomic_t terminationReceived = 0;

void sigIntHandler(int s) {
	interruptionReceived = 1;
}

void sigTermHandler(int s) {
	terminationReceived = 1;
}

bool isInterrupted() {
	if (interruptionReceived > 0) {
		interruptionReceived = 0;
		return true;
	}
	return false;
}

bool isTerminated() {
	if (terminationReceived > 0) {
		terminationReceived = 0;
		return true;
	}
	return false;
}
//====================================================
//Variables
//====================================================

std::string alsaDeviceName;
std::string launchpadDeviceName;
bool verbose;

//====================================================
//Methods
//====================================================

//Cleanup and termination
void endProgram(int status) {

	if (verbose) {fprintf(stderr, "Closing MIDI connections...\n");}
	MidiHandler::close();

	if (verbose) {fprintf(stderr, "Exiting...\n");}
	exit(status);
}

//Signal Handling preparation
void prepareSignalHandling() {

	struct sigaction sigIntAct;
	struct sigaction sigTermAct;

	sigIntAct.sa_handler = sigIntHandler;
	sigTermAct.sa_handler = sigTermHandler;

	sigemptyset(&sigIntAct.sa_mask);
	sigemptyset(&sigTermAct.sa_mask);

	sigIntAct.sa_flags = 0;
	sigTermAct.sa_flags = 0;

	sigaction(SIGINT, &sigIntAct, 0);
	sigaction(SIGTERM, &sigTermAct, 0);
}

//Command line argument handling using TCLAP
void handleTCLAP(int argc, char* argv[]) {

	try {
		//Program definition
		TCLAP::CmdLine cmd("LPVisualizer displays real-time audio spectrum on a Novation Launchpad.", ' ', "0.0");

		//Argument definitions
		TCLAP::ValueArg<std::string> alsaDeviceArg	("i", "input", "Device name of the ALSA audio input to be analyzed", true, "", "string");
		TCLAP::ValueArg<std::string> launchpadDeviceArg	("o", "output", "Device name of the ALSA MIDI output to be used as a display", true, "", "string");
		TCLAP::SwitchArg verboseArg		("v", "verbose", "Give verbose output", false);
		TCLAP::SwitchArg ledBufferingArg("b", "buffered-leds", "Update the launchpad LEDs by using the launchpad's internal buffers");

		//Add argument definitions into program definition
		cmd.add(alsaDeviceArg);
		cmd.add(launchpadDeviceArg);
		cmd.add(verboseArg);
		cmd.add(ledBufferingArg);

		//Parse arguments
		cmd.parse( argc, argv);

		alsaDeviceName 				= alsaDeviceArg.getValue();
		launchpadDeviceName 		= launchpadDeviceArg.getValue();
		verbose 					= verboseArg.getValue();
		MidiHandler::ledBuffering 	= ledBufferingArg.getValue();

	} catch (TCLAP::ArgException &e) {
		std::cerr << "Error: " << e.error() << " for arg " << e.argId() << std::endl;
		endProgram(1);
	}
}

//Initialization
int initProgram() {

	int status;

	prepareSignalHandling();

	//Prepare MIDI
	if (verbose) {fprintf(stderr, "Initializing MIDI...\n");}
	status = MidiHandler::init(launchpadDeviceName);
	if (status != 0) {
		fprintf(stderr, "Error: Could not initialize midi interface\n");
		endProgram(2);
	}

	GameOfLife::init();
}

//Updating
void updateProgram() {

	//TODO

	MidiHandler::testMIDI();

}


//====================================================
//Main program
//====================================================

int main (int argc, char *argv[]) {

	handleTCLAP(argc, argv);
	initProgram();

	if (verbose) {fprintf(stderr, "Running...\n");}
	//Main loop
	bool isRunning = true;
	while (isRunning) {

		updateProgram();

		//Stop when SIGINT or SIGTERM is received
		if (isInterrupted() || isTerminated()) {
			if (verbose) {fprintf(stderr, "\nProgram Interrupted: Terminating...\n");}
			isRunning = false;
		}
	}

	endProgram(0);
}

