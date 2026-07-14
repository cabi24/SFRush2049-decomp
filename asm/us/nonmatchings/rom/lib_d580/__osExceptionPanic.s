nonmatching __osExceptionPanic, 0x8

glabel __osExceptionPanic
    /* DE98 8000D298 0C003EE8 */  jal        __osEnqueueAndYield
    /* DE9C 8000D29C 00002025 */   or        $a0, $zero, $zero
endlabel __osExceptionPanic
