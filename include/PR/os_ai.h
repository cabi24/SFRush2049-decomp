/**
 * @file os_ai.h
 * @brief N64 Audio Interface (AI) definitions
 */

#ifndef _OS_AI_H_
#define _OS_AI_H_

#include "types.h"

/* Function prototypes */
s32 osAiSetNextBuffer(void *addr, u32 size);
s32 osAiSetFrequency(u32 frequency);

#endif /* _OS_AI_H_ */
