/* Test-only boundary doubles. These are never matching/TU helper candidates. */
#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "../../../../cloud/work/small_overlay_service_pair/service_pair.h"

SmallPlayer small_players[8];
SmallModel small_models[8];
SmallS8 small_teams[8];
SmallS16 small_player_count;
static int calls, transform_calls, angle_calls;
static int got_source, got_target, got_reason;
static float result_angle, seen_direction[3];
static void *seen_matrix;
static int callback_action;
static float *mutate_origin;

void func_800C55E4(SmallS32 a, SmallS32 b, SmallS32 c)
{
    ++calls;
    got_source = a; got_target = b; got_reason = c;
    assert(small_players[1].remaining == 0 || b != 1);
    if (callback_action == 1) small_players[1].car_index = 3;
    if (callback_action == 2) small_player_count = 2;
    if (callback_action == 3) {
        mutate_origin[0] = 1000.0f;

    }
    if (callback_action == 4) small_player_count = 3;
}
void func_800A61B0(float *in, float *out, void *matrix)
{
    ++transform_calls;
    memcpy(seen_direction, in, sizeof(seen_direction));
    seen_matrix = matrix;
    out[0] = 12.0f; out[1] = 13.0f; out[2] = 14.0f;
}
float func_8008C768(float x, float z)
{
    ++angle_calls;
    assert(x == 12.0f && z == 14.0f);
    return result_angle;
}
static void reset(void)
{
    int i;
    memset(small_players, 0, sizeof(small_players));
    memset(small_models, 0, sizeof(small_models));
    memset(small_teams, 0, sizeof(small_teams));
    small_player_count = 2;
    calls = transform_calls = angle_calls = callback_action = 0;
    got_source = got_target = got_reason = -1;
    result_angle = 0.0f;
    mutate_origin = 0;
    for (i = 0; i < 8; ++i) {
        small_players[i].car_index = (SmallS8)i;
        small_players[i].enabled = 1;
        small_players[i].remaining = 20000;
        small_teams[i] = (SmallS8)i;
    }
}
static void layouts(void)
{
    assert(sizeof(SmallS8) == 1 && sizeof(SmallS16) == 2);
    assert(sizeof(SmallS32) == 4 && sizeof(float) == 4);
    assert(sizeof(SmallPlayer) == 952 && sizeof(SmallModel) == 2056);
    assert(offsetof(SmallPlayer, position) == 8);
    assert(offsetof(SmallPlayer, enabled) == 0x308);
    assert(offsetof(SmallPlayer, excluded) == 0x359);
    assert(offsetof(SmallPlayer, car_index) == 0x35b);
    assert(offsetof(SmallPlayer, kind) == 0x384);
    assert(offsetof(SmallPlayer, remaining) == 0x386);
    assert(offsetof(SmallPlayer, last_amount) == 0x388);
    assert(offsetof(SmallPlayer, flags) == 0x38c);
    assert(offsetof(SmallModel, excluded) == 0x640);
}
static void d3a4(void)
{
    SmallPlayer *s, *t;
    reset(); s = &small_players[0]; t = &small_players[1];
    small_models[1].excluded = -1;
    small_8038D3A4(s,t,25); assert(t->remaining == 20000);
    small_models[1].excluded = 0;
    small_8038D3A4(t,t,25); assert(t->remaining == 20000);
    small_teams[1] = small_teams[0];
    small_8038D3A4(s,t,25); assert(t->remaining == 20000);
    small_teams[1] = 1;
    small_8038D3A4(s,t,25);
    assert(t->remaining == 19975 && t->last_amount == 25 && calls == 0);
    t->flags = 4;
    small_8038D3A4(s,t,29);
    assert(t->last_amount == 5 && t->remaining == 19970);
    small_8038D3A4(s,t,-29);
    assert(t->last_amount == -5 && t->remaining == 19975);
    t->flags = 0; t->remaining = 7;
    small_8038D3A4(s,t,7);
    assert(t->remaining == 0 && t->last_amount == 7 && calls == 1);
    assert(got_source == 0 && got_target == 1 && got_reason == 1);
    assert(small_models[1].excluded == 1);
    small_8038D3A4(s,t,99); assert(t->last_amount == 7 && calls == 1);
    reset(); t->remaining = 1;
    small_8038D3A4(s,t,65537);
    assert(t->last_amount == 1 && t->remaining == 0 && calls == 1);
    reset(); t->remaining = 1;
    small_8038D3A4(s,t,32770);
    assert(t->last_amount == -32766 && t->remaining == 32767 && calls == 0);
    reset(); t->remaining = 32767;
    small_8038D3A4(s,t,-1);
    assert(t->remaining == 0 && t->last_amount == -1 && calls == 1);
    reset(); t->remaining = 0;
    small_8038D3A4(s,t,0); assert(calls == 1);
    reset(); t->remaining = 1; callback_action = 1;
    small_8038D3A4(s,t,1);
    assert(small_models[1].excluded == 0 && small_models[3].excluded == 1);
}
static void d798(void)
{
    float origin[3], endpoint[3];
    SmallPlayer *t;
    SmallU32 nan_bits;
    float nan_value;
    reset(); memset(origin,0,sizeof(origin)); memset(endpoint,0,sizeof(endpoint));
    t = &small_players[1];
    small_player_count = 0;
    small_8038D798(0,0,0,400.0f,1600); assert(calls == 0);
    small_player_count = -1;
    small_8038D798(0,0,0,400.0f,1600); assert(calls == 0);
    small_player_count = 2;
    small_8038D798(origin,0,0,400.0f,1600);
    assert(t->last_amount == 1600 && t->remaining == 18400);
    assert(small_players[0].remaining == 20000 && transform_calls == 0);
    reset(); t->position[0] = 20.0f;
    small_8038D798(origin,0,0,400.0f,1600); assert(t->last_amount == 0);
    t->position[0] = 10.0f;
    small_8038D798(origin,0,0,400.0f,1600); assert(t->last_amount == 900);
    reset(); t->position[0] = 1.0f;
    small_8038D798(origin,0,0,4.0f,29); assert(t->last_amount == 16);
    reset(); t->enabled = 0;
    small_8038D798(origin,0,0,400.0f,1600); assert(t->last_amount == 0);
    t->enabled = -1; t->excluded = -1;
    small_8038D798(origin,0,0,400.0f,1600); assert(t->last_amount == 0);
    t->excluded = 0; small_models[1].excluded = 1;
    small_8038D798(origin,0,0,400.0f,1600); assert(t->last_amount == 0);
    reset(); t->kind = 5; endpoint[0] = 2; endpoint[1] = 3; endpoint[2] = 4;
    small_8038D798(origin,endpoint,0,400.0f,1600);
    assert(t->last_amount == 560 && transform_calls == 1 && angle_calls == 1);
    assert(seen_direction[0] == 2 && seen_direction[1] == 3 && seen_direction[2] == 4);
    assert(seen_matrix == (void *)((unsigned char *)t + 0x2c));
    reset(); t->kind = 5; result_angle = -1.0f;
    small_8038D798(origin,origin,0,400.0f,1600);
    assert(t->last_amount == 560);
    assert(seen_direction[0] == 0 && seen_direction[1] == 0 && seen_direction[2] == 0);
    reset(); t->kind = 5; result_angle = 1.35f;
    small_8038D798(origin,origin,0,400.0f,1600); assert(t->last_amount == 1600);
    reset(); t->kind = 5; result_angle = -1.35f;
    small_8038D798(origin,origin,0,400.0f,1600); assert(t->last_amount == 1600);
    reset(); t->flags = 4;
    small_8038D798(origin,0,0,400.0f,1600); assert(t->last_amount == 320);
    reset(); small_8038D798(origin,0,0,0.0f,1600); assert(t->last_amount == 0);
    small_8038D798(origin,0,0,-1.0f,1600); assert(t->last_amount == 0);
    reset(); small_player_count = 3; t->remaining = 1; callback_action = 2;
    small_8038D798(origin,0,0,400.0f,1600);
    assert(calls == 1 && small_players[2].last_amount == 0);
    reset(); small_player_count = 3; t->remaining = 1; callback_action = 3;
    mutate_origin = small_players[3].position;
    small_8038D798(mutate_origin,0,0,400.0f,1600);
    assert(calls == 1 && small_players[2].last_amount == 0);
    assert(small_players[2].enabled == 1 && mutate_origin[0] == 1000.0f);
    reset(); t->remaining = 1; callback_action = 4;
    small_8038D798(origin,0,0,400.0f,1600);
    assert(calls == 1 && small_players[2].last_amount == 1600);
    reset(); nan_bits = 0x7fc00000U;
    memcpy(&nan_value, &nan_bits, sizeof(nan_value));
    small_8038D798(origin,0,0,nan_value,1600); assert(t->last_amount == 0);
    origin[0] = nan_value;
    small_8038D798(origin,0,0,400.0f,1600); assert(t->last_amount == 0);
    origin[0] = 0.0f;
    reset(); t->kind = 5; result_angle = nan_value;
    small_8038D798(origin,origin,0,400.0f,1600); assert(t->last_amount == 1600);
}
int main(void)
{
    layouts(); d3a4(); d798();
    puts("small overlay service pair: layout and behavioral scenarios passed");
    return 0;
}
