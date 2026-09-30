typedef struct SNode { u8 pad0[52]; u16 slot; u8 pad36[6]; struct SNode *next; } SNode;
typedef struct { u8 pad[22]; u8 state; u8 pad2[9]; } SSlot;
extern s32 D_80149788;
extern SNode *D_80149450[];
extern SSlot D_80140BF0[];
void sound_stop(SNode *n) {
    s32 i;
    s32 last;
    if (n == 0) {
        return;
    }
    do {
        for (i = 0; i < D_80149788; i++) {
            if (D_80149450[i] == n) {
                break;
            }
        }
        D_80140BF0[n->slot].state = 2;
        last = D_80149788 - 1;
        D_80149450[i] = D_80149450[last];
        D_80149788 = last;
        D_80149450[last] = n;
        n = n->next;
    } while (n != 0);
}
