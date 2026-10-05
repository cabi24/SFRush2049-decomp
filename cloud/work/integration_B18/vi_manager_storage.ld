/* Original SDK VI module owns this complete real storage block. */
SECTIONS
{
    .vi_manager_bss 0x800354F0 (NOLOAD) : SUBALIGN(16)
    {
        __vi_manager_bss_start = .;
        build/us/src/rom/lib_7630.o(.bss.vi_manager);
        __vi_manager_bss_end = .;
    }
    ASSERT(SIZEOF(.vi_manager_bss) == 0x1220, "VI original BSS size changed")
    ASSERT(gViMgrThreadArg == ADDR(.vi_manager_bss), "VI thread alias moved")
    ASSERT(gViMgrMesgQueue == ADDR(.vi_manager_bss) + 0x11B0, "VI queue alias moved")
    ASSERT(gViMgrMesgBuffer == ADDR(.vi_manager_bss) + 0x11C8, "VI buffer alias moved")
    ASSERT(gViMgrEventDP == ADDR(.vi_manager_bss) + 0x11E0, "VI first event alias moved")
    ASSERT(gViMgrEventSP == ADDR(.vi_manager_bss) + 0x11F8, "VI second event alias moved")
    ASSERT(gViMgrRetraceCounter == ADDR(.vi_manager_bss) + 0x1210, "VI retrace alias moved")
}
INSERT AFTER .data;
