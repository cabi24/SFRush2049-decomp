/* flags: -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef unsigned char u8;
typedef unsigned int u32;
typedef struct Link {struct Link *next,*prev;} Link;
typedef struct List {u8 indirect,doubly,pad[2];u32 count;Link *head,*tail;} List;
typedef struct Vec3 {float x,y,z;} Vec3;
typedef struct Packet {Link link;u8 type,ready,opaque[2];Vec3 a,b,c,d;} Packet;
typedef struct OSMesgQueue OSMesgQueue;
extern OSMesgQueue D_80142728;
extern List D_80146170,D_80146188;
extern void *D_8011025C;
extern int osRecvMesg(OSMesgQueue *,void **,int);
extern int osJamMesg(OSMesgQueue *,void *,int);
extern void func_8009211C(List *,Link *);
extern void func_80091FBC(List *,Link *,Link *);
Packet *func_800D63EC(Vec3 *a,Vec3 *b,Vec3 *c,Vec3 *d) {
    Packet *packet;
    Packet *result;
    if(!D_8011025C) return (Packet *)-1;
    osRecvMesg(&D_80142728,0,1);
    packet=(Packet *)D_80146170.head;
    func_8009211C(&D_80146170,&packet->link);
    packet->type=0;
    result=packet;
    packet->a.x=a->x;
    packet->a.y=a->y;
    packet->a.z=a->z;
    packet->b.x=b->x;
    packet->b.y=b->y;
    packet->b.z=b->z;
    packet->c.x=c->x;
    packet->c.y=c->y;
    packet->c.z=c->z;
    packet->d.x=d->x;
    packet->d.y=d->y;
    packet->d.z=d->z;
    func_80091FBC(&D_80146188,&packet->link,D_80146188.head);
    packet->ready=1;
    osJamMesg(&D_80142728,0,0);
    return result;
}

void func_8009211C(List *list, Link *object) {
    Link *node, *current, *header, *next;
    if (object == 0) return;
    if (list->indirect) node = object->next;
    else node = object;
    if (list->doubly) {
        if (node->next) {
            if (list->indirect) node->next->next->prev = node->prev;
            else node->next->prev = node->prev;
        } else list->tail = node->prev;
        if (node->prev) {
            if (list->indirect) node->prev->next->next = node->next;
            else node->prev->next = node->next;
        } else list->head = node->next;
    } else if (object == list->head) {
        list->head = node->next;
        if (list->head == 0) list->tail = 0;
    } else if (list->indirect) {
        current = list->head;
        while (current) {
            header = current->next;
            next = header->next;
            if (object == next) {
                header->next = node->next;
                if (node->next == 0) list->tail = current;
                break;
            }
            current = next;
        }
    } else {
        current = list->head;
        while (current) {
            header = current->next;
            if (object == header) {
                current->next = node->next;
                if (node->next == 0) list->tail = current;
                break;
            }
            current = header;
        }
    }
    list->count--;
}

void func_80091FBC(List *list, Link *object, Link *before) {
    Link *node, *at, *current, *header, *next;
    if (object == 0) return;
    if (list->indirect) {
        node = object->next;
        if (before) at = before->next;
        else at = 0;
    } else {
        node = object;
        at = before;
    }
    if (before) {
        if (list->doubly) {
            if (at->prev) {
                if (list->indirect) at->prev->next->next = object;
                else at->prev->next = object;
            } else list->head = object;
            node->prev = at->prev;
            at->prev = object;
        } else if (before == list->head) list->head = object;
        else if (list->indirect) {
            current = list->head;
            while (current) {
                header = current->next;
                next = header->next;
                if (before == next) {
                    header->next = object;
                    next = current->next->next;
                }
                current = next;
            }
        } else {
            current = list->head;
            while (current) {
                header = current->next;
                if (before == header) {
                    current->next = object;
                    header = object;
                }
                current = header;
            }
        }
    } else {
        if (list->doubly) node->prev = list->tail;
        if (list->tail) {
            if (list->indirect) list->tail->next->next = object;
            else list->tail->next = object;
        } else list->head = object;
        list->tail = object;
    }
    node->next = before;
    list->count++;
}
