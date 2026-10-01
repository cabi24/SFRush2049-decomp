/* Generated from rom_owned_data.json storage_blocks; do not hand edit. */
SECTIONS
{
    .vi_manager_bss 0x800354f0 (NOLOAD) : SUBALIGN(16)
    {
        __vi_manager_bss_start = .;
        build/us/src/rom/lib_7630.o(.bss.vi_manager);
        __vi_manager_bss_end = .;
    }
    ASSERT(SIZEOF(.vi_manager_bss) == 0x1220, "owned storage size changed: vi_manager")
    ASSERT(gViMgrThreadArg == ADDR(.vi_manager_bss) + 0x0, "owned storage alias moved: gViMgrThreadArg")
    ASSERT(gViMgrMesgQueue == ADDR(.vi_manager_bss) + 0x11b0, "owned storage alias moved: gViMgrMesgQueue")
    ASSERT(gViMgrMesgBuffer == ADDR(.vi_manager_bss) + 0x11c8, "owned storage alias moved: gViMgrMesgBuffer")
    ASSERT(gViMgrEventDP == ADDR(.vi_manager_bss) + 0x11e0, "owned storage alias moved: gViMgrEventDP")
    ASSERT(gViMgrEventSP == ADDR(.vi_manager_bss) + 0x11f8, "owned storage alias moved: gViMgrEventSP")
    ASSERT(gViMgrRetraceCounter == ADDR(.vi_manager_bss) + 0x1210, "owned storage alias moved: gViMgrRetraceCounter")
}
INSERT AFTER .data;
