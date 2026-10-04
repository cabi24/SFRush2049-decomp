/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* Adapted from cloud/matches/boot_tail/func_80014C18.c: file-local type names AudioSlot suffixed _80014C18 so several bodies share one ROM TU; no other change. */
typedef struct AudioSlot_80014C18 {
    unsigned char active;
    unsigned char pending;
    unsigned char unknown02[18];
    unsigned int position;
    unsigned char unknown18[80];
} AudioSlot_80014C18;

extern AudioSlot_80014C18 *D_80038294;

unsigned int func_80014C18(int index)
{
    return D_80038294[index].position;
}
