/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef struct Resource {u8 *data;} Resource;
typedef struct Object {u8 other[44];Resource *resource;} Object;
typedef struct Handle {Object *object;} Handle;
typedef struct Player76 {u8 index,selector;u8 other2[62];u16 half64;u8 other66[6];Handle *handle;} Player76;
typedef struct {u32 word0,maximum,rate,total;u16 frames,extra;u32 counters[10];u32 other60;} Stats64;
typedef struct {u8 other0[8];u32 maximum;u8 other12[8];s32 total;u8 other24[48];s16 counters[10];u8 other92[28];} Live120;
extern Player76 input_rec0[];
extern s16 active_player_count;
extern s8 D_8014978C;
extern Object *D_80146150[];
extern Stats64 D_80151410[];
extern Live120 D_80152038[];
extern s32 D_80140804;
extern void func_800CD8EC(Handle *,u8);
void audio_update_a(void)
{
    s32 player,which,j;
    s32 selector=D_8014978C-14;
    Player76 *record;
    Live120 *live;
    Stats64 *stats;
    u32 value;
    for(player=0;player<active_player_count;player++) {
        record=&input_rec0[player];
        if(record->handle==0) record->handle=(Handle *)&D_80146150[record->selector];
        if(record->handle->object->resource==0) return;
        live=&D_80152038[player];
        for(which=0;which<2;which++) {
            if(which==0) stats=(Stats64 *)(record->handle->object->resource->data+1292)+selector;
            else stats=&D_80151410[selector];
            if(stats->maximum<live->maximum) stats->maximum=live->maximum;
            value=live->total/D_80140804;
            if(stats->rate<value) stats->rate=value;
            stats->total+=live->total;
            stats->frames+=D_80140804;
            stats->extra+=record->half64;
            for(j=0;j<10;j++) stats->counters[j]+=live->counters[j];
        }
        func_800CD8EC(record->handle,(u8)D_8014978C);
    }
}
