#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "Process.h"

int main(void) {
  Process* io[4];
  io[0] = createProcess("Jim", "A", 2, 5);
  io[1] = createProcess("Mary", "B", 2, 3);
  io[2] = createProcess("Sue", "D", 5, 5);
  io[3] = createProcess("Mary", "C", 6, 2);
  ProcessList* list = mallocProcessList();

  printf("Time\tJob\n");
  int time = 0;
  do {
    if(time<4) {
      addProcess(list, io[time]);
    }
    runProcess(list, time);
    time++;
  } while(!isEmpty(list));

  printf("%d\tIDLE\n", time);
}

//allocates memory for a Process
Process* mallocProcess() {
  Process* p = malloc(sizeof(Process)+sizeof(char*)*2+sizeof(int)*2+sizeof(Process*));
  if(p==NULL) return NULL;
  p->next = NULL;
  return p;
}

//creates a Process with the given data
Process* createProcess(char* user, char* process, int arrival, int duration) {
  Process* p = mallocProcess();
  p->user = user;
  p->process = process;
  p->arrival = arrival;
  p->duration = duration;
  return p;
}

//prints a Process' data
void printProcess(Process* p, int time) {
  if(p != NULL) {
    printf("%d\t%s\n", time, p->process);
  }
}

//decrements the duration of a Process
void decrementDuration(Process* p) {
  p->duration -= 1;
}

//allocates memory for a ProcessList
ProcessList* mallocProcessList() {
  ProcessList* pl = malloc(sizeof(ProcessList)+sizeof(Process*));
  return pl;
}

//adds a Process to the a ProcessList ordered by duration
void addProcess(ProcessList* pl, Process* p) {
  Process* current = pl->head;

  //if head is null
  if(current == NULL) {
    pl->head = p;
    return;
  }

  //if p duration is less than head
  if(p->duration < current->duration) {
    p->next = current;
    current->prev = p;
    pl->head = p;
    return;
  }

  //if p duration is equal to head and p arrival is less than head arrival
  if(p->duration == current->duration && p->arrival < current->arrival) {
    p->next = current;
    current->prev = p;
    pl->head = p;
    return;
  }

  while(current->next != NULL) {
    if(p->duration < current->next->duration) {
      insertProcess(current, p);
      return;
    }
    else if (p->duration == current->next->duration && p->arrival < current->next->arrival) {
      insertProcess(current, p);
      return;
    }
    current = current->next;
  }
  current->next = p;

}

void insertProcess(Process* current, Process* p) {
  current->next->prev = p;
  p->next = current->next;
  current->next = p;
  p->prev = current;
}

//removes the first Process in a ProcessList
void removeProcess(ProcessList* pl, Process* p) {
  if(p == pl->head) {
    pl->head = p->next;
  }
  else{
    p->prev->next = p->next;
  }
}

//returns true if the ProcessList is empty; false otherwise
bool isEmpty(ProcessList* pl){
  if(pl->head == NULL){
    return true;
  }
  return false;
}

//finds the first ready process in a ProcessList
Process* findReadyProcess(ProcessList* pl, int time) {
  Process* current = pl->head;
  while(current != NULL) {
    if(current->arrival <= time) {
      break;
    }
    current = current->next;
  }
  return current;
}

void runProcess(ProcessList* pl, int time) {
  Process* ready = findReadyProcess(pl, time);
  printProcess(ready, time);
  if(ready != NULL) {
    decrementDuration(ready);
    if(ready->duration == 0) removeProcess(pl, ready);
  }
}
