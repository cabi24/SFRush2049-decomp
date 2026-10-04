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
    unsigned char unknown_1204[8];
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

unsigned char func_800255F0(unsigned int token)
{
    if (D_8002D480[0]) {
        token = func_80025264(token);
        if (token == -1) {
            return 0;
        }
        return D_80056230[token].busy != 0;
    }
    return 0;
}
