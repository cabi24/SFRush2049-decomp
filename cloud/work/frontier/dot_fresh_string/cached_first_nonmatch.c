typedef unsigned char u8;
int func_800BE744(u8 *s) {int n=0; u8 *p=s; u8 first=*p; if(first==255){++p;while(p[0]||p[1]){p+=2;n++;}}else{++p;if(first)do{n++;}while(*p++);}return n;}
