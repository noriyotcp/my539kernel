#include "process.h"

int next_sch_pid, curr_sch_pid;

process_t *next_process;

void scheduler_init();
process_t *get_next_process();
// EAX, ECX, EDX, EBX, ESP, EBP, ESI, EDI, EIP
// pass them to the C function in the order they are pushed on the stack
void scheduler(int eip, int edi, int esi, int ebp, int esp, int ebx, int edx,
               int ecx, int eax);
void run_next_process();
