/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* __osPfsRequestData (static, boot 0x800102EC, 180 B, segment 0x10d60 -> src/rom/lib_10d60.c, IDO 5.3)
 * = ultralib src/io/pfsisplug.c __osPfsRequestData verbatim (VERSION_J+: loop bound __osMaxControllers).
 * Project names: ultralib __osPfsPifRam = project __osPfsBuffer (u8[64], m2c_types.h; read through an OSPifRam
 *   view for pifstatus), __osContLastCmd = __osPfsRequestType (u8), __osMaxControllers = __osPfsRequestType2 (u8),
 *   __osPfsGetInitData = func_800103A0 (locked, w16g). CONT_CMD_* constants written as their values.
 * w23c: no shaping. Matches the retail .s word for word under IDO 5.3 (tools/asmcmp.py: 0 differing of 45);
 * there is NO inventory target for this function yet (see RESULTS.md). */
typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef struct {
    u8 dummy, txsize, rxsize, cmd, typeh, typel, status, dummy1;
} __OSContRequesFormat;
typedef struct {
    u32 ramarray[15];
    u32 pifstatus;
} OSPifRam;
extern u8 __osPfsBuffer[64];
extern u8 __osPfsRequestType;
extern u8 __osPfsRequestType2;

void __osPfsRequestData(u8 cmd) {
    u8* ptr = (u8*)&__osPfsBuffer;
    __OSContRequesFormat requestformat;
    int i;

    __osPfsRequestType = cmd;
    ((OSPifRam*)__osPfsBuffer)->pifstatus = 1;
    requestformat.dummy = 0xFF;
    requestformat.txsize = 1;
    requestformat.rxsize = 3;
    requestformat.cmd = cmd;
    requestformat.typeh = 0xFF;
    requestformat.typel = 0xFF;
    requestformat.status = 0xFF;
    requestformat.dummy1 = 0xFF;

    for (i = 0; i < __osPfsRequestType2; i++) {
        *((__OSContRequesFormat*)ptr) = requestformat;
        ptr += sizeof(__OSContRequesFormat);
    }

    *ptr = 0xFE;
}
