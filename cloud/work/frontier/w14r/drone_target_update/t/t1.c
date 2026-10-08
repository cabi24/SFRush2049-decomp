typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef signed short s16;
typedef unsigned int u32;
typedef struct Object Object;
typedef struct Handle { Object *object; } Handle;
typedef struct Player76 {
    u8 index, selector;
    u8 opaque[22];
    u32 bindings[10];
    u16 count;
    u8 opaque66[6];
    Handle *handle;
} Player76;
typedef struct Bindings10 { s8 values[10]; } Bindings10;
extern s16 active_player_count;
extern Player76 input_rec0[];
extern s8 D_80139300[];
extern Bindings10 D_8013C068[];
extern s8 D_8013C228[];
extern u32 D_801118E4[];
extern Object *D_80146150[];
extern void func_800CCE5C(Handle *, s8 *);
void drone_target_update(int mode)
{
    /*@{DC*/Player76 *record, *end;/*@| Player76 *end, *record; @}*/
    s8 *config;
    int i;
    /*@{BND*//*@| u32 *bnd; @}*/
    /*@{SEL*/int sel;/*@| @}*/
    if (/*@{IFX*/active_player_count > 0/*@| 0 < active_player_count @}*/) {
        record = input_rec0;
        /*@{END*//*@| end = input_rec0 + active_player_count; @}*/
        do {
            /*@{SEL*/sel = record->selector;/*@| @}*/
            /*@{CFG*/if (mode) config = D_8013C228;
            else config = D_8013C068[record->selector].values;/*@| config = mode ? D_8013C228 : D_8013C068[record->selector].values; @}*/
            if (D_80139300[/*@{SEL*/sel/*@| record->selector @}*/] && /*@{SEL*/sel/*@| record->selector @}*/ != 5) {
                D_80139300[record->selector] = 0;
                /*@{CH*/if (record->handle) func_800CCE5C(record->handle, config);/*@| if (record->handle != 0) func_800CCE5C(record->handle, config); @}*/
                /*@{CM*/if (!record->handle || mode)/*@| if (mode || !record->handle) @}*/
                    func_800CCE5C((Handle *)&D_80146150[record->selector], config);
            }
            /*@{END*/end = input_rec0 + active_player_count;/*@| @}*/
            /*@{BND*//*@| bnd = record->bindings; @}*/
            /*@{BND*/record->bindings[0]/*@| bnd[0] @}*/ = D_801118E4[config[0]];
            /*@{BND*/record->bindings[1]/*@| bnd[1] @}*/ = D_801118E4[config[1]];
            /*@{UNR*/for (i = 2; i < 10; i++)
                record->bindings[i] = D_801118E4[config[i]];/*@| record->bindings[2] = D_801118E4[config[2]]; record->bindings[3] = D_801118E4[config[3]]; record->bindings[4] = D_801118E4[config[4]]; record->bindings[5] = D_801118E4[config[5]]; record->bindings[6] = D_801118E4[config[6]]; record->bindings[7] = D_801118E4[config[7]]; record->bindings[8] = D_801118E4[config[8]]; record->bindings[9] = D_801118E4[config[9]];  @}*/
            record++;
        } while (record < end);
    }
}
