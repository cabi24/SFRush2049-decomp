/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* COMPLETE-NONMATCH: resource/program lookup and guarded dispatch. */
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
extern ResourceHeader *D_80038398[];
extern void func_80014594(void);
extern void func_800145DC(void);
extern int func_800178B0(void *, void *, Program *, void *, void *);
int func_8001558C(u16 id, u16 program_id, void *stream, void *options, u8 unlocked)
{
    int i;
    ResourceHeader *resource;
    void *bank_a;
    void *bank_b;
    Program *program;
    int result;
    if (D_8002C630) {
        for (i = 0; i < D_80038390; i++) {
            resource = D_80038398[i];
            if (resource->id == id) {
                if (resource->type == 0) {
                    bank_a = (u8 *)resource + resource->bank_a + 8;
                    bank_b = (u8 *)resource + resource->bank_b + 8;
                    program = (Program *)((u8 *)resource + resource->programs + 8);
                    for (; program->id != 0xffff; program++) {
                        if (program->id == program_id) {
                            if (unlocked) {
                                result = func_800178B0(bank_a, bank_b, program, stream, options);
                            }
                            else {
                                func_80014594();
                                result = func_800178B0(bank_a, bank_b, program, stream, options);
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
