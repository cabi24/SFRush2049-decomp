/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* Adapted from cloud/matches/boot_tail/func_80025670.c: file-local type names StreamState suffixed _80025670 so several bodies share one ROM TU; no other change. */
typedef struct StreamState_80025670 {
    unsigned char unknown_0000[4573];
    signed char state;
    signed char scale;
    unsigned char unknown_11DF[33];
    volatile unsigned char busy;
    unsigned char value1;
    unsigned char value2;
    unsigned char value3;
    unsigned char unknown_1204[8];
    int handle;
    void *buffer;
    unsigned int buffer_count;
    unsigned char request_state;
    unsigned char unknown_1219[3];
    unsigned int processed;
    float field_1220;
    unsigned int token;
} StreamState_80025670;
extern StreamState_80025670 D_80056230[2];
extern volatile unsigned char D_8002D480[];
extern int func_80025264(unsigned int);
extern int func_800250F0(void);
extern void func_80025120(int);
extern void func_8001C77C(int, unsigned char, unsigned char, unsigned char, unsigned char);

int func_80025670(unsigned int token, unsigned char value1,
                  unsigned char value2, unsigned char value3,
                  unsigned char value4)
{
    int queue_token;

    if (D_8002D480[0]) {
        token = func_80025264(token);
        if (token != (unsigned int)-1) {
            queue_token = func_800250F0();
            if (D_80056230[token].handle != -1) {
                func_8001C77C(D_80056230[token].handle, value1, value2, value3, value4);
            }
            D_80056230[token].value2 = value2;
            D_80056230[token].value3 = value3;
            D_80056230[token].value1 = value1;
            func_80025120(queue_token);
            return 1;
        }
    }
    return 0;
}
