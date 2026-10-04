/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* Adapted from cloud/matches/boot_tail/func_80013C84.c: file-local type names AudioState suffixed _80013C84 so several bodies share one ROM TU; no other change. */
typedef struct AudioState_80013C84 {
    unsigned char active;
    unsigned char unknown01[7];
    double position;
    unsigned int current;
    unsigned int previous;
    unsigned char unknown18[8];
    unsigned short initial_count;
    unsigned char unknown22[6];
    unsigned short release_count;
    unsigned char unknown2A[0x1E];
    unsigned short count;
    unsigned short unknown4A;
    float value;
    unsigned int step;
    float scale;
    float saved_value;
    unsigned char state;
    unsigned char unknown5D[0xB];
} AudioState_80013C84;
extern AudioState_80013C84 *D_80038294;
extern unsigned short D_8003829C;
void func_80013C84(void)
{
    int i;
    for (i = 0; i < D_8003829C; i++) {
        if (D_80038294[i].active != 0) {
            D_80038294[i].previous = D_80038294[i].current;
            D_80038294[i].current = (unsigned int) D_80038294[i].position;
        }
    }
}
