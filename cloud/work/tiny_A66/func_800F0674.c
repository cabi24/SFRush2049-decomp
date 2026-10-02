/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef unsigned char u8;typedef signed short s16;typedef int s32;
extern u8 *D_80156940,*D_80156948;extern s16 D_80156950;
s32 func_800F0674(u8 *pattern,u8 *text) {
 u8 *start;s32 current,character,first;s16 progress;
restart:
 D_80156940=pattern;
scan:
 current=*text;if(!current)return 0;
 first=*pattern;character=first;
 if(current!=first){text++;goto scan;}
 start=text;
 while(current==character){text++;current=*text;}
 while(first&&first==pattern[1]){pattern++;first=*pattern;}
advance_pattern:
 pattern++;first=*pattern;character=first;
 while(first==32){pattern++;first=*pattern;character=first;}
 if(!first)goto found;
 progress=0;
alphanumeric:
 while(!((current>=65&&current<91)||(current>=48&&current<58))){text++;current=*text;}
 D_80156950=progress;
 if(current==character)goto matched;
 if(current){text++;if(progress){current=*text;progress--;goto alphanumeric;}D_80156950=progress;}
 D_80156948=start;text=start+1;pattern=D_80156940;goto restart;
matched:
 text++;current=*text;
 if(current!=character)goto advance_pattern;
 if(character!=pattern[1])goto matched;
 goto advance_pattern;
found:
 D_80156948=start;
 while(start<text){*start++=33;}
 D_80156948=start;
 func_800F0674(D_80156940,text);
 return 1;
}
