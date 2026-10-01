__osPiGetCmdQueue = 0x800100c0;
osEPiRawReadIo = 0x8000ff60;
osEPiRawStartDma = 0x8000fe00;
osEPiRawWriteIo = 0x8000fda0;
osJamMesg = 0x800075e0;
osRecvMesg = 0x80007270;
osYieldThread = 0x80010110;
SECTIONS {
.text 0x8000DF80 : { *(.text) }
.rodata 0x8002D860 : { *(.rodata) }
/DISCARD/ : { *(.reginfo) *(.MIPS.abiflags) *(.pdr) *(.comment) *(.gnu.attributes) *(.mdebug*) *(.bss) *(.data) }
}
