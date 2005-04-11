/*

Gravit - A gravity simulator
Copyright 2003-2005 Gerald Kaszuba

Gravit is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

Gravit is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Gravit; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

*/

#include "gravit.h"

#define TIMER_NAME_LENGTH 255
#define TIMER_COMMAND_LENGTH CONSOLE_LENGTH
#define TIMER_MAX 50

typedef struct {

	char name[TIMER_NAME_LENGTH];
	float interval;		// how often to execute
	int repetitions;	// how many times to execute
	int executed;		// number of executions
	Uint32 lastExec;	// timestamp of last execution
	char command[TIMER_COMMAND_LENGTH];
	struct timer_t *next;

} timer_t;

timer_t *timers[50];	// array of pointers...

void timerInit() {

	memset(timers, 0, sizeof(timers));

}

void timerFree() {

	int i;
	timer_t *t;

	for (i = 0; i < TIMER_MAX; i++) {

		t = timers[i];
		if (!t) continue;
		
		free(timers[i]);
		timers[i] = 0;
		return;

	}

}

timer_t *timerNew() {

	int i;
	
	for (i = 0; i < TIMER_MAX; i++) {

		if (timers[i])
			continue;

		timers[i] = (timer_t*)malloc(sizeof(timer_t));
		return timers[i];

	}

	return NULL;

}

void timerAdd(char *name, Uint32 interval, int reps, char *command) {

	timer_t *t = timerNew();
	if (!t) {
		conAdd(2, "Sorry, Maximum timers reached");
		return;
	}
	strncpy(t->name, name, TIMER_NAME_LENGTH);
	strncpy(t->command, command, TIMER_COMMAND_LENGTH);
	t->interval = interval;
	t->repetitions = reps;
	t->executed = 0;
	t->lastExec = getMS();

	conAdd(1, "Added timer '%s'", name);

}

void timerDelbyID(int i) {

	if (!timers[i]) return;
	conAdd(1, "Deleting timer '%s'", timers[i]->name);
	free(timers[i]);
	timers[i] = 0;

}

void timerDel(char *name) {

	int i;
	timer_t *t;

	for (i = 0; i < TIMER_MAX; i++) {

		t = timers[i];
		if (!t) continue;
		if (strncmp(t->name, name, TIMER_NAME_LENGTH)) continue;
		timerDelbyID(i);
		return;

	}

}

void timerUpdate() {

	int i;
	timer_t *t;
	Uint32 ms;

	ms = getMS();

	for (i = 0; i < TIMER_MAX; i++) {

		t = timers[i];
		if (!t) continue;
		if (ms < t->lastExec + t->interval) continue;
		t->lastExec = ms;
		cmdExecute(t->command);
		t->executed++;
		if (t->repetitions == 0) continue;
		if (t->executed == t->repetitions)
			timerDelbyID(i);
		
		return;

	}

}

