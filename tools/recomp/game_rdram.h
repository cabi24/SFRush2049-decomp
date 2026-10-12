/* Explicit N64 field views corresponding to include/game_types.h.
 * Do NOT include that header and use native offsetof for pointer-bearing
 * structs: a host pointer is not an N64 address and changes later offsets.
 * These views never copy padding or convert an address token to a pointer.
 * The fixture suite checks every entry against the source header's o32 layout.
 */
#ifndef RUSH_RECOMP_GAME_RDRAM_H
#define RUSH_RECOMP_GAME_RDRAM_H
#include "rdram.h"

/* X(struct, member, guest byte offset, accessor suffix, host value type) */
#define RUSH_GAME_RDRAM_FIELDS(X) \
 X(InputRecord, active, 0x01, u8, uint8_t) \
 X(InputRecord, unk04, 0x04, s32, int32_t) \
 X(InputRecord, unk08, 0x08, s32, int32_t) \
 X(InputRecord, unk0C, 0x0c, s32, int32_t) \
 X(InputRecord, unk10, 0x10, f32, float) \
 X(InputRecord, unk14, 0x14, f32, float) \
 X(InputRecord, unk4D, 0x4d, u8, uint8_t) \
 X(D_80156958_Entry, unk0, 0x00, f32, float) \
 X(D_80156958_Entry, unk4, 0x04, f32, float) \
 X(SegmentTableEntry, unk58, 0x58, addr, rush_addr) \
 X(SegmentTableEntry, unk7C, 0x7c, addr, rush_addr) \
 X(D_8014A160_Target, unk8, 0x08, s32, int32_t) \
 X(D_8012E6E0_Node, next, 0x00, addr, rush_addr) \
 X(GameCar, unkE8, 0x0e8, s32, int32_t) \
 X(GameCar, unk35B, 0x35b, u8, uint8_t) \
 X(GameCar, unk380, 0x380, u8, uint8_t) \
 X(GameCar, unk3A3, 0x3a3, u8, uint8_t) \
 X(PlaygameSettings, unk39, 0x39, u8, uint8_t) \
 X(PlaygameSettings, unk3A, 0x3a, u8, uint8_t) \
 X(PlaygameSettings, unk3B, 0x3b, u8, uint8_t) \
 X(CountdownObject, unk1F0, 0x1f0, s32, int32_t) \
 X(CountdownObject, unk1F4, 0x1f4, s32, int32_t) \
 X(CountdownObject, unk1F8, 0x1f8, s32, int32_t) \
 X(CountdownObject, unk1FC, 0x1fc, s32, int32_t) \
 X(CountdownObject, unk200, 0x200, s32, int32_t) \
 X(CountdownDetail, unk19C, 0x19c, addr, rush_addr) \
 X(CountdownDetail, unk200, 0x200, addr, rush_addr) \
 X(CountdownDetail, unk204, 0x204, addr, rush_addr) \
 X(CountdownDetail, unk208, 0x208, addr, rush_addr) \
 X(CountdownState, unk0, 0x00, addr, rush_addr) \
 X(CountdownState, unk4, 0x04, addr, rush_addr) \
 X(CountdownState, unk8, 0x08, addr, rush_addr) \
 X(CountdownState, unkC, 0x0c, addr, rush_addr) \
 X(CountdownState, unk10, 0x10, addr, rush_addr) \
 X(PadConfig, unk0, 0x00, addr, rush_addr) \
 X(PadConfig, unk4, 0x04, addr, rush_addr) \
 X(PadConfig, unk8, 0x08, s16, int16_t) \
 X(PadConfig, unkA, 0x0a, s16, int16_t) \
 X(PadConfig, unkC, 0x0c, s16, int16_t) \
 X(PadConfig, unkE, 0x0e, s16, int16_t) \
 X(PadConfig, unk10, 0x10, s16, int16_t) \
 X(PadConfig, unk12, 0x12, s16, int16_t) \
 X(PadConfig, unk14, 0x14, u8, uint8_t) \
 X(PadConfig, unk15, 0x15, u8, uint8_t) \
 X(PadConfig, unk16, 0x16, u8, uint8_t) \
 X(PadConfig, unk18, 0x18, s16, int16_t) \
 X(PadConfig, unk1A, 0x1a, s16, int16_t) \
 X(PadConfig, unk1C, 0x1c, s16, int16_t) \
 X(PadConfig, unk1E, 0x1e, s16, int16_t) \
 X(D_80138670_Entry, unk0, 0x00, addr, rush_addr) \
 X(D_80138670_Entry, unk4, 0x04, s32, int32_t) \
 X(D_80143FD8_Record, unk00, 0x00, u16, uint16_t) \
 X(D_80143FD8_Record, unk02, 0x02, u16, uint16_t) \
 X(D_80143FD8_Record, unk60, 0x60, s32, int32_t) \
 X(D_80143FD8_Record, unk64, 0x64, s32, int32_t) \
 X(D_8014A250_Record, unk7C6, 0x7c6, u16, uint16_t) \
 X(D_8014A250_Record, unk7E8, 0x7e8, u8, uint8_t) \
 X(D_80146108_Record, unk0C, 0x0c, u8, uint8_t) \
 X(D_80146108_Record, unk0D, 0x0d, u8, uint8_t) \
 X(D_80146108_Record, unk0E, 0x0e, u8, uint8_t) \
 X(Track_Data, loop_chkpnt, 0x40, s16, int16_t) \
 X(Track_Data, finish_line, 0x42, s16, int16_t) \
 X(Track_Data, before_finish, 0x44, s16, int16_t) \
 X(Track_Data, number_of_laps, 0x46, s16, int16_t) \
 X(SoundClearRecord, unk0, 0x00, s32, int32_t) \
 X(SoundClearRecord, unk4, 0x04, s16, int16_t) \
 X(SoundClearRecord, unk6, 0x06, s16, int16_t) \
 X(SoundClearRecord, unk8, 0x08, s16, int16_t) \
 X(SoundClearRecord, unkA, 0x0a, s16, int16_t) \
 X(SoundClearRecord, unkC, 0x0c, s16, int16_t) \
 X(SoundClearRecord, unkE, 0x0e, s16, int16_t) \
 X(SoundClearRecord, unk10, 0x10, s16, int16_t) \
 X(SoundClearRecord, unk12, 0x12, s16, int16_t) \
 X(SoundClearRecord, unk14, 0x14, s32, int32_t) \
 X(SoundClearRecord, unk18, 0x18, s32, int32_t) \
 X(SoundClearRecord, unk1C, 0x1c, addr, rush_addr) \
 X(SoundClearRecord, unk20, 0x20, s32, int32_t) \
 X(SoundState, unk4, 0x04, addr, rush_addr) \
 X(SoundState, unk8, 0x08, addr, rush_addr) \
 X(SoundState, unk12, 0x12, s16, int16_t) \
 X(SoundState, unk14, 0x14, s16, int16_t) \
 X(SoundState, unk16, 0x16, s16, int16_t) \
 X(SoundState, unk18, 0x18, s8, int8_t) \
 X(SoundState, unk1C, 0x1c, s16, int16_t) \
 X(SoundState, unk1E, 0x1e, s16, int16_t) \
 X(SoundState, unk20, 0x20, s16, int16_t) \
 X(SoundState, unk22, 0x22, s16, int16_t) \
 X(SoundState, unk28, 0x28, addr, rush_addr) \
 X(SoundState, unk2C, 0x2c, s32, int32_t) \
 X(SoundState, unk3C, 0x3c, addr, rush_addr)

#define RUSH_GAME_RDRAM_ARRAYS(X) \
 X(Track_Data, start_time, 0x00, 8, f32, float) \
 X(Track_Data, end_time, 0x20, 8, f32, float)

#define RUSH_GAME_FIELD(type, field, offset, kind, value_type) \
enum { rush_##type##_##field##_offset = offset }; \
static inline bool rush_##type##_read_##field(rush_rdram_view view, value_type *out) { \
    rush_rdram_view field_view; \
    return rush_rdram_subview(view, offset, sizeof(value_type), &field_view) && \
           rush_rdram_read_##kind(field_view.memory, field_view.address, out); \
} \
static inline bool rush_##type##_write_##field(rush_rdram_view view, value_type value) { \
    rush_rdram_view field_view; \
    return rush_rdram_subview(view, offset, sizeof(value_type), &field_view) && \
           rush_rdram_write_##kind(field_view.memory, field_view.address, value); \
}
RUSH_GAME_RDRAM_FIELDS(RUSH_GAME_FIELD)
#undef RUSH_GAME_FIELD

#define RUSH_GAME_ARRAY(type, field, offset, count, kind, value_type) \
enum { rush_##type##_##field##_offset = offset, rush_##type##_##field##_count = count }; \
static inline bool rush_##type##_read_##field(rush_rdram_view view, size_t index, value_type *out) { \
    rush_rdram_view field_view; \
    return index < count && rush_rdram_subview(view, offset + index * sizeof(value_type), sizeof(value_type), &field_view) && \
           rush_rdram_read_##kind(field_view.memory, field_view.address, out); \
} \
static inline bool rush_##type##_write_##field(rush_rdram_view view, size_t index, value_type value) { \
    rush_rdram_view field_view; \
    return index < count && rush_rdram_subview(view, offset + index * sizeof(value_type), sizeof(value_type), &field_view) && \
           rush_rdram_write_##kind(field_view.memory, field_view.address, value); \
}
RUSH_GAME_RDRAM_ARRAYS(RUSH_GAME_ARRAY)
#undef RUSH_GAME_ARRAY

/* The one embedded struct in game_types.h: return its guest-memory view,
 * not an OSMesgQueue containing host pointers. Individual queue accesses
 * remain the caller's responsibility (libultra's pointer layout is separate).
 */
enum { rush_SegmentHeader_unk9CC0_offset = 0x9cc0 };
static inline bool rush_SegmentHeader_view_unk9CC0(rush_rdram_view view, rush_rdram_view *out) {
    return rush_rdram_subview(view, rush_SegmentHeader_unk9CC0_offset, 0x18, out);
}
#endif
