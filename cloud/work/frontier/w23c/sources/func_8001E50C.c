/* flags: -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
/* func_8001E50C - lib_1ece0 (IDO 7.1). Sample pitch from a sample info word (the MusyX sndGetPitch idea; not
 * found in the fetched GameCube files, behaviour only): ENCODED_VALUE = root key << 24 | base rate (0xFFFFFFFF ->
 * key 64, 22050 Hz); when NOTE differs from the root key the rate is scaled by the semitone ratio table
 * D_8002C640 (up) or D_8002C840 (down); returns rate * 4096 / D_8004F800 (the output rate) as u32.
 * w23c levers (D10 C11-rate-convert true=350): `note != reference` operand order; the ratio is a conditional
 * expression multiplied in place (a named `factor` is a variable web, which uopt puts second in the mul.s; retail
 * has the ratio first: 60 -> 0); -Wab,-r4300_mul (retail has the R4300 nop after mul.s; NEUTRAL_FLAGS).
 * No devices, no unused locals, no own data (4096.0f and the u32 conversion constants are lui immediates). */
typedef unsigned char u8;
typedef unsigned int u32;
extern const float D_8002C640[];
extern const float D_8002C840[];
extern int D_8004F800;

u32 func_8001E50C(u8 note, u32 encoded_value)
{
    u8 reference;
    float value;
    if (encoded_value == 0xFFFFFFFFU) {
        encoded_value = 0x40005622U;
    }
    reference = encoded_value >> 24;
    if (note != reference) {
        value = (reference < note ? D_8002C640[note - reference] : D_8002C840[reference - note]) *
                (float)(encoded_value & 0xFFFFFFU);
    } else {
        value = (float)(encoded_value & 0xFFFFFFU);
    }
    return (u32)((value * 4096.0f) / (float)D_8004F800);
}
