/* flags: -g1 -O1 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
#include "static_debug_context.h"
int sprintf(char *output, const char *format, ...) {
    int result;
    char *arguments;
    arguments = (char *)&format + sizeof(format);
    result = fcvt(output, format, arguments);
    return result;
}
