/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef struct AudioSlot {
    unsigned char active;
    unsigned char pending;
    unsigned char unknown02[18];
    unsigned int position;
    unsigned char unknown18[16];
    unsigned short release_count;
    unsigned char unknown2A[55];
    unsigned char release_pending;
    unsigned char unknown62[6];
} AudioSlot;
extern AudioSlot *D_80038294;
extern void func_80011A3C(AudioSlot *);
void func_8001489C(int index, unsigned char release)
{
    D_80038294[index].release_count = 20;
    if (release) {
        func_80011A3C(&D_80038294[index]);
    }
}
