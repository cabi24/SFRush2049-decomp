/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* Research submission: stock IDO 7.1 emits the complete native body, but
 * this real 64-byte task definition lacks a registered ROM-TU BSS owner.
 * Do not install or count coverage until storage/layout gates pass.
 *
 * For nonzero command count, configure the task's header, microcode and
 * command fields; write back count * 2576 bytes; invoke the submit hook;
 * then mark completion pending. Other descriptor fields stay untouched.
 * The local pointer's lifetime starts before the count guard and includes
 * the callback argument. Together with compiler-visible task storage this
 * is load-bearing. No padding, unused locals, false prototypes or oracle.
 * OSTask-compatible layout is inferred from native stores and callback ABI;
 * no arcade-source ancestry is established.
 */
typedef struct AudioTask {
    unsigned int type;
    unsigned int flags;
    void *boot;
    unsigned int boot_size;
    void *code;
    unsigned int code_size;
    void *data;
    unsigned int data_size;
    void *stack;
    unsigned int stack_size;
    void *output;
    void *output_size;
    void *commands;
    unsigned int command_count;
    void *yield_data;
    unsigned int yield_size;
} AudioTask;
AudioTask D_800382F8;
extern unsigned char D_80027DD0[];
extern unsigned char D_80027EA0[];
extern unsigned char D_8002A050[];
extern unsigned char D_8002E210[];
extern void (*D_80038000)(AudioTask *);
extern volatile unsigned char D_800382CD;
extern void osWritebackDCache(void *, int);
void func_80011910(void *commands, unsigned short count)
{
    AudioTask *task = &D_800382F8;
    if (count != 0) {
        task->type = 2;
        task->flags = 0;
        task->boot = D_80027DD0;
        task->boot_size = (unsigned int) D_80027EA0 - (unsigned int) D_80027DD0;
        task->code = D_8002A050;
        task->code_size = 4096;
        task->data = D_8002E210;
        task->data_size = 2048;
        task->commands = commands;
        task->command_count = count;
        osWritebackDCache(commands, count * 2576);
        D_80038000(task);
        D_800382CD = 1;
    }
}
