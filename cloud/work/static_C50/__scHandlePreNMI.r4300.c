/* flags: -g0 -O0 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef unsigned int u32;
typedef unsigned char u8;
typedef struct Task {struct Task *next;u32 state;u32 flags;void *framebuffer;union {struct {u32 type;} t;} list;} Task;
typedef struct Scheduler {u8 opaque0[628];Task *current;} Scheduler;
extern void osDpWait(void);
void __scHandlePreNMI(Scheduler *scheduler) {
    if(1==scheduler->current->list.t.type) {
        scheduler->current->state|=0x10;
        osDpWait();
    } else {
    }
}
