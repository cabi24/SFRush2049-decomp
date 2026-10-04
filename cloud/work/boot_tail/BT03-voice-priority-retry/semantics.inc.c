/* Actual-source fixture: synthetic unlink publishes a new valid table state. */
ChannelLink D_800504C8[32];
u8 D_80050548[256];
GroupLink D_80050648[256];
u16 D_80050A48;
static ChannelLink after_links[32];
static u8 after_heads[256];
static GroupLink after_groups[256];
static u16 after_head;
static int calls, slot;

void func_8001EE9C(VoicePrefix *state)
{
    assert(state->channel2E == 255);
    assert((state->identifier60 & 255) == (u32)slot);
    calls++;
    memcpy(D_800504C8, after_links, sizeof(after_links));
    memcpy(D_80050548, after_heads, sizeof(after_heads));
    memcpy(D_80050648, after_groups, sizeof(after_groups));
    D_80050A48 = after_head;
    state->identifier60 = 0xABCDEF00U + ((slot + 17) % 32);
}

int main(void)
{
    VoicePrefix state, expected_state;
    ChannelLink links[32];
    u8 heads[256];
    GroupLink groups[256];
    u16 head, previous, next;
    int priorities[4] = {0, 64, 128, 254};
    int n, ch, active, i, link_slot, cases;
    cases = 0;
    for (slot = 0; slot < 32; slot++) {
        for (n = 0; n <= 4; n++) {
            for (ch = 0; ch < 256; ch++) {
                for (active = 0; active < 3; active++) {
                    memset(after_links, 0, sizeof(after_links));
                    memset(after_heads, 255, sizeof(after_heads));
                    memset(after_groups, 255, sizeof(after_groups));
                    after_head = n ? 0 : 65535;
                    for (i = 0; i < n; i++) {
                        link_slot = (slot + i + 1) % 32;
                        after_heads[priorities[i]] = (u8)link_slot;
                        after_links[link_slot].active = 1;
                        after_links[link_slot].previous = 255;
                        after_links[link_slot].next = 255;
                        after_groups[priorities[i]].previous = i ? priorities[i-1] : 65535;
                        after_groups[priorities[i]].next = i + 1 < n ? priorities[i+1] : 65535;
                    }
                    memcpy(D_800504C8, after_links, sizeof(after_links));
                    memcpy(D_80050548, after_heads, sizeof(after_heads));
                    memcpy(D_80050648, after_groups, sizeof(after_groups));
                    D_80050A48 = after_head;
                    memset(&state, 0xA5, sizeof(state));
                    state.identifier60 = 0x12345600U + slot;
                    state.channel2E = 255;
                    D_800504C8[slot].active = (u16)active;
                    calls = 0;
                    if (active == 1) {
                        /* This valid old list differs from all post-call lists. */
                        memset(D_800504C8, 0, sizeof(D_800504C8));
                        memset(D_80050548, 255, sizeof(D_80050548));
                        memset(D_80050648, 255, sizeof(D_80050648));
                        D_800504C8[slot].active = 1;
                        D_800504C8[slot].previous = 255;
                        D_800504C8[slot].next = 255;
                        D_80050548[255] = (u8)slot;
                        D_80050A48 = 255;
                    }
                    if (active == 1 && ch == 255) {
                        memcpy(links, D_800504C8, sizeof(links));
                        memcpy(heads, D_80050548, sizeof(heads));
                        memcpy(groups, D_80050648, sizeof(groups));
                        head = D_80050A48;
                        memcpy(&expected_state, &state, sizeof(state));
                        func_8001EF8C(&state, (u8)ch);
                        assert(calls == 0);
                    } else {
                        memcpy(links, after_links, sizeof(links));
                        memcpy(heads, after_heads, sizeof(heads));
                        memcpy(groups, after_groups, sizeof(groups));
                        head = after_head;
                        memcpy(&expected_state, &state, sizeof(state));
                        if (active == 1)
                            expected_state.identifier60 = 0xABCDEF00U + ((slot + 17) % 32);
                        expected_state.channel2E = (u8)ch;
                        links[slot].active = 1;
                        links[slot].previous = 255;
                        links[slot].next = heads[ch];
                        if (heads[ch] != 255) {
                            links[heads[ch]].previous = (u8)slot;
                        } else {
                            previous = 65535;
                            next = head;
                            while (next != 65535 && next < (u16)ch) {
                                previous = next;
                                next = groups[next].next;
                            }
                            groups[ch].previous = previous;
                            groups[ch].next = next;
                            if (previous == 65535) head = (u16)ch;
                            else groups[previous].next = (u16)ch;
                            if (next != 65535) groups[next].previous = (u16)ch;
                        }
                        heads[ch] = (u8)slot;
                        func_8001EF8C(&state, (u8)ch);
                        assert(calls == (active == 1));
                    }
                    assert(memcmp(&state, &expected_state, sizeof(state)) == 0);
                    assert(memcmp(links, D_800504C8, sizeof(links)) == 0);
                    assert(memcmp(heads, D_80050548, sizeof(heads)) == 0);
                    assert(memcmp(groups, D_80050648, sizeof(groups)) == 0);
                    assert(head == D_80050A48);
                    cases++;
                }
            }
        }
    }
    assert(cases == 122880);
    return 0;
}
