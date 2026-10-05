/* Compile the unchanged candidate with a host-native Blit representation. */
#include <assert.h>
#include <string.h>
#include CANDIDATE_SOURCE
s32 D_80149D98;
u32 D_80117358;
static int initial_image, changed_image;
static s32 initial_callback(Blit *b) { return b != 0; }
static s32 changed_callback(Blit *b) { return b != 0; }
static int *output, event_count, first_hide, second_hide, mutate;
static int image_token(const Blit *b) {
    if (b->image == &initial_image) return 0;
    if (b->image == &D_80117358) return 1;
    assert(b->image == &changed_image); return 2;
}
static int callback_token(const Blit *b) {
    if (b->AnimFunc == 0) return 0;
    if (b->AnimFunc == initial_callback) return 1;
    assert(b->AnimFunc == changed_callback); return 2;
}
void Input_ApplyPadConfig(Blit *b) {
    int *event = output + 6 + 4 * event_count;
    int hide = event_count ? second_hide : first_hide;
    assert(event_count < 2);
    event[0] = b->Hide; event[1] = image_token(b);
    event[2] = callback_token(b); event[3] = D_80149D98;
    event_count++;
    if (hide != 256) b->Hide = hide;
    if (mutate & 1) {
        b->image = &changed_image;
        b->AnimFunc = changed_callback;
    }
    if (mutate & 2) D_80149D98 = !D_80149D98;
}
void run_case(const int *in, int *out) {
    Blit b, untouched;
    memset(&b, 0x5a, sizeof(b)); memset(out, 0, 14 * sizeof(int));
    b.Hide = in[1]; b.image = &initial_image; b.AnimFunc = initial_callback;
    memcpy(&untouched, &b, sizeof(b));
    D_80149D98 = in[0]; first_hide = in[2]; second_hide = in[3]; mutate = in[4];
    event_count = 0; output = out;
    out[0] = state_update_global(&b); out[1] = b.Hide;
    out[2] = image_token(&b); out[3] = callback_token(&b);
    out[4] = D_80149D98; out[5] = event_count;
    untouched.Hide = b.Hide; untouched.image = b.image; untouched.AnimFunc = b.AnimFunc;
    assert(memcmp(&untouched, &b, sizeof(b)) == 0);
}
