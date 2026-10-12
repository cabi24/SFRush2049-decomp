/* Genuine controller baseline from draft #366, before current work.
 * Source-only reference; compiled by the packet with its genuine context. */
void func_80396FA0(s32 player)
{
    while (!D_803BAD00[player][D_803BAD80[player]] ||
           func_80395E24(player,D_803BAC80[player])==1 ||
           (D_803BAD80[player]==11 && func_80396F44(player)==1) ||
           (D_803BAD80[player]==12 && func_80396EE8(player)==1)) {
        D_803BADB8[player]=0;
        D_803BAD80[player]--;
        if(D_803BAD80[player]<0)D_803BAD80[player]=13;
        if(D_803BAD80[player]>=3 && D_803BAD80[player]<11 && D_803BAD00[player][D_803BAD80[player]]) {
            if(D_803BAC80[player]) {
                do {
                    D_803BAC80[player]=(*D_803BAC80[player])->previous;
                } while(D_803BAC80[player] && (*(*D_803BAC80[player])->file)->controller!=D_803BADBC[player]);
            }
            if(!D_803BAC80[player]) {
                D_803BAC80[player]=D_8012E6E4;
                while(D_803BAC80[player] && (*(*D_803BAC80[player])->file)->controller!=D_803BADBC[player])
                    D_803BAC80[player]=(*D_803BAC80[player])->previous;
            }
        } else D_803BAC80[player]=0;
    }
    if(D_8014A118[player].pressed&1) {
        resource_type_select(D_8014A118[player].pressed);
        if(D_803BAD80[player]>=3 && D_803BAD80[player]<11) {
            D_8014A118[player].owner=D_803BAC80[player];
            func_80394564(-1);
        } else {
            D_8014A118[player].owner=&D_80146150[player];
            func_80394564(-1);
        }
        func_80394B00(player,1);
    } else if(D_8014A118[player].pressed&2) {
        resource_type_select(D_8014A118[player].pressed);
        if(D_803BAD80[player]>=3 && D_803BAD80[player]<11) {
            D_8014A118[player].owner=D_803BAC80[player];
            func_80394564(-1);
            if(D_803BADB8[player]==1) {
                if(func_800CCEFC(D_8014A118[player].owner)) {
                    D_8015418C[player]=1;
                    func_800A3424((*D_8014A118[player].owner)->file,1);
                    func_80394B00(player,2);
                }
            } else func_80394B00(player,1);
        } else switch(D_803BAD80[player]) {
        case 0:
            D_8014A118[player].owner=&D_80146150[player];
            func_80394564(-1);
            if(D_803BADB8[player]==1) {
                D_8015418C[player]=1;
                func_800A3424((*D_8014A118[player].owner)->file,1);
                func_80394B00(player,2);
            } else func_80394B00(player,1);
            break;
        case 11:func_80394B00(player,3);break;
        case 12:D_803BADC8[player]=0;func_80394B00(player,4);break;
        case 13:func_80394B00(player,6);break;
        }
    } else if(D_8014A118[player].pressed&4) {
        resource_type_select(D_8014A118[player].pressed);
        D_803B5ED4=2;
    }
    if(D_8014A118[player].repeat&0x2000) {
        if(D_803BAD80[player]==0 || (D_803BAD80[player]>=3 && D_803BAD80[player]<11)) {
            D_803BADB8[player]=!D_803BADB8[player];
            audio_doppler(D_8014A118[player].pressed);
        } else if(D_803BAD80[player]==1) {
            D_803BADBC[player]++;
            if(D_803BADBC[player]>=4)D_803BADBC[player]=0;
            func_80394564(player);
            D_803BA888[player]=1;
            audio_doppler(D_8014A118[player].pressed);
        }
    } else if(D_8014A118[player].repeat&0x1000) {
        if(D_803BAD80[player]==0 || (D_803BAD80[player]>=3 && D_803BAD80[player]<11)) {
            D_803BADB8[player]=!D_803BADB8[player];
            audio_doppler(D_8014A118[player].pressed);
        } else if(D_803BAD80[player]==1) {
            D_803BADBC[player]--;
            if(D_803BADBC[player]<0)D_803BADBC[player]=3;
            func_80394564(player);
            D_803BA888[player]=1;
            audio_doppler(D_8014A118[player].pressed);
        }
    } else if(D_8014A118[player].repeat&0x400) {
        audio_distance_atten(D_8014A118[player].pressed);
        D_803BADB8[player]=0;
        do {
            if(D_803BAD80[player]==3 && D_803BAD00[player][D_803BAD80[player]]) {
                if(D_803BAC80[player]) {
                    do {
                        D_803BAC80[player]=(*D_803BAC80[player])->previous;
                    } while(D_803BAC80[player] && (*(*D_803BAC80[player])->file)->controller!=D_803BADBC[player]);
                }
                if(D_803BAC80[player])break;
            }
            D_803BAD80[player]--;
            if(D_803BAD80[player]<0)D_803BAD80[player]=13;
            if(D_803BAD80[player]>=3 && D_803BAD80[player]<11 && D_803BAD00[player][D_803BAD80[player]]) {
                if(D_803BAC80[player]) {
                    do {
                        D_803BAC80[player]=(*D_803BAC80[player])->previous;
                    } while(D_803BAC80[player] && (*(*D_803BAC80[player])->file)->controller!=D_803BADBC[player]);
                }
                if(!D_803BAC80[player]) {
                    D_803BAC80[player]=D_8012E6E4;
                    while(D_803BAC80[player] && (*(*D_803BAC80[player])->file)->controller!=D_803BADBC[player])
                        D_803BAC80[player]=(*D_803BAC80[player])->previous;
                }
            } else D_803BAC80[player]=0;
        } while(!D_803BAD00[player][D_803BAD80[player]] || func_80395E24(player,D_803BAC80[player])==1 ||
                (D_803BAD80[player]==11 && func_80396F44(player)==1) ||
                (D_803BAD80[player]==12 && func_80396EE8(player)==1));
    } else if(D_8014A118[player].repeat&0x800) {
        audio_distance_atten(D_8014A118[player].pressed);
        D_803BADB8[player]=0;
        do {
            if(D_803BAD80[player]==10 && D_803BAD00[player][D_803BAD80[player]]) {
                if(D_803BAC80[player]) {
                    do {
                        D_803BAC80[player]=(*D_803BAC80[player])->next;
                    } while(D_803BAC80[player] && (*(*D_803BAC80[player])->file)->controller!=D_803BADBC[player]);
                }
                if(D_803BAC80[player])break;
            }
            D_803BAD80[player]++;
            if(D_803BAD80[player]>=14)D_803BAD80[player]=0;
            if(D_803BAD80[player]>=3 && D_803BAD80[player]<11 && D_803BAD00[player][D_803BAD80[player]]) {
                if(D_803BAC80[player]) {
                    do {
                        D_803BAC80[player]=(*D_803BAC80[player])->next;
                    } while(D_803BAC80[player] && (*(*D_803BAC80[player])->file)->controller!=D_803BADBC[player]);
                }
                if(!D_803BAC80[player]) {
                    D_803BAC80[player]=D_8012E6E0;
                    while(D_803BAC80[player] && (*(*D_803BAC80[player])->file)->controller!=D_803BADBC[player])
                        D_803BAC80[player]=(*D_803BAC80[player])->next;
                }
            } else D_803BAC80[player]=0;
        } while(!D_803BAD00[player][D_803BAD80[player]] || func_80395E24(player,D_803BAC80[player])==1 ||
                (D_803BAD80[player]==11 && func_80396F44(player)==1) ||
                (D_803BAD80[player]==12 && func_80396EE8(player)==1));
    }
}
