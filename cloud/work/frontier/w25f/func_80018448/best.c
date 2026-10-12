/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* func_80018448 (lib_17dc0, IDO 7.1) - NEAR MISS, not a match. w25f best: LOCK true=310 strict=310 reloc_blind=24
 * objdump-dr DIFFERS(23 rows) (w23a best: true=325, 27 rows). The first part (rows 1-52) now matches retail.
 * DISCLOSED SHAPING DEVICE: line 37 `(u8)(first & 255) & 127` (redundant mask + cast; first is u8). uopt folds
 * `(first & 255) & 127` and plain casts, but keeps this cvtl; ugen evaluates it into a temp (`and $t,$3,255`) and
 * as1 deletes the andi (v1 is known zero-extended from the lbu in the dominating do-loop head), so the temp ring
 * advances and line 37's andi/sll take retail's t6/t7. Proven first at ucode level (cvtl 8 inserted after line 37's
 * `lod first` in uopt's r.O: same 24 rows). The second part (lines 46-61, all register names) is not a single
 * missing pop: no single narrowing or label insertion in lines 43-61 changes it (99 ucode variants, w25f RESULTS). */
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
void func_80018448(void)
{
    int i;
    u8 done;
    u8 first,second;
    s16 change;
    u32 now,deadline;
    int width;
    for (i=0;i<64;i++) {
        done=0;
        if (D_8004BE80->tracks[i].second14 != 0) {
            do {
                first=D_8004BE80->tracks[i].second14[0];
                second=D_8004BE80->tracks[i].second14[1];
                if (first==128 && second==0) {
                    D_8004BE80->tracks[i].second14=0;
                    done=1;
                } else {
                    if (first & 128) {
                        change=(((u8)(first & 255) & 127) << 8) | second;
                        width=2;
                    } else {
                        change=first;
                        width=1;
                    }
                    deadline=D_8004BE80->tracks[i].secondTime20+change;
                    now=D_8004BE80->tracks[i].whole+D_8004BE80->lookahead120;
                    if (now >= deadline) {
                        D_8004BE80->tracks[i].second14 += width;
                        D_8004BE80->tracks[i].secondTime20=deadline;
                        first=D_8004BE80->tracks[i].second14[0];
                        second=D_8004BE80->tracks[i].second14[1];
                        if (first & 128) {
                            change=(first << 8) | second;
                            change <<= 1;
                            change >>= 1;
                            D_8004BE80->tracks[i].second14 += 2;
                        } else {
                            change=(s8)first;
                            ++D_8004BE80->tracks[i].second14;
                        }
                        D_8004BE80->tracks[i].second1A += change;
                        func_80020610(1,D_8004BE80->tracks[i].channel24,(u8)D_8004BE78,D_8004BE80->tracks[i].second1A >> 7);
                        func_80020610(33,D_8004BE80->tracks[i].channel24,(u8)D_8004BE78,D_8004BE80->tracks[i].second1A & 127);
                    } else {
                        done=1;
                    }
                }
            } while (!done);
        }
    }
}
