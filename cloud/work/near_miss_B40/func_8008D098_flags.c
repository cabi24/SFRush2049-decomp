/* flags: -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef unsigned short u16;
typedef struct Header {u16 tag;u16 flags;} Header;
void func_8008D098(Header *header,u16 mask) {header->flags &= ~mask;}
