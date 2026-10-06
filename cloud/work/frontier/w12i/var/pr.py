HP=('''#define MODE(m) FIELD(m,s8,1996)''','''void probe4(void *,void *,void *,void *);
void func_800E0048(s32 a, s32 b)
{
    s32 c,d;
    probe4(&a,&b,&c,&d);
}
#define MODE(m) FIELD(m,s8,1996)''')
CALL=('''    if(FIELD(model,s16,1628)==1) {''','''    func_800E0048(1,2);
    if(FIELD(model,s16,1628)==1) {''')
V={'pr':[HP,CALL]}
