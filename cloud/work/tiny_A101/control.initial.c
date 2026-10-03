/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef unsigned char u8;
typedef short s16;
typedef unsigned int u32;
typedef struct Resource Resource;
typedef struct Object { u8 opaque[44]; Resource *resource; } Object;
typedef struct Handle { Object *object; } Handle;
typedef struct Player { u8 opaque[72]; Handle *handle; } Player;
extern u32 D_801170FC;
extern void *D_80117338,*D_8011733C,*D_80117340,*D_80117344,*D_80117348;
extern u32 D_8011734C;
extern Player input_rec0[];
extern s16 active_player_count;
extern void sound_stop(void *);
extern void func_800B4DA4(Handle *);
extern void sound_handles_clear(int);
void func_800B4E68(void)
{
    int i;
    Player *player;
    switch(D_801170FC) {
    case 0:
        if(D_80117338) {
            sound_stop(D_80117338);
            D_80117338=0;
        }
        for(i=0,player=input_rec0;i<active_player_count;i++,player++) {
            if(player->handle->object->resource)func_800B4DA4(player->handle);
        }
    case 1: case 2: case 3: case 4: case 5: case 6: case 7: case 8:
        if(D_8011733C) {sound_stop(D_8011733C);D_8011733C=0;}
        if(D_80117340) {sound_stop(D_80117340);D_80117340=0;}
        if(D_80117344) {sound_stop(D_80117344);D_80117344=0;}
        if(D_80117348) {sound_stop(D_80117348);D_80117348=0;}
        break;
    }
    sound_handles_clear(0);
    D_8011734C=0;
}
