/*******************project2_week1_main.cpp ************************/

#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <windows.h>
#include "queues.h"  // Header file includes Prototypes defined in queues.cpp
#include "nodes.h"  // Header file includes Prototypes defined in nodes.cpp
#include "sound.h" // Header file includes Prototypes defined in sound.cpp
#include "audio.h" // Header file includes Prototypes defined in audio.cpp
#include "RS232Comm.h" // Header file includes Prototypes defined in RS232Comm.cpp

#define STRSIZE 100


void main(int argc, char *argv[]) {

    extern short iBigBuf[];									// buffer
    extern long  lBigBufSize;								// total number of samples

    int run = TRUE;
    int unreadMessages = 0;
    int totalMessages = 0;
    char choice[STRSIZE];
    char msg_text[MSGSIZE];

    // initialize the message queue
    InitQueue();
	initPort();

    while (run == TRUE) {

        // menu
        printf("\nChoose an option:\n");
        printf("[R]ecord Audio Message\n");
        printf("[P]lay Audio Message\n");
        printf("[L]istening mode\n");
        printf("[S]end Text Message\n");
        printf("[C]heck Text Messages ");
        printf("- %d messages in queue (%d unread)\n", totalMessages, unreadMessages);
        printf("[Q]uit\n");
        printf("\nEnter R, P, L, S, C, or Q: ");

        fgets(choice, STRSIZE, stdin);

        switch (tolower(choice[0])) {
        case 'r':
            InitializeRecording();
            RecordBuffer(iBigBuf, lBigBufSize);
            CloseRecording();
            save_and_send(iBigBuf, lBigBufSize);
            break;
        case 'p':
            play_audio_file(lBigBufSize);
            break;
        case 'l':
            printf("\nListening Mode active\n");
            StartListeningMode(&unreadMessages, &totalMessages);
            break;
        case 's':
            // get message input from user
            printf("\nEnter message text (up to %d characters) and press ENTER\n", MSGSIZE);
            printf("-> ");
            fgets(msg_text, MSGSIZE, stdin);

            // send message to com port
            sendMessToPort(msg_text);
            break;
        case 'c':
            if (IsQueueEmpty()) {
                printf("\n--- Message queue is empty ---\n");
                break;
            }
            else {
                // print all messages in queue from oldest to newest
                PrintMessages();
                unreadMessages = 0;
                break;
            }
        case 'q':
            run = FALSE;
			endCOM();
            break;
        default:
            printf("\nError: That option is not valid\n\n");
            break;
        }
    } // end while main menu

}