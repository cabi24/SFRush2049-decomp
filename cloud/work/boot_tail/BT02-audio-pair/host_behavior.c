#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct AudioBufferNode {
    struct AudioBufferNode *next;
    struct AudioBufferNode *previous;
    unsigned char *buffer;
    unsigned int unknown0C;
    unsigned int unknown10;
} AudioBufferNode;
typedef struct AudioCacheNode {
    struct AudioCacheNode *next;
    struct AudioCacheNode *previous;
    unsigned int unknown08;
    unsigned int unknown0C;
    unsigned int unknown10;
    unsigned char *buffer;
} AudioCacheNode;
AudioBufferNode *D_80038338;
unsigned char *D_8003833C;
unsigned int D_80038340;
AudioBufferNode *D_80038344;
AudioBufferNode *D_80038348;
unsigned char *D_8003834C;
AudioCacheNode *D_80038350;
AudioCacheNode *D_80038354;
AudioCacheNode *D_80038358;
AudioCacheNode *D_8003835C;
static AudioBufferNode prior_buffer_node;
static AudioCacheNode prior_cache_node;
static unsigned char *blocks[2];
static unsigned int sizes[2];
static unsigned int allocations;
static unsigned int invalidations;
static unsigned int count;
static unsigned int family;
static unsigned int calls;
static void *allocate(unsigned int bytes, unsigned int mode)
{
    unsigned int wanted;
    assert(mode == 0 && allocations < 2);
    if (family == 0) {
        assert(invalidations == 0);
        wanted = allocations == 0 ? count * sizeof(AudioBufferNode) : count * 256;
    } else {
        assert(invalidations == allocations);
        wanted = allocations == 0 ? count * 1536 : count * sizeof(AudioCacheNode);
    }
    assert(bytes == wanted);
    blocks[allocations] = malloc(bytes + 32);
    assert(blocks[allocations] != NULL);
    memset(blocks[allocations], 0xA5, bytes + 32);
    sizes[allocations] = bytes;
    return blocks[allocations++] + 16;
}
void *(*D_80038018)(unsigned int, unsigned int) = allocate;
void func_800084E0(void *buffer, int bytes)
{
    unsigned int expected_allocation;
    assert(invalidations == 0);
    expected_allocation = family == 0 ? 1 : 0;
    assert(allocations == expected_allocation + 1);
    assert(buffer == blocks[expected_allocation] + 16);
    assert(bytes == (int)(count * (family == 0 ? 256 : 1536)));
    assert(((unsigned char *)buffer)[0] == 0xA5);
    assert(((unsigned char *)buffer)[bytes - 1] == 0xA5);
    invalidations++;
}
void func_80011F60(int);
void func_800123A8(unsigned short);
static void one(unsigned int n, unsigned int kind)
{
    unsigned int i;
    unsigned int j;
    count = n;
    family = kind;
    allocations = invalidations = 0;
    D_80038340 = 0x12345678U;
    D_80038344 = D_80038348 = &prior_buffer_node;
    D_80038354 = D_80038358 = D_8003835C = &prior_cache_node;
    if (kind == 0) {
        func_80011F60((int)n);
        assert(D_80038338 == (void *)(blocks[0] + 16));
        assert(D_8003833C == blocks[1] + 16);
        assert(D_80038340 == 0 && D_80038344 == NULL);
        assert(D_80038348 == D_80038338);
        for (i = 0; i < n; i++) {
            assert(D_80038338[i].next == (i + 1 < n ? &D_80038338[i + 1] : NULL));
            assert(D_80038338[i].previous == (i > 0 ? &D_80038338[i - 1] : NULL));
            assert(D_80038338[i].buffer == D_8003833C + i * 256);
            assert(D_80038338[i].unknown0C == 0xA5A5A5A5U);
            assert(D_80038338[i].unknown10 == 0xA5A5A5A5U);
        }
    } else {
        func_800123A8((unsigned short)n);
        assert(D_8003834C == blocks[0] + 16);
        assert(D_80038350 == (void *)(blocks[1] + 16));
        assert(D_80038354 == NULL && D_80038358 == NULL);
        assert(D_8003835C == D_80038350);
        for (i = 0; i < n; i++) {
            assert(D_80038350[i].next == (i + 1 < n ? &D_80038350[i + 1] : NULL));
            assert(D_80038350[i].previous == (i > 0 ? &D_80038350[i - 1] : NULL));
            assert(D_80038350[i].buffer == D_8003834C + i * 1536);
            assert(D_80038350[i].unknown08 == 0xA5A5A5A5U);
            assert(D_80038350[i].unknown0C == 0xA5A5A5A5U);
            assert(D_80038350[i].unknown10 == 0xA5A5A5A5U);
        }
        assert(D_80038340 == 0x12345678U);
    }
    assert(allocations == 2 && invalidations == 1);
    for (j = 0; j < 2; j++) {
        for (i = 0; i < 16; i++) {
            assert(blocks[j][i] == 0xA5);
            assert(blocks[j][16 + sizes[j] + i] == 0xA5);
        }
        free(blocks[j]);
    }
    calls++;
}
int main(void)
{
    unsigned int i;
    unsigned int family_index;
    for (family_index = 0; family_index < 2; family_index++) {
        for (i = 1; i <= 257; i++) {
            one(i, family_index);
        }
        one(511, family_index);
        one(512, family_index);
        one(1023, family_index);
        one(65535, family_index);
    }
    printf("PASS: %u actual-source calls; list links, allocator/cache order, extents and canaries\n", calls);
    return 0;
}
