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
void func_8009211C(List *list, u32 item) {
    u32 node, p, q, r, t, ind;
    if (item != 0) {
        ind = list->indirect;
        node = ind ? W(item) : item;
        if (list->doubly != 0) {
            item = W(node);
            if (item != 0) {
                if (ind != 0) {
                    W(W(item) + 4) = W(node + 4);
                } else {
                    W(item + 4) = W(node + 4);
                }
            } else {
                list->tail = W(node + 4);
            }
            r = W(node + 4);
            if (r != 0) {
                if (list->indirect != 0) {
                    W(W(r)) = W(node);
                } else {
                    W(r) = W(node);
                }
            } else {
                list->head = W(node);
            }
        } else {
            q = list->head;
            if (item == q) {
                if ((list->head = W(node)) == 0) {
                    list->tail = 0;
                }
            } else if (ind != 0) {
                if (q != 0) {
                    p = q;
                    do {
                        q = W(p);
                        t = W(q);
                        if (item == t) {
                            W(q) = W(node);
                            if (W(node) == 0) {
                                list->tail = p;
                            }
                            break;
                        }
                        p = t;
                    } while (t != 0);
                }
            } else {
                if (q != 0) {
                    p = q;
                    do {
                        q = W(p);
                        if (item == q) {
                            W(p) = W(node);
                            if (W(node) == 0) {
                                list->tail = p;
                            }
                            break;
                        }
                        p = q;
                    } while (q != 0);
                }
            }
        }
        list->count = list->count - 1;
    }
}
