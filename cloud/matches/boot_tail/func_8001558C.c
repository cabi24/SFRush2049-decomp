/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* Resource/program lookup and optionally guarded sequence dispatch.
 * Ordinary o32: two u16 IDs, stream/options address words, and u8 unlocked.
 * Address-word formals agree with the promoted callers in lib_15a10/lib_17dc0.
 * The callee's u32 handle is assigned to the int result/return used by those
 * callers. IDO's 32-bit two's-complement conversion preserves all handle bits,
 * including 0xffffffff; the native and host tests check high-bit results.
 * Program is a packed access view of the 132-byte MidiSetup record. The callee
 * tags/return agree with its promoted definition; opaque payloads stay opaque.
 * ResourceGroup is the existing TU table view; ResourceHeader extends its
 * accessed payload without changing the shared table declaration.
 * Stop at the first matching resource, then scan packed 132-byte programs.
 * Unknown original source; MusyX sequence-play attribution is a hypothesis.
 *
 * Shaping disclosure: result = id below is an otherwise dead initialization
 * of the existing return-value local. IDO 7.1 numbers its integer-promotion
 * expression before the table-address expression in the loop preheader; as1
 * then schedules the ID copy into the native count-guard position. Removing
 * it gives four differing words; the frame remains 56 bytes in both forms.
 * This does not add locals, frame padding, arguments, calls, or memory reads.
 * Every path that returns result first overwrites it with the dispatch result.
 * Stock flags and all 87 words verified by dot_static_1558c_20261013.
 */
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef struct {
    u32 metadata;
    u16 id;
    u16 type;
    u32 header[5];
    u32 bank_a;
    u32 bank_b;
    u32 programs;
} ResourceHeader;
#pragma pack(1)
typedef struct {
    u16 id;
    u8 data[130];
} Program;
#pragma pack()
extern u8 D_8002C630;
extern short D_80038390;
typedef struct ResourceGroup { int size; u16 id; u16 type; u32 offsets[6]; } ResourceGroup;
extern ResourceGroup *D_80038398[];
extern void func_80014594(void);
extern void func_800145DC(void);
typedef struct Page_800178B0 Page_800178B0;
typedef struct MidiSetup_800178B0 MidiSetup_800178B0;
typedef struct Arr_800178B0 Arr_800178B0;
typedef struct PlayPara_800178B0 PlayPara_800178B0;
extern u32 func_800178B0(Page_800178B0 *, Page_800178B0 *, MidiSetup_800178B0 *, Arr_800178B0 *, PlayPara_800178B0 *);
int func_8001558C(u16 id, u16 program_id, u32 stream, u32 options, u8 unlocked)
{
    int i;
    ResourceHeader *resource;
    void *bank_a;
    void *bank_b;
    Program *program;
    int result;
    if (D_8002C630) {
        i = 0;
        result = id;
        for (; i < D_80038390; i++) {
            resource = (ResourceHeader *)D_80038398[i];
            if (resource->id == id) {
                if (resource->type == 0) {
                    bank_a = (u8 *)resource + resource->bank_a + 8;
                    bank_b = (u8 *)resource + resource->bank_b + 8;
                    program = (Program *)((u8 *)resource + resource->programs + 8);
                    for (; program->id != 0xffff; program++) {
                        if (program->id == program_id) {
                            if (unlocked) {
                                result = func_800178B0(bank_a, bank_b, (MidiSetup_800178B0 *)program, (Arr_800178B0 *)stream, (PlayPara_800178B0 *)options);
                            }
                            else {
                                func_80014594();
                                result = func_800178B0(bank_a, bank_b, (MidiSetup_800178B0 *)program, (Arr_800178B0 *)stream, (PlayPara_800178B0 *)options);
                                func_800145DC();
                            }
                            return result;
                        }
                    }
                }
                return -1;
            }
        }
    }
    return -1;
}
