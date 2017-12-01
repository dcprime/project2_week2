/*	Filename: audio.cpp
Author: Michael Galle, Darren Conley
Date: Nov 2017
Details: Testing mainline for Windows sound API
Modifications: 
Nov 24, 2017 - created cpp and header files to run audio operations from previously created main.cpp
*/

#include "sound.h"
#include "audio.h"
#include <stdio.h>
#include <windows.h>

char save;
char replay;
char c;                                                 // used to flush extra input
char filename[STRSIZE] = "recordings\\recording.dat";   // file to save/read audio
FILE* f;

int save_and_send(short* iBigBuf, long lBigBufSize) {
    printf("Would you like to save your audio recording? (y/n): ");
    scanf_s("%c", &save, 1);
    while ((c = getchar()) != '\n' && c != EOF) {}								// Flush other input
    if ((save == 'y') || (save == 'Y')) {
        /* Open input file */
        fopen_s(&f, filename, "wb");
        if (!f) {
            printf("unable to open %s\n", filename);
            return 0;
        }
        printf("Writing to sound file ...\n");
        fwrite(iBigBuf, sizeof(short), lBigBufSize, f);
        fclose(f);
        printf("Sending audio recording to receiver ...\n");
        // sending code goes here
        printf("Audio recording sent!\n");
        return 1;
    }
}

int play_audio_file(long lBigBufSize) {

    // buffer used for reading recorded sound from file
    short* iBigBufNew = (short*)malloc(lBigBufSize * sizeof(short));

    /* Open input file */
    fopen_s(&f, filename, "rb");
    if (!f) {
        printf("unable to open %s\n", filename);
        return 0;
    }
    printf("Reading from sound file ...\n");
    fread(iBigBufNew, sizeof(short), lBigBufSize, f);				// Record to new buffer iBigBufNew
    fclose(f);
    InitializePlayback();
    printf("\nPlaying recording from saved file ...\n");
    PlayBuffer(iBigBufNew, lBigBufSize);
    ClosePlayback();
}