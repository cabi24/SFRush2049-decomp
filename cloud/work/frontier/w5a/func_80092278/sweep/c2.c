/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef unsigned char u8;typedef unsigned int u32;typedef int s32;typedef float f32;
typedef struct Link {struct Link *next,*prev;} Link;
typedef struct List {u8 indirect,doubly,pad[2];u32 count;Link *head,*tail;} List;
typedef struct Node68 {Link link;List *list;u32 tag,state,p20;u8 flag,code,live,mode;u32 p28;f32 f32,f36,f40,f44,f48;u32 index,value,p60,p64;} Node68;
typedef struct Object24 {u8 p0,p1,kind,p3;u32 value;f32 f8,f12,f16;Node68 *node;} Object24;
typedef struct Queue24 {u32 opaque[6];} Queue24;
extern List D_80143FF8,D_80144C50;extern u32 D_80110254,D_801460F8,D_80146104;
extern Queue24 D_80142728,D_801427A8;extern u8 D_8011F5CC[];
s32 osRecvMesg(Queue24 *,void **,s32);s32 osJamMesg(Queue24 *,void *,s32);
Object24 *func_80091B00(void);
void func_8009211C(List *,Link *);void func_80091FBC(List *,Link *,Link *);
Node68 *func_80092278(void) {
 Node68 *node=(Node68*)D_80143FF8.head;
 if(node==0){}
 node->tag=(D_80110254<<D_801460F8)|(node->tag&D_80146104);
 if(D_80110254>=((~D_80146104)>>(D_801460F8+1)))D_80110254=1;
 else D_80110254++;
 node->p64=0;node->f36=0.0f;node->f40=0.0f;node->f44=0.0f;
 node->f32=-1.0f;node->f48=1.0f;
 func_8009211C(&D_80143FF8,(Link*)node);
 func_80091FBC(&D_80144C50,(Link*)node,D_80144C50.head);
 node->list=&D_80144C50;
 return node;
}
u32 entity_flags_apply(u32 index,u32 other,u32 value,u8 mode) {
 Object24 *object;u32 result;
 osRecvMesg(&D_80142728,0,1);
 object=func_80091B00();object->kind=2;object->value=value;
 object->node=func_80092278();object->node->p20=0;
 object->node->index=index;object->node->value=other;object->node->mode=mode;
 object->node->state=1;object->node->flag=0;object->node->live=1U;
 object->node->code=D_8011F5CC[index];
 object->f8=1.0f;object->f12=0.0f;object->f16=-2.0f;
 result=object->node->tag;
 osJamMesg(&D_80142728,0,0);
 osJamMesg(&D_801427A8,object,0);
 return result;
}
u32 high_scores_display(u32 index,u32 other,u32 value,u8 mode,f32 x,f32 y,f32 z) {
 Object24 *object;u32 result;
 osRecvMesg(&D_80142728,0,1);
 object=func_80091B00();object->kind=2;object->value=value;
 object->node=func_80092278();object->node->p20=0;
 object->node->index=index;object->node->value=other;object->node->mode=mode;
 object->node->state=1;object->node->flag=0;object->node->live=1U;
 object->node->code=D_8011F5CC[index];
 if(x<0.0f)object->f8=0.0f;else if(1.0f<x)object->f8=1.0f;else object->f8=x;
 if(y<-1.0f)object->f12=-1.0f;else if(1.0f<y)object->f12=1.0f;else object->f12=y;
 if(z<-1.0f)object->f16=-1.0f;else if(1.0f<z)object->f16=1.0f;else object->f16=z;
 result=object->node->tag;
 osJamMesg(&D_80142728,0,0);
 osJamMesg(&D_801427A8,object,0);
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
