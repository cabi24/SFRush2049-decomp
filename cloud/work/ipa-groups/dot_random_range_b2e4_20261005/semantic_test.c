/* Host behavior of the exact candidate pair; -fwrapv defines rand's signed
 * overflow as the two's-complement operation implemented by IDO/MIPS. */
#include <stdint.h>
#include <stdio.h>
#include <string.h>
int D_8011735C;
extern float func_8008B2E4(float);
int main(void) {
    uint32_t input[2], output[2];
    float scale, result;
    while (fread(input, sizeof(input), 1, stdin) == 1) {
        memcpy(&D_8011735C, &input[0], 4);
        memcpy(&scale, &input[1], 4);
        result = func_8008B2E4(scale);
        memcpy(&output[0], &D_8011735C, 4);
        memcpy(&output[1], &result, 4);
        if (fwrite(output, sizeof(output), 1, stdout) != 1) return 2;
    }
    return ferror(stdin) ? 3 : 0;
}
