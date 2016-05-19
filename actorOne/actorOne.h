//ActorOne.h

#ifndef _ACC_ACTOR_H_
#define _ACC_ACTOR_H_


#include <config.h>
#include <include.h>


typedef enum
{
   ActorOnestdby,
   ActorOneOn,
   ActorOneNoMsg

} ActorOnemsg_enum;

typedef enum
{
   ActorOneINT =0,
   ActorOneOFF =1,
	
} APPmsg_enum;


void ActorOneState_set(ActorOneState_enum state);

ActorOneState_enum ActorOneState_get();


void processMsg_accelerometer(ACTOR_enum ACTORID);


#endif

