#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "Process.h"
#include "Summary.h"

int main(void) {
  SummaryList* summaryList = mallocSummaryList();
  ProcessList* list = mallocProcessList();

  char* input = scanf("%s");
  printf()

  printf("Time\tJob\n");
  int time = 0;
  do {
    addProcess(list, io[time]);
    addSummary(summaryList, runProcess(list, time));
    time++;
  } while(!isEmpty(list));

  printf("%d\tIDLE\n\n", time);
  printSummaryList(summaryList);
}

//allocates memory for a Process
Process* mallocProcess() {
  Process* p = malloc(sizeof(Process)+sizeof(char*)*2+sizeof(int)*2+sizeof(Process*)*2);
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
  else {
    printf("%d\tIDLE\n", time);
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

struct summary* runProcess(ProcessList* pl, int time) {
  Summary* output = NULL;
  Process* ready = findReadyProcess(pl, time);
  printProcess(ready, time);
  if(ready != NULL) {
    decrementDuration(ready);
    if(ready->duration == 0) {
      removeProcess(pl, ready);
      output = createSummary(ready, time);
    }
  }
  return output;
}

Summary* mallocSummary() {
  Summary* s = malloc(sizeof(Summary)+sizeof(char*)+sizeof(int)+sizeof(Summary*));
  if(s==NULL) return NULL;
  s->next = NULL;
  return s;
}

//creates a Process with the given data
Summary* createSummary(struct process* p, int time) {
  Summary* s = mallocSummary();
  s->user = p->user;
  s->total = time + 1;
  return s;
}

void addToTotal(Summary* total, Summary* pToAdd) {
  total->total = pToAdd->total;
}

//allocates memory for a ProcessList
SummaryList* mallocSummaryList() {
  SummaryList* sl = malloc(sizeof(SummaryList)+sizeof(Summary*));
  return sl;
}

void addSummary(SummaryList* sl, Summary* s) {
  if(s==NULL) {
    return;
  }
  Summary* current = sl->head;
  if(current == NULL) {
    sl->head = s;
  }
  while(current != NULL) {
    if(!strcmp(current->user, s->user)) {
      addToTotal(current, s);
      return;
    }
    else if(current->next == NULL) {
      current->next = s;
      return;
    }
    current = current->next;
  }
}

void printSummaryList(SummaryList* sl) {
  printf("Summary\n");

  Summary* current = sl->head;
  while(current != NULL){
    printf("%s\t%d\n", current->user, current->total);
    current = current->next;
  }
  printf("\n");
}
