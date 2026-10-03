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
extern int func_80025264(unsigned int);
extern int func_800250F0(void);
extern void func_80025120(int);

float func_800259A8(unsigned int token, float *value)
{
    int queue_token;
    float ratio;

    if (token != (unsigned int)-1 && D_8002D480[0]) {
        token = func_80025264(token);
        if (token != (unsigned int)-1) {
            queue_token = func_800250F0();
            if (D_80056230[token].busy == 2) {
                ratio = (float)D_80056230[token].processed / (float)D_80056230[token].rate;
            } else {
                ratio = 0.0f;
            }
            *value = D_80056230[token].field_1220;
            func_80025120(queue_token);
            return ratio;
        }
    }
    return 0.0f;
}
