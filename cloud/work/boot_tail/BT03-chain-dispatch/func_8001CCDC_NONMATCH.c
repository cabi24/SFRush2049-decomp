/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* SetFXParameters source lead: AxioDL/musyx, snd3d.c, CC0-1.0.
 * The N64 body has four dedicated controller helpers and no paraInfo loop.
 * All six O32 inputs, including the unused yPan, are proven by native callers.
 */
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef struct Emitter {
    u8 unknown00[8];
    u32 flags08;
    u8 unknown0C[40];
    u32 identifier34;
    u8 unknown38[8];
    float fade40;
} Emitter;
extern u8 func_8001CC9C(u8);
extern u16 func_8001CCC0(u32);
extern int func_8001B7C0(u32, u8);
extern int func_8001B29C(u32, u8);
extern int func_8001B3A0(u32, u8);
extern int func_8001B4A4(u32, u16);
void func_8001CCDC(Emitter *const em, float vol, float xPan, float yPan,
                   float zPan, float doppler)
{
    u32 identifier;
    identifier = em->identifier34;
    if (em->flags08 & 0x100000) {
        func_8001B7C0(identifier, func_8001CC9C((u32)((em->fade40 * vol) * 127.0f)));
    } else {
        func_8001B7C0(identifier, func_8001CC9C((u32)(vol * 127.0f)));
    }
    func_8001B29C(identifier, func_8001CC9C((u32)((1.0f + xPan) * 64.0f)));
    func_8001B3A0(identifier, func_8001CC9C((u32)((1.0f - zPan) * 64.0f)));
    func_8001B4A4(identifier, func_8001CCC0(doppler * 8192.0f));
}
