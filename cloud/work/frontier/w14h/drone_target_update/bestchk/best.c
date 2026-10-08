/* flags: -g0 -O3 -mips2 -G 0 -non_shared -Wab,-r4300_mul -- NOT A MATCH: 70 differing words standalone (vbatch, w14h).
 * Unused locals bp, cnt and sel-stores are vbatch residue: not disclosed as frame residual, do not splice. See ../RESULTS.md */
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
    Player76 *record, *end, *bp;
    s8 *config;
    int i;
    int sel;
    s16 cnt;
    if (active_player_count > 0) {
         
        record = input_rec0;
        do {
             bp = record; 
             sel = record->selector; 
            if (mode) config = D_8013C228;
            else config = D_8013C068[record->selector].values;
             if (D_80139300[sel] && sel != 5) { 
                D_80139300[record->selector] = 0;
                if (record->handle) func_800CCE5C(record->handle, config);
                if (!record->handle || mode)
                    func_800CCE5C((Handle *)&D_80146150[record->selector], config);
            }
            end = input_rec0 + active_player_count;
             record->bindings[0] = D_801118E4[config[0]];
            record->bindings[1] = D_801118E4[config[1]];
            for (i = 2; i < 10; i++)
                record->bindings[i] = D_801118E4[config[i]]; 
            record++;
        } while (record < end);
    }
}
