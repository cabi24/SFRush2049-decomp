# Source: E240.s
# Address: 0x8000D640

glabel func_8000D640
    /* E240 8000D640 3C0EA404 */  lui        $t6, %hi(SP_STATUS_REG)
    /* E244 8000D644 03E00008 */  jr         $ra
    /* E248 8000D648 ADC40010 */   sw        $a0, %lo(SP_STATUS_REG)($t6)