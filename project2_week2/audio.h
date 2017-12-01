#pragma once

#define STRSIZE 100

/******************* function prototypes **********************/

// save audio file and send to receiver
int save_and_send(short* iBigBuf, long lBigBufSize);

// play received audio file
int play_audio_file(long lBigBufSize);