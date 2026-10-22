void func_803A9088(s32 player)
{
    f32 scale=D_803B5D10;
    s32 y,x;
    u8 *label,text[128];
    s32 draw_x,i,seen;
    s16 point[2],top,lines,limit;
    s8 selected;
    f32 delta;
    Profile **node;
    if(D_8014A108>=2)scale*=0.5f;
    if(D_8014A108>=2)object_create(12);else object_create(13);
    label=countdown_state.text[140];
    dispatch_handler(D_801146AC[player]);
    func_800B669C(0,1);
    brake_light_update(player,D_803B470C[player][16].position,&D_80150B70[player],0,point);
    x=point[0];y=point[1];
    func_800F6928(D_803B470C[player][16].position[2]-10.0f);
    dispatch_handler(1);
    state_utility(sound_pitch_diff_halved(label,x),y,label);
    y+=object_bytes_sum_global();
    if(D_8014A108>=2)object_create(12);else object_create(10);
    if(D_803BADCC[player]==1) {
        func_800B669C(0,3);
        func_800F6928(0.0f);
        y+=object_bytes_sum_global();
        top=y-4;
        fcvt_wrapper(text,D_803B89A4,countdown_state.text[141],D_803BADD8[player],countdown_state.text[142]);
        dispatch_handler(1);
        func_800B669C(1,3);
        camera_auto_follow(x,y,D_803B5D50[D_8014A108-1][player].width,
                           D_803B5D50[D_8014A108-1][player].y+D_803B5D50[D_8014A108-1][player].height-y,-1,0,text);
        func_800B669C(0,3);
        lines=camera_zoom_fov(D_803B5D50[D_8014A108-1][player].width,text);
        y+=object_bytes_sum_global()*(lines+1);
        label=countdown_state.indexed[countdown_state.indices[24]];
        dispatch_handler(D_803BADD0[player]==1 ? 22 : 1);
        state_utility(sound_pitch_diff_calc(label,x-5),y,label);
        label=countdown_state.indexed[countdown_state.indices[24]+1];
        dispatch_handler(D_803BADD0[player]==0 ? 22 : 1);
        state_utility(x+5,y,label);
        y+=object_bytes_sum_global();
        crowd_cheer_play(D_803BAC08[player],x-D_803B5D50[D_8014A108-1][player].width/2-4,top,
                         x+D_803B5D50[D_8014A108-1][player].width/2+4,y+4);
    } else if(D_803BADD4[player]==1) {
        func_800B669C(0,3);
        func_800F6928(0.0f);
        y+=object_bytes_sum_global();
        top=y-4;
        fcvt_wrapper(text,countdown_state.text[145],D_803BADD8[player]);
        dispatch_handler(1);
        func_800B669C(1,3);
        camera_auto_follow(x,y,D_803B5D50[D_8014A108-1][player].width,
                           D_803B5D50[D_8014A108-1][player].y+D_803B5D50[D_8014A108-1][player].height-y,-1,0,text);
        func_800B669C(0,3);
        lines=camera_zoom_fov(D_803B5D50[D_8014A108-1][player].width,text);
        y+=object_bytes_sum_global()*lines;
        crowd_cheer_play(D_803BAC08[player],x-D_803B5D50[D_8014A108-1][player].width/2-4,top,
                         x+D_803B5D50[D_8014A108-1][player].width/2+4,y+4);
    } else {
        crowd_cheer_play(D_803BAC08[player],-20,0,-10,10);
        for(i=0;i<14;i++) {
            if(D_803B470C[player][i].angle>0.6981316804885864f && D_803B470C[player][i].angle<2.4434609413146973f)continue;
            if(!D_803B470C[player][i].alpha)continue;
            func_800ED66C((f32)(u32)D_803B470C[player][i].alpha);
            if(D_803B470C[player][i].angle<1.5707963705062866f) {
                func_800BEA3C(D_801146BC,D_801146C0);
                dispatch_handler(1);
            } else {
                func_800BEA3C(D_801146BC,D_801146C0);
                dispatch_handler(22);
            }
            func_800F6928(D_803B470C[player][i].position[2]-10.0f);
            brake_light_update(player,D_803B470C[player][i].position,&D_80150B70[player],0,point);
            if(D_803B470C[player][i].kind==9)draw_x=point[0]-105.0f*scale*300.0f/D_803B5D24;
            else if(D_803B470C[player][i].kind==7)draw_x=point[0]-65.0f*scale*300.0f/D_803B5D24;
            else draw_x=point[0]-45.0f*scale*300.0f/D_803B5D24;
            y=point[1];
            if(i==0 || (i>=3 && i<11)) {
                brake_light_update(player,D_803B470C[player][14].position,&D_80150B70[player],0,point);
                x=point[0]-45.0f*scale*300.0f/D_803B5D24;
            } else if(i==1) {
                brake_light_update(player,D_803B470C[player][15].position,&D_80150B70[player],0,point);
                x=point[0]-25.0f*scale*300.0f/D_803B5D24;
            }
            if(i==1) {
                dispatch_handler(D_803BADC8[player]==1 ? 22 : 1);
                label=countdown_state.indexed[countdown_state.indices[21]+i];
                state_utility(draw_x,y,label);
                label=D_8011EAD0[D_803BADBC[player]];
                state_utility(x,y,label);
            } else if(i==2) {
                delta=65.f*scale*300.0f/D_803B5D24;
                switch(D_803BADC0[player]) {
                case -4:label=countdown_state.text[165];break;
                case -3:label=countdown_state.text[169];break;
                case 0:label=countdown_state.text[148];break;
                case -2:label=countdown_state.text[166];break;
                case -5:label=countdown_state.text[161];break;
                case -1:label=countdown_state.text[168];break;
                }
                draw_x+=delta;
                dispatch_handler(0);
                state_utility(sound_pitch_diff_halved(label,draw_x),y,label);
            } else if(i>=3 && i<11) {
                node=D_803BAC80[player];seen=0;
                if(node) {
                    selected=D_803BADC8[player];
                    do {
                        if((*node)->file && (*(*node)->file)->controller==D_803BADBC[player]) {
                            if(PROFILE_DISTANCE(i-selected)==seen)break;
                            seen++;
                        }
                        if(i<selected)node=(*node)->previous;else node=(*node)->next;
                    } while(node);
                }
                if(!node) {
                    selected=D_803BADC8[player];
                    if(selected<i || D_803BADC0[player]<8)node=D_8012E6E0;
                    else node=D_8012E6E4;
                    while(node) {
                        if((*node)->file && (*(*node)->file)->controller==D_803BADBC[player]) {
                            if(PROFILE_DISTANCE(i-((selected<i || D_803BADC0[player]<8) ? 3 : 10))==seen) {seen++;break;}
                            seen++;
                        }
                        if(selected<i || D_803BADC0[player]<8)node=(*node)->next;
                        else node=(*node)->previous;
                    }
                }
                if(node) {
                    label=(*node)->name;
                    if(D_8014A108==1)limit=98;
                    else limit=(u16)(D_8014A108==2 ? 70 : 55);
                    if((u32)limit<(u32)object_manager_update(label,-1))mode_byte_set(0);
                    if(i==D_803BADC8[player] && node==D_803BAC80[player])dispatch_handler(22);
                    else {
                        dispatch_handler(1);
                        if(func_80395E24(player,node))dispatch_handler(0);
                        if(!D_803BAD00[player][i])dispatch_handler(4);
                    }
                    music_fade(draw_x,y,limit,label);
                    mode_byte_set(-1);
                }
            }
            object_bytes_sum_global();
        }
    }
    func_800B669C(0,3);
    func_800ED66C(-1.0f);
    render_helper(0.0f);
}
