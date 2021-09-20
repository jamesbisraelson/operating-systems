/*
  os_info.c
  COMP 3430 Operating Systems
  
  An example of using the /proc pseudo file system in Linux. This 
  program reads and prints OS info from /proc/sys/kernel.
  
  To compile: gcc os_info.c
*/
#include <stdio.h>
#include <stdlib.h>

#define BUFSIZE 81

int main (){
  FILE* fp;
  char info[BUFSIZE];
  
  printf("OS information\n");
  printf("==============\n");
  fp = fopen ("/proc/sys/kernel/ostype", "r");
  if(fp == NULL)
    printf("file open failed\n");
  else{
    fgets(info, BUFSIZE, fp);
    fclose (fp);
    printf("     OS: %s", info);
   }
  fp = fopen ("/proc/sys/kernel/version", "r");
  if(fp == NULL)
    printf("file open failed\n");
  else{
    fgets(info, BUFSIZE, fp);
    fclose (fp);
    printf("Version: %s", info);
   }
  fp = fopen ("/proc/sys/kernel/osrelease", "r");
  if(fp == NULL)
    printf("file open failed\n");
  else{
    fgets(info, BUFSIZE, fp);
    fclose (fp);
    printf("Release: %s", info);
   }
  
  return 0;
}// main
