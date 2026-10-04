/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef struct OSMesgQueue_s OSMesgQueue;
extern void func_80010714(void *, void *, unsigned int);
extern int osJamMesg(OSMesgQueue *, void *, int);

void func_8002506C(void *source, void *destination, unsigned int count,
                   OSMesgQueue *queue)
{
    func_80010714(destination, source, count);
    osJamMesg(queue, 0, 1);
}
