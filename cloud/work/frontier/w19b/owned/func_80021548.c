/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* func_80021548 - lib_21210 (IDO 7.1). Translates a macro controller number 0x80..0x85 to the
 * sequencer's controller (0x80->0x80, 0x81->0x82, 0x82->0xA0, 0x83->0xA1, 0x84->0x83, 0x85->0x84);
 * any other value is returned unchanged.
 * w19b: every case assigns and breaks to one `return controller` (returns inside the cases give
 * a different tail: true=60..165). The switch jump table jtbl_8002D930 (6 words + 8 B pad, .rodata, ROM data.bin
 * 0x6160..0x617F) is this function's own data: code identical, owned .rodata needed (see w19b RESULTS.md). */
typedef unsigned char u8;

u8 func_80021548(u8 controller)
{
    switch (controller) {
    case 0x80: controller = 0x80; break;
    case 0x81: controller = 0x82; break;
    case 0x82: controller = 0xA0; break;
    case 0x83: controller = 0xA1; break;
    case 0x84: controller = 0x83; break;
    case 0x85: controller = 0x84; break;
    }
    return controller;
}
