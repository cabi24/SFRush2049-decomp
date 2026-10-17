typedef signed int s32;
extern s32 D_8012E60C,D_8012E668,D_8012E610,D_8012E674,D_8002AFC0,D_8002AFC4;
void func_8008A3E4(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    s32 temp_v0;
    s32 temp_v0_2;

    if (arg0 < 0) {
        D_8012E60C = 0;
    } else {
        D_8012E60C = arg0;
    }
    if (arg1 < 0) {
        D_8012E668 = 0;
    } else {
        D_8012E668 = arg1;
    }
    temp_v0 = D_8002AFC0 - 1;
    if (temp_v0 < arg2) {
        D_8012E610 = temp_v0;
    } else {
        D_8012E610 = arg2;
    }
    temp_v0_2 = D_8002AFC4 - 1;
    if (temp_v0_2 < arg3) {
        D_8012E674 = temp_v0_2;
        return;
    }
    D_8012E674 = arg3;
}

