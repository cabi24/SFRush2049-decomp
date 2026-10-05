/* Native dependencies are O32 contract hooks, not implementations of the game. */
#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "candidate.c"

Row48 D_8011753C[4];
f32 D_801249D0;
Node24 *D_801391F0;
Model952 player_array[4];
static Actor96 actor;
static Node24 node, old_head, new_head;
static u32 fixture[7], events[48], event_count;
static f32 *matrix;

static u32 bits(f32 value)
{
    u32 result;
    memcpy(&result, &value, 4);
    return result;
}

static void set_bits(f32 *where, u32 value) { memcpy(where, &value, 4); }

static u32 tag(const void *pointer)
{
    if (!pointer) return 0;
    if (pointer == &old_head) return 1;
    if (pointer == &new_head) return 2;
    if (pointer == &node) return 3;
    assert(pointer == &actor);
    return 4;
}

static void trace(u32 call, u32 a, u32 b, u32 c, u32 d)
{
    assert(event_count + 12 <= 48);
    events[event_count++] = call;
    events[event_count++] = a;
    events[event_count++] = b;
    events[event_count++] = c;
    events[event_count++] = d;
    events[event_count++] = node.resource;
    events[event_count++] = bits(node.time);
    events[event_count++] = actor.flags;
    events[event_count++] = (unsigned short)actor.state;
    events[event_count++] = (unsigned short)actor.index;
    events[event_count++] = (u8)actor.player;
    events[event_count++] = tag(D_801391F0);
}

static void mutate(int phase)
{
    if (fixture[5] & (1U << phase)) {
        actor.index = (s16)((actor.index + 1) % 4);
        actor.player = (s8)((actor.player + 1) % 4);
        actor.flags ^= (u8)(0x80 >> phase);
        D_801391F0 = &new_head;
        if (phase == 0) set_bits(&D_801249D0, bits(D_801249D0) ^ 0x00800000U);
    }
}

Node24 *func_80090284(void)
{
    trace(1, 0, 0, 0, 0);
    mutate(0);
    return fixture[0] ? 0 : &node;
}

void vector_normalize_length(f32 *direction, f32 (*destination)[3])
{
    int player, i;
    player = -1;
    for (i = 0; i < 4; i++) if (direction == player_array[i].direction) player = i;
    assert(player >= 0);
    matrix = &destination[0][0];
    trace(2, (u32)player, 1, 0, 0);
    for (i = 0; i < 9; i++) set_bits(&destination[i / 3][i % 3], bits(direction[i % 3]) ^ ((u32)i << 12));
    mutate(1);
}

void math_utility(void *source, void *destination)
{
    assert(source == matrix && destination == &actor.basis);
    trace(3, 1, 1, 0, 0);
    memcpy(destination, source, 36);
    mutate(2);
}

int stat_lap_split(int event, int player, f32 *position, u8 mode)
{
    assert(position == actor.position && mode == 2);
    trace(4, (u32)event, (u32)player, 1, mode);
    mutate(3);
    return 123;
}

void run(const u32 *input, u32 *output)
{
    unsigned char before_actor[96], before_models[sizeof(player_array)], before_rows[sizeof(D_8011753C)];
    unsigned char *bytes;
    int i, j, allowed, cursor;
    memcpy(fixture, input, sizeof(fixture));
    memset(&actor, 0xa5, sizeof(actor));
    memset(&node, 0xa5, sizeof(node));
    memset(player_array, 0xa5, sizeof(player_array));
    memset(D_8011753C, 0xa5, sizeof(D_8011753C));
    memset(events, 0, sizeof(events));
    memset(output, 0, 71 * sizeof(*output));
    event_count = 0;
    actor.index = (s16)fixture[1];
    actor.player = (s8)fixture[2];
    actor.flags = (u8)fixture[3];
    set_bits(&D_801249D0, fixture[4]);
    node.next = &old_head;
    node.owner = 0;
    D_801391F0 = &old_head;
    for (i = 0; i < 4; i++) {
        D_8011753C[i].resource = fixture[6] ^ (0x12340000U + (u32)i);
        D_8011753C[i].event = fixture[6] ^ (0x56780000U + (u32)i);
        for (j = 0; j < 3; j++) set_bits(&player_array[i].direction[j], 0x3f000000U + (u32)i * 0x100000U + (u32)j * 0x10000U);
    }
    memcpy(before_actor, &actor, sizeof(actor));
    memcpy(before_models, player_array, sizeof(player_array));
    memcpy(before_rows, D_8011753C, sizeof(D_8011753C));
    func_8010E72C(&actor);
    assert(sizeof(actor) == 96 && sizeof(Model952) == 952 && sizeof(Row48) == 48);
    assert(!memcmp(before_models, player_array, sizeof(player_array)));
    assert(!memcmp(before_rows, D_8011753C, sizeof(D_8011753C)));
    bytes = (unsigned char *)&actor;
    for (i = 0; i < 96; i++) {
        allowed = i == 4 || i == 16 || i == 17 || (i >= 20 && i < 56) || i == 90 || i == 91 || i == 92;
        if (!allowed) assert(bytes[i] == before_actor[i]);
    }
    for (i = 0; i < 6; i++) assert(node.before_owner[i] == 0xa5);
    cursor = 0;
    output[cursor++] = (unsigned short)node.state;
    output[cursor++] = node.resource;
    output[cursor++] = bits(node.time);
    output[cursor++] = tag(node.owner);
    output[cursor++] = tag(node.next);
    output[cursor++] = tag(D_801391F0);
    output[cursor++] = actor.flags;
    output[cursor++] = (unsigned short)actor.state;
    output[cursor++] = (unsigned short)actor.index;
    output[cursor++] = (u8)actor.player;
    for (i = 0; i < 9; i++) output[cursor++] = bits(actor.basis.m[i]);
    for (i = 0; i < 3; i++) output[cursor++] = bits(actor.position[i]);
    output[cursor++] = event_count / 12;
    for (i = 0; i < (int)event_count; i++) output[cursor++] = events[i];
}

#ifdef HOST_MAIN
int main(void)
{
    u32 input[7], output[71];
    int i, count;
    count = 0;
    while (scanf("%u %u %u %u %u %u %u", &input[0], &input[1], &input[2], &input[3], &input[4], &input[5], &input[6]) == 7) {
        run(input, output);
        for (i = 0; i < 71; i++) printf("%u%c", output[i], i == 70 ? '\n' : ' ');
        count++;
    }
    return count ? 0 : 1;
}
#endif
