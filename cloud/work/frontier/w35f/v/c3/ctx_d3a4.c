/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* Image B [0x8038D3A4, 0x8038D498), opponent damage application.
 * Native reconstruction; original names and arcade ancestry are unknown.
 * Ignore disabled victims, self-hits and same-team hits. Flag 4 scales damage
 * by the actual binary32 0.2 literal; record damage, subtract from health,
 * clamp a depleted victim to zero, post a one-point event, then disable it.
 * The event helper may change victim->index; the final store reloads it.
 * Ordinary o32 ABI: two player pointers and signed 32-bit damage; void result.
 * effect_cleanup's accepted source establishes all three signed-byte inputs.
 * Shaping: victim-first equality expressions preserve retail operand/load order.
 * No pressure locals, volatile, fake helpers, padding locals or assembly.
 * Unknown byte arrays retain real player/vehicle storage. Valid indices must
 * address both tables; signed arithmetic and float conversion must be defined.
 * Narrowing damage/health uses the fixed IDO target's signed-halfword semantics.
 */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;
typedef unsigned int u32;
typedef struct Player { u8 unknown000[0x35b]; s8 index; u8 unknown35c[0x2a]; s16 health; s16 damage; u8 unknown38a[2]; u32 flags; u8 unknown390[0x28]; } Player;
typedef struct Vehicle { u8 unknown000[0x640]; s8 disabled; u8 unknown641[0x1c7]; } Vehicle;
extern Vehicle D_8014A250[];
extern s8 D_8012E67C[];
extern void effect_cleanup(s8 attacker, s8 victim, s8 amount);
void func_8038D3A4(Player *attacker, Player *victim, s32 damage)
{
    if (D_8014A250[victim->index].disabled) return;
    if (victim->index == attacker->index) return;
    if (D_8012E67C[victim->index] == D_8012E67C[attacker->index]) return;
    if (victim->flags & 4) damage = damage * 0.2f;
    victim->damage = damage;
    victim->health -= damage;
    if (victim->health <= 0) {
        victim->health = 0;
        effect_cleanup(attacker->index, victim->index, 1);
        D_8014A250[victim->index].disabled = 1;
    }
}
