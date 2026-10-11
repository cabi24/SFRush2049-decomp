/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* func_80014CF4 (static, boot 0x80014CF4, 8 B, segment 0x11640 -> src/rom/lib_11640.c, converted)
 * Empty no-argument hook: `jr ra; nop`. Its only caller, func_80017108 (lib_16320), calls it with no argument
 * set-up after clearing its record table. IDO -O2 homes every declared parameter of an empty function
 * (`sw a0,0(sp)`...), so the absence of home stores fixes the prototype as (void). Presumably a debug/cache hook
 * compiled out of the retail build; semantics beyond "does nothing" are not recoverable. No shaping devices. */
void func_80014CF4(void)
{
}
