/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* Adapted from cloud/matches/boot_tail/func_80011C84.c: file-local type names AudioState suffixed _80011C84 so several bodies share one ROM TU; no other change. */
/* Reset and activate one 104-byte audio record. Unknown bytes retain observed record stride. */
typedef struct AudioState_80011C84 {
    unsigned char active;
    unsigned char unknown01[0x1F];
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
} AudioState_80011C84;
extern AudioState_80011C84 *D_80038294;
extern void func_80011A10(AudioState_80011C84 *);
void func_80011C84(unsigned short index)
{
    AudioState_80011C84 *audio = &D_80038294[index];
    func_80011A10(audio);
    audio->active = 1;
}
