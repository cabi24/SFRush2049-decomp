/* flags: -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef unsigned short u16;
typedef unsigned char u8;
typedef struct Header {u16 count[6]; u8 reserved[4];} Header;
extern Header *D_801526DC;
extern Header *D_801526F0;
extern u8 *D_80152034;
extern u8 *D_80124EEC;
extern u8 *D_801497F8;
extern u8 *D_8015201C;
extern u8 *D_801525EC;
extern u16 D_8015267C;
extern u8 *D_80152568;
extern u8 *D_80152460;
void func_800AB638(void) {
    Header *header = D_801526DC;
    D_801526F0 = header;
    D_80152034 = (u8 *)(header + 1);
    D_80124EEC = D_80152034 + header->count[0] * 132;
    D_801497F8 = D_80124EEC + header->count[1] * 20;
    D_8015201C = D_801497F8 + header->count[2] * 24;
    D_801525EC = D_8015201C + header->count[3] * 8;
    D_8015267C = header->count[4];
    D_80152568 = D_801525EC + header->count[4] * 32;
    D_80152460 = D_80152568 + header->count[5];
}
