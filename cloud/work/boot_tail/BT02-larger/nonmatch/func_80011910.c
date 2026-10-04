/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
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
extern AudioTask D_800382F8;
extern unsigned char D_80027DD0[];
extern unsigned char D_80027EA0[];
extern unsigned char D_8002A050[];
extern unsigned char D_8002E210[];
extern void (*D_80038000)(AudioTask *);
extern volatile unsigned char D_800382CD;
extern void osWritebackDCache(void *, int);
void func_80011910(void *commands, unsigned short count)
{
    if (count != 0) {
        D_800382F8.type = 2;
        D_800382F8.flags = 0;
        D_800382F8.boot = D_80027DD0;
        D_800382F8.boot_size = (unsigned int) D_80027EA0 - (unsigned int) D_80027DD0;
        D_800382F8.code = D_8002A050;
        D_800382F8.code_size = 4096;
        D_800382F8.data = D_8002E210;
        D_800382F8.data_size = 2048;
        D_800382F8.commands = commands;
        D_800382F8.command_count = count;
        osWritebackDCache(commands, count * 2576);
        D_80038000(&D_800382F8);
        D_800382CD = 1;
    }
}
