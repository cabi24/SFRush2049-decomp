/* flags: -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef int s32;
typedef void *OSMesg;
typedef struct OSMesgQueue { void *mtqueue; void *fullqueue; s32 validCount, first, msgCount; OSMesg *msg; } OSMesgQueue;
typedef struct BufferHandle { u8 *data; } BufferHandle;
typedef struct Resource Resource;
typedef struct ResourceNode { Resource *resource; } ResourceNode;
struct Resource {
    ResourceNode *next;
    u8 unknown04[12];
    u8 channel, file_no;
    u8 unknown12[46];
    u32 size;
    u32 unknown44;
    BufferHandle *handle;
};
extern u8 D_80144030[];
extern ResourceNode *D_80144D68[];
extern s8 D_8011194C, D_8011EAE8;
extern OSMesgQueue D_801497D0, D_80152770;
extern OSMesg D_801527E4;
extern u8 D_8008A6A4[];
extern void (*D_80144008)(s32, s32, s32, s32, s8 *, s8 *, u8 *);
extern BufferHandle *audio_task_complete(void *, s32);
extern void *memset(void *, s32, u32);
extern void osInvalDCache(void *, s32);
extern void osCreateMesgQueue(OSMesgQueue *, OSMesg *, s32);
extern s32 osJamMesg(OSMesgQueue *, OSMesg, s32);
extern s32 osRecvMesg(OSMesgQueue *, OSMesg *, s32);
/* Historical symbol: observed retail ABI is osPfsReadWriteFile. */
extern s32 osPfsGetFileSize(void *, s32, s32, s32, s32, void *);
extern s32 func_800A1E94(s32);
extern void AdjustSpeed(ResourceNode *);
extern void audio_reverb_update(void *, BufferHandle *, s32);
void drone_set_catchup(ResourceNode *node, s32 offset, s32 count) {
    Resource *resource;
    u8 channel, file_no;
    u8 *file_record;
    u8 *control;
    s32 rounded_count, result;
    s8 retry, callback_byte;
    OSMesg first_message, second_message;
    ResourceNode *current;
    BufferHandle *handle;
    resource = node->resource;
    channel = resource->channel;
    file_no = resource->file_no;
    if (resource->handle == (void *)0) {
        file_record = D_80144030 + channel * 772 + file_no * 40;
        resource->handle = audio_task_complete((void *)0, *(u16 *)(file_record + 136) + resource->size);
        memset(resource->handle->data + resource->size, 0, *(u16 *)(file_record + 136));
        osInvalDCache(resource->handle->data, resource->size);
        if (D_8011194C == 0) {
            D_8011194C = 1;
            osCreateMesgQueue(&D_801497D0, &D_801527E4, 1);
            osJamMesg(&D_801497D0, (void *)0, 0);
        }
        osRecvMesg(&D_801497D0, &first_message, 1);
        control = D_80144030 + channel * 772;
        rounded_count = ((u32)(count + 31) >> 5) << 5;
        for (;;) {
            result = osPfsGetFileSize(control + 12, file_no, 0, offset, rounded_count, resource->handle->data);
            if (result == 0) break;
            *(s32 *)(control + 116) = func_800A1E94(result);
            if (*(s32 *)(control + 116) == 9) {
                osJamMesg(&D_801497D0, (void *)0, 0);
                AdjustSpeed(node);
                return;
            }
            osJamMesg(&D_801497D0, (void *)0, 0);
            D_8011EAE8 = channel;
            D_80144008(channel, *(s32 *)(control + 116), 0, 0, &retry, &callback_byte, D_8008A6A4);
            D_8011EAE8 = -1;
            if (D_8011194C == 0) {
                D_8011194C = 1;
                osCreateMesgQueue(&D_801497D0, &D_801527E4, 1);
                osJamMesg(&D_801497D0, (void *)0, 0);
            }
            osRecvMesg(&D_801497D0, &second_message, 1);
            if (retry == 0) {
                current = *(ResourceNode **)((u8 *)D_80144D68 + channel * 16);
                while (current != (void *)0) {
                    if (node == current) {
                        handle = resource->handle;
                        if (handle != (void *)0) {
                            osRecvMesg(&D_80152770, (void *)0, 1);
                            audio_reverb_update((void *)0, handle, 0);
                            osJamMesg(&D_80152770, (void *)0, 0);
                            resource->handle = (void *)0;
                        }
                        break;
                    }
                    current = current->resource->next;
                }
                osJamMesg(&D_801497D0, (void *)0, 0);
                return;
            }
        }
        osJamMesg(&D_801497D0, (void *)0, 0);
        *(s8 *)(file_record + 134) = count != resource->size;
    }
}
