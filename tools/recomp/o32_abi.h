#ifndef RUSH_RECOMP_O32_ABI_H
#define RUSH_RECOMP_O32_ABI_H

/* ROM-free helpers for gen_wrapper.py. Runtime context stays in recomp.h. */
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <limits>
#include "recomp.h"
#include "rdram.h"

static_assert(sizeof(gpr) == 8, "N64Recomp uses 64-bit GPR storage");
static_assert(sizeof(float) == 4 && std::numeric_limits<float>::is_iec559,
              "o32 wrappers require IEEE binary32");
static_assert(sizeof(double) == 8 && std::numeric_limits<double>::is_iec559,
              "o32 wrappers require IEEE binary64");

/* memcpy preserves bits without union punning or out-of-range signed casts. */
template<class To, class From>
static inline To rush_o32_bits(From from) {
    static_assert(sizeof(To) == sizeof(From), "bit width mismatch");
    To to;
    std::memcpy(&to, &from, sizeof(to));
    return to;
}

static inline gpr rush_o32_extend(uint32_t word) {
    return word & UINT32_C(0x80000000)
        ? UINT64_C(0xffffffff00000000) | word : word;
}

/* The wrapper ABI has no error return. Invalid stack access fails before the
 * host target is called. Accept only zero/sign-extended 32-bit KSEG0 addresses;
 * the accessor validates the KSEG0 range and complete word span. */
static inline uint32_t rush_o32_stack_word(const rush_rdram* memory,
                                         gpr sp, uint32_t slot) {
    rush_addr address32;
    size_t ignored;
    if (!rush_addr_from_gpr(sp, &address32) ||
        (address32 & 7) != 0 || slot < 4 ||
        !rush_rdram_span(memory, address32, 0, &ignored)) {
        std::abort();
    }
    const uint64_t address = static_cast<uint64_t>(address32) +
                             static_cast<uint64_t>(slot) * 4;
    uint32_t word;
    if (address > UINT32_MAX ||
        !rush_rdram_read_u32(memory, static_cast<rush_addr>(address), &word)) {
        std::abort();
    }
    return word;
}

static inline rush_addr rush_o32_pointer(gpr value) {
    rush_addr address;
    if (!rush_addr_from_gpr(value, &address)) std::abort();
    return address;
}

#endif
