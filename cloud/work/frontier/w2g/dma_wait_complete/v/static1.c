/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef signed int s32;
typedef unsigned int u32;
typedef signed short s16;
typedef float f32;

typedef struct {
    char pad[0x274];
    void *volatile cur;
} Sched;

extern char D_80152788[];
extern char D_801527A8[];
extern Sched D_8002E8E8;
extern f32 D_80152748;
extern void (*D_8011EAA8)(void);
extern void *D_8002AFA0;
extern f32 D_8002AFB8;

void osCreateMesgQueue(void *, void *, s32);
void osScAddClient(void *, void *, void *);
s32 osRecvMesg(void *, void *, s32);
void func_800205E4(void);

void controller_rumble_thunk(void) {
    func_800205E4();
}

static void func_wait(void) {
    while (D_8002AFA0 != 0 && D_8002AFA0 == D_8002E8E8.cur) {
    }
}

void dma_wait_complete(void *arg) {
    s32 client[2];
    s16 *msg = 0;

    osCreateMesgQueue(D_80152788, D_801527A8, 8);
    osScAddClient(&D_8002E8E8, client, D_80152788);
    D_80152748 = 0.0f;
    while (1) {
        osRecvMesg(D_80152788, &msg, 1);
        switch (*msg) {
        case 1:
            if (D_8011EAA8 != 0) {
                func_wait();
                D_8011EAA8();
            }
            D_80152748 += D_8002AFB8;
            if (D_80152748 > 14400.0f) {
                D_80152748 -= 14400.0f;
            }
            break;
        case 4:
            controller_rumble_thunk();
            break;
        }
    }
}
