/**
 * @file audio.c
 * @brief Audio system for Rush 2049 N64
 *
 * Based on arcade audio system
 * Handles sound effects, music, engine sounds
 *
 * Key functions:
 * - func_800B37E8 (sound_control) - main audio dispatcher
 * - func_800B358C (sound_stop) - stop sounds
 * - func_8008A6D0 (audio_msg_send) - send audio messages
 * - func_8008A710 (audio_task_sync) - audio task sync
 */

#include "types.h"

/* External declarations */
extern u8 gstate;
extern u32 frame_counter;

/* Sound command constants */
#define SND_STOP        -1
#define SND_ENGINE      0x100
#define SND_COLLISION   0x200
#define SND_CHECKPOINT  0x300

/**
 * Main audio control function
 * Address: 0x800B37E8
 *
 * Called every frame to process audio state.
 * -1 parameter stops all sounds.
 */
void audio_control_update(void);  /* TODO: Decompile */

/**
 * Stop sound
 * Address: 0x800B358C
 *
 * Called when stopping a specific sound channel.
 */
void sound_stop(void);  /* TODO: Decompile */

/* External OS functions */
extern s32 osJamMesg(void *mq, void *msg, s32 flags);

/* Audio message queue (0x801597D0) */
extern void *audio_msg_queue;

/**
 * Send audio message via OS message queue
 * Address: 0x8008A6D0
 * Size: 44 bytes
 *
 * Sends a null message to the audio thread's message queue
 * to trigger audio processing.
 */
void audio_msg_send(void) {
    osJamMesg(audio_msg_queue, NULL, 0);  /* OS_MESG_NOBLOCK */
}

/**
 * Audio task synchronization
 * Address: 0x8008A710
 *
 * Synchronizes audio task with main thread.
 */
void audio_task_sync(void);  /* TODO: Decompile */
