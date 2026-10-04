/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* Adapted from cloud/matches/boot_tail/func_800149DC.c: file-local type names AudioSlot suffixed _800149DC so several bodies share one ROM TU; no other change. */
typedef struct AudioSlot_800149DC {
    unsigned char active;
    unsigned char pending;
    unsigned char unknown02[18];
    unsigned int position;
    unsigned char unknown18[80];
} AudioSlot_800149DC;

extern AudioSlot_800149DC *D_80038294;

void func_800149DC(int index)
{
    D_80038294[index].pending = 0;
}
