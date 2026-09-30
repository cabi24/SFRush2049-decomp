typedef signed int s32;
typedef struct Node { s32 pad0[2]; struct Node *next; s32 pad1[1]; s32 state; } Node;

extern Node D_80144C50;
extern void func_8009211C(Node *a, Node *b);
extern void func_80091FBC(Node *a, Node *b, Node *c);
extern s32 func_800987E8(s32);
extern void audio_effect_setup(Node *n);
extern s32 D_80140000;

void audio_pitch_adjust(Node *n)
{
    n->state = 3;
    func_8009211C(n->next, n);
    func_80091FBC(&D_80144C50, n, D_80144C50.next);
    n->next = &D_80144C50;
}

/* stand-in callers (real ones: entity_ai_pathfind, func_80098FB8) */
void caller_a(Node *n, s32 k)
{
    while (n) {
        Node *nx = n->next;
        if (func_800987E8(k) == 0) {
            if (*((char *)n + 25)) audio_pitch_adjust(n); else audio_effect_setup(n);
        }
        n = nx;
    }
}

void caller_b(Node *n, s32 k)
{
    Node *h = n;
    while (h) {
        if (func_800987E8(k) == 0) {
            if (*((char *)h + 25)) audio_pitch_adjust(h); else audio_effect_setup(h);
        }
        h = h->next;
    }
}
