/* region blob_8010fd7c: 0x8010FD7C-0x801249F0 */
.set noreorder
.set noat

.section .blobdata.data_8010fd7c, "aw", @progbits
    .incbin "build/game_code.bin", 561964, 80628

.section .blobdata.rodata_80123870, "a", @progbits
    .incbin "build/game_code.bin", 642592, 4480

