import sys
hdr=open('hdr.h').read()
pre='''    D_8015F738 = 0;
    D_80161380 = 0;
    D_80161398 = 0;
    D_801613A4 = 0;
    D_80154188 = 90.0f;
    while (D_80000300 == 0) {}
'''
calls='''    viewport_setup(D_80000300, get_tv_offset(), D_8002AFC0, D_8002AFC4);
    D_8002EBB0 = audio_dma_sync(0, 0x22620);
    audio_loop_control(D_8002EBB0, 0);
    D_80156CEC = sound_play_menu(0, 0x22620);
    audio_loop_control(D_80156CEC, 0);
    D_80157240 = sound_play_menu(0, 0x22620);
    audio_loop_control(D_80157240, 0);
'''
align_chain='''    D_80156BE0[0].buffer = D_80156CEC = (void *)(((u32)D_80156CEC + 32) & ~63u);
    D_8002EBB0 = (void *)(((u32)D_8002EBB0 + 32) & ~63u);
    D_80156BE0[1].buffer = D_80157240 = (void *)(((u32)D_80157240 + 32) & ~63u);
'''
align_plain='''    D_80156CEC = (void *)(((u32)D_80156CEC + 32) & ~63u);
    D_8002EBB0 = (void *)(((u32)D_8002EBB0 + 32) & ~63u);
    D_80157240 = (void *)(((u32)D_80157240 + 32) & ~63u);
    D_80156BE0[0].buffer = D_80156CEC;
    D_80156BE0[1].buffer = D_80157240;
'''
tail='''    D_80156BE0[0].kind = 2;
    D_80156BE0[1].kind = 2;
    D_80156BE0[0].resource = D_800586D0;
    D_80156BE0[1].resource = D_8006A090;
    D_8015B250 = D_800586D0;
    D_8015B260 = (u8 *)D_8015B250 + 1728;
    D_801497C8 = (u8 *)D_8015B250 + 0x9cc0;
    D_801497F4 = D_801497C8;
    func_800A5B3C();
    func_800A5488();
    particle_velocity_set();
}
'''
vol=hdr.replace('extern void *D_8015B250,*D_8015B260,*D_801497C8,*D_801497F4;','extern void *D_8015B250,*D_8015B260,*D_801497F4; extern void * volatile D_801497C8;')
V={
 'h': vol+'void world_effect_update(void) {\n'+pre+calls+align_chain+tail,
 'i': vol+'void world_effect_update(void) {\n'+pre+calls+align_plain+tail,
 'j': hdr+'void world_effect_update(void) {\n'+pre+calls+align_chain+tail,
}
for k,v in V.items(): open(k+'.c','w').write(v)
stores=pre.replace('    while (D_80000300 == 0) {}\n','')
W={
 'k': vol+'void func_800EE8AC(void) {\n    while (D_80000300 == 0) {}\n}\nvoid world_effect_update(void) {\n'+stores+'    func_800EE8AC();\n'+calls+align_chain+tail,
 'l': vol+'void func_800EE8AC(void) {\n'+pre+'}\nvoid world_effect_update(void) {\n    func_800EE8AC();\n'+calls+align_chain+tail,
 'm': vol+'void world_effect_update(void) {\n'+pre.replace('while (D_80000300 == 0) {}','do {} while (D_80000300 == 0);')+calls+align_chain+tail,
 'n': vol+'void world_effect_update(void) {\n'+pre.replace('while (D_80000300 == 0) {}','if (D_80000300 == 0) { while (D_80000300 == 0) {} }')+calls+align_chain+tail,
}
for k,v in W.items(): open(k+'.c','w').write(v)
