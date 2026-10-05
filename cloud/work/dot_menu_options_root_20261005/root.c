/* Complete control-flow reconstruction of func_8010AEAC, not native-ready.
 * N64 rewrite; no established arcade donor. See REPORT.md.
 * Header storage capacity requires original provenance.
 * A test fixture may instantiate this source for behavioral verification;
 * its declarations are not evidence for the native stack frame or ABI.
 */
#include "menu_context.h"
#ifndef MENU_OPTIONS_TEST_FIXTURE
#error Native compilation is blocked on header capacity
#endif
#ifndef MENU_OPTIONS_HEADER_CAPACITY
#error A test must explicitly supply its own header storage capacity
#endif

s32 func_8010AEAC(void *state)
{
    s32 index;
    s32 y;
    s16 visible;
    char header[MENU_OPTIONS_HEADER_CAPACITY];
    void *label;

    (void)state; /* Genuine SoundClearRecord callback input, unused here. */

    if (D_80116D0C == 1) {
        func_8010A8D0();
        return 1;
    }
    render_helper(0.0f);
    osRecvMesg(D_801461D0, 0, 1);
    slot_state_setup(13);
    osJamMesg(D_801461D0, 0, 0);
    dispatch_handler(1);
    if ((D_801174B4 & 0x007C0000) && D_80116DA8) {
        fcvt_wrapper(header, D_80121018, D_8015698C + 1,
            D_8017A4E0.labels[233]);
        state_utility(160 - (object_manager_update(header, -1) >> 1),
            10, header);
    } else {
        label = D_8017A4E0.labels[50];
        state_utility(160 - (object_manager_update(label, -1) >> 1),
            10, label);
    }
    y = object_bytes_sum_global() * 2 + 10;
    osRecvMesg(D_801461D0, 0, 1);
    slot_state_setup(11);
    osJamMesg(D_801461D0, 0, 0);
    visible = 0;
    for (index = 0; index < 14; index++) {
        if (!D_80116D14[index] || index < D_80149DA2 ||
            visible >= D_80149B84) {
            continue;
        }
        switch (index) {
        case 1:
            func_8010A7A4(1, 160, y, D_8017A4E0.labels[13],
                D_8017A4E0.values[D_80146108[10] + D_8017A4E0.header->string_offset]);
            break;
        case 2:
            func_8010A7A4(2, 160, y, D_8017A4E0.labels[15],
                D_8017A4E0.values[D_80146108[0] + D_8017A4E0.header->string_offset]);
            break;
        case 3:
            func_8010A7A4(3, 160, y, D_8017A4E0.labels[16],
                D_8017A4E0.values[D_80146108[1] + D_8017A4E0.header->string_offset]);
            break;
        case 4:
            func_8010A7A4(4, 160, y, D_8017A4E0.labels[17],
                D_8017A4E0.values[D_80146108[2] + D_8017A4E0.header->string_offset]);
            break;
        case 5:
            func_8010A7A4(5, 160, y, D_8017A4E0.labels[18],
                D_8017A4E0.values[D_80146108[3] + D_8017A4E0.header->string_offset]);
            break;
        case 6:
            func_8010A7A4(6, 160, y, D_8017A4E0.labels[19],
                D_8017A4E0.values[D_80146108[4] + D_8017A4E0.header->string_offset]);
            break;
        case 7:
            func_8010A7A4(7, 160, y, D_8017A4E0.labels[20],
                D_8017A4E0.values[D_80146108[5] + D_8017A4E0.header->string_offset]);
            break;
        case 8:
            func_8010A7A4(8, 160, y, D_8017A4E0.labels[21],
                D_8017A4E0.values[D_80146108[6] + D_8017A4E0.header->string_offset]);
            break;
        case 9:
            func_8010A7A4(9, 160, y, D_8017A4E0.labels[22],
                D_8017A4E0.values[D_80146108[7] + D_8017A4E0.header->string_offset]);
            break;
        case 10:
            func_8010A7A4(10, 160, y, D_8017A4E0.labels[23],
                D_8017A4E0.values[D_80146108[8] + D_8017A4E0.header->string_offset]);
            break;
        case 11:
            func_8010A7A4(11, 160, y, D_8017A4E0.labels[24],
                D_8017A4E0.values[D_80146108[9] + D_8017A4E0.header->string_offset]);
            break;
        case 12:
            func_8010A7A4(12, 160, y, D_8017A4E0.labels[26],
                D_8017A4E0.values[D_80146108[18] + D_8017A4E0.header->string_offset]);
            break;
        case 13:
            func_8010A7A4(13, 160, y, D_8017A4E0.labels[14],
                D_8017A4E0.values[D_80146108[11] + D_8017A4E0.header->string_offset]);
            break;
        case 0:
            func_8010A7A4(0, 160, y, D_8017A4E0.labels[25], D_801164D0[D_8013FEC8]);
            break;
        }
        y += object_bytes_sum_global();
        visible++;
    }
    func_800ED66C(-1.0f);
    func_800BEA3C(D_801146BC, D_801146C0);
    render_helper(-1.0f);
    return 1;
}
