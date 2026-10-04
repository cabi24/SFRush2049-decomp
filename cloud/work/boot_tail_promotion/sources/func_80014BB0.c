/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* Adapted from cloud/matches/boot_tail/func_80014BB0.c: file-local type names AudioSlot suffixed _80014BB0 so several bodies share one ROM TU; no other change. */
typedef struct AudioSlot_80014BB0 {
    unsigned char active;
    unsigned char pending;
    unsigned char unknown02[18];
    unsigned int position;
    unsigned char unknown18[80];
} AudioSlot_80014BB0;

extern AudioSlot_80014BB0 *D_80038294;

void func_80014BB0(int index)
{
    D_80038294[index].active = 0;
}
