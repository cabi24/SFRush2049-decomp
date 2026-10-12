#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include "group.c"
Vehicle D_8014A250[4];
s8 D_8012E67C[4];
static Player players[4];
static int calls, called_attacker, called_victim, called_amount, change_index;
void effect_cleanup(s8 attacker, s8 victim, s8 amount)
{
    calls++;
    called_attacker = attacker;
    called_victim = victim;
    called_amount = amount;
    if (change_index) players[victim].index = 3;
}
static void check(int value) { if (!value) abort(); }
int main(void)
{
    int same, disabled, shield, health, damage, count = 0;
    Player expected[4];
    Vehicle ev[4];
    check(sizeof(Player) == 952 && sizeof(Vehicle) == 2056);
    check(offsetof(Player, index) == 0x35b && offsetof(Player, health) == 0x386);
    check(offsetof(Player, damage) == 0x388 && offsetof(Player, flags) == 0x38c);
    check(offsetof(Vehicle, disabled) == 0x640);
    for (same = 0; same < 2; same++)
    for (disabled = 0; disabled < 2; disabled++)
    for (shield = 0; shield < 2; shield++)
    for (health = 0; health < 3; health++)
    for (damage = 0; damage < 4; damage++) {
        static const int values[4] = {0, 5, 30, 160};
        int d = values[damage], remaining, depleted = 0;
        memset(players, 0x5a, sizeof(players));
        memset(D_8014A250, 0x5a, sizeof(D_8014A250));
        players[0].index = 0; players[1].index = 1;
        players[1].health = (s16)(health * 10);
        players[1].flags = shield ? 4 : 0;
        D_8014A250[1].disabled = (s8)disabled;
        D_8012E67C[0] = 0; D_8012E67C[1] = (s8)(same ? 0 : 1);
        memcpy(expected, players, sizeof(players));
        memcpy(ev, D_8014A250, sizeof(ev));
        calls = change_index = 0;
        if (!same && !disabled) {
            if (shield) d = (int)((float)d * 0.2f);
            expected[1].damage = (s16)d;
            remaining = expected[1].health - d;
            expected[1].health = (s16)(remaining > 0 ? remaining : 0);
            if (remaining <= 0) { ev[1].disabled = 1; depleted = 1; }
        }
        func_8038D3A4(&players[0], &players[1], values[damage]);
        check(memcmp(expected, players, sizeof(players)) == 0);
        check(memcmp(ev, D_8014A250, sizeof(ev)) == 0);
        check(calls == depleted);
        if (depleted) check(called_attacker == 0 && called_victim == 1 && called_amount == 1);
        count++;
    }
    players[0].index = 0; players[1].index = 1; players[1].health = 4; players[1].flags = 0;
    D_8014A250[1].disabled = 0; D_8014A250[3].disabled = 0;
    D_8012E67C[0] = 0; D_8012E67C[1] = 1;
    calls = 0; change_index = 1;
    func_8038D3A4(&players[0], &players[1], 5);
    check(calls == 1 && players[1].index == 3 && players[1].health == 0);
    check(D_8014A250[1].disabled == 0 && D_8014A250[3].disabled == 1);
    check(count == 96);
    puts("PASS: 96 source fixtures and callback reload");
    return 0;
}
