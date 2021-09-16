#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "Process.h"

int main(void) {
  Process* io[4];
  io[0] = createProcess("James", "A", 2, 5);
  io[1] = createProcess("James", "B", 2, 3);
  io[2] = createProcess("James", "C", 5, 5);
  io[3] = createProcess("James", "D", 6, 2);
  ProcessList* list = mallocProcessList();

  int i = 0;
  while(!isEmpty(list) || i==0) {
    addProcess(list, io[i]);
    decrementDuration(getHead(list));
    printProcess(list->head);
    i++;
  }
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
void printProcess(Process* p) {
  printf("%s\t%s\t%d\t%d\n", p->user, p->process, p->arrival, p->duration);
}

//decrements the duration of a Process
void decrementDuration(Process* p){
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

  if(current == NULL) {
    pl->head = p;
    return;
  }

  if(p->duration < current->duration) {
    p->next = current;
    pl->head = p;
    return;
  }

  while(current->next != NULL) {
    if(p->duration >= current->duration && p->duration < current->next->duration) {
      p->next = current->next;
      current->next = p;
      return;
    }
    current = current->next;
  }
  current->next = p;

}

//removes the first Process in a ProcessList
void removeHead(ProcessList* pl) {
  pl->head = pl->head->next;
}

Process* getHead(ProcessList* pl) {
  return pl->head;
}

//returns true if the ProcessList is empty; false otherwise
bool isEmpty(ProcessList* pl){
  if(pl->head == NULL){
    return true;
  }
  return false;
}

//prints every Process in a ProcessList
void printProcessList(ProcessList* p) {
  Process* current = p->head;
  while(current != NULL) {
    printProcess(current);
    current = current->next;
  }
}
