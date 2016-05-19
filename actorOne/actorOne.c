// ACTOR ONE

#include "ActorOneAdapter.h"
#include "ActorOneActor.h"


ActorOneState_enum ActorOneState = SLEEP;

void ActorOneState_set(ActorOneState_enum state)
{
	ActorOneState = state;
}


ActorOneState_enum ActorOneState_get()
{	
	return ActorOneState;
}


void ActorOne(g_EventInfo *Event)
{	
		
//	eat_trace("inside Actor One");

	processMsg_actorOne(ACTOR_One);
	
        switch(ActorOneState)
	{
		case ActorOne_On: eat_trace("State = ActorOne_On");                          
			break; 

		case ActorOne_Off: eat_trace("State = ActorOne_Off");                          
			break; 

		default:	
			break;
	}

}


void processMsg_actorOne(ACTOR_enum ACTORID)
{
	g_ACTORNode *ActiveACTOR;
	g_ACTORNode *MessageNode;
	g_messageInfo msg_data;

	g_messageInfo msgSend;
	
	char msgPayload[MAX_LENGTH_PAYLOAD];

	int i=0;
	
	ActiveACTOR = g_ACTORList;

	while(ActiveACTOR != NULL)
	{
		if(ActiveACTOR->ACTORID == ACTORID)
			break;
		else
			ActiveACTOR = ActiveACTOR->nextACTOR;
	}

//	eat_trace("Check if ACTOR %d has any messages in its queue \n",ActiveACTOR->ACTORID);

	MessageNode = ActiveACTOR->nextMessage;

	while (MessageNode != NULL)
	{
		eat_trace("Yes, There is a message\n");
		
		eat_trace("Message length is %d ",MessageNode->nextMsg_Queue->data.length_payload);
		eat_trace("and message is %s\n",MessageNode->nextMsg_Queue->data.payload);
		eat_trace("and STATE is %d\n",MessageNode->nextMsg_Queue->data.msgSTATE);
		eat_trace("and source is %d\n",MessageNode->nextMsg_Queue->data.source);
		eat_trace("and MSG_ID is %d\n",MessageNode->nextMsg_Queue->data.msg_ID);

		msg_data.msgSTATE = MessageNode->nextMsg_Queue->data.msgSTATE;
		strcpy(msgPayload,MessageNode->nextMsg_Queue->data.payload);

		eat_trace("Message read. Now deleting it\n");

		deleteMsg_queue(ActiveACTOR,MessageNode->nextMsg_Queue->data.msg_ID);
		
		MessageNode = MessageNode->nextMessage;	

		switch(msg_data.msgSTATE)
		{
			case ActorOnestdby: eat_trace("power off actor one\n");
						   ActorOneState = ActorOne_off;
				break;

			case ActorOneOn: eat_trace("Switch on Actor One");
						   ActorOneState = ActorOne_on;
				break;						
			
			case NoMsgActorOne: eat_trace("No msg for Actor one");
				break;

			default: eat_trace("default case\n");
				break;
		}
	}
}

