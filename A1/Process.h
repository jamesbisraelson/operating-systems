#ifndef PROCESS_H
#define PROCESS_H

typedef struct process {
  char* user;
	char* process;
	int arrival;
  int duration;
  struct process* next;
} Process;

typedef struct processlist {
  Process* head;
} ProcessList;

Process* mallocProcess();
Process* createProcess(char* user, char* process, int arrival, int duration);
void printProcess(Process* p);
void decrementDuration(Process* p);

ProcessList* mallocProcessList();
void addProcess(ProcessList* pl, Process* p);
void removeHead(ProcessList* pl);
Process* getHead(ProcessList* pl);
bool isEmpty(ProcessList* pl);
void printProcessList(ProcessList* p);

#endif
