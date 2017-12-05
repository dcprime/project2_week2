/******************* queues.cpp **********************
*                                                    *
*  Functions adapted from ESE lecture slides         *
*  on creating queue functions                       *
*                                                    *
*****************************************************/

#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>
#include <Windows.h>
#include "nodes.h"
#include "queues.h"
#include "RS232Comm.h"



static Node *pNode, *pHead, *pTail;

void InitQueue(void)
{
    pHead = pTail = NULL;
}

// Returns non-zero if queue is empty. 
int  IsQueueEmpty(void)
{
    return (pHead == NULL);
}

void  AddToQueue(Node *pn)
{
    if (pHead == NULL) {
        pHead = pn; //make first item ‘head’
    }
    else {
        pTail->pNext = pn;// make the last 
    }           //input point to the new item 
    pn->pNext = NULL;
    pTail = pn; // make this last input
} // item the ‘tail’   

// Return item at the head
Node *DeQueue(void)
{
    Node *pTemp;
    if (pHead == NULL) return(NULL);
    pTemp = pHead;
    pHead = pHead->pNext; // make next item new ‘head’
    return(pTemp);	  // return old ‘head’ 	
}

void AddMessToQueue(char* msg_text) {
    // create node to hold message content
    if (IsQueueEmpty()) {
        pNode = (link)malloc(sizeof(Node)); 	          // Make first Node
        pHead = pNode;				                          // save its location
        pTail = pNode;
    }
    else {
        pNode->pNext = (link)malloc(sizeof(Node));      // Make Node i
        pNode = pNode->pNext;			                      // Get pointer to Node i
    }

    // add message content to node and add node to queue
    memcpy(pNode->Data.text, msg_text, strlen(msg_text) + 1);
    AddToQueue(pNode);
    printf("\n--- Message added to queue ---\n");
}

void PrintMessages(void) {
    printf("\n--- Messages in queue from oldest to newest ---\n\n");
    traverse(pHead, visit);
    printf("\n--- End of messages ---\n");
}

/************************ waiting mode (text) ************************/
void StartWaitingMode(int* unreadMessages, int* totalMessages) {
    int run = TRUE;
    char msgIn[MSGSIZE];        // buffer to hold incoming message
    int success = 0;
    short dot_counter = 0;
    unsigned long timeout = 0;
    
    while (run == TRUE) {
        success = inputFromPort(msgIn, MSGSIZE);	// Receive string from port
        if (success == 1) {
            // add message to node and node to queue
            AddMessToQueue(msgIn);

            // increment number of unread messages
            (*unreadMessages)++;
			(*totalMessages)++;

            // update the listening status 
            printf("\n%d unread messages in queue\n\n", *unreadMessages);

            // reset success status
            success = 0;
        }
        else if (success == -1) {
            run = FALSE;
        }

        // print a dot every LOOPDELAY times through the loop to show Listening Mode is active
        dot_counter++;
        if (dot_counter == LOOPDELAY) {
            printf(".");
            dot_counter = 0;
        }

        // listening mode time out
        timeout++;
        if (timeout == TIMEOUT) {
            printf("\n\n------------- Waiting Mode timed out -------------\n");
            run = FALSE;
        }

    } // end while loop
}