/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef unsigned char u8;
typedef short s16;
typedef float f32;
typedef struct Vec3 {f32 x,y,z;} Vec3;
typedef struct Node {u8 opaque[12];Vec3 a,b,c,d;} Node;
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
    Player76 *player;
    if(active_player_count>0) {
    player=input_rec0;
    for(i=0;i<active_player_count;i++,player++) {
        Node *node=player->node;
        if(node!=(Node *)-1)
            main_menu_input(node,&D_8014A250[player->selector].position,&D_801141B0,
                &D_80150B70[i].orientation,&D_80150B70[i].position);
    }
    }
}

typedef struct Queue Queue;
extern Queue D_80142728;
extern int osRecvMesg(void *,void *,int);
extern int osJamMesg(void *,void *,int);
extern void func_800D52CC(Node *);
void main_menu_input(Node *object,Vec3 *a,Vec3 *b,Vec3 *c,Vec3 *d)
{
    if(object!=(Node *)-1) {
        osRecvMesg(&D_80142728,0,1);
        func_800D52CC(object);
        object->a.x=a->x; object->a.y=a->y; object->a.z=a->z;
        object->b.x=b->x; object->b.y=b->y; object->b.z=b->z;
        object->c.x=c->x; object->c.y=c->y; object->c.z=c->z;
        object->d.x=d->x; object->d.y=d->y; object->d.z=d->z;
        osJamMesg(&D_80142728,0,0);
    }
}
