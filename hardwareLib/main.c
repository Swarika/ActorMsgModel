#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "config.h"
#include "include.h"


void main()
{
	RegisterACTOR(ActorOne, ACTOR1_GSM);			// ACTOR 2 - GSM
	RegisterACTOR(ActorTwo, ACTOR4_Application);	    	// ACTOR 5 - Application
	RegisterACTOR(ActorThree, ACTOR3_Accelerometer);	// ACTOR 4 - Accelerometer
	RegisterACTOR(ActorFour, ACTOR2_GPS);			// ACTOR 3 - GPS

	ACTORs_EventManager();          		 //this will be called at regular small intervals	

	//19.05.16
}
