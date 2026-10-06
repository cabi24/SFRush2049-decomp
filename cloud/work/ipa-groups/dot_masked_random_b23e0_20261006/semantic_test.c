/* Executes the exact three candidate files. -fwrapv defines accepted rand's
 * signed overflow; fixtures provide all 256 indexable words without claiming
 * that the retail object has that capacity. All terminating masks are nonzero. */
#include <stdint.h>
#include <stdio.h>
#include <string.h>
int D_8011735C;
unsigned int D_80123418[256];
extern unsigned char func_800B23E0(unsigned char);
int main(void) {
    uint32_t input[3], output[2], before[256];
    unsigned int i;
    while (fread(input, sizeof(input), 1, stdin) == 1) {
        memcpy(&D_8011735C, &input[0], 4);
        for (i=0; i<256; ++i) D_80123418[i]=0xA5F00000U+i;
        D_80123418[input[1] & 255]=input[2];
        memcpy(before,D_80123418,sizeof(before));
        output[1]=func_800B23E0((unsigned char)input[1]);
        memcpy(&output[0],&D_8011735C,4);
        if (memcmp(before,D_80123418,sizeof(before))) return 4;
        if (fwrite(output,sizeof(output),1,stdout)!=1) return 2;
    }
    return ferror(stdin) ? 3 : 0;
}
