#ifndef _INCLUDE_H_
#define _INCLUDE_H_


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "config.h"
#include "logManager.h"


typedef struct
{
	ACTOR_enum ACTORID;
	u8 STATEID;  
	
} g_EventInfo;

typedef struct Message_info
{
	int msg_ID;
	ACTOR_enum source;
	ACTOR_enum destination;
	u8 msgSTATE;
	int length_payload;
	char payload[MAX_LENGTH_PAYLOAD];
} g_messageInfo;

typedef struct msg_ptr
{
	g_messageInfo data;
	struct msg_ptr *nextMsg;

} g_ACTORMsg;


typedef void (*g_EventHandler)(g_EventInfo *Event);       //Callback


typedef struct Record_ptr
{
	ACTOR_enum ACTORID;
	g_EventHandler Handler;
	g_ACTORMsg *nextMsg_Queue;
	u8 presentState;
	struct Record_ptr *nextACTOR;
	struct Record_ptr *nextMessage;

}g_ACTORNode;


extern g_EventInfo g_EventID;

extern g_ACTORNode *g_ACTORList;    //Entry Point to the list of ACTORs

extern g_ACTORMsg *g_MessageList;    //Entry Point to the message queue


void newMsg_global(g_messageInfo msg_data);
void deleteMsg_global(int MSG_Id);

void newMsg_queue(g_ACTORNode *ActiveACTOR, g_ACTORMsg *ActiveMsg);
void deleteMsg_queue(g_ACTORNode *ACTORAdd, int MSG_Id);


void RegisterACTOR(g_EventHandler HandlerFunction, ACTOR_enum ACTORID);

void ACTORs_EventManager(void);

void ActorOne(g_EventInfo *Event);
void ActorTwo(g_EventInfo *Event);
void ActorThree(g_EventInfo *Event);
void ActorApplication(g_EventInfo *Event);


#endif   //_TAG_APP_MAIN_H_
