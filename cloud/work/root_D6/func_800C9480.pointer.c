/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef int s32;
typedef unsigned int u32;
typedef unsigned char u8;
typedef struct Table {s32 value[3][5];} Table;
extern Table D_80151690[12];
extern u8 D_80151968[260];
extern unsigned short D_80151A78[20];
extern void draw_minimap(void);
extern void *memset(void *,int,u32);
void func_800C9480(void) {
    s32 j,k;
    Table *table;
    draw_minimap();
    for(table=D_80151690;table!=D_80151690+12;table++)
        for(j=0;j<3;j++)
            for(k=0;k<5;k++) table->value[j][k]=-1;
    memset(D_80151968,0,260);
    memset(D_80151A78,0,40);
}
