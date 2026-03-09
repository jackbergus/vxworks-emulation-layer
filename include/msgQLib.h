#pragma once

#include "vxworks.h"
#include <sys/types.h>

typedef void* MSG_Q_ID;

/************************** Helper structs ***********************************/
#include <semaphore.h>

// Struct for containing the data required to implement our message queues
typedef struct {
    sem_t semRecv;
    sem_t semSend;
    pthread_mutex_t mutex;

    int numMsgs;
    int maxMsgs;
    int maxMsgLength;
    char* ringBuffer;
    int start;
    int end;
} vxworksMsgQ_t;

#define MSG_Q_FIFO      0x00
#define MSG_Q_PRIORITY  0x01

#define MSG_PRI_NORMAL  0
#define MSG_PRI_URGENT  1

/**
 * Creates and initializes a VxWorks style message queue
 */
MSG_Q_ID msgQCreate(int maxMsgs, int maxMsgLength, int options);

/**
 * Deletes a VxWorks style message queue
 */
STATUS msgQDelete(MSG_Q_ID msgQId);

/**
 * Sends a message on a message queue
 */
STATUS msgQSend(MSG_Q_ID msgQId, char* buffer, uint nBytes, int timeout, int priority);

/**
 * Receives a message on a message queue
 */
STATUS msgQReceive(MSG_Q_ID msgQId, char* buffer, uint maxNBytes, int timeout);

/**
 * Get the number of messages queued to a message queue, or -1 if error
 */
int msgQNumMsgs(MSG_Q_ID msgQId);
