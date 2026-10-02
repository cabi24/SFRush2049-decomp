/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef struct Object {
    u8 pad0[1990]; s16 index,active,ai; s8 kind;u8 tail[59];
} Object;
typedef struct Metadata {u8 pad0[5];u8 skin,flags,mode;} Metadata;
typedef struct Player {u8 pad0[238];u8 skin;u8 pad239[620];s8 index;u8 tail[92];} Player;
extern Object D_8014A250[];
extern Metadata D_80153E88[6];
extern Player D_80152818[];
extern s16 D_801543CA,D_8015274C,D_80152768,D_80153FD2;
extern s8 D_80152744;
extern s16 D_80143A40[],D_801527D8[],D_80152808[];
extern int D_80143FF4;
void func_800EC914(void) {
    int i;
    Object *object;
    Metadata *meta;
    int index;
    D_80152744=0;
    for(i=0;i<D_801543CA;i++){
        D_8014A250[i].kind=0;
        D_8014A250[i].ai=(D_80153E88[i].mode==0);
        D_8014A250[i].active=((D_80153E88[i].flags&128)!=0);
        if(D_8014A250[i].active){
            D_8014A250[D_80152744].index=i;
            D_80152744++;
            D_80152818[i].index=i;
            D_80152818[i].skin=D_80153E88[i].skin;
            if(D_80153E88[i].mode<6)D_8014A250[i].kind=1;
            else D_8014A250[i].kind=2;
        }
    }
    for(i=D_801543CA;i<6;i++){
        D_8014A250[i].ai=(D_80153E88[i].mode==0);
        D_8014A250[i].kind=0;
        D_8014A250[i].active=0;
    }
    D_8015274C=0;
    D_80152768=D_80153FD2=D_8015274C;
    for(object=D_8014A250;object<D_8014A250+D_80152744;object++){
        index=object->index;
        if(D_8014A250[index].kind==2){
            D_80143A40[D_80153FD2]=index;
            D_80153FD2++;
        }else{
            D_801527D8[D_80152768]=index;
            D_80152768++;
            if(D_8014A250[index].ai){
                D_80152808[D_8015274C]=index;
                D_8015274C++;
            }
        }
    }
    D_80143FF4=0;
}
