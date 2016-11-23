#ifndef __DATA_H__
#define __DATA_H__

#define MAX_MESSAGE_DATA 1000

#pragma pack(1)

typedef struct {
	long time;
	int status;
	char data[MAX_MESSAGE_DATA+1];
	long index;
} Message;

#pragma pack()

#endif
