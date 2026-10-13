/* Host semantic harness. Included source is the delivered C, not a rewrite. */
#include <stdint.h>
#include <limits.h>

typedef char require_u32_uint[(UINT_MAX == 0xffffffffU) ? 1 : -1];
unsigned int D_800382F4;
void *D_800382E8;
void *D_800382F0;
static unsigned int allocation_result;
static unsigned int observed[6];

static void *allocate(unsigned int bytes, unsigned int mode)
{
    observed[0]++;
    observed[1] = bytes;
    observed[2] = mode;
    observed[3] = D_800382F4;
    observed[4] = (unsigned int)(uintptr_t)D_800382E8;
    observed[5] = (unsigned int)(uintptr_t)D_800382F0;
    return (void *)(uintptr_t)allocation_result;
}
void *(*D_80038018)(unsigned int, unsigned int) = allocate;

void run_case(unsigned int rate, unsigned int duration, unsigned int result,
              unsigned int count, unsigned int state, unsigned int buffer,
              unsigned int *output)
{
    unsigned int i;
    D_800382F4 = count;
    D_800382E8 = (void *)(uintptr_t)state;
    D_800382F0 = (void *)(uintptr_t)buffer;
    allocation_result = result;
    for (i = 0; i < 6; i++) observed[i] = 0;
    func_80010DD8(rate, (unsigned short)duration);
    output[0] = D_800382F4;
    output[1] = (unsigned int)(uintptr_t)D_800382E8;
    output[2] = (unsigned int)(uintptr_t)D_800382F0;
    for (i = 0; i < 6; i++) output[i + 3] = observed[i];
}
