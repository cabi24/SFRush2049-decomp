/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;
typedef unsigned int u32;

typedef struct NameKey {
    u32 hdr;
    char name[28];
} NameKey;
typedef struct Node104 { u8 b[104]; } Node104;
typedef struct Entry { Node104 *node; char name[16]; } Entry;

extern s16 D_80149D90;
/*@{ty*/extern volatile s32 D_80149B80;/*@| extern Node104 *D_80149B80; @| extern Node104 * volatile D_80149B80; @}*/
extern s32 D_801497FC;
extern s32 D_80149818;
extern void pointer_offset_wrapper(s32 arg0, s32 arg1);
s32 func_800A473C(s32 arg0, u8 *arg1);
/*@{ty*/void differential_output(s32 arg0, s16 arg1);/*@| void differential_output(Node104 *arg0, s16 arg1); @| void differential_output(Node104 *arg0, s16 arg1); @}*/
void *entity_name_copy(void *key, void *base, u32 n, u32 size, s32 (*compar)(void *, void *));
void func_800AB638(void);

s16 wheel_torque_apply(u8 *arg0, s16 arg1) {
    NameKey key;
    /*@{p*//*@| char *p; @}*/
    /*@{p*//*@| p = key.name; @}*/
    /*@{st*/D_80149D90 = -1;
    func_800A473C((s32) key.name, arg0);/*@| func_800A473C((s32) key.name, (D_80149D90 = -1, arg0)); @| D_80149D90 = -1; func_800A473C((s32) key.name, arg0); @}*/
    /*@{ty*/D_80149B80 = *(s32 *)/*@| D_80149B80 = ((Entry *) @| D_80149B80 = ((Entry *) @}*/entity_name_copy(&key, (void *) D_80149818, *(u32 *) D_801497FC, 20,
                                           (s32 (*)(void *, void *)) pointer_offset_wrapper)/*@{ty*//*@|)->node@|)->node@}*/;
    differential_output(D_80149B80, arg1);
    func_800AB638();
    return D_80149D90;
}
