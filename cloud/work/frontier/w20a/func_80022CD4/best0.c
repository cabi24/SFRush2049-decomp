/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* Conditional native reconstruction. The original table object and producer
 * domain are UNRESOLVED. See README.md for mandatory storage, termination,
 * divisor and target-C implementation preconditions. Never run unvalidated
 * inputs. This address is an interior cursor anchor, not an array declaration.
 * Integer address arithmetic and integer/pointer conversions use the N64/IDO
 * mapping; this is not portable ISO C or a proof of pointer provenance.
 * Source-family lead: CC0 AxioDL/musyx@78d2e16e4905fc675952162d331c24d5198b2687,
 * src/musyx/runtime/synthmacros.c, DoSetPitch. No donor table is imported.
 */
typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef unsigned int u32;
#pragma pack(1)
typedef struct PitchState {
    u8 unknown00[0x24];
    u32 flags24;
    u8 unknown28[0x28];
    u16 note50;
    u8 unknown52[0xA];
    u32 sample5C;
    u8 unknown60[0x60];
    s8 detuneC0;
} PitchState;
#pragma pack(0)
typedef struct PitchCommand { u32 word[2]; } PitchCommand;

/* Target linker address token only, not a storage definition or extent. */
extern u16 D_8002D476;

u8 func_80022CD4(PitchState *state, PitchCommand *command)
{
    u32 f;
    u32 of;
    u32 i;
    u32 frq;
    u32 ofrq;
    u32 no;
    u32 cursor;

    frq = (command->word[0] >> 8) & 0xFFFF;
    ofrq = state->sample5C & 0xFFFFFF;
    if (ofrq == frq) {
        state->note50 = (u8)(state->sample5C >> 24);
        state->detuneC0 = 0;
    } else if (ofrq < frq) {
        f = (frq << 12) / ofrq;
        of = f >> 12;
        for (no = 0; no < 11; no++) {
            if (of < (1 << (no + 1))) {
                break;
            }
        }
        f /= 1 << no;
        cursor = (u32)&D_8002D476;
        for (i = 11; ; i--, cursor -= 2) {
            if (f > *(u16 *)cursor) {
                break;
            }
        }
        state->note50 = (u8)(state->sample5C >> 24) + no * 12 + i;
        state->detuneC0 = ((f - *(u16 *)cursor) * 100) / (*(u16 *)(cursor + 2) - *(u16 *)cursor);
    } else {
        f = (ofrq << 12) / frq;
        of = f >> 12;
        for (no = 0; no < 11; no++) {
            if (of < (1 << (no + 1))) {
                break;
            }
        }
        f /= 1 << no;
        cursor = (u32)&D_8002D476;
        for (i = 11; ; i--, cursor -= 2) {
            if (f > *(u16 *)cursor) {
                break;
            }
        }
        state->note50 = (u8)(state->sample5C >> 24) - no * 12 - i;
        state->detuneC0 = -(s8)(((f - *(u16 *)cursor) * 100) / (*(u16 *)(cursor + 2) - *(u16 *)cursor));
    }
    state->flags24 |= 0x100;
    return 0;
}
