#include "rom_tu.h"
extern s8 gDmaInitialized;
extern OSMesg gDmaMessageBuffer;
extern OSMesgQueue gDmaMessageQueue;
typedef struct { unsigned sign:1; unsigned exponent:11; unsigned fraction:20; unsigned low; } DoubleBits;
typedef union { double value; DoubleBits bits; } DoubleUnion;
#pragma GLOBAL_ASM("build/C68/lib_34a0/modf.s")
#pragma GLOBAL_ASM("build/C68/lib_34a0/modff.s")
int __isinf(register double x) { DoubleUnion v; v.value=x; if(v.bits.exponent==2047) { v.bits.exponent=0; return v.value != 0.0; } return 0; }

int __isnan(register double x) { DoubleUnion v; v.value=x; if(v.bits.exponent==2047) { v.bits.exponent=0; return v.value == 0.0; } return 0; }

#pragma GLOBAL_ASM("build/C68/lib_34a0/fcvt.s")
#pragma GLOBAL_ASM("build/C68/lib_34a0/__ecvt_internal.s")
#pragma GLOBAL_ASM("build/C68/lib_34a0/__round_helper.s")
#pragma GLOBAL_ASM("build/C68/lib_34a0/__write_exponent.s")
#pragma GLOBAL_ASM("build/C68/lib_34a0/sprintf.s")
