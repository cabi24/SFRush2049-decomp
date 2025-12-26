/**
 * audio.h - Audio system for Rush 2049 N64
 *
 * Based on arcade audio system
 * Handles sound effects, music, engine sounds
 *
 * Key functions:
 * - func_800B37E8 (sound_control) - main audio dispatcher
 * - func_800B358C (sound_stop) - stop sounds
 * - audio_msg_send - send audio messages via OS
 * - func_8008A710 (audio_task_sync) - audio task sync
 */

#ifndef AUDIO_H
#define AUDIO_H

#include "types.h"

/* Sound command constants */
#define SND_STOP        (-1)
#define SND_ENGINE      0x100
#define SND_COLLISION   0x200
#define SND_CHECKPOINT  0x300

/* External audio message queue */
extern void *audio_msg_queue;

/**
 * Main audio control function
 * Address: 0x800B37E8
 *
 * Called every frame to process audio state.
 * -1 parameter stops all sounds.
 */
void func_800B37E8(void);  /* TODO: Decompile */

/**
 * Stop sound
 * Address: 0x800B358C
 *
 * Called when stopping a specific sound channel.
 */
void func_800B358C(void);  /* TODO: Decompile */

/**
 * Send audio message via OS message queue
 * Address: 0x8008A6D0
 * Size: 44 bytes
 *
 * Sends a null message to the audio thread's message queue
 * to trigger audio processing.
 */
void audio_msg_send(void);

/**
 * Audio task synchronization
 * Address: 0x8008A710
 *
 * Synchronizes audio task with main thread.
 */
void func_8008A710(void);  /* TODO: Decompile */

#endif /* AUDIO_H */
