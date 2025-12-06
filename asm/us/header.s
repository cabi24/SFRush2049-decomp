.section .data

.word 0x80371240       /* PI BSB Domain 1 register */
.word 0x0000000F       /* Clockrate setting */
.word 0x80000400       /* Entrypoint address */
.word 0x0000144A       /* Revision */
.word 0xB9A9ECA2       /* Checksum 1 */
.word 0x17AAE48E       /* Checksum 2 */
.word 0x00000000       /* Unknown 1 */
.word 0x00000000       /* Unknown 2 */
.ascii "Rush 2049           " /* Internal name */
.word 0x00000000       /* Unknown 3 */
.word 0x0000004E       /* Cartridge */
.ascii "RU"            /* Cartridge ID */
.ascii "E"             /* Country code */
.byte 0x00             /* Version */
