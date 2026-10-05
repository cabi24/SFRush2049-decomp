#include "candidate.c"
#define ASSERT(name, condition) typedef char name[(condition) ? 1 : -1]
#define OFF(type, field) ((unsigned int)&((type *)0)->field)
ASSERT(pointer_is_4, sizeof(void *) == 4);
ASSERT(ostask_is_64, sizeof(OSTask) == 64);
ASSERT(ossc_task_is_88, sizeof(OSScTask) == 88);
ASSERT(next_at_0, OFF(OSScTask, next) == 0);
ASSERT(state_at_4, OFF(OSScTask, state) == 4);
ASSERT(flags_at_8, OFF(OSScTask, flags) == 8);
ASSERT(framebuffer_at_12, OFF(OSScTask, framebuffer) == 12);
ASSERT(list_at_16, OFF(OSScTask, list) == 16);
ASSERT(queue_at_80, OFF(OSScTask, msgQueue) == 80);
ASSERT(message_at_84, OFF(OSScTask, msg) == 84);
