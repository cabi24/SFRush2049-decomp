/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef struct StreamState {
    unsigned char unknown_0000[4573];
    signed char state;
    signed char scale;
    unsigned char unknown_11DF[33];
    volatile unsigned char busy;
    unsigned char value1;
    unsigned char value2;
    unsigned char value3;
    unsigned char unknown_1204[4];
    unsigned int rate;
    int handle;
    void *buffer;
    unsigned int buffer_count;
    unsigned char request_state;
    unsigned char unknown_1219[3];
    unsigned int processed;
    float field_1220;
    unsigned int token;
} StreamState;
extern StreamState D_80056230[2];
extern volatile unsigned char D_8002D480[];
typedef struct OSMesgQueue OSMesgQueue;
typedef struct ServiceHooks {
    unsigned char unknown_00[24];
    void *(*allocate)(unsigned int, int);
    void (*release)(void *);
} ServiceHooks;
extern ServiceHooks D_80038000;
extern unsigned int D_800586A0;
extern unsigned int D_80058680;
extern void *D_8005868C;
extern void *D_80058698[2];
extern void (*D_80038024)(void);
extern void func_80025EB0(StreamState *, void *, void *, unsigned int,
                         void (*)(void *, void *, unsigned int, OSMesgQueue *));
extern void func_80024FB0(StreamState *);
extern void func_800250AC(void);
extern unsigned int func_8001C770(unsigned int);
extern void osInvalDCache(void *, int);
extern void func_8002574C(void);

void func_80025C68(unsigned int flags)
{
    int i;
    unsigned int size;

    D_800586A0 = flags;
    D_8005868C = 0;
    for (i = 0; i < 2; i++) {
        func_80025EB0(&D_80056230[i], 0, 0, 0, 0);
        func_80024FB0(&D_80056230[i]);
        D_80056230[i].busy = 0;
    }
    func_800250AC();
    if (D_800586A0 & 1) {
        size = func_8001C770(4320);
        for (i = 0; i < 2; i++) {
            D_80058698[i] = D_80038000.allocate(size, 0);
            osInvalDCache(D_80058698[i], size);
        }
    }
    D_80038024 = func_8002574C;
    D_8002D480[0] = 1;
    D_80058680 = 0;
}
