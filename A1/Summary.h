#ifndef SUMMARY_H
#define SUMMARY_H

typedef struct summary {
  char* user;
  int total;
  struct summary* next;
} Summary;

typedef struct summarylist {
  Summary* head;
} SummaryList;

void addSummary(SummaryList* sl, Summary* s);
void printSummaryList(SummaryList* sl);
Summary* mallocSummary();
Summary* createSummary(struct process* p, int time);
void addToTotal(Summary* total, Summary* pToAdd);
SummaryList* mallocSummaryList();
void addSummary(SummaryList* sl, Summary* s);

#endif
