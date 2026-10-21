s32 func_803AD57C(void *callback_context)
{
    s32 pad_top[2];
    s32 i, x, y, cx;
    u8 *str;
    u8 str1[64];
    s16 screenPos[2];
    s32 pad_mid[3];
    render_helper(0.0f);
    switch(D_803BA804) {
    case 0:func_803AA3D8();break;
    case 1:
        func_800B669C(1,1);
        brake_light_update(0,D_803B5F28[17].position,&D_80150B70[0],0,screenPos);
        cx=screenPos[0];y=screenPos[1];
        func_800F6928(D_803B5F28[17].position[2]-10.0f);
        dispatch_handler(1);object_create(13);dispatch_handler(1);
        {u8 *t0; t0=!D_803BA800 ? countdown_state.text[176] : (*D_803BA800)->name; music_fade(cx,y,140,t0);}
        func_800B669C(0,1);object_bytes_sum_global();object_create(11);
        for(i=0;i<2;i++) {
            if(D_803B5F28[i+11].angle>0.6981316804885864f && D_803B5F28[i+11].angle<2.4434609413146973f)continue;
            if(!D_803B5F28[i+11].alpha)continue;
            func_800ED66C((f32)(u32)D_803B5F28[i+11].alpha);
            if(D_803B5F28[i+11].angle<1.5707963705062866f) {func_800BEA3C(D_801146BC,D_801146C0);dispatch_handler(1);}
            else {func_800BEA3C(D_801146BC,D_801146C0);dispatch_handler(22);}
            func_800F6928(D_803B5F28[i+11].position[2]-10.0f);
            brake_light_update(0,D_803B5F28[i+11].position,&D_80150B70[0],0,screenPos);
            x=screenPos[0]-35.0f*D_803B64E8*300.0f/D_803B64FC;y=screenPos[1];
            brake_light_update(0,D_803B5F28[15].position,&D_80150B70[0],0,screenPos);
            cx=screenPos[0]-55.0f*D_803B64E8*300.0f/D_803B64FC;
            state_utility(x,y,NAME(51,i));
            x=cx;
            /*@{b*//*@| str=str1; @}*/
            if(i==0) {
                if(D_803BA880==4) {
                    /*@{a*/fcvt_wrapper(str1,D_803B8AD8,NAME(44,D_803BA860),countdown_state.text[42]);str=str1;/*@| str=str1;fcvt_wrapper(str,D_803B8AD8,NAME(44,D_803BA860),countdown_state.text[42]); @}*/
                } else if(D_803BA880==5)str=countdown_state.text[176];
                else if(D_803BA858<12 && (D_803BA858&1)) {
                    /*@{a*/fcvt_wrapper(str1,D_803B8AE0,D_8011A8D8[D_803BA858>=13 ? D_803BA858-6 : D_803BA858/2],countdown_state.text[6]);str=str1;/*@| str=str1;fcvt_wrapper(str,D_803B8AE0,D_8011A8D8[D_803BA858>=13 ? D_803BA858-6 : D_803BA858/2],countdown_state.text[6]); @}*/
                } else str=D_8011A8D8[D_803BA858>=13 ? D_803BA858-6 : D_803BA858/2];
                state_utility(x,y,str);
            } else if(i==1)state_utility(x,y,NAME(52,D_803BA818[D_803BA880]));
        }
        func_800B669C(0,3);func_800ED66C(-1.0f);render_helper(0.0f);
        switch(D_803BA818[D_803BA880]) {
        case 0:func_803AD0D4();break;
        case 1:func_803ACF5C(0);break;
        case 2:func_803ACF5C(1);break;
        case 3:func_803ACF5C(2);break;
        case 4:func_803ACD5C();break;
        case 5:func_803ACA94();break;
        case 6: {
            s32 stat_index, stat_x, stat_y;
            u8 *stats;
            u8 totals_text[40];
            if(D_803BA800)stats=PROFILE_DATA+12+D_803BA858*64;
            else stats=D_80151410+D_803BA858*64-1280;
            object_create(11);dispatch_handler(1);
            for(stat_index=0;stat_index<10;stat_index++) {
                if(stat_index==0) {stat_x=110;stat_y=90;}
                else if(stat_index==5) {stat_x=250;stat_y=90;}
                fcvt_wrapper(totals_text,D_803B8B60,NAME(37,stat_index));
                state_utility(sound_pitch_diff_calc(totals_text,stat_x-4),stat_y,totals_text);
                fcvt_wrapper(totals_text,D_803B8B64,U32_AT(stats,20+stat_index*4));
                state_utility(stat_x+4,stat_y,totals_text);stat_y+=object_bytes_sum_global();
            }
            crowd_cheer_play(D_803B6530,20,86,300,94+object_bytes_sum_global()*5);
            break;
        }
        case 7:func_803AC7F4();break;
        case 8:func_803AC54C();break;
        case 9:func_803ABEC8();break;
        case 10:func_803AB848();break;
        case 11:func_803AB5FC();break;
        case 12:func_803AAF0C();break;
        }
        func_800B669C(0,3);
        break;
    }
    render_helper(-1.0f);
    return 1;
}
