/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef struct ServiceHooks {
    unsigned char unknown_00[20];
    void *(*translate)(void *);
    void *(*allocate)(unsigned int, int);
    void (*release)(void *);
} ServiceHooks;
extern ServiceHooks D_80038000;
extern volatile unsigned char D_8002D480[];
extern unsigned char D_8002D484;
extern void *D_80058684;
extern void *D_80058688;
extern unsigned int *D_8005868C;
extern unsigned int D_80058690;
extern void func_80010628(void *, void *, unsigned int);

int func_80025AB4(void *source, void *data)
{
    unsigned int *count;
    unsigned int byte_count;

    if (D_8002D480[0]) {
        if (D_8005868C && D_8002D484) {
            D_80038000.release(D_8005868C);
        }
        if (((unsigned int)source & 0xF0000000) != 0x80000000) {
            D_80058688 = D_80058684 = D_80038000.translate(source);
            count = D_80038000.allocate(4, 0);
            if (!count) {
                return 0;
            }
            func_80010628(count, D_80058684, 4);
            D_80058690 = *count;
            D_80038000.release(count);
            byte_count = D_80058690 * 4;
            D_8005868C = D_80038000.allocate(byte_count, 0);
            if (!D_8005868C) {
                return 0;
            }
            byte_count = D_80058690 * 4;
            func_80010628(D_8005868C, (unsigned char *)D_80058684 + 4, byte_count);
            D_8002D484 = 1;
            return 1;
        } else {
            D_80058684 = source;
            D_80058688 = D_80038000.translate(data);
            D_8005868C = (unsigned int *)source + 1;
            D_80058690 = *(unsigned int *)source;
            D_8002D484 = 0;
            return 1;
        }
    }
    return 0;
}
