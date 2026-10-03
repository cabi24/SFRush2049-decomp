/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef struct AudioSlot {
    unsigned char active;
    unsigned char pending;
    unsigned char unknown02[18];
    unsigned int position;
    unsigned char unknown18[80];
} AudioSlot;

extern AudioSlot *D_80038294;

void func_80014BB0(int index)
{
    D_80038294[index].active = 0;
}
