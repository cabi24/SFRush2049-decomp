/* flags: -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
/*
 * Set the two RGBA endpoints used by dispatch_handler's mode-22 blend.
 * Native N64 interface: two four-byte colors passed by value. IDO homes
 * these aggregate arguments while copying their complete words to globals.
 * The Color4 layout agrees with the accepted dispatch_handler consumer.
 * No arcade equivalent has been established; this is N64 rendering state.
 */
typedef unsigned char u8;
typedef struct Color4 {
    u8 r, g, b, a;
} Color4;

extern Color4 D_80118E28;
extern Color4 D_80118E2C;

void func_800BEA3C(Color4 first, Color4 second)
{
    D_80118E28 = first;
    D_80118E2C = second;
}
