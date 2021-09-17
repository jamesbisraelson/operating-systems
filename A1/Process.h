#ifndef PROCESS_H
#define PROCESS_H

typedef struct process {
  char* user;
	char* process;
	int arrival;
  int duration;
  struct process* next;
  struct process* prev;
} Process;

typedef struct processlist {
  Process* head;
} ProcessList;

Process* mallocProcess();
Process* createProcess(char* user, char* process, int arrival, int duration);
void printProcess(Process* p, int time);
void decrementDuration(Process* p);
void insertProcess(Process* current, Process* p);

ProcessList* mallocProcessList();
void addProcess(ProcessList* pl, Process* p);
bool isEmpty(ProcessList* pl);
//void printProcessList(ProcessList* p);
Process* findReadyProcess(ProcessList* pl, int time);
void runProcess(ProcessList* pl, int time);
void removeProcess(ProcessList* pl, Process* p);

#endif
