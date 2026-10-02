/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef unsigned char u8;typedef signed char s8;typedef signed short s16;typedef int s32;
typedef struct Car2056 {u8 pad0[1990];s16 selected,active,empty;s8 mode;u8 pad1997[59];} Car2056;
typedef struct Input8 {u8 pad0[5],value,flags,code;} Input8;
typedef struct Object952 {u8 pad0[238],value,pad239[620],index,pad860[92];} Object952;
extern s16 D_801543CA;extern s8 D_80152744;extern Car2056 D_8014A250[];extern Input8 D_80153E88[];extern Object952 D_80152818[];
extern s16 D_8015274C,D_80152768,D_80153FD2;extern s16 D_80143A40[],D_801527D8[],D_80152808[];extern s32 D_80143FF4;
void func_800EC914(void) {
 s32 i,count=D_801543CA;Car2056 *car,*selected,*end;Input8 *input;Object952 *object;s16 index;
 D_80152744=0;input=D_80153E88;car=D_8014A250;
 i=0;if(count>0)do {
  car->mode=0;car->empty=input->code==0;car->active=(input->flags&0x80)!=0;
  if(car->active) {
   D_8014A250[D_80152744].selected=i;D_80152744++;
   object=&D_80152818[i];object->index=i;object->value=input->value;
   if(input->code<6)car->mode=1;else car->mode=2;
  }
 i++;input++;car++;count=D_801543CA;
 }while(i<count);
 i=D_801543CA;if(i<6) {
  car=&D_8014A250[i];input=&D_80153E88[i];
  do {car->empty=input->code==0;car->mode=0;car->active=0;car++;input++;}while(input<D_80153E88+6);
 }
 D_8015274C=0;D_80152768=D_8015274C;D_80153FD2=D_8015274C;
 if(D_80152744>0) {
  end=D_8014A250+D_80152744;
  selected=D_8014A250;do {
   index=selected->selected;car=&D_8014A250[index];
   if(car->mode==2){D_80143A40[D_80153FD2]=index;D_80153FD2++;}
   else {D_801527D8[D_80152768]=index;D_80152768++;
    if(car->empty){D_80152808[D_8015274C]=index;D_8015274C++;}
   }
   selected++;
  }while(selected<end);
 }
 D_80143FF4=0;
}
