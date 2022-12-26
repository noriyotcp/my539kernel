typedef enum process_state { READY, RUNNING } process_state_t;

typedef struct process_context {
    int eax, ebx, ecx, edx, esp, ebp, esi, edi, eip;
} process_context_t;

typedef struct process {
    int pid;
    process_state_t state;
    process_context_t context;
    int *base_address;
} process_t;
