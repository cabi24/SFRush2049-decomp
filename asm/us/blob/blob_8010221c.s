/* region blob_8010221c: 0x8010221C-0x80104B14 */
.set noreorder
.set noat

.section .blobdata.op_8010221c, "ax", @progbits
    .incbin "build/game_code.bin", 505804, 10300

.section .text.highscore_entry_anim, "ax", @progbits
.globl highscore_entry_anim
highscore_entry_anim:
    .word 0x0C02DD28
    .word 0x00000000
    .word 0x3C108011
    .word 0x261049A8
    .word 0x02002025
    .word 0x0C02CFE9
    .word 0x2405FFFF
    .word 0x0002C842
    .word 0x02797823
    .word 0x000F2400
    .word 0x001E2C00
    .word 0x0005CC03
    .word 0x0004C403
    .word 0x03002025
    .word 0x03202825
    .word 0x0C02DC75
    .word 0x02003025
    .word 0x8FAE0018
    .word 0x2401FFFF
    .word 0x26730024
    .word 0x85CF0000
    .word 0x24040010
    .word 0x15E10003
    .word 0x00000000
    .word 0x10000001
    .word 0x24040016
    .word 0x0C02DD28
    .word 0x00000000
    .word 0x3C108011
    .word 0x261049AC
    .word 0x02002025
    .word 0x0C02CFE9
    .word 0x2405FFFF
    .word 0x00027042
    .word 0x026EC023
    .word 0x00182400
    .word 0x001E2C00
    .word 0x00057403
    .word 0x0004CC03
    .word 0x03202025
    .word 0x01C02825
    .word 0x0C02DC75
    .word 0x02003025
    .word 0x8FBF0014
    .word 0x27BD0088
    .word 0x03E00008
    .word 0x00000000

