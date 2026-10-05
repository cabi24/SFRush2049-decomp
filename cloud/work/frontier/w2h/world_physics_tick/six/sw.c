/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef unsigned char u8;
typedef signed char s8;
typedef short s16;
typedef unsigned int u32;
typedef int s32;
typedef struct Reference {
    u32 word0;
    u8 opaque4[62];
    u8 flags;
    u8 opaque67[9];
} Reference;
typedef struct Car {
    void * object;
    u8 opaque4[4];
    s8 byte8;
    s8 byte9;
    s8 byte10;
    s8 byte11;
    s8 byte12;
    s8 byte13;
    s8 byte14;
    s8 byte15;
    u8 opaque16[228];
    float value244;
    float value248;
    float value252;
    float value256;
    float value260;
    float value264;
    float value268;
    float value272;
    float value276;
    float value280;
    float value284;
    float value288;
    u8 opaque292[728];
    s32 word1020;
    float value1024;
    u8 opaque1028[592];
    float radius;
    u8 opaque1624[108];
    s16 half1732;
    u8 opaque1734[82];
    float preserved;
    u8 opaque1820[168];
    s16 half1988;
    s16 half1990;
    s16 half1992;
    s16 half1994;
    s8 byte1996;
    s8 byte1997;
    s8 byte1998;
    s8 byte1999;
    u8 opaque2000[4];
    s32 word2004;
    u8 opaque2008[4];
    s8 byte2012;
    s8 byte2013;
    u8 opaque2014[4];
    s16 half2018;
    s16 half2020;
    s16 half2022;
    u8 opaque2024[2];
    s8 byte2026;
    s8 byte2027;
    float scale;
    float time_boost;
    u8 opaque2036[20];
} Car;
typedef struct Player {
    u8 opaque0[232];
    s32 word232;
    s8 byte236;
    u8 opaque237[1];
    s8 byte238;
    u8 opaque239[17];
    float distance;
    float last_distance;
    u8 opaque264[513];
    s8 byte777;
    u8 opaque778[7];
    s8 byte785;
    u8 opaque786[70];
    s8 byte856;
    u8 opaque857[2];
    s8 byte859;
    s8 byte860;
    s8 byte861;
    s8 byte862;
    u8 opaque863[33];
    Reference * reference;
    u8 opaque900[52];
} Player;
typedef struct Config {u8 field0,body,field2,field3,field4,place,flags,owner;} Config;
typedef struct Bounds {float front,rear,side,height;} Bounds;
typedef struct Route {s16 field0,first,field4,field6,count;} Route;
extern Car D_8014A250[];
extern Player D_80152818[];
extern Config D_80153E88[];
extern Reference D_8014A118[];
extern Bounds D_8011F844[];
extern Route D_80151CE8;
extern s8 D_80146180[];
extern s8 D_8011103C[],D_80111080[],D_8011119C[],D_80111230[],D_8011157C[];
extern s8 D_80152744;
extern s8 D_80152015;
extern volatile s16 D_801543CA;
extern volatile s16 D_80153E84;
extern volatile s16 D_80153FD2;
extern s16 D_8014A108,D_8015274C,D_80153F08,D_80153F24,D_80153F40;
extern void *D_80110D08;
extern s32 state_word_a;
extern void func_800B27E4(Player *);
extern void func_800EC270(Car *,Player *);
extern float sqrtf(float);
void world_physics_tick(void) {
    s32 i,j,index;
    Car *car;
    Player *player;
    Config *config;
    Bounds *bounds;
    float preserved,value,maxlen;
    D_80152744=0;
    for(i=0;i<D_801543CA;i++) {
        car=&D_8014A250[i];
        player=&D_80152818[i];
        config=&D_80153E88[i];
        preserved=car->preserved;
        for(j=0;j<sizeof(Car);j++) ((s8 *)car)[j]=0;
        car->preserved=preserved;
        for(j=0;j<sizeof(Player);j++) ((s8 *)player)[j]=0;
        car->half1994=config->owner==0;
        car->object=D_80110D08;
        player->byte862=2;
        player->byte860=-1;
        player->byte861=player->byte862;
        if(6==config->owner) index=6-i;
        else index=i-D_8014A108+1;
        player->distance=(float)-index;
        car->byte2027=1;
        car->byte2026=1;
        player->byte785=-1;
        player->byte777=car->byte2027;
        car->half2022=0;
        car->scale=1.0f;
        car->time_boost=1.0f;
        player->last_distance=-100.0f;
        car->value1024=1.0f;
        car->half1988=0;
        car->half1732=-1;
        player->byte856=0;
        car->word1020=-1;
        func_800B27E4(player);
        car->half1992=(config->flags&0x80)!=0;
        if(car->half1992) {
            D_8014A250[D_80152744++].half1990=i;
            player->byte859=i;
            player->byte238=config->place;
            car->byte1996=config->owner<6?1:2;
            if(car->byte1996==2) {
                player->reference=&D_8014A118[i];
                if(D_80146180[i]==1) {
                    player->word232=112;
                    car->word2004=112;
                } else if(D_80146180[i]==2) {
                    player->word232=16;
                    car->word2004=16;
                }
            }
            if(player->reference) {
                car->byte8=config->body;
                car->byte9=player->reference->flags;
                car->byte10=(car->byte9&1)^1;
                index=car->half1990;
                car->byte11=D_8011103C[(index+1)*13+config->body];
                car->byte12=D_80111080[(index+1)*13+config->body];
                car->byte13=D_8011119C[(index+1)*13+config->body];
                car->byte14=D_80111230[(index+1)*13+config->body];
                car->byte15=D_8011157C[(index+1)*13+config->body];
                car->byte1997=config->field2;
                car->byte1998=config->field3;
                car->byte1999=config->field4;
                car->byte2013=1;
                player->byte236=1;
            } else if(car->half1994) {
                car->byte8=config->body;
                car->byte9=0;
                car->byte10=1;
                car->byte11=D_8011103C[config->body];
                car->byte12=car->half1990&3;
                car->byte13=D_8011119C[config->body];
                car->byte14=D_80111230[config->body];
                car->byte15=D_8011157C[config->body];
                car->byte1997=config->field2;
                car->byte1998=config->field3;
                car->byte1999=config->field4;
                car->byte2013=1;
                player->byte236=1;
            }
            car->half2018=0;
            index=car->half2018+1;
            if(index==D_80151CE8.count) index=D_80151CE8.first;
            car->half2020=index;
            func_800EC270(car,player);
            if(!player->reference&&!car->half1994) {
                bounds=&D_8011F844[(u8)car->byte8];
                value=bounds->side;
                car->value268=value;
                car->value244=value;
                value=-bounds->side;
                car->value280=value;
                car->value256=value;
                value=bounds->height;
                car->value260=value;
                car->value248=value;
                value=bounds->height;
                car->value284=value;
                car->value272=value;
                value=bounds->front;
                car->value264=value;
                car->value252=value;
                value=-bounds->rear;
                car->value288=value;
                car->value276=value;
                maxlen=bounds->front>bounds->rear?bounds->front:bounds->rear;
                car->radius=sqrtf((maxlen*maxlen+bounds->side*bounds->side)+bounds->height*bounds->height);
            }
        }
    }
    for(i=D_801543CA;i<6;i++) {
        car=&D_8014A250[i];
        config=&D_80153E88[i];
        car->half1994=config->owner==0;
        car->byte1996=0;
        car->half1992=0;
    }
    if(state_word_a&8) {
        D_8014A250[0].byte9=0;
        D_8014A250[0].byte10=1;
    }
    D_80152015=0;
    D_80153E84=D_8015274C;
    if(D_80153FD2>=2) D_80153E84+=7;
    D_80153F08=D_80153E84;
    D_80153F24=-1;
    D_80153F40=0;
}
