/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* func_800103A0 (static, boot 0x800103A0, 176 B, segment 0x10d60 -> src/rom/lib_10d60.c, unconverted)
 * = ultralib __osPfsGetInitData (src/io/pfsisplug.c, BUILD_VERSION >= VERSION_J: loop bound
 *   __osMaxControllers). Its segment neighbours are osPfsIsPlug and __osPfsRequestData, the rest of pfsisplug.c.
 * Project names: ultralib __osPfsPifRam = project __osPfsBuffer (u8[64], m2c_types.h);
 *   ultralib __osMaxControllers = project __osPfsRequestType2 (u8, 0x80037AE1). */
typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef struct OSContStatus {
    u16 type;
    u8 status;
    u8 errno;
} OSContStatus;
typedef struct {
    u8 dummy, txsize, rxsize, cmd, typeh, typel, status, dummy1;
} __OSContRequesFormat;

#define CHNL_ERR(format) (((format).rxsize & 0xC0) >> 4)

extern u8 __osPfsBuffer[64];
extern u8 __osPfsRequestType2;

void func_800103A0(u8* pattern, OSContStatus* data) {
    u8* ptr;
    __OSContRequesFormat requestformat;
    int i;
    u8 bits = 0;

    ptr = (u8*)&__osPfsBuffer;

    for (i = 0; i < __osPfsRequestType2; i++, ptr += sizeof(requestformat), data++) {
        requestformat = *((__OSContRequesFormat*)ptr);
        data->errno = CHNL_ERR(requestformat);

        if (data->errno != 0) {
            continue;
        }

        data->type = ((requestformat.typel << 8) | requestformat.typeh);
        data->status = requestformat.status;
        bits |= (1 << i);
    }
    *pattern = bits;
}
