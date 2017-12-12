/*	Filename: audio.cpp
Author: Michael Galle, Darren Conley
Date: Nov 2017
Details: Testing mainline for Windows sound API
Modifications:
Nov 24, 2017 - created cpp and header files to run audio operations from previously created main.cpp
*/

#include <stdio.h>
#include <Windows.h>
#include "sound.h"
#include "audio.h"
#include "RS232Comm.h"

char replay;
char c;                                                 // used to flush extra input
char filename[STRSIZE] = "recordings\\recording.dat";   // file to save/read audio
FILE* f;

extern short iBigBufIn[];	// receiving buffer
extern long lBigBufSize;	// total number of samples in buffer
const int audio_as_char = SAMPLES_SEC * RECORD_TIME * (sizeof(short) / sizeof(char));
char audio_out[audio_as_char];

int save_and_send(short* iBigBuf, long lBigBufSize) {
	char send;
	printf("\nWould you like to send your audio recording? (y/n): ");
	scanf_s("%c", &send, 1);
	while ((c = getchar()) != '\n' && c != EOF) {}		// Flush other input
	if ((send == 'y') || (send == 'Y')) {
		/* ----- function to write out to file ---------
		fopen_s(&f, filename, "wb");
		if (!f) {
		printf("unable to open %s\n", filename);
		return 0;
		}
		printf("Writing to sound file ...\n");
		fwrite(iBigBuf, sizeof(short), lBigBufSize, f);
		fclose(f);*/

		printf("\nSending audio recording to receiver ...\n");
		memcpy(audio_out, iBigBuf, audio_as_char);
		outputToPort(audio_out, audio_as_char);
		Sleep(1000); // play with this number to see how short (or eliminate?)
		purgePort();
		return 1;
	}
}



int play_audio_file(int totalAudio) {

	/* code for opening audio from external file
	fopen_s(&f, filename, "rb");
	if (!f) {
	printf("unable to open %s\n", filename);
	return 0;
	}
	printf("Reading from sound file ...\n");
	fread(iBigBufNew, sizeof(short), lBigBufSize, f);				// Record to new buffer iBigBufNew
	fclose(f); */
	InitializePlayback();
	printf("\nPlaying recording from received file...\n");
	PlayBuffer(iBigBufIn, lBigBufSize);
	ClosePlayback();
	return 1;
}

void StartListeningMode(int* unlistenedAudio, int* totalAudio) {

	int run = TRUE;
	int success = 0;
	char audioIn[audio_as_char];
	short dot_counter = 0;
	unsigned long timeout = 0;

	while (run == TRUE) {
		success = inputFromPort(audioIn, audio_as_char);	// Receive audio from port
		if (success == 1) {
			// copy audio to iBigBufIn
			memcpy(iBigBufIn, audioIn, audio_as_char);

			// increment number of unread messages
			(*unlistenedAudio)++;
			(*totalAudio)++;

			// update the listening status 
			printf("\n%d unheard messages in queue\n\n", *unlistenedAudio);

			// reset success status
			success = 0;
			run = FALSE;
		}
		else if (success == -1) {
			run = FALSE;
		}

		/* print a dot every LOOPDELAY times through the loop to show Listening Mode is active
		dot_counter++;
		if (dot_counter == AUDIOLOOPDELAY) {
			printf(".");
			dot_counter = 0;
		}*/

		// listening mode time out
		timeout++;
		if (timeout == AUDIOTIMEOUT) {
			printf("\n\n------------- Waiting Mode timed out -------------\n");
			run = FALSE;
		}

	} // end while loop
}