#ifndef __FFTHANDLER__
#define __FFTHANDLER__
#include <fftw3.h>

class FFTHandler {
	public:
	int init();
	void close();
};
#endif
