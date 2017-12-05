#pragma once

#define STRSIZE 100
#define AUDIOLOOPDELAY 1
#define AUDIOTIMEOUT 30

/******************* function prototypes **********************/

// save audio file and send to receiver
int save_and_send(short* iBigBuf, long lBigBufSize);

// play received audio file
int play_audio_file(int totalAudio);

void StartListeningMode(int* unlistenedAudio, int* totalAudio);