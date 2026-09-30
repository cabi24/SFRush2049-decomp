typedef unsigned int u32;
typedef unsigned char u8;
typedef int s32;
#define W(a) (*(u32 *)(a))
typedef struct List {
    u8 indirect;
    u8 doubly;
    u8 pad[2];
    s32 count;
    u32 head;
    u32 tail;
} List;
void func_80091FBC(List *list, u32 item, u32 next0) {
    u32 next = next0;
    u32 node, nn, p, q, t, ind, h, t2;
    if (item != 0) {
        ind = list->indirect;
        node = item;
        nn = next;
        if (ind != 0) {
            node = W(item);
            if (next != 0) {
                nn = W(next);
            } else {
                nn = 0;
            }
        }
        if (next != 0) {
            if (list->doubly != 0) {
                t = W(nn + 4);
                if (t != 0) {
                    if (ind != 0) {
                        W(W(t)) = item;
                    } else {
                        W(t) = item;
                    }
                } else {
                    list->head = item;
                }
                W(node + 4) = W(nn + 4);
                W(nn + 4) = item;
            } else {
                h = list->head;
                if (next == h) {
                    list->head = item;
                } else if (ind != 0) {
                    if (h != 0) {
                        p = h;
                        do {
                            q = W(p);
                            t = W(q);
                            if (next == t) {
                                W(q) = item;
                                t = W(W(p));
                            }
                            p = t;
                        } while (p != 0);
                    }
                } else {
                    if (h != 0) {
                        p = h;
                        do {
                            q = W(p);
                            if (next == q) {
                                W(p) = item;
                                q = item;
                            }
                            p = q;
                        } while (p != 0);
                    }
                }
            }
        } else {
            if (list->doubly != 0) {
                W(node + 4) = list->tail;
            }
            t2 = list->tail;
            if (t2 != 0) {
                if (list->indirect != 0) {
                    W(W(t2)) = item;
                } else {
                    W(t2) = item;
                }
            } else {
                list->head = item;
            }
            list->tail = item;
        }
        W(node) = next;
        list->count = list->count + 1;
    }
}
