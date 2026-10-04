/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef signed char s8;
typedef signed short s16;
typedef struct Track {
    u32 fraction,whole,unknown08;
    u8 *notes0C,*first10,*second14;
    u16 first18,second1A;
    u32 firstTime1C,secondTime20;
    u8 channel24,unknown25[3];
} Track;
typedef struct Context {u8 unknown000[0x120];u32 lookahead120;u8 unknown124[0x444];Track tracks[64];} Context;
extern Context *D_8004BE80;
extern u32 D_8004BE78;
extern void func_80020610(u8,u8,u8,u8);
void func_8001824C(void)
{
    int i;
    u8 done;
    u8 first,second;
    u16 increment;
    s16 change;
    u32 now,deadline;
    int width;
    for (i=0;i<64;i++) {
        done=0;
        if (D_8004BE80->tracks[i].first10 != 0) {
            do {
                first=D_8004BE80->tracks[i].first10[0];
                second=D_8004BE80->tracks[i].first10[1];
                if (first==128 && second==0) {
                    D_8004BE80->tracks[i].first10=0;
                    done=1;
                } else {
                    now=D_8004BE80->tracks[i].whole+D_8004BE80->lookahead120;
                    if (first & 128) {
                        increment=((first & 127) << 8) | second;
                        width=2;
                    } else {
                        increment=first;
                        width=1;
                    }
                    deadline=D_8004BE80->tracks[i].firstTime1C+increment;
                    if (now >= deadline) {
                        D_8004BE80->tracks[i].first10 += width;
                        D_8004BE80->tracks[i].firstTime1C=deadline;
                        first=D_8004BE80->tracks[i].first10[0];
                        second=D_8004BE80->tracks[i].first10[1];
                        if (first & 128) {
                            change=(s16)(u16)((((u32)first << 8) | second) << 1);
                            change=(s16)(change >> 1);
                            D_8004BE80->tracks[i].first10 += 2;
                        } else {
                            change=(s16)(u16)((u32)first << 9);
                            change=(s16)(change >> 9);
                            ++D_8004BE80->tracks[i].first10;
                        }
                        D_8004BE80->tracks[i].first18 += change;
                        func_80020610(128,D_8004BE80->tracks[i].channel24,(u8)D_8004BE78,D_8004BE80->tracks[i].first18 >> 7);
                        func_80020610(129,D_8004BE80->tracks[i].channel24,(u8)D_8004BE78,D_8004BE80->tracks[i].first18 & 127);
                    } else {
                        done=1;
                    }
                }
            } while (!done);
        }
    }
}
