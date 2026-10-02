/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef unsigned char u8;
typedef short s16;
typedef float f32;
typedef struct Vec3 {f32 x,y,z;} Vec3;
typedef struct Node Node;
typedef struct Player76 {u8 selector;u8 opaque[67];Node *node;void *handle;} Player76;
typedef struct History2056 {u8 opaque[556];Vec3 position;u8 tail[1488];} History2056;
typedef struct Display152 {u8 opaque[12];Vec3 position,orientation;u8 tail[116];} Display152;
extern Player76 input_rec0[];
extern s16 active_player_count;
extern History2056 D_8014A250[];
extern Display152 D_80150B70[];
extern Vec3 D_801141B0;
extern void main_menu_input(Node *,Vec3 *,Vec3 *,Vec3 *,Vec3 *);
void main_menu_render(void)
{
    int i;
    Player76 *player=input_rec0;
    for(i=0;i<active_player_count;i++,player++) {
        if(player->node!=(Node *)-1)
            main_menu_input(player->node,&D_8014A250[player->selector].position,&D_801141B0,
                &D_80150B70[i].orientation,&D_80150B70[i].position);
    }
}
