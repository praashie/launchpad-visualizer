#ifndef __AUDIOHANDLER__
#define __AUDIOHANDLER__
#include <stdio.h>
#include <stdlib.h>

#include <fftw3.h>
#include <alsa/asoundlib.h>

class audioHandler {
	public static snd_pcm_t *pcm_handle;
	public static snd_pcm_stream_t stream_capture = SND_PCM_STREAM_CAPTURE;
}
#endif
