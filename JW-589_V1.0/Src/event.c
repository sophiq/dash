#include "event.h"

/*!<*/
#define EVENT_BUFFER_LENGTH 10
typedef struct{
	uint8_t buffer[EVENT_BUFFER_LENGTH];
	uint8_t in;
	uint8_t out;
}event_t;

/*!<*/
static event_t stEvent;

/*!<*/
void PushEvent(uint8_t eid)
{
	uint8_t i;

	if(stEvent.in >= EVENT_BUFFER_LENGTH){
		stEvent.in = 0;
	}

	for(i = 0; i <= stEvent.in; i++){
		if(eid == stEvent.buffer[i]){
			return;
		}
	}

	stEvent.buffer[stEvent.in] = eid;
	stEvent.in++;
}
/*!<*/
uint8_t PopEvent(void)
{
	uint8_t eid;

	if(stEvent.out >= EVENT_BUFFER_LENGTH){
		stEvent.out = 0;
		eid = stEvent.buffer[stEvent.out];
	}
	else{
		if(stEvent.out >= stEvent.in){
			stEvent.out = 0;
			stEvent.in = 0;
			eid = 0xff;
		}
		else{
			eid = stEvent.buffer[stEvent.out];
			stEvent.buffer[stEvent.out] = 0xff;
			stEvent.out++;
		}
	}

	return eid;
}
