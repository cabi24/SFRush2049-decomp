/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef struct AudioConfiguration {
    unsigned int frequency;
    unsigned char mode;
    unsigned char sample_bits;
    unsigned char unknown06[256];
    signed char text[256];
} AudioConfiguration;
extern volatile unsigned char D_800382CD;
extern unsigned int D_800382A0;
extern unsigned int D_800382A4;
extern unsigned int D_8004F808;
extern AudioConfiguration D_8004F810;
extern signed char D_8002D890[];
extern signed char D_8002D8A4[];
extern void func_80011104(unsigned short, unsigned int);
extern void func_800114C0(unsigned short, unsigned int, unsigned char);
extern void func_80010DD8(unsigned int, unsigned short);
extern void func_800140F8(void);

void func_80014198(unsigned int *frequency, unsigned short count,
                   unsigned short size, unsigned char mode)
{
    int i;
    float frame_samples;
    if (count > 32) {
        count = 32;
    }
    D_800382CD = 0;
    func_80011104(count, *frequency);
    func_800114C0(count, *frequency, mode);
    func_80010DD8(*frequency, size);
    func_800140F8();
    frame_samples = (float)*frequency / 60.0f;
    D_800382A4 = (unsigned int)(frame_samples / 192.0f + 0.5f);
    D_800382A0 = D_800382A4;
    D_8004F808 = D_800382A4 * 192;
    D_8004F810.frequency = *frequency;
    D_8004F810.sample_bits = 16;
    D_8004F810.mode = 1;
    i = 0;
    while (D_8002D890[i] != 0) {
        D_8004F810.text[i] = D_8002D8A4[i];
        i = (i + 1) & 0xFF;
    }
    D_8004F810.text[i] = 0;
}
