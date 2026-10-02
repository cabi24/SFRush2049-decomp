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
    .timer_services_bss 0x80037c30 (NOLOAD) : SUBALIGN(16)
    {
        __timer_services_bss_start = .;
        build/us/src/rom/lib_cc50.o(.bss.timer_services);
        __timer_services_bss_end = .;
    }
    ASSERT(SIZEOF(.timer_services_bss) == 0x40, "owned storage size changed: timer_services")
    ASSERT(__osBaseTimer == ADDR(.timer_services_bss) + 0x0, "owned storage alias moved: __osBaseTimer")
    ASSERT(gViTimeAccumHi == ADDR(.timer_services_bss) + 0x20, "owned storage alias moved: gViTimeAccumHi")
    ASSERT(gViLastCount == ADDR(.timer_services_bss) + 0x28, "owned storage alias moved: gViLastCount")
    ASSERT(gViRetraceCount == ADDR(.timer_services_bss) + 0x2c, "owned storage alias moved: gViRetraceCount")
    ASSERT(__osTimerCounter == ADDR(.timer_services_bss) + 0x30, "owned storage alias moved: __osTimerCounter")
    .sdk_initialize_bss 0x800367d0 (NOLOAD) : SUBALIGN(16)
    {
        __sdk_initialize_bss_start = .;
        build/us/src/rom/lib_8a80.o(.bss.sdk_initialize);
        __sdk_initialize_bss_end = .;
    }
    ASSERT(SIZEOF(.sdk_initialize_bss) == 0x10, "owned storage size changed: sdk_initialize")
    ASSERT(gSpTaskState == ADDR(.sdk_initialize_bss) + 0x0, "owned storage alias moved: gSpTaskState")
}
INSERT AFTER .data;
