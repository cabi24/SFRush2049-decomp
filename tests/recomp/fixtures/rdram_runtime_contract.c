/* Optional compatibility probe using the real pinned upstream header.
 * All data is synthetic. No upstream header is vendored by this test.
 */
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include "recomp.h"
#include "tools/recomp/rdram.h"

int main(void) {
    /* Dynamic storage supplies alignment without a conflicting declared type
     * for the upstream MEM_* typed lvalues. */
    uint8_t *rdram = (uint8_t *)calloc(64, 1);
    rush_rdram memory = {rdram, 64};
    gpr base = UINT64_C(0xffffffff80000000);
    uint8_t byte; uint16_t half; uint32_t word; uint64_t dword;
    assert(rdram != NULL);
    MEM_W(0, base) = INT32_C(0x11223344);
    MEM_W(4, base) = INT32_C(0x55667788);
    assert(rush_rdram_read_u64(&memory, 0x80000000, &dword));
    assert(dword == UINT64_C(0x1122334455667788));
    assert(dword == LD(0, base));
    MEM_B(1, base) = -17;
    assert(rush_rdram_read_u8(&memory, 0x80000001, &byte) && byte == (uint8_t)-17);
    MEM_H(2, base) = -1234;
    assert(rush_rdram_read_u16(&memory, 0x80000002, &half) && half == (uint16_t)-1234);
    assert(rush_rdram_write_u8(&memory, 0x80000003, 0xde) && MEM_BU(3, base) == 0xde);
    assert(rush_rdram_write_u16(&memory, 0x80000006, 0xcdef) && MEM_HU(6, base) == 0xcdef);
    assert(rush_rdram_write_u32(&memory, 0x80000008, UINT32_C(0xfedcba98)));
    assert((uint32_t)MEM_W(8, base) == UINT32_C(0xfedcba98));
    SD(UINT64_C(0x89abcdef01234567), 16, base);
    assert(rush_rdram_read_u64(&memory, 0x80000010, &dword) && dword == UINT64_C(0x89abcdef01234567));
    assert(rush_rdram_write_u64(&memory, 0x80000018, UINT64_C(0x0123456789abcdef)));
    assert(LD(24, base) == UINT64_C(0x0123456789abcdef));
    assert(rush_rdram_read_u32(&memory, 0x80000018, &word) && word == UINT32_C(0x01234567));
    free(rdram);
    puts("Pinned N64Recomp MEM/LD/SD compatibility fixture passed");
    return 0;
}
