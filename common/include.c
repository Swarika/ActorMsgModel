#include "include.h"

g_ACTORNode *g_ACTORList = NULL;    //Entry Point to the list of ACTORs

g_ACTORMsg *g_MessageList = NULL;    //Entry Point to the message queue

char g_TimerCount;

g_EventInfo g_EventID =        
{
	ACTOR0_Test,
	0
};



void RegisterACTOR(g_EventHandler HandlerFunction, ACTOR_enum ACTORID)
{
	g_ACTORNode *NewACTORPtr;
	g_ACTORNode *ActiveACTOR;

	eat_trace("Register an ACTOR with Actor ID as %d", ACTORID);

	NewACTORPtr = malloc(sizeof(g_ACTORNode));

	if(NewACTORPtr == NULL)
		eat_trace("Memory could not be allocated");
	else 
	{
		NewACTORPtr->Handler = HandlerFunction;
		NewACTORPtr->nextMsg_Queue = NULL;     //Message Queue of the ACTOR
		NewACTORPtr->ACTORID = ACTORID;
		NewACTORPtr->nextMessage = NULL;
		NewACTORPtr->presentState = 0;
		NewACTORPtr->nextACTOR = NULL;

		eat_trace("after malloc, Actor ID is %d", NewACTORPtr->ACTORID);
		
		if(g_ACTORList == NULL)                // first node or first STATE
		{
			g_ACTORList = NewACTORPtr;
		}
		else
		{			
			ActiveACTOR = g_ACTORList;

			while(ActiveACTOR->nextACTOR != NULL)
			{
				//eat_trace("ActiveACTOR->nextACTOR has ID as %d", ActiveACTOR->ACTORID);

				ActiveACTOR = ActiveACTOR->nextACTOR;

			}

			ActiveACTOR->nextACTOR = NewACTORPtr;
		}
	}

	//eat_trace("At the end Actor ID is %d\n\n", ActiveACTOR->ACTORID);

}


void ACTORs_EventManager(void)
{
	g_ACTORNode *ActiveACTOR;
	g_ACTORMsg *ActiveMsg;
	
	ActiveACTOR = g_ACTORList;

//	eat_trace("Inside Event Manager");
	
//	while(i<5)
	{

		ActiveMsg = g_MessageList;
		ActiveACTOR = g_ACTORList;

//		eat_trace("Serve all the ACTORs once\n");
		
		while(ActiveACTOR != NULL)
		{
			if(ActiveACTOR->Handler != NULL)
			{
	//			eat_trace("Calling handler of activeActor with ID as %d", ActiveACTOR->ACTORID);

				ActiveACTOR->Handler(&g_EventID);
				
			}

			ActiveACTOR = ActiveACTOR->nextACTOR;
		}
		
//		eat_trace("All ACTORs are served\n");

		ActiveMsg = g_MessageList;
		
		while(ActiveMsg != NULL)	
		{
//			eat_trace("Check for Messages in global queue\n");
			
			ActiveACTOR = g_ACTORList;

			while((ActiveACTOR != NULL) && (ActiveACTOR->ACTORID != ActiveMsg->data.destination))
			{
				ActiveACTOR = ActiveACTOR->nextACTOR;
			}
			
			eat_trace("There is a message for ACTOR %d\n",ActiveACTOR->ACTORID);
			
			newMsg_queue(ActiveACTOR,ActiveMsg);
			
			deleteMsg_global(ActiveMsg->data.msg_ID);

			ActiveMsg = g_MessageList;
		}		
	}
}



void newMsg_global(g_messageInfo msg_data)
{
	g_ACTORMsg *NewMsg;
	g_ACTORMsg *CurrentNode;

	eat_trace("Add a node to global queue\n");
	
	NewMsg = malloc(sizeof(g_ACTORMsg));

	eat_trace("payload received is - %s", msg_data.payload);
	
	if(NewMsg == NULL)
	{
		eat_trace("Memory could not be allocated\n");
	}
	else
	{
		NewMsg->data.destination = msg_data.destination;
		NewMsg->data.source = msg_data.source;
		NewMsg->data.msgSTATE = msg_data.msgSTATE;
		NewMsg->data.length_payload = msg_data.length_payload;
		strcpy(NewMsg->data.payload,msg_data.payload);
		NewMsg->nextMsg = NULL;

		if(g_MessageList== NULL)
		{
			g_MessageList= NewMsg;
			NewMsg->data.msg_ID = 0;

			eat_trace("Added as the first message in the list\n");
		}
		else
		{
			CurrentNode = g_MessageList;
			
			while(CurrentNode->nextMsg != NULL)
			{
				CurrentNode = CurrentNode->nextMsg;
			}

			NewMsg->data.msg_ID = (CurrentNode->data.msg_ID) + 1;
			
			CurrentNode->nextMsg = NewMsg;	

			eat_trace("Added as the next message in the list\n");
		}

		eat_trace("Message added is:\n Destination: %d, source: %d, STATE: %d, Length: %d, payload: %s, msgID: %d\n", 
			NewMsg->data.destination, NewMsg->data.source, NewMsg->data.msgSTATE, NewMsg->data.length_payload, NewMsg->data.payload, NewMsg->data.msg_ID);
	}
}


void deleteMsg_global(int MSG_Id)
{
	g_ACTORMsg *CurrentNode;
	g_ACTORMsg *PrevNode;

	CurrentNode = g_MessageList;
	PrevNode = g_MessageList;

	eat_trace("Delete the node from global queue\n");
	
	if(CurrentNode == NULL)
		eat_trace("Empty List \n");

	else if(CurrentNode->nextMsg == NULL && CurrentNode->data.msg_ID == MSG_Id)       //This is the only node in the list & msg_id also matches
	{
		eat_trace("Node to be deleted is the only node of the list\n");
		g_MessageList = NULL;

		free(CurrentNode);
	}

	else if(CurrentNode->nextMsg != NULL && CurrentNode->data.msg_ID == MSG_Id)
	{
		eat_trace("Node to be deleted is the first node of the list\n");
		g_MessageList = CurrentNode->nextMsg;

		free(CurrentNode);
	}

	else
	{
		CurrentNode = CurrentNode->nextMsg;
		
		while(CurrentNode != NULL)
		{
			if(CurrentNode->data.msg_ID == MSG_Id)
			{
				eat_trace("node found\n");
			}
			else
			{
				CurrentNode = CurrentNode->nextMsg;
				PrevNode = PrevNode->nextMsg;
			}	
		}

		if(CurrentNode->data.msg_ID != MSG_Id)
			eat_trace("Node with the given msg_ID not found\n");
		else
		{
			PrevNode->nextMsg = CurrentNode->nextMsg;

			free(CurrentNode);
		}
	}	

	eat_trace("exiting from delete_node function.\n");
}		


void newMsg_queue(g_ACTORNode *ActiveACTOR, g_ACTORMsg *ActiveMsg)
{
	g_ACTORMsg *NewMsg = NULL;
	g_ACTORNode *CurrentNode = NULL;
	g_ACTORNode *NewNode = NULL;
	
	eat_trace("Add a node to the ACTOR's message queue\n");

	eat_trace("ACTOR ID is %d \n",ActiveACTOR->ACTORID);

	CurrentNode = ActiveACTOR;

	NewNode = malloc(sizeof(g_ACTORNode));
	NewMsg = malloc(sizeof(g_ACTORMsg));


	if(NewNode == NULL || NewMsg == NULL)
	{
		eat_trace("Memory could not be allocated\n");
	}
	else
	{
		//eat_trace("inside else\n");

		NewNode->nextMsg_Queue = NewMsg;

		NewNode->nextMsg_Queue->data.destination = ActiveMsg->data.destination;
		NewNode->nextMsg_Queue->data.source = ActiveMsg->data.source;
		NewNode->nextMsg_Queue->data.msgSTATE = ActiveMsg->data.msgSTATE;
		NewNode->nextMsg_Queue->data.length_payload = ActiveMsg->data.length_payload;
		strcpy(NewNode->nextMsg_Queue->data.payload,ActiveMsg->data.payload);
		NewNode->nextMsg_Queue->data.msg_ID = ActiveMsg->data.msg_ID;
		NewNode->nextMsg_Queue->nextMsg = NULL;
		NewNode->nextACTOR = NULL;
		NewNode->ACTORID = ActiveACTOR->ACTORID;
		NewNode->nextACTOR = NULL;
		NewNode->Handler = NULL;
		NewNode->nextMessage = NULL;

		if(ActiveACTOR->nextMessage == NULL)
		{
			ActiveACTOR->nextMessage = NewNode;

			eat_trace("Added as the first Node in the ACTOR's message queue.\n");
		}
		else
		{
			CurrentNode = ActiveACTOR;

			while(CurrentNode->nextMessage != NULL)
			{				
				CurrentNode = CurrentNode->nextMessage;
				
				eat_trace("Current Message:\n Destination: %d, source: %d, STATE: %d, Length: %d, payload: %s, msgID: %d\n", 
									CurrentNode->nextMsg_Queue->data.destination, CurrentNode->nextMsg_Queue->data.source, 
									CurrentNode->nextMsg_Queue->data.msgSTATE, CurrentNode->nextMsg_Queue->data.length_payload, 
									CurrentNode->nextMsg_Queue->data.payload, CurrentNode->nextMsg_Queue->data.msg_ID);
			}
			
			CurrentNode->nextMessage = NewNode;

			eat_trace("Added as the next message in the ACTOR's message queue.\n");
		}
	}

	eat_trace("Message added is:\n Destination: %d, source: %d, STATE: %d, Length: %d, payload: %s, msgID: %d\n", 
			ActiveMsg->data.destination, ActiveMsg->data.source, 
			ActiveMsg->data.msgSTATE, ActiveMsg->data.length_payload, ActiveMsg->data.payload, ActiveMsg->data.msg_ID);
}



void deleteMsg_queue(g_ACTORNode *ACTORAdd, int MSG_Id)
{

	g_ACTORNode *CurrentNode;
	g_ACTORNode *PrevNode;

	eat_trace("Delete from the ACTOR's message queue the message with MSG_ID as %d\n", MSG_Id);

	CurrentNode = ACTORAdd;

	if(CurrentNode->nextMessage == NULL || CurrentNode->nextMessage->nextMsg_Queue == NULL)
		eat_trace("Empty List \n");
	
	else if((CurrentNode->nextMessage->nextMessage == NULL) && (CurrentNode->nextMessage->nextMsg_Queue->data.msg_ID == MSG_Id))
	{
		eat_trace("The node to be deleted is the only node of the list\n");
		ACTORAdd->nextMessage = NULL;
		
		free(CurrentNode->nextMessage);
	}

	else if((CurrentNode->nextMessage->nextMessage != NULL) && (CurrentNode->nextMessage->nextMsg_Queue->data.msg_ID == MSG_Id))
	{
		eat_trace("The node to be deleted is the first node\n");

		CurrentNode->nextMessage = CurrentNode->nextMessage->nextMessage;

		free(CurrentNode->nextMessage->nextMsg_Queue);

		free(CurrentNode->nextMessage);
	}
	else
	{
		eat_trace("inside else\n");
		
		while(CurrentNode->nextMessage != NULL)
		{
				CurrentNode = CurrentNode->nextMessage;
		}

		if(CurrentNode->nextMessage->nextMsg_Queue->data.msg_ID != MSG_Id)
			eat_trace("Node with the given msg_ID not found\n");
		else
		{
			CurrentNode->nextMessage = CurrentNode->nextMessage->nextMessage;

			free(CurrentNode->nextMessage);
		}
	}	

	eat_trace("exiting from delete_node function.\n");
}

