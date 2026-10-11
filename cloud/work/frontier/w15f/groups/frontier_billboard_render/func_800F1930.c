/* Full native save-menu input flow; all unavailable helpers remain external. */
typedef signed char s8;
typedef signed int s32;
typedef unsigned int u32;
extern s32 D_80144DA0;
extern s8 D_801460C0, D_801460FC, D_80146131, D_801460F0, D_80146148;
extern s32 D_801461C0[4];
extern u32 D_80156944, D_8015694C;
extern void *D_8014A160, *D_801461A8;
extern s32 func_800CC040(s32, void *, void *, s32);
extern void func_800F1210(s32);
extern void func_800F0F44(s32);

void func_800F1930(void)
{
    s32 finished;
    while (D_80144DA0 == 1 || (D_80144DA0 == 2 && D_801461C0[D_801460C0] != 0 &&
                             D_801461C0[D_801460C0] != 1)) {
        D_80144DA0--;
        if (D_80144DA0 < 0) D_80144DA0 = 3;
    }
    if (D_801460FC == 1 && (D_80156944 & 0x3000)) {
        if (D_8015694C & 0x3000) D_80146131 = !D_80146131;
    } else if (D_80144DA0 == 0 && (D_80156944 & 0x3000)) {
        if (D_8015694C & 0x1000) {
            D_801460C0--;
            if (D_801460C0 < 0) D_801460C0 = 3;
        } else if (D_8015694C & 0x2000) {
            D_801460C0++;
            if (D_801460C0 > 3) D_801460C0 = 0;
        }
    } else if (D_8015694C & 0xC00) {
        if (D_8015694C & 0x400) {
            do {
                D_80144DA0--;
                if (D_80144DA0 < 0) D_80144DA0 = 3;
            } while (D_80144DA0 == 1 || (D_80144DA0 == 2 && D_801461C0[D_801460C0] != 0 &&
                                       D_801461C0[D_801460C0] != 1));
        } else if (D_8015694C & 0x800) {
            do {
                D_80144DA0++;
                if (D_80144DA0 > 3) D_80144DA0 = 0;
            } while (D_80144DA0 == 1 || (D_80144DA0 == 2 && D_801461C0[D_801460C0] != 0 &&
                                       D_801461C0[D_801460C0] != 1));
        }
    } else if (D_8015694C & 3) {
        finished = 0;
        if (D_801460F0 == 1) {
            D_801460F0 = 0;
        } else if (D_801460FC == 1) {
            if ((D_8015694C & 2) && D_80146131 == 1) {
                if (func_800CC040(D_801460C0, D_8014A160, D_801461A8, 0)) {
                    D_80146148 = 1;
                }
            }
            D_801460FC = 0;
        } else if (D_80146148 == 1) {
            D_80146148 = 0;
            finished = 1;
        } else if ((D_8015694C & 2) && D_80144DA0 == 2) {
            if (D_801461C0[D_801460C0] == 0) {
                if (func_800CC040(D_801460C0, D_8014A160, D_801461A8, 0)) {
                    D_80146148 = 1;
                }
            } else if (D_801461C0[D_801460C0] == 1) {
                D_801460FC = 1;
                D_80146131 = 0;
                D_80146148 = 0;
            } else if (D_801461C0[D_801460C0] == 2) {
                D_801460F0 = 1;
            }
        }
        if (D_80144DA0 == 3) {
            func_800CC040(D_801460C0, D_8014A160, D_801461A8, 1);
            D_801461A8 = 0;
            finished = 1;
        }
        if (finished == 1) func_800F1210(5);
    }
    func_800F0F44(D_801460C0);
}
