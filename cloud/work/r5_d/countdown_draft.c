/* flags: -g0 -O3 -mips2 -G 0 -non_shared (IPA group member; draft, does not match) */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;

/* player records, stride 0x808 at 0x8014A250 */
typedef struct {
  u8 pad0[0x7C6];
  s16 idx;
  u8 pad1[0x7E8 - 0x7C8];
  s8 team;
  u8 pad2[0x808 - 0x7E9];
} CdPlayer;
/* input records, stride 0x4C at 0x8014A118 */
typedef struct {
  u8 slot;
  u8 pad[0x4C - 1];
} CdInput;
/* car structs, stride 0x3B8 at 0x80152818 */
typedef struct {
  u8 pad0[0xE8];
  s32 flags;
  u8 pad1[0xEF - 0xEC];
  s8 b0EF;
  u8 pad2[0x35B - 0xF0];
  s8 cpak;
  u8 pad3[0x380 - 0x35C];
  u8 *pad380;
  u8 pad4[0x3A3 - 0x384];
  s8 b3A3;
  u8 pad5[0x3B8 - 0x3A4];
} CdCar;
/* scoring entries, stride 0x78 at 0x80152038 */
typedef struct {
  u8 pad0[20];
  s32 key;
  u8 pad1[0x78 - 24];
} CdEnt;

extern s32 D_801174B4;
extern s32 D_801174B8;
extern s32 D_801174BC;
extern s8 D_80150F14;
extern s8 D_80150EFC;
extern s8 D_801146F0;
extern s8 D_80114650;
extern s8 D_80114654;
extern CdPlayer D_8014A250[];
extern f32 D_8002AFB4;
extern s32 D_8014A110;
extern s32 D_801407BC;
extern s32 D_80140804;
extern s32 D_80140B08;
extern s32 D_80140BD8;
extern f32 D_8014401C;
extern f32 D_801543CC;
extern s8 D_80142699;
extern s8 D_8013FECB;
extern s16 D_8014A108;
extern s8 D_80142760;
extern CdEnt D_80152038[];
extern s32 D_80140A00;
extern s32 D_80140AD8;
extern CdCar D_80152818[];
extern CdInput D_8014A118[];
extern s8 D_8015256C[];
extern s8 D_8012E67C[];
extern s32 D_80142510;
extern s8 D_80142690;
extern s32 D_80143F10;
extern s16 D_80152734;
extern f32 D_801525F4;
extern s8 D_80152744;
extern s16 D_80151AD0;
extern s32 *D_80152698[];
extern s32 D_80150B94[];

void init_state_begin();
void init_state_continue();
s32 setup_state_main();
void set_race_state();
void func_800C813C();
void camera_race_setup();
void race_init_helper();
void race_setup_1();
void race_setup_2();
void viScheduleTick(f32);
void func_800E762C();
void func_800FBF2C();
void func_800FBE30();
void func_800B61A8();
void func_800D6160();
void func_800FBE60();
f32 func_800FBED8();
void render_viewport_init();
void func_800F7F3C();
void ghost_race_setup();
void records_screen();
s32 func_800CF604();
s32 func_800E7DD0();
s32 viDeadlinePassed();
void memset();
void speed_set();
void func_800D5828();
void cpak_read();
void players_frame_update();
void func_800D5374();
void finish_state_normal();
void finish_state_alt();
void func_800AB18C();

void countdown(void)
{
  s32 i;
  s32 ok;
  s32 cnt;
  s32 v;
  s32 *pp;

  v = D_801174B4;
  if (v & 0x40000) {
    D_801174BC = 1;
    init_state_begin();
    init_state_continue();
    D_80150F14 = D_80150EFC;
    D_801174B8 = 0x80000;
    goto L3;
  }
  if (v & 0x80000) {
  L3:
    if (setup_state_main(1, 0) != 0) {
      D_801146F0 = 1;
      D_801174B8 = 0x200000;
      set_race_state(0);
      goto L7;
    }
    func_800C813C(0, 1);
    goto done;
  }
  if (v & 0x200000) {
  L7:
    if (D_80114650 != 0) {
      goto done;
    }
    camera_race_setup();
    race_init_helper();
    race_setup_1();
    race_setup_2(D_8014A250[0].idx);
    if (D_801146F0 != 0) {
      viScheduleTick(3.5f);
      func_800E762C((s32)(3.5f * D_8002AFB4));
      D_801146F0 = 0;
    }
    func_800FBF2C();
    func_800FBE30();
    func_800C813C(0, 1);
    goto done;
  }
  if (v & 0x100000) {
    if (D_8014A110 == 1) {
      viScheduleTick(1800.0f);
    } else if (D_8014A110 == 4) {
      if (D_801407BC == 0) {
        viScheduleTick((f32)(D_80140804 * 60));
      } else {
        viScheduleTick(600.0f);
      }
    } else if (D_8014A110 == 5) {
      viScheduleTick(300.0f);
    } else if (D_8014A110 == 6) {
      if (D_80140B08 == 1) {
        viScheduleTick((f32)(D_80140BD8 * 60));
      } else {
        viScheduleTick(1200.0f);
      }
    } else {
      viScheduleTick(func_800FBED8());
    }
    func_800B61A8(0x4E, 0, 1, 0);
    func_800D6160();
    func_800FBE60();
    D_801174B8 = 0x400000;
    D_8014401C = D_801543CC;
    goto done;
  }
  if (v & 0x400000) {
    if (D_8014401C + 3.0f < D_801543CC) {
      func_800C813C(0, 1);
      D_8014401C = D_801543CC;
    }
    render_viewport_init();
    func_800FBE30();
    if (D_80142699 != 0) {
      func_800F7F3C();
      D_801174B8 = 0x1000000;
      ghost_race_setup();
      records_screen();
    } else if (D_8013FECB != 0) {
      ok = 1;
      for (i = 0; i < D_8014A108; i++) {
        if (D_80142760 == 0) {
          if (func_800CF604((s16)i) == 0) {
            ok = 0;
          }
        }
      }
      if (func_800E7DD0() != 0 && ok != 0) {
        if (D_8014A110 == 4) {
          func_800F7F3C();
        }
        D_801174B8 = 0x1000000;
        ghost_race_setup();
        records_screen();
      }
      if (viDeadlinePassed() == 0 && D_8014A110 != 4 && D_8014A110 != 5 && D_8014A110 != 6) {
        D_8013FECB = 0;
        D_80142690 = 0;
      }
    } else if (D_8014A110 == 4) {
      switch (D_801407BC) {
      case 0:
        if (viDeadlinePassed() != 0) {
          D_8013FECB = 1;
          D_80142690 = 1;
        }
        break;
      case 1:
        for (i = 0; i < D_8014A108; i++) {
          if (D_80152038[i].key >= D_80140A00) {
            D_8013FECB = 1;
            D_80142690 = 1;
          }
        }
        break;
      case 2:
        cnt = 0;
        for (i = 0; i < D_8014A108; i++) {
          if ((s32)*(u16 *)(D_80152818[i].pad380 + 0x40) >= D_80140AD8) {
            *((u8 *)&D_80152818[i] + 0x359) = 1;
            cnt++;
          }
        }
        if (cnt == D_8014A108) {
          D_8013FECB = 1;
          D_80142690 = 1;
        }
        break;
      }
    } else if (D_8014A110 == 6) {
      if (D_80140B08 != 0) {
        if (D_80140B08 == 1 && viDeadlinePassed() != 0) {
          func_800F7F3C();
          D_8013FECB = 1;
          D_80142690 = 1;
        }
      } else {
        memset(D_8015256C, 0, 4);
        for (i = 0; i < D_8014A108; i++) {
          D_8015256C[D_8012E67C[i]] += D_80152818[i].b3A3;
        }
        for (i = 0; i < 4; i++) {
          if (D_80114654 == 0 && D_80142510 == D_8015256C[i] + 1) {
            D_80114654 = 1;
            func_800B61A8(2, 0, 2, 0);
          }
          if (D_8015256C[i] >= D_80142510) {
            func_800F7F3C();
            D_8013FECB = 1;
            D_80142690 = 1;
          }
        }
      }
    } else if (D_8014A110 == 0 || D_8014A110 == 1 || D_8014A110 == 3) {
      for (i = 0; i < D_8014A108; i++) {
        if (*((s8 *)&D_80152818[D_8014A118[i].slot] + 0xEF) == 0) {
          goto done;
        }
      }
      D_80143F10 = 0;
      for (i = 0; i < D_8014A108; i++) {
        if (D_80152734 == D_8014A250[i].team) {
          D_80143F10 = 1;
        }
      }
      if (D_8013FECB == 0) {
        if (D_80142760 != 0) {
          D_801525F4 = 0.0f;
        }
        D_8013FECB = 1;
        func_800F7F3C();
        speed_set();
      }
    } else if (viDeadlinePassed() != 0) {
      if (D_8013FECB == 0) {
        D_8013FECB = 1;
        func_800F7F3C();
        speed_set();
      }
      for (i = 0; i < D_8014A108; i++) {
        if (D_80152734 != D_8014A250[D_8014A118[i].slot].team) {
          if (D_8014A110 != 2 || (pp = D_80152698[D_8014A118[i].slot]) == 0 ||
              *(s8 *)(*(s32 *)(*(s32 *)pp + 0x28) + 0) + 5 < 0) {
            func_800B61A8(0x16, 0, 1, 0);
          }
          D_80142690 = 1;
          break;
        }
      }
    }
    goto done;
  }
  if (v & 0x1000000) {
    for (i = 0; i < D_80152744; i++) {
      func_800D5828(D_8014A250[i].idx);
      D_80152818[i].flags &= ~8;
      cpak_read(D_80152818[i].cpak);
    }
    players_frame_update();
    func_800D5374();
    viScheduleTick(4.0f);
    D_801174B8 = 0x2000000;
    goto done;
  }
  if (v & 0x2000000) {
    finish_state_normal();
    goto done;
  }
  if (v & 0x800000) {
    finish_state_alt();
  }
done:
  if (D_801174B4 & 0x600000) {
    for (i = 0; i < D_80151AD0; i++) {
      func_800AB18C(i, &D_80150B94[i * 38]);
    }
  }
}
