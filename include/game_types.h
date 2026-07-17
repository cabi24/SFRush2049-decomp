/* Game-structure type context for the game-loop cluster's m2c seeds
 * (005-game-context-bootstrap, T018/T019). Included last in autodecomp's
 * context chain (after m2c_types.h), extracted-population only (SC-005:
 * this file must never change static-population seed text).
 *
 * Scope (FR-004): every definition here exists because a named blocker in
 * the T011 cluster compile-probe (specs/005-game-context-bootstrap/
 * research/t011-blockers.md, `autodecomp clusters --population extracted
 * --targets @tools/conveyor/clusters/game_loop.txt`) required it, or
 * because data-model.md's committed content plan named it explicitly
 * (GState, Track_Data, the 9-entry game symbol table). Rules per
 * m2c_types.h: types/typedefs/struct defs and `extern` declarations only,
 * no function bodies; unknown fields keep `unkN` (offset N) names.
 *
 * IMPORTANT — read before adding more: several of the blockers below are
 * NOT fixable by adding more declarations here. `research/t019-stall.md`
 * documents a confirmed disasm.py symbolizer gap (pipeline/disasm.py's
 * normalize_objdump only recognizes a global reached by `lui`+`imm(reg)`;
 * it does not track `lui`+`addiu $r,$r,imm` register-pointer formation, so
 * several committed game-symbol-table entries — frame_counter, msgq_ptr in
 * most callers, input_rec0/input_rec1, player_array — stay numeric in the
 * derived asm regardless of being declared here). Declarations for those
 * symbols are kept below anyway (data-model.md requires them, and they're
 * correct/harmless — they'll take effect the moment the symbolizer gap is
 * fixed) but do not re-derive or re-verify them against a fresh cluster
 * probe expecting them to unlock anything; see the stall writeup first.
 */
#ifndef GAME_TYPES_H
#define GAME_TYPES_H

/* --- Main game state -----------------------------------------------------
 * rushtherock: game/game.h:104-117 (enum), game/globals.h:142 (gstate decl).
 * N64: game_loop (0x800FD464) loads/stores gstate as a single byte around
 * its state dispatch (research/cluster-data-refs.md cross-check, confirmed)
 * -- the arcade's C `enum` is 4 bytes; this ROM's build packed it to u8, so
 * the extern below types it `u8`, not `GState`, to match the real access
 * width (the enum itself is kept for its member values / documentation).
 */
typedef enum GState {
    ATTRACT, TRKSEL, CARSEL, PLAYGAME, ENDGAME, GAMEOVER, HISCORE,
    PREPLAY, PREPLAY2, COUNTDOWN, NUM_GAME_STATES
} GState;

/* --- Committed game symbol table (data-model.md) --------------------------
 * research/cluster-data-refs.md is the evidence for every address; see that
 * file's per-function tables and the "Cross-check of proposed globals"
 * section for gstate/player_array/frame_counter confirmation.
 */
extern u8 gstate;                 /* 0x801146EC - game_loop state dispatch, confirmed */
extern s32 frame_counter;         /* 0x80142AFC - game_loop init/read/increment; NOT
                                    * currently symbolized (addiu gap, see header note) */
extern s32 game_state_flags;      /* 0x801146E8 - word R/W adjacent to gstate */
extern s32 state_word_a;          /* 0x801174B4 - word R/W, secondary state (110 refs) */
extern s32 state_word_b;          /* 0x801174B8 - word R/W */

/* msgq_ptr (0x801497C8): attract_or_transition's struct-pattern note records
 * offsets {0x00,0x04,0x0C} read/written through it, which line up exactly
 * with OSMesgQueue's real mtqueue(0x0)/fullqueue(0x4)/first(0xC) fields
 * (OSMesgQueue is declared in m2c_types.h's context chain, included before
 * this file) -- strong evidence this is a real OSMesgQueue*, not a game-
 * specific struct. NOT currently symbolized in attract_or_transition.s or
 * Input_ProcessGameplayPad.s (both reach it via an addiu-formed pointer);
 * IS symbolized in game_loop.s/game_mode_handler.s (direct lui+lw), where
 * it compiles today as-is.
 */
extern OSMesgQueue *msgq_ptr;     /* 0x801497C8 */

/* --- Input records (process_inputs, 0x800C997C) ---------------------------
 * rushtherock shape references (not layout truth): globals.h:143-147,213-215
 * (switch/pot globals) + modeldat.h:472-493 (fastintyp/mainintyp) describe
 * the arcade's per-control-channel scalars this N64 code plays the same
 * role as. Observed N64 shape (research/cluster-data-refs.md, process_inputs
 * table): two base addresses each read/written at offsets
 * {0x01(u8),0x04(s32),0x08(s32),0x0C(s32),0x10(f32),0x14(f32)}, so this
 * struct models the surveyed prefix and the observed byte at +0x4D.
 * NOT currently symbolized (addiu gap): input_rec0/input_rec1 are formed
 * via lui+addiu in process_inputs.s, so this type does not yet reach the
 * derived seed.
 */
typedef struct {
    u8 pad00[1];   /* 0x00 */
    u8 active;     /* 0x01 - byte R */
    u8 pad02[2];   /* 0x02 */
    s32 unk04;     /* 0x04 - word W */
    s32 unk08;     /* 0x08 - word W */
    s32 unk0C;     /* 0x0C - word W */
    f32 unk10;     /* 0x10 - float W */
    f32 unk14;     /* 0x14 - float W */
    u8 _pad18[0x4D - 0x18];
    u8 unk4D;      /* 0x4D - byte R; research/cluster-data-refs.md */
} InputRecord;

extern InputRecord input_rec0;    /* 0x8014A118 */
extern InputRecord input_rec1;    /* 0x8014A164 */

/* --- process_inputs's per-player lookup tables (T019 residue close-out) ---
 * Not in research/cluster-data-refs.md's process_inputs table: these bases
 * are formed by a fixed lui+addiu (constant address) but then dereferenced
 * with a variable byte offset (sll+addu), which the survey pass's static
 * offset walk does not model. Evidence is a direct citation of the derived
 * assembly (third amendment, contracts/seed-derivation.md):
 * build/m2c_asm/process_inputs.s .L800C99A0-.L800C99B0 (D_80156978),
 * .L800C999C-.L800C99B4 (D_80156998), .L800C9998-.L800C99B8 (D_80143A00),
 * .L800C9994-.L800C99BC (D_80156958). Each is indexed by the active_a2/a1
 * player index (0..3): the first three via `sll $v0,$aN,2` (word stride 4,
 * one element per player), the last via `sll $tN,$aN,3` (8-byte stride,
 * two adjacent floats per player read by lwc1 at +0/+4).
 */
extern s32 D_80156978[4];
extern s32 D_80156998[4];
extern s32 D_80143A00[4];
typedef struct {
    f32 unk0;   /* 0x0 - float R, process_inputs var_v1->unk10/var_a0->unk10 */
    f32 unk4;   /* 0x4 - float R, process_inputs var_v1->unk14/var_a0->unk14 */
} D_80156958_Entry;
extern D_80156958_Entry D_80156958[4];

/* --- attract_or_transition's segment table (T019 residue close-out) -------
 * Not in research/cluster-data-refs.md (indexed via a fixed lui+addiu base
 * combined with a variable-shifted index, same class as the process_inputs
 * tables above -- the survey's static offset walk does not model it).
 * Evidence, direct derived-asm citation (third amendment):
 * build/m2c_asm/attract_or_transition.s .L800EDDD4-.L800EDDD8
 * (lui $t9,0x8015 / addiu $t9,$t9,27616 forms 0x80156BE0), indexed by
 * sll $t8,$t7,7 (D_8015F72D * 128, stride 0x80) then addu $t2,$t8,$t9.
 * Two fields are dereferenced off the indexed element: +0x58
 * (.L800EDDE4, lw $t6,88($t2)) is read and then itself used as a pointer
 * offset by +0x9CC0 (.L800EDE00/.L800EDE08, li $at,0x9cc0 / addu
 * $t8,$t6,$at) -- a second-level pointee, padded to that single observed
 * field per FR-004 (no wider claim about its contents); +0x7C
 * (.L800EE004, lw $a0,124($t8) with $t8 reloaded from the same base saved
 * at sp+0x48) is passed directly to osVirtualToPhysical(void *).
 * The +0x9CC0 field itself is immediately used the same way msgq_ptr is
 * used elsewhere in this function (assigned into msgq_ptr, then advanced
 * 8 bytes at a time writing word pairs) -- the same OSMesgQueue-shaped
 * 8-byte-stride idiom the msgq_ptr header note above already documents,
 * so it is typed OSMesgQueue here too rather than a generic word.
 */
typedef struct {
    u8 pad0000[0x9CC0];
    OSMesgQueue unk9CC0;   /* 0x9CC0 - written via msgq_ptr's own idiom */
} SegmentHeader;
typedef struct {
    u8 pad00[0x58];
    SegmentHeader *unk58;  /* 0x58 - pointer R */
    u8 pad5C[0x7C - 0x5C];
    void *unk7C;           /* 0x7C - pointer, arg to osVirtualToPhysical */
} SegmentTableEntry;
extern SegmentTableEntry D_80156BE0[];  /* 0x80156BE0, element stride 0x80,
                                         * element count not evidenced */

/* --- playgame_state_change's typed globals (T019 residue close-out) ------
 * research/cluster-data-refs.md already lists both addresses (word R) for
 * playgame_state_change; the plain-s32 type from the placeholder-coverage
 * block doesn't support the pointer chains m2c infers from their actual
 * call-argument/dereference use, so each gets its own typed declaration
 * instead (per stall report's "double-pointer pointee" and "local-member
 * chain" failure classes).
 *
 * D_8014A160 (0x8014A160): the seed's call site is
 * `func_800a3424((*D_8014A160)->unk8, 0, var_a2)` -- a double-pointer
 * pointee (contract's authorized class): D_8014A160 must be T** so
 * *D_8014A160 is T* and ->unk8 accesses T. func_800a3424's first parameter
 * is declared s32 below, so unk8 is typed s32 to match; nothing else about
 * T is evidenced.
 */
typedef struct {
    u8 pad0[8];
    s32 unk8;   /* 0x8 - playgame_state_change: passed as func_800a3424's
                 * first (s32) argument */
} D_8014A160_Target;
extern D_8014A160_Target **D_8014A160;  /* 0x8014A160 */

/* D_8012E6E0 (0x8012E6E0): a linked-list head consumed as
 * `object_render_cleanup((void **) var_s0); var_s0 = **var_s0;` -- each
 * node's first field is a pointer to the next node (self-referential),
 * matched here the same way msgq_ptr's OSMesgQueue-shaped idiom is: a
 * named struct whose first member is `self *next` lets m2c's `**var_s0`
 * chain (dereference-then-follow) type-check without a cast.
 */
typedef struct D_8012E6E0_Node {
    struct D_8012E6E0_Node *next;  /* 0x0 - list-traversal pointer,
                                     * see object_render_cleanup's own
                                     * void ** parameter */
} D_8012E6E0_Node;
extern D_8012E6E0_Node *D_8012E6E0;  /* 0x8012E6E0 */

/* --- Partial player/car struct (countdown, 0x800FBF88) --------------------
 * rushtherock CAR_DATA (modeldat.h:403-467) is cited as a shape reference,
 * not layout truth (data-model.md) -- the N64 build's field order/offsets
 * are its own. Only the offsets countdown actually dereferences off
 * player_array are declared (research/cluster-data-refs.md: base
 * 0x80152818, offsets {0x380,0x3A3}, indexed with a per-element stride of
 * 0x3B8 confirmed via the same function's `x3B8`-scaled address forms);
 * everything else is explicit padding per FR-004 (bounded scope).
 * NOT currently symbolized (addiu gap): player_array is formed via
 * lui+addiu wherever the cluster reaches it, so this type does not yet
 * reach the derived seed either.
 *
 * T019 residue close-out: two further offsets, evidenced directly from
 * build/m2c_asm/countdown.s (`&player_array[var_s0_5]` then `lw/sw
 * 232($v0)` and `lb 859($v0)`, both within the previously-blanket
 * pad000[0x380] region so must be carved out by name): +0xE8 (word,
 * read-modify-write with a `& ~8` bitmask -- a flags word) and +0x35B
 * (byte, read directly into cpak_read's s8 parameter).
 */
typedef struct {
    u8 pad000[0xE8];
    s32 unkE8;              /* 0xE8 - word R/W, `& ~8` flag clear */
    u8 pad0EC[0x35B - 0xEC];
    u8 unk35B;               /* 0x35B - byte R, cpak_read's s8 argument */
    u8 pad35C[0x380 - 0x35C];
    u8 unk380;             /* 0x380 */
    u8 pad381[0x3A3 - 0x381];
    u8 unk3A3;              /* 0x3A3 */
    u8 pad3A4[0x3B8 - 0x3A4];  /* stride to next element, per-cluster evidence only */
} GameCar;

extern GameCar player_array[8];   /* 0x80152818 - element count not evidenced, N64 max players */

/* --- T019 symbolization-gap additions ------------------------------------
 * Each declaration is bounded to the access pattern recorded in
 * research/cluster-data-refs.md; padding represents observed offsets, not
 * a claim that the complete game structure has been recovered.
 */
typedef struct {
    u8 pad00[0x39];
    u8 unk39;
    u8 unk3A;
    u8 unk3B;
} PlaygameSettings;
extern PlaygameSettings playgame_settings; /* 0x80114658 - playgame_state_change:
                                            * bytes +0x39/+0x3A/+0x3B */

typedef struct {
    u8 pad000[0x1F0];
    s32 unk1F0;
    s32 unk1F4;
    s32 unk1F8;
    s32 unk1FC;
    s32 unk200;
} CountdownObject;
typedef struct {
    u8 pad000[0x19C];
    void *unk19C;
    u8 pad1A0[0x200 - 0x1A0];
    void *unk200;
    void *unk204;
    void *unk208;
} CountdownDetail;
typedef struct {
    void *unk0;
    CountdownDetail *unk4;
    void *unk8;
    void *unkC;
    void **unk10;
} CountdownState;
extern CountdownState countdown_state; /* 0x8017A4E0 - countdown_handler:
                                        * pointer fields +0x04/+0x0C/+0x10 */
extern CountdownObject *countdown_object; /* 0x8017A4E4 - countdown_handler:
                                          * pointee +0x1F0..+0x200 */

typedef struct PadConfig_s {
    struct PadConfig_s *unk0;  /* 0x0 - pointer R; dereferenced at ->unk14
                                * elsewhere in Input_ProcessGameplayPad
                                * (func_8008a148's first arg), matching this
                                * struct's own unk14 -- self-referential
                                * (research/cluster-data-refs.md already
                                * records 0x80140BF0/+0x0 as a pointer with
                                * sub-offsets {0x14,0x1C}, a subset of this
                                * struct's own fields) */
    struct PadConfig_s *unk4;  /* 0x4 - pointer R; dereferenced at ->unk14,
                                * ->unk10, ->unk15 (T019 residue: local
                                * `temp_v1 = var_s8->unk4` chain) --
                                * cluster-data-refs.md records 0x80140BF4/+0x4
                                * as a pointer with sub-offsets
                                * {0x10,0x12,0x14,0x15,0x16,0x18}, again a
                                * subset of this struct's own fields */
    s16 unk8;
    s16 unkA;
    s16 unkC;
    s16 unkE;
    s16 unk10;
    s16 unk12;
    u8 unk14;
    u8 unk15;
    u8 unk16;
    u8 pad17;
    s16 unk18;
    s16 unk1A;
    s16 unk1C;
    s16 unk1E;
} PadConfig;
extern PadConfig pad_config;      /* 0x80140BF0 - Input_ProcessGameplayPad:
                                   * config base fields +0x00..+0x1E */

/* --- Input_ProcessGameplayPad's second config table (T019 residue) --------
 * Not in research/cluster-data-refs.md (same lui+addiu-base / variable-index
 * class the other third-amendment entries above cover). Evidence:
 * build/m2c_asm/Input_ProcessGameplayPad.s .L800A0CAC-.L800A0CB0
 * (lui $t9,0x8014 / addiu $t9,$t9,-31120 forms 0x80138670), indexed by
 * sra $t7,$t6,0xa / sll $t8,$t7,3 (stride 8) then addu $v1,$t8,$t9.
 * +0x0 (.L800A0CD0, lw $v0,0($v1)) is read and immediately dereferenced at
 * +0x14 (.L800A0CDC, lw $a0,20($v0)) and passed to func_8008a148's first
 * (s32) argument -- the same role PadConfig's own unk14 plays at every
 * other func_8008a148 call site in this function, so the pointee is typed
 * PadConfig* here too (no wider claim). +0x4 (.L800A0CC0, lw $t6,4($v1))
 * is only ever compared against zero.
 */
typedef struct {
    PadConfig *unk0;  /* 0x0 - pointer R, see above */
    s32 unk4;          /* 0x4 - word, boolean-checked */
} D_80138670_Entry;
extern D_80138670_Entry D_80138670[];  /* 0x80138670, element stride 8,
                                        * element count not evidenced */

extern s32 game_loop_tick;        /* 0x8002EB64 - game_loop: word R,
                                   * base 0x8002E8E8 + 0x27C */
extern s16 active_player_count;   /* 0x8014A108 - process_inputs/countdown/
                                   * playgame_state_change: halfword R/W */
extern s32 gameplay_mode;         /* 0x8014A110 - countdown and
                                   * playgame_state_change: word R */

/* --- Complete cluster-survey placeholder coverage -----------------------
 * Every declaration in this block comes directly from the per-function
 * tables in research/cluster-data-refs.md (second 2026-07-17 gate).  Names
 * intentionally follow D_<ADDR>; widths are exactly the surveyed widths.
 */
extern f32 D_8002AFB4, D_8002AFB8;
extern s32 D_8002AFC0, D_8002AFC4, D_8002EBB0;
extern u16 D_8002EB70;
extern u8 D_80035470, D_80035471, D_80035472;
extern s32 D_80111958;
extern u8 D_80114650, D_80114654, D_801146F0;
extern s32 D_801146F8, D_801170FC;
extern u8 D_80117350, D_80117354;
extern s32 D_801174BC;
extern u8 D_8011ED0B;
extern u16 D_8011ED0C[];  /* Input_ProcessGameplayPad.s .L800A0A74/.L800A0A88:
                             lui+addu runtime index, lhu -4852($s3) —
                             u16 array adjacent to D_8011ED0B */
extern f32 D_80123FB4, D_80123FB8, D_80123FBC, D_801242A8;
extern u8 D_80124F84;
extern s32 D_80124FC8;
extern u8 D_8012E67C, D_8013FECB;
extern s32 D_80140008;
extern u16 D_80140618;
extern s32 D_801406B8, D_801407BC, D_80140804, D_80140A00;
extern s32 D_80140AD8, D_80140B08, D_80140BD8;
extern u8 D_80140C26;
extern s32 D_80140D70, D_80141428, D_80142510;
extern u8 D_80142690, D_80142699, D_80142760;
extern s32 D_80143F10;
extern f32 D_8014401C;
extern u8 D_801461F8, D_80146204, D_80146205, D_80149414;
extern s32 D_80149438;
extern u8 D_80149774, D_80149794, D_801497C4;
extern s32 D_801497F4, D_80149D98;
extern u8 D_8014B240, D_80150EFC, D_80150F14;
extern s32 D_80150000;
extern u16 D_80151AD0;
extern u8 D_80151AD8, D_8015256C, D_80152744, D_80152F29;
extern s32 D_8015204C, D_801520C4, D_80153308;
extern f32 D_801525F4, D_801543CC;
extern u16 D_80152734;
extern s32 D_8015698C;
extern u8 D_80156994, D_80156CF0, D_80157244, D_8015F72D;
extern s32 D_8015B250, D_8015B260, D_8015F738;
extern s32 D_80161380, D_80161398, D_801613A4, D_801613AC;
extern s32 D_801613B0, D_80161434, D_8017A4B0, D_8017A508;
extern s32 D_8017A638;

/* Multi-offset bases from research/cluster-data-refs.md.  Only surveyed
 * offsets are named; every gap is explicit padding. */
typedef struct {
    u16 unk00;
    u16 unk02;
    u8 pad04[0x60 - 0x04];
    s32 unk60;
    s32 unk64;
} D_80143FD8_Record;
extern D_80143FD8_Record *D_80143FD8; /* 0x80143FD8, offsets 0,2,0x60,0x64 */

typedef struct {
    u8 pad000[0x7C6];
    u16 unk7C6;
    u8 pad7C8[0x7E8 - 0x7C8];
    u8 unk7E8;
} D_8014A250_Record;
extern D_8014A250_Record D_8014A250; /* 0x8014A250, offsets 0x7C6,0x7E8 */

typedef struct {
    u8 pad00[0x0C];
    u8 unk0C;
    u8 unk0D;
    u8 unk0E;
} D_80146108_Record;
extern D_80146108_Record D_80146108; /* 0x80146108, bytes +0x0C..+0x0E */

/* --- Track_Data (countdown, playgame_state_change) -------------------------
 * rushtherock: game/checkpoint.h:101-109, verbatim (platform-neutral, no
 * N64 divergence noted).
 */
typedef struct {
    F32 start_time[8];
    F32 end_time[8];
    S16 loop_chkpnt;
    S16 finish_line;
    S16 before_finish;
    S16 number_of_laps;
} Track_Data;

/* --- sound_control's own clear-record parameter (0x800B37E8) --------------
 * research/cluster-data-refs.md: "Argument a2 is a clear record pointer
 * with offsets {0x00,0x04,0x06,0x08,0x0A,0x0C,0x0E,0x10,0x12,0x14,0x18,
 * 0x1C,0x20} (word and halfword fields)". This is the function's own
 * parameter (not a global reached through lui/addiu), so declaring
 * sound_control's real signature here is sufficient for m2c to type it --
 * confirmed fixable, no symbolizer dependency. Field widths are read
 * directly off the gaps between the evidenced offsets; 0x1C is used as a
 * callback (`temp_t7(temp_v0)` in the m2c-generated body) so it is typed a
 * function pointer, and var_s0 += 0x24 in the same body confirms the
 * struct's 0x24-byte stride.
 */
typedef struct SoundClearRecord {
    s32 unk0;                     /* 0x00 */
    s16 unk4;                     /* 0x04 */
    s16 unk6;                     /* 0x06 */
    s16 unk8;                     /* 0x08 */
    s16 unkA;                     /* 0x0A */
    s16 unkC;                     /* 0x0C */
    s16 unkE;                     /* 0x0E */
    s16 unk10;                    /* 0x10 */
    s16 unk12;                    /* 0x12 */
    s32 unk14;                    /* 0x14 */
    s32 unk18;                    /* 0x18 */
    s32 (*unk1C)(void *);         /* 0x1C - called directly in sound_control */
    s32 unk20;                    /* 0x20 */
} SoundClearRecord;

/* Partial record returned by func_800b3704. Field widths and offsets are
 * observed via m2c offsets in the sound_control seed. */
typedef struct SoundState {
    u8 _pad0[0x4];
    struct SoundState *unk4;      /* 0x04 - pointer W */
    s32 (*unk8)(void *);          /* 0x08 - callback W */
    u8 _padC[0x12 - 0xC];
    s16 unk12;                    /* 0x12 - halfword W */
    s16 unk14;                    /* 0x14 - halfword W */
    s16 unk16;                    /* 0x16 - halfword W */
    s8 unk18;                     /* 0x18 - byte W */
    u8 _pad19[0x1C - 0x19];
    s16 unk1C;                    /* 0x1C - halfword W */
    s16 unk1E;                    /* 0x1E - halfword W */
    s16 unk20;                    /* 0x20 - halfword W */
    s16 unk22;                    /* 0x22 - halfword W */
    u8 _pad24[0x28 - 0x24];
    s32 (*unk28)(void *);         /* 0x28 - callback W */
    s32 unk2C;                    /* 0x2C - word W */
    u8 _pad30[0x3C - 0x30];
    struct SoundState *unk3C;     /* 0x3C - pointer W */
} SoundState;

extern SoundState *func_800b3704(s32, s32, s32, s32); /* observed via m2c
                                                        * offset in sound_control seed */
extern SoundState *sound_control(s16 arg0, s16 arg1, SoundClearRecord *arg2, s16 arg3);

/* --- Cluster-internal call graph (game_loop.txt's own 10 targets calling
 * each other) -- every target needs the others' prototypes so callers
 * compile. All void/no-args except where the caller's own inferred call
 * site shows an argument (game_loop calls Input_ProcessGameplayPad(0)).
 */
extern void game_loop(void);
extern void game_mode_handler(void);
extern void attract_or_transition(void);
extern void process_inputs(void);
extern void playgame_state_change(void);
extern void RaceStateMachine_Update(void);
extern void countdown(void);
extern void countdown_handler(void);
extern void Input_ProcessGameplayPad(s32 pad);

/* --- game_loop's other callees (research/cluster-data-refs.md's Calls list
 * for game_loop; m2c could not infer these return types on its own --
 * literal `?` return types are a hard parse error, see t011-blockers.md).
 */
extern void Effects_UpdateEmitters(void);
extern void PhysicsObjectList_Update(void);
extern void UpdateActiveObjects(void);
extern void input_aux_handler(void);
extern void sound_stop(s32 sound_id);   /* also called with a pointer-typed
                                          * local in sound_control; s32
                                          * matches the concrete (non-m2c-
                                          * guessed) call site in game_loop */

/* --- game_mode_handler's callees --- */
extern void input_init_flag_get(void);
extern void viUpdateTime(void);

/* --- attract_or_transition's callees --- */
extern void sound_init(void);
extern s32 wheel_render_full(s32, s32, s32, s32);
extern void world_trigger_check(void);

/* --- process_inputs's callee --- */
extern void controller_poll(void);

/* --- sound_control's other callees --- */
extern void Input_ApplyPadConfig(void *);

/* --- playgame_state_change's callees --- */
extern void InitMaxPath(void);           /* real signature takes s32 record
                                           * (game/maxpath.c); this call site
                                           * passes none -- matches the m2c-
                                           * inferred arity, not the arcade
                                           * one (see header note) */
extern void audio_frame_sync(s32, s32, s32, s32, s32);
extern void display_enable(s32);
extern void func_800a3424(s32, s32, s32);
extern void func_800a7480(s32, s32, u8, u8, s32, s32, s32);
extern void func_800c813c(s32, s32);      /* also called from countdown */
extern void func_800c885c(void);
extern void func_800c9480(void);
extern void hud_setup(s32, s32, s32, s32, s32, f32, f32, s32);
extern void hud_speed_display(s32, s32, s32, s32, s32);
extern void init_state_begin(void);       /* also called from countdown */
extern void object_create(s32);
extern void object_render_cleanup(void **);
extern void player_cleanup_slots(void);
extern void player_mode_set(s32, s32);
extern void player_state_set(s32, s32);
extern void resource_slots_clear_multiple(void);
extern void scene_cleanup_slots(void);
extern void speed_set(void);              /* real signature takes s32 speed
                                            * (src/game/game.c); every call
                                            * site the cluster generates is
                                            * 0-arg -- see header note */
extern void state_change_preprocess(void);
extern void sync_entry_register(s32, s32);
extern void tire_compound_set(void);
extern void visual_objects_update(s32);

/* --- countdown's callees --- */
extern void billboard_render(void);
extern void camera_race_setup(void);
extern void cpak_read(s8);
extern s32 display_list_flush(s32, s32);
extern void entity_audio_update(s32);
extern void finish_state_alt(void);
extern void func_800ab18c(s32, s32);
extern void func_800b61a8(s32, s32, s32, s32);
extern void func_800d5374(void);
extern void func_800d5828(s16);
extern void func_800d6160(void);
extern void func_800e762c(s32);
extern void func_800f7f3c(void);
extern void func_800fbe30(void);
extern void func_800fbe60(void);
extern void func_800fbf2c(void);
extern void ghost_race_setup(void);
extern void init_state_continue(void);
extern void players_frame_update(void);
extern void race_init_helper(void);
extern void race_setup_1(void);
extern void race_setup_2(s16);
extern void records_screen(void);
extern void render_viewport_init(void);
extern void *memset(void *s, s32 c, u32 n);  /* matches the convention already
                                               * used across src/game/*.c; the
                                               * one call site in countdown
                                               * passes a 4th argument (an
                                               * m2c call-arg-detection
                                               * artifact -- see header note),
                                               * which stays broken here */
extern void viScheduleTick(f32);          /* two call sites in countdown and
                                            * one in playgame_state_change
                                            * show 0-arg calls to the same
                                            * symbol -- an m2c call-arg-
                                            * detection artifact unrelated to
                                            * this declaration; those specific
                                            * call sites stay broken (see
                                            * header note) */

/* --- countdown_handler's callees --- */
extern void dispatch_handler(s32);
extern void func_800a4770(void *, s32);
extern s32 object_manager_update(void *, s32);
extern void slot_state_setup(void);
extern void state_utility(s16, s32, void *);
extern void sprintf(s8 *buf, s8 *fmt, ...);  /* matches src/game/game.c's
                                               * existing declaration for the
                                               * libc sprintf wrapper at
                                               * 0x80004990 */

/* --- Input_ProcessGameplayPad's callees --- */
extern void func_8008705c(s32, u8, s32);
extern void func_80087110(s32, s16, s32, s32, s32, s32);
extern void func_800878e0(s32, u8, s32);
extern void func_8008a148(s32, u8, u8, s32);
extern void func_8008a38c(u8);
extern void func_8008a3e4(s16, s16, s32, s32);
extern void func_8008a46c(s16, s16, s32, s32, void *);
extern void func_8008a644(u16);
extern void func_8009f058(s32);
extern void object_render(s32, u8, u8, u16, s32, s32, s32, s32, s32, s32, s32);

#endif
