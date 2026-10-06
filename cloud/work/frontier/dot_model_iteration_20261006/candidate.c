/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* NONMATCH: func_800E6AF8, N64 model iteration and drone schedule.
 * Current padding-free baseline 314/334 -> 125/334 differing words;
 * emitted 332 words, frame 144 versus native 334 words/frame 160.
 * Actual donor ancestry: rushtherock game/mdrive.c:model_iteration,
 * update_drone_models, update_link_cars, commit
 * 845329d7b36f5a384c5625ed9a0aef584ab46139 (copyright 1996 Atari Corporation).
 * N64 multiplayer scheduling, recording and model-time behavior are adapted.
 *
 * repro.py supplies real E56F8/E4B58/path bodies and reuses specifically
 * accepted qualifications: D_801525F0 volatile from D60AC/players_frame_update
 * (with the native unsigned16 read width), D_80153FD2 volatile s16 from EC914.
 * These are source-contract precedents, not proof of asynchronous mutation.
 * The reconstructed scheduler boundary follows a real donor operation, but
 * its exact N64 boundary/identity is a hypothesis. No retail stub is claimed.
 * No fake caller, fabricated frame object, unused formal, or assembly.
 */

void update_drone_models_n64(void) {
    s16 n, k;
    Row3 *row;
    D_8014A250_Record *car;
        D_80153F24 = D_80153F24 + 1;
        row = (Row3 *) ((u8 *) D_80120E74 + (D_80153E84 * 6));
        if (row->v[D_80153F24] == -1) {
            D_80153F24 = 0;
            D_80153E84 = D_80153F08;
            row = (Row3 *) ((u8 *) D_80120E74 + (D_80153E84 * 6));
        }
        n = row->v[D_80153F24];
        if (n != 0) {
            k = 0;
            if (n > 0) {
                do {
                    car = &D_8014A250[D_80152808[D_80153F40]];
                    car->dt = (f32) (D_80143FF4 - car->lastTick) * D_8002AFB8;
                    car->lastTick = D_80143FF4;
                    car->tickTime = D_801543CC;
                    func_800E56F8(car->unk7C6);
                    D_80153F40 += 1;
                    k += 1;
                    if (D_80153F40 >= D_8015274C) {
                        D_80153F40 = 0;
                    }
                } while (k < n);
            }
        }
}





void func_800E6AF8(void) {
    f32 fz, fx, fy;
    s32 i;
    s32 j;
    s32 v;
    s32 r;
    f32 t;
    Inp *rec;
    D_8014A250_Record *car;
    GC2 *gc;

    if ((state_word_a & 0x600008) && (D_801525F0 != 0)) {
        D_80143FF4 = D_80143FF4 + 1;
        D_801543CC = (f32) D_80143FF4 * D_8002AFB8;
        for (v = 0; v != 6; v++) {
            if (((GC2 *) player_array)[v].w380 == 0) {
                car = &D_8014A250[v];
                if (car->s7CA == 0) {
                    fz = car->vel[2];
                    fx = car->vel[0];
                    fy = car->vel[1];
                    car->speed = sqrtf((fz * fz) + ((fx * fx) + (fy * fy)));
                }
            }
        }
        func_800E681C();
        i = 0;
        if (D_80153FD2 > 0) {
            rec = input_rec0;
            do {
                car = &D_8014A250[rec->car];
                j = 0;
                for (;;) {
                    if (gameplay_mode == 2) {
                        r = func_800E5D64(i, &t);
                        if (r < 0) {
                            break;
                        }
                        car->lastTick += 1;
                        car->dt = t;
                        car->tickTime = (f32) car->lastTick * t;
                    } else {
                        car->dt = (f32) (D_80143FF4 - car->lastTick) * D_8002AFB8;
                        car->lastTick = D_80143FF4;
                        car->tickTime = D_801543CC;
                    }
                    func_800E5C9C(car);
                    if ((car->b732 != 0) && ((gameplay_mode != 6) || (car->b640 != 0)) && (state_word_a & 0x400000) && (car->s6C4 == -1)) {
                        gc = &((GC2 *) player_array)[rec->car];
                        if ((gc->bEF == 0) && (gc->b359 == 0) && ((s8) D_8013FECB == 0) && (((s8) D_80142760 == 0) || ((car->speed < 10.0f) && (gc->b358 == 0)))) {
                            if (gameplay_mode == 4) {
                                func_800C3578(i);
                            }
                            car->b6CC = 1;
                            func_800C54F0(rec->car, 0);
                        }
                    }
                    if (((GC2 *) player_array)[rec->car].b359 < 2) {
                        func_800E56F8(rec->car);
                    } else {
                        menu_audio_settings(car);
                    }
                    if ((gameplay_mode == 2) && (j == 0) && (r > 0)) {
                        j = 1;
                        continue;
                    }
                    break;
                }
                i += 1;
                rec++;
            } while (i < D_80153FD2);
        }
        update_drone_models_n64();
        if (D_80153F40 == 0) {
            players_race_update();
        }
        main_menu_render();
    }
}
