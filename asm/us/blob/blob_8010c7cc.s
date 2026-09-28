/* region blob_8010c7cc: 0x8010C7CC-0x8010FD60 */
.set noreorder
.set noat

.section .blobdata.op_8010c7cc, "ax", @progbits
    .incbin "build/game_code.bin", 548220, 13288

.section .text.sync_acquire_menu, "ax", @progbits
.globl sync_acquire_menu
sync_acquire_menu:
    /* compiled from src/blob/sync_acquire_menu.c */
    .word 0x27BDFFE8
    .word 0xAFBF0014
    .word 0x3C048015
    .word 0x24842750
    .word 0x00002825
    .word 0x0C001C9C
    .word 0x24060001
    .word 0x8FBF0014
    .word 0x27BD0018
    .word 0x03E00008
    .word 0x00000000

.section .blobdata.op_8010fbe0, "ax", @progbits
    .incbin "build/game_code.bin", 561552, 20

.section .text.save_context_stub, "ax", @progbits
.globl save_context_stub
save_context_stub:
    .word 0x27BDFFE8
    .word 0x00802825
    .word 0xAC2E5288
    .word 0xAC20528C
    .word 0xAFBF0014
    .word 0x3C018015
    .word 0x240F0002
    .word 0x3C048015
    .word 0xAC2F5240
    .word 0x24845248
    .word 0x0C001F1A
    .word 0x24060040
    .word 0x3C048003
    .word 0x3C058015
    .word 0x24A55238
    .word 0x2484E960
    .word 0x0C001D78
    .word 0x24060001
    .word 0x3C048003
    .word 0x2484E928
    .word 0x2405029E
    .word 0x0C001D78
    .word 0x24060001
    .word 0x8FBF0014
    .word 0x27BD0018
    .word 0x03E00008
    .word 0x00000000

.section .blobdata.op_8010fc60, "ax", @progbits
    .incbin "build/game_code.bin", 561680, 32

.section .text.resource_alloc_init, "ax", @progbits
.globl resource_alloc_init
resource_alloc_init:
    /* compiled from src/blob/resource_alloc_init.c */
    .word 0x27BDFFE0
    .word 0xAFA50024
    .word 0x00802825
    .word 0xAFBF0014
    .word 0xAFA40020
    .word 0x0C025D1C
    .word 0x00002025
    .word 0xAFA2001C
    .word 0x00402025
    .word 0x0C0258B5
    .word 0x00002825
    .word 0x8FBF0014
    .word 0x8FA2001C
    .word 0x27BD0020
    .word 0x03E00008
    .word 0x00000000

.section .text.synced_model_render, "ax", @progbits
.globl synced_model_render
synced_model_render:
    .word 0x27BDFFE0
    .word 0xAFA40020
    .word 0xAFBF001C
    .word 0x3C048015
    .word 0xAFB10018
    .word 0xAFB00014
    .word 0x24842770
    .word 0x00002825
    .word 0x0C001C9C
    .word 0x24060001
    .word 0x8FA50020
    .word 0x0C0257F6
    .word 0x00003025
    .word 0x3C048015
    .word 0x24842770
    .word 0x00002825
    .word 0x0C001D78
    .word 0x00003025
    .word 0x8FBF001C
    .word 0x8FB00014
    .word 0x8FB10018
    .word 0x03E00008
    .word 0x27BD0020

.section .text.struct_init_and_call, "ax", @progbits
.globl struct_init_and_call
struct_init_and_call:
    .word 0x27BDFFE8
    .word 0x10A0000A
    .word 0xAFBF0014
    .word 0x3C028015
    .word 0x24423F10
    .word 0x240E0001
    .word 0xAC440008
    .word 0xA4450002
    .word 0xAC46000C
    .word 0xA4400004
    .word 0x0C022AF9
    .word 0xA04E0000
    .word 0x8FBF0014
    .word 0x27BD0018
    .word 0x24020001
    .word 0x03E00008
    .word 0x00000000

