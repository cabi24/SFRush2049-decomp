/* ROM-free host replacement memory API. See README.md: RDRAM accessors.
 * A guest byte at KSEG0 address a occupies data[(a - 0x80000000) ^ 3].
 * This is the N64Recomp 81213c18 MEM_B/MEM_H/MEM_W/LD/SD layout, not a
 * byte-swapped host struct. Never cast a guest address to a native pointer.
 */
#ifndef RUSH_RECOMP_RDRAM_H
#define RUSH_RECOMP_RDRAM_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <limits.h>
#include <float.h>

#if defined(__cplusplus)
#define RUSH_RDRAM_ASSERT(c, m) static_assert(c, m)
#else
#define RUSH_RDRAM_ASSERT(c, m) _Static_assert(c, m)
#endif
RUSH_RDRAM_ASSERT(CHAR_BIT == 8, "RDRAM requires eight-bit bytes");
RUSH_RDRAM_ASSERT(sizeof(float) == 4 && FLT_RADIX == 2 && FLT_MANT_DIG == 24 &&
                  FLT_MAX_EXP == 128, "RDRAM requires binary32 float");
RUSH_RDRAM_ASSERT(sizeof(double) == 8 && DBL_MANT_DIG == 53 && DBL_MAX_EXP == 1024,
                  "RDRAM requires binary64 double");

/* Data and function pointers are guest address tokens, including null (0).
 * The token may be stored or compared without being a dereferenceable address.
 */
typedef uint32_t rush_addr;
typedef struct rush_rdram {
    uint8_t *data;
    size_t size; /* allocated bytes; whole words, at most one KSEG0 segment */
} rush_rdram;
typedef struct rush_rdram_view {
    const rush_rdram *memory;
    rush_addr address;
} rush_rdram_view;

/* Accept o32 zero/sign extension only; do not truncate a native pointer. */
static inline bool rush_addr_from_gpr(uint64_t value, rush_addr *out) {
    uint32_t low = (uint32_t)value;
    uint64_t upper = value >> 32;
    if (!out || !(upper == 0 || (upper == UINT32_MAX && low >= UINT32_C(0x80000000))))
        return false;
    *out = low;
    return true;
}

static inline bool rush_addr_add(rush_addr base, size_t offset, rush_addr *out) {
    if (!out || offset > UINT32_MAX - base) return false;
    *out = base + (uint32_t)offset;
    return true;
}

/* Only KSEG0 RDRAM is mapped. Null, physical, KSEG1, MMIO, TLB addresses and
 * accesses outside the supplied allocation fail instead of being masked.
 * All functions return false without changing outputs or RDRAM on failure.
 * Descriptor and output storage must be ordinary valid host objects.
 */
static inline bool rush_rdram_span(const rush_rdram *memory, rush_addr address,
                                   size_t width, size_t *offset) {
    size_t at;
    if (!memory || !memory->data || !offset || memory->size == 0 ||
        (memory->size & 3) != 0 || memory->size > UINT32_C(0x20000000) ||
        address < UINT32_C(0x80000000) || address >= UINT32_C(0xa0000000))
        return false;
    at = (size_t)(address - UINT32_C(0x80000000));
    if (at > memory->size || width > memory->size - at) return false;
    *offset = at;
    return true;
}

/* Integer accesses assemble logical big-endian bytes, including unaligned
 * spans crossing word boundaries. No alignment/aliasing or host-endian UB.
 * This is a data-view API, not an emulator of MIPS alignment exceptions.
 */
#define RUSH_RDRAM_UNSIGNED(bits, type) \
static inline bool rush_rdram_read_u##bits(const rush_rdram *memory, rush_addr address, type *out) { \
    size_t at, i; type value = 0; \
    if (!out || !rush_rdram_span(memory, address, sizeof(type), &at)) return false; \
    for (i = 0; i < sizeof(type); ++i) \
        value = (type)((value << 8) | memory->data[(at + i) ^ (size_t)3]); \
    *out = value; return true; \
} \
static inline bool rush_rdram_write_u##bits(const rush_rdram *memory, rush_addr address, type value) { \
    size_t at, i; \
    if (!rush_rdram_span(memory, address, sizeof(type), &at)) return false; \
    for (i = 0; i < sizeof(type); ++i) \
        memory->data[(at + i) ^ (size_t)3] = (uint8_t)(value >> ((sizeof(type) - i - 1) * 8)); \
    return true; \
}
RUSH_RDRAM_UNSIGNED(8, uint8_t)
RUSH_RDRAM_UNSIGNED(16, uint16_t)
RUSH_RDRAM_UNSIGNED(32, uint32_t)
RUSH_RDRAM_UNSIGNED(64, uint64_t)
#undef RUSH_RDRAM_UNSIGNED

#define RUSH_RDRAM_SIGNED(bits, stype, utype) \
static inline bool rush_rdram_read_s##bits(const rush_rdram *memory, rush_addr address, stype *out) { \
    utype raw; stype value; \
    if (!out || !rush_rdram_read_u##bits(memory, address, &raw)) return false; \
    memcpy(&value, &raw, sizeof(value)); *out = value; return true; \
} \
static inline bool rush_rdram_write_s##bits(const rush_rdram *memory, rush_addr address, stype value) { \
    return rush_rdram_write_u##bits(memory, address, (utype)value); \
}
RUSH_RDRAM_SIGNED(8, int8_t, uint8_t)
RUSH_RDRAM_SIGNED(16, int16_t, uint16_t)
RUSH_RDRAM_SIGNED(32, int32_t, uint32_t)
RUSH_RDRAM_SIGNED(64, int64_t, uint64_t)
#undef RUSH_RDRAM_SIGNED

#define RUSH_RDRAM_FLOAT(bits, type, utype) \
static inline bool rush_rdram_read_f##bits(const rush_rdram *memory, rush_addr address, type *out) { \
    utype raw; \
    if (!out || !rush_rdram_read_u##bits(memory, address, &raw)) return false; \
    memcpy(out, &raw, sizeof(raw)); return true; \
} \
static inline bool rush_rdram_write_f##bits(const rush_rdram *memory, rush_addr address, type value) { \
    utype raw; memcpy(&raw, &value, sizeof(raw)); \
    return rush_rdram_write_u##bits(memory, address, raw); \
}
RUSH_RDRAM_FLOAT(32, float, uint32_t)
RUSH_RDRAM_FLOAT(64, double, uint64_t)
#undef RUSH_RDRAM_FLOAT

static inline bool rush_rdram_read_addr(const rush_rdram *memory, rush_addr address, rush_addr *out) {
    return rush_rdram_read_u32(memory, address, out);
}
static inline bool rush_rdram_write_addr(const rush_rdram *memory, rush_addr address, rush_addr value) {
    return rush_rdram_write_u32(memory, address, value);
}

/* Derive a subview without wrapping the guest address. Checks the requested
 * span too; use zero span to obtain an address and a scalar accessor to read.
 */
static inline bool rush_rdram_subview(rush_rdram_view view, size_t offset,
                                      size_t width, rush_rdram_view *out) {
    rush_addr address;
    size_t ignored;
    if (!out || !rush_rdram_span(view.memory, view.address, 0, &ignored) ||
        !rush_addr_add(view.address, offset, &address) ||
        !rush_rdram_span(view.memory, address, width, &ignored)) return false;
    out->memory = view.memory;
    out->address = address;
    return true;
}

#endif
