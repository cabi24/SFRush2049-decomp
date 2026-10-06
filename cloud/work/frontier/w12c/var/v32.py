M='''#define kill_scrape_sound(p) \\
    scheduler_recv(D_80140AE0[p]); \\
    D_80140AE0[p] = -1; \\
    D_80140A08[p] = 0; \\
    D_80140B10[p] = 0.0f
'''
F='''static __inline void kill_scrape_sound(s32 p) {
    scheduler_recv(D_80140AE0[p]);
    D_80140AE0[p] = -1;
    D_80140A08[p] = 0;
    D_80140B10[p] = 0.0f;
}
'''
F2=F.replace('static __inline','__inline')
F3=F.replace('s32 p','s16 p')
V={'inl':[(M,F)],'inl2':[(M,F2)],'inl16':[(M,F3)]}
