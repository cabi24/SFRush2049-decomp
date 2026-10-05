/* Execute the unchanged candidate source with real host float arithmetic. */
#include <stdio.h>
#include <string.h>
#include <math.h>
float D_8011F010[4] = { 0.0f, 0.78539816339744830962f,
                        1.57079632679489661923f, 0.78539816339744830962f };
float func_8009C3F8(float, int);
float camera_update_c(float);
float select_screen_update(float);
int main(void)
{
    unsigned word, result;
    int mode;
    float x, y, z;
    while (scanf("%x %d", &word, &mode) == 2) {
        memcpy(&x, &word, 4);
        y = func_8009C3F8(x, mode);
        z = mode ? select_screen_update(x) : camera_update_c(x);
        memcpy(&result, &y, 4);
        printf("%08x ", result);
        memcpy(&result, &z, 4);
        printf("%08x\n", result);
    }
    return 0;
}
