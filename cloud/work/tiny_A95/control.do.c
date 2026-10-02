/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef unsigned char u8;
typedef signed char s8;
typedef signed short s16;
typedef struct Node {u8 other0[8];s8 active,dirty;} Node;
typedef struct Player76 {u8 other0[68];Node *pending;void *handle;} Player76;
typedef struct List24 {u8 other0[8];Node *first;u8 other12[12];} List24;
typedef struct Queue Queue;
extern Player76 input_rec0[];
extern s16 active_player_count;
extern Queue D_80142728;
extern List24 D_80146170,D_80146188;
extern int osRecvMesg(Queue *,void **,int);
extern int osJamMesg(Queue *,void *,int);
extern void func_800D52CC(Node *);
extern void func_8009211C(List24 *,Node *);
extern void func_80091FBC(List24 *,Node *,Node *);
void func_800D5374(void)
{
    Player76 *player;
    Node *node;
    if(active_player_count<=0)return;
    player=input_rec0;
    do {
        node=player->pending;
        if(node!=(Node *)-1) {
            osRecvMesg(&D_80142728,0,1);
            func_800D52CC(node);
            if(node->dirty!=0) {
                func_8009211C(&D_80146188,node);
                node->dirty=0;
            }
            func_80091FBC(&D_80146170,node,D_80146170.first);
            node->active=1;
            osJamMesg(&D_80142728,0,0);
        }
        player->pending=(Node *)-1;
        player++;
    } while(player<&input_rec0[active_player_count]);
}
