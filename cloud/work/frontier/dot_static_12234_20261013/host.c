/* Actual candidate with a pointer-independent field oracle; C89 host only.
 * Native-width layout is separately asserted in verify.py using IDO 7.1. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../../matches/boot_tail/func_80012234.c"
#define N 12
#define CHECK(x) do { if (!(x)) { fprintf(stderr,"host failure line %d case %d\n",__LINE__,case_no); exit(1); } } while (0)
AudioNode *D_80038344, *D_80038348;
void *D_8003802C;
unsigned int D_80038038, D_8003803C;
unsigned short D_80038028;
static AudioNode nodes[N];
static unsigned char buffers[N][256];
static DmaEntry_80012234 queues[2][65536];
static int case_no, selected, mutation, calls, expected_head, expected_free;
static unsigned int key, answer, expected_cursor, expected_command;
static unsigned short expected_count;
static int expected_queue;
struct Model { int next, previous, buffer; unsigned int key; unsigned short age, spare; };
static struct Model model[N];

static AudioNode *node_ptr(int i) { return i < 0 ? NULL : &nodes[i]; }
static void check_nodes(void)
{
    int i;
    CHECK(D_80038344 == node_ptr(expected_head));
    CHECK(D_80038348 == node_ptr(expected_free));
    for (i=0;i<N;i++) {
        CHECK(nodes[i].next == node_ptr(model[i].next));
        CHECK(nodes[i].previous == node_ptr(model[i].previous));
        CHECK(nodes[i].buffer == buffers[model[i].buffer]);
        CHECK(nodes[i].unknown0C == model[i].key);
        CHECK(nodes[i].countdown == model[i].age);
        CHECK(nodes[i].unknown12 == model[i].spare);
    }
}

unsigned int func_8002C5E0(unsigned int command, unsigned int cursor)
{
    CHECK(++calls == 1);
    CHECK(command == expected_command && cursor == expected_cursor);
    CHECK(D_80038038 == expected_cursor && D_8003803C == expected_command);
    CHECK(D_80038028 == expected_count && D_8003802C == queues[0]);
    check_nodes();
    if (mutation & 1) D_80038028 = 3;
    if (mutation & 2) D_8003802C = queues[1];
    if (mutation & 4) nodes[selected].buffer = buffers[11];
    return answer;
}

static void one(int active, int free_count, int hit, int seed, int mode, int wrap)
{
    int i,j,q,expired,found,next,last,expected_calls;
    unsigned char *result;
    static const unsigned int keys[4] = {0U,0xffffffffU,0x12345678U,0x80000000U};
    static const unsigned int answers[3] = {0U,0xffffffffU,0xabcdef00U};
    static const unsigned short ages[3] = {1,2,65535};
    static const unsigned short counts[3] = {0,1,7};
    ++case_no; mutation=mode;calls=0;key=keys[seed%4];answer=answers[seed%3];
    expired=active ? seed%active : -1;
    expected_head=active?0:-1;expected_free=free_count?active:-1;
    expected_count=wrap?65535:counts[seed%3];expected_queue=0;
    expected_cursor=(seed%2)?0xfffffffcU:0x12345000U;
    expected_command=(seed%2)?0x87654321U:0;
    for(i=0;i<N;i++) {
        model[i].next=model[i].previous=-1;model[i].buffer=i;
        model[i].key=0xa5a5a5a5U;model[i].age=0xa5a5;model[i].spare=0xa5a5;
    }
    for(i=0;i<active+free_count;i++) {
        j=i<active?0:active;last=i<active?active:active+free_count;
        model[i].next=i+1<last?i+1:-1;model[i].previous=i>j?i-1:-1;
        model[i].key=(i==hit || (hit>=0 && i==hit+1 && (seed&1)))?key:key^0x11223344U;
        model[i].age=i==expired?0:ages[(i+seed)%3];
    }
    for(i=0;i<N;i++) {
        nodes[i].next=node_ptr(model[i].next);nodes[i].previous=node_ptr(model[i].previous);
        nodes[i].buffer=buffers[i];nodes[i].unknown0C=model[i].key;
        nodes[i].countdown=model[i].age;nodes[i].unknown12=model[i].spare;
    }
    for(q=0;q<2;q++) for(j=0;j<13;j++) {
        i=j==12?65535:j;queues[q][i].length=0x12345678;
        queues[q][i].source=0xa5a5a5a5U;queues[q][i].destination=buffers[10];
    }
    D_80038344=node_ptr(expected_head);D_80038348=node_ptr(expected_free);
    D_8003802C=queues[0];D_80038028=expected_count;
    D_80038038=expected_cursor;D_8003803C=expected_command;
    found=-1;
    for(i=0;i<active;i++) if(model[i].key==key) {found=i;break;}
    expected_calls=found<0;
    if(found>=0) selected=found;
    else if(free_count) {
        selected=active;next=model[selected].next;expected_free=next;
        if(next>=0) model[next].previous=-1;
        if(active) model[0].previous=selected;
        model[selected].next=expected_head;expected_head=selected;
    } else {
        selected=-1;
        for(i=0;i<active;i++) if(model[i].age==0) {selected=i;break;}
        CHECK(selected>=0);
    }
    model[selected].age=2;
    if(expected_calls) {model[selected].key=key;expected_cursor+=12U;}
    result=func_80012234(key);
    if(expected_calls) {
        if(mutation&1) expected_count=3;
        if(mutation&2) expected_queue=1;
        if(mutation&4) model[selected].buffer=11;
        expected_command=answer;
    }
    check_nodes();CHECK(calls==expected_calls);
    CHECK(result==buffers[model[selected].buffer]);
    CHECK(D_8003803C==expected_command && D_80038038==expected_cursor);
    CHECK(D_8003802C==queues[expected_queue]);
    for(q=0;q<2;q++) for(j=0;j<13;j++) {
        i=j==12?65535:j;
        if(expected_calls && q==expected_queue && i==expected_count) {
            CHECK(queues[q][i].length==256 && queues[q][i].source==key);
            CHECK(queues[q][i].destination==buffers[model[selected].buffer]);
        } else {
            CHECK(queues[q][i].length==0x12345678 && queues[q][i].source==0xa5a5a5a5U);
            CHECK(queues[q][i].destination==buffers[10]);
        }
    }
    CHECK(D_80038028==(unsigned short)(expected_count+expected_calls));
}

int main(void)
{
    int a,f,h,s,m;
    static const int free_counts[3]={0,1,3};
    static const int mutations[5]={0,1,2,4,7};
    for(a=0;a<7;a++) for(f=0;f<3;f++) for(h=-1;h<a;h++) for(s=0;s<6;s++) {
        if(h<0 && free_counts[f]==0 && a==0) continue;
        for(m=0;m<5;m++) one(a,free_counts[f],h,s,mutations[m],0);
    }
    one(2,1,-1,1,0,1);
    printf("%d actual-C field-oracle cases passed\n",case_no);
    return 0;
}
