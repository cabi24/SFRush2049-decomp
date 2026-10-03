typedef signed int s32;typedef unsigned int u32;typedef unsigned char u8;
typedef struct Command {u32 first,second;} Command;
void camera_smooth_follow(Command *commands,s32 x,s32 y,s32 right,s32 bottom) {
    s32 width=-1,height=-1,size;
    u8 opcode;
    for(;;commands++){
        opcode=(commands->first&0xff000000)>>24;
        if((opcode&0xc0)==0x40 || (opcode&0xc0)==0x80 || (opcode>=9 && opcode<64) || (opcode>=192 && opcode<214))continue;
        if(opcode==242){
            if(x>=0){
                size=((commands->second&0x00fff000)>>12)+4;
                if(width<0)width=size;
                else if(size<width){
                    do{width>>=1;x>>=1;}while(size<width);
                }
                commands->first=(commands->first&0xff000fff)|(x<<12);
            }
            if(y>=0){
                size=(commands->second&0xfff)+4;
                if(height<0)height=size;
                else if(size<height){
                    do{height>>=1;y>>=1;}while(size<height);
                }
                commands->first=(commands->first&0xfffff000)|y;
            }
            if(right>=0)commands->second=(commands->second&0xff000fff)|(right<<12);
            if(bottom>=0)commands->second=(commands->second&0xfffff000)|bottom;
        }else if(opcode==223)break;
    }
}
