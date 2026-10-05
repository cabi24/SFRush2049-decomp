SECTIONS { .text 0x80008730 : { *(.text) } /DISCARD/ : { *(.reginfo) *(.options) } }
gSinCoeffs = 0x8002d750;
gTwoOverPi = 0x8002d778;
gPiOver2Hi = 0x8002d780;
gPiOver2Lo = 0x8002d788;
gNaN = 0x8002d790;
gCosCoeffs = 0x8002d7a0;
gCosAngleScale = 0x8002d7c8;
gCosPiOver2Hi = 0x8002d7d0;
gCosPiOver2Lo = 0x8002d7d8;
gCosOne = 0x8002d7e0;
gNaNf = 0x8002d880;
