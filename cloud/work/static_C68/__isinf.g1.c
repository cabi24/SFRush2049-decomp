/* flags: -g1 -O1 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef struct { unsigned sign:1; unsigned exponent:11; unsigned fraction:20; unsigned low; } DoubleBits;
typedef union { double value; DoubleBits bits; } DoubleUnion;
int __isinf(register double x) { DoubleUnion v; v.value=x; if(v.bits.exponent==2047) { v.bits.exponent=0; return v.value != 0.0; } return 0; }
