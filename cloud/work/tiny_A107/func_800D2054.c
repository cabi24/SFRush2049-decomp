/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef unsigned char u8;
typedef short s16;
typedef unsigned int u32;
typedef float f32;
extern u32 D_801174B4;
extern int D_8014A110;
extern s16 active_player_count;
extern f32 D_80149A78[][8];
extern u8 D_80144018[];
extern f32 D_80144DA8[];
void func_800D2054(int player,f32 value)
{
    int i;
    f32 *history,*current,*cursor;
    int count;
    if(D_801174B4&8)return;
    if(D_8014A110==1)return;
    if(player>=active_player_count)return;
    history=D_80149A78[player];
    count=D_80144018[player];
    current=&history[count];
    *current=value;
    for(i=0,cursor=history;i<count;i++,cursor++)*current-=*cursor;
    if(*current<D_80144DA8[player])D_80144DA8[player]=*current;
    D_80144018[player]=count+1;
}
