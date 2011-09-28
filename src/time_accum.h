#ifndef TIME_ACCUM_H
#define TIME_ACCUM_H

#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdio.h>

#ifndef CACHE_LINE_SIZE
#define CACHE_LINE_SIZE 64
#endif

#define BILLION 1000000000LL


/** Edit these events for each benchmark **/
enum Events {
	EVENT1_GLOBAL,
	EVENT1,
	EVENT2_GLOBAL,
	EVENT2,
	TM_ACCUM_NUM_EVENTS //Don't delete this event, used keep track of total events 
};

extern char eventAccumNames[TM_ACCUM_NUM_EVENTS][30];
void time_accum_init_names() {
	strcpy(eventAccumNames[EVENT1_GLOBAL], "Event1 Global");
	strcpy(eventAccumNames[EVENT1], "Event1");
	strcpy(eventAccumNames[EVENT2], "Event2");
	strcpy(eventAccumNames[EVENT2_GLOBAL], "Event2 Global");
}

struct EventAccum {
	long long * times;
	struct timespec * start;
	int maxCores;
};

extern struct EventAccum eventAccum;



void time_accum_init(int cores) {
	time_accum_init_names();
	int numEvents = TM_ACCUM_NUM_EVENTS;
	if (TM_ACCUM_NUM_EVENTS < (CACHE_LINE_SIZE/sizeof(long long))) {
		numEvents = CACHE_LINE_SIZE/sizeof(long long) + 1; // plus one just to make sure
	}
	int arraySize = cores*numEvents*sizeof(long long);
	eventAccum.times = (long long *)malloc(arraySize);
	eventAccum.start = (struct timespec *)malloc(arraySize);

	memset(eventAccum.times, 0, arraySize);
	memset(eventAccum.start, 0, arraySize);
	eventAccum.maxCores = cores;
}

void time_accum_start(int core, enum Events e) {
	//struct timespec& ts = ((eventAccum->start)[eventAccum->maxCores*TM_ACCUM_NUM_EVENTS+e]);
	clock_gettime(CLOCK_REALTIME, &((eventAccum.start)[core*TM_ACCUM_NUM_EVENTS+e]));
}

void time_accum_stop(int core, enum Events e) {
	struct timespec stop;
	clock_gettime(CLOCK_REALTIME, &stop);
	struct timespec start = (eventAccum.start)[core*TM_ACCUM_NUM_EVENTS+e];
	(eventAccum.times)[core*TM_ACCUM_NUM_EVENTS+e] += (stop.tv_sec - start.tv_sec) * BILLION + stop.tv_nsec - start.tv_nsec;
}

void time_accum_print() {
	int core, event;
	for (core = 0; core < eventAccum.maxCores; core++) {
		for (event = 0; event < TM_ACCUM_NUM_EVENTS; event++) {
			printf("%i\t%s\t%lld\n", core, eventAccumNames[event], (eventAccum.times)[core*TM_ACCUM_NUM_EVENTS+event]);
		}
	}
}

void time_accum_cleanup() {
//  do nothing for now
//	free (eventAccum.times);
//	free (eventAccum.start);
}

#endif // TIME_ACCUM_H
