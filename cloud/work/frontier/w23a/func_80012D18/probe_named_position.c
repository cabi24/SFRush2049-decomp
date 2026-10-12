/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* w23a PROBE (not a candidate): `position = pos` / `excess = frames - end + position` as named u32 locals declared
 * between end/state and state/address. cfe area 68 -> 76 like retail, but uopt copy-propagates both variables, so the
 * two expression webs (CVT w348, add w358) keep their spill homes: 4 homes, frame 128 (retail 120), 582 rows. */
/* w22c from w21c best.c (true 15620 -> 12735): `offset` assigned in every leaf of the address/length chain
 * (6 defs like retail, no top-level offset = 0): trace71 shows its net benefit 4 -> 7 > caller cost 5, so it is
 * coloured t5 like retail instead of split. NEAR MISS, not a submission. */
#include "types.h"
#include "boot_tail_audio_record.h"
typedef struct AudioCacheNode { struct AudioCacheNode *next; struct AudioCacheNode *previous; unsigned int unknown08; unsigned int unknown0C; unsigned int unknown10; unsigned char *buffer; } AudioCacheNode;
typedef struct Fetch_80012D18 { unsigned char *first; unsigned char *second; short first_length; short second_length; } Fetch_80012D18;
typedef struct Voice_80012D18 {
    unsigned char active, pending;
    unsigned short flags;
    unsigned short rate;
    unsigned char unknown06[2];
    double position;
    unsigned char unknown10[8];
    short level[4];
    unsigned char unknown20[12];
    unsigned char *data;
    unsigned int length;
    unsigned int loop_start;
    unsigned int loop_length;
    unsigned int tail;
    short target[4];
    unsigned char unknown48[4];
    float gain;
    unsigned char unknown50[13];
    unsigned char format;
    unsigned char unknown5E[2];
    unsigned char slot;
} Voice_80012D18;
typedef struct Command_80012D18 {
    int volume[4];
    int delta[4];
    unsigned short fraction;
    unsigned short rate;
    Fetch_80012D18 main;
    Fetch_80012D18 loop;
    unsigned char main_blocks, loop_blocks, main_offset, loop_offset;
    union { void *book; struct { short main_count, loop_count; } pcm; } u;
    unsigned int unknown44;
    short end48, end4A, position4C, start4E;
} Command_80012D18;
typedef struct Block_80012D18 { unsigned short count; unsigned short sample; unsigned int changed; void *work; void *loop; Command_80012D18 commands[32]; } Block_80012D18;
extern unsigned short *D_800382D0;
extern unsigned short *D_800382D4;
extern unsigned int D_80038034;
extern unsigned int D_80038030;
extern unsigned int func_8002C5E0(unsigned int, unsigned int);
extern double func_8001E688(double value);
extern void func_80011A64(AudioState *audio, unsigned short samples, unsigned char *active);
extern void func_80011D74(AudioState *audio, unsigned short samples);
extern void *func_80012234(void *);
extern void func_80012730(Fetch_80012D18 *fetch, unsigned int addr, unsigned int length, unsigned int tag);
#define V ((Voice_80012D18 *)audio)
void func_80012D18(AudioState *audio, unsigned short samples, unsigned char index)
{
    unsigned int frames;
    unsigned int end;
    unsigned int position;
    unsigned char state;
    unsigned int excess;
    unsigned int address[2];
    unsigned int length[2];
    int offset;
    int mode;
    unsigned int padded[2];
    Command_80012D18 *cmd;
    unsigned short mainRem;
    unsigned short loopRem;
    double pos;

    cmd = &((Block_80012D18 *)D_800382D0)[*D_800382D4].commands[((Block_80012D18 *)D_800382D0)[*D_800382D4].count];
    D_80038030 += 0x50;
    D_80038034 = func_8002C5E0(D_80038034, D_80038030);
    frames = ((unsigned int)(V->rate * 192 + 0xFFF) >> 12) + 6;
    pos = func_8001E688(V->position);
    end = V->loop_start + V->loop_length;
    state = 2;
    if (V->loop_length != 0 && (V->pending != 0 || V->tail == 0) && end >= (position = pos)) {
        if (end >= position + frames) {
            offset = 0;
            address[0] = position;
            length[0] = frames - 6;
            length[1] = 0;
            mode = -1;
        } else if (V->loop_start >= position) {
            address[0] = position;
            length[1] = 0;
            length[0] = V->loop_start - position + V->loop_length;
            offset = 0;
            mode = V->loop_start - position;
        } else if (V->loop_length < (excess = frames - end + position) || excess >= position - V->loop_start) {
            address[0] = V->loop_start;
            offset = position - V->loop_start;
            length[1] = 0;
            mode = 0;
            length[0] = V->loop_length;
        } else {
            address[0] = position;
            length[0] = end - position;
            length[1] = excess - 6;
            mode = 0;
            address[1] = V->loop_start;
            offset = 0;
        }
    } else {
        mode = -1;
        if ((position = pos) + frames >= V->length) {
            address[0] = position;
            offset = 0;
            length[1] = 0;
            length[0] = V->length - position + 0x20;
            mode = -2;
            state = 0;
        } else {
            address[0] = position;
            offset = 0;
            length[0] = frames - 6;
            length[1] = 0;
        }
    }
    padded[0] = length[0] + 6;
    if (length[1] != 0) {
        padded[1] = length[1] + 6;
    }
    if (V->flags & 1) {
        switch (V->format) {
        case 3:
            if (length[1] != 0) {
                loopRem = address[1] & 0x3F;
                func_80012730(&cmd->loop, (address[1] >> 6) * 40 + (unsigned int)V->data + 0x100, ((((padded[1] + (loopRem & 0x1F) + 0x1F) >> 5) + (loopRem >> 5) + 1) >> 1) * 40, (unsigned int)V->data + 0x100);
                cmd->loop_offset = loopRem;
                loopRem &= 0x1F;
                cmd->loop_blocks = (padded[1] + loopRem + 0x1F) >> 5;
            } else {
                cmd->loop.first = 0;
                cmd->loop_blocks = 0;
            }
            mainRem = address[0] & 0x3F;
            func_80012730(&cmd->main, (address[0] >> 6) * 40 + (unsigned int)V->data + 0x100, ((((padded[0] + (mainRem & 0x1F) + 0x1F) >> 5) + (mainRem >> 5) + 1) >> 1) * 40, (unsigned int)V->data + 0x100);
            cmd->u.book = func_80012234(V->data);
            cmd->main_offset = mainRem;
            mainRem &= 0x1F;
            cmd->main_blocks = (padded[0] + mainRem + 0x1F) >> 5;
            break;
        case 0:
            if (length[1] != 0) {
                loopRem = address[1] & 3;
                func_80012730(&cmd->loop, (address[1] & 0xFFFC) * 2 + (unsigned int)V->data, ((padded[1] + loopRem + 3) & 0xFFFC) * 2, (unsigned int)V->data);
                cmd->u.pcm.loop_count = padded[1];
                cmd->loop_offset = loopRem;
            } else {
                cmd->loop.first = 0;
                cmd->u.pcm.loop_count = 0;
            }
            mainRem = address[0] & 3;
            func_80012730(&cmd->main, (address[0] & 0xFFFC) * 2 + (unsigned int)V->data, ((padded[0] + mainRem + 3) & 0xFFFC) * 2, (unsigned int)V->data);
            cmd->main_blocks = 0;
            cmd->main_offset = mainRem;
            cmd->u.pcm.main_count = padded[0];
            break;
        }
    } else {
        switch (V->format) {
        case 3:
            if (length[1] != 0) {
                loopRem = address[1] & 0x3F;
                cmd->loop.first = (address[1] >> 6) * 40 + V->data + 0x100;
                cmd->loop.second_length = 0;
                cmd->loop_offset = loopRem;
                cmd->loop.first_length = ((((padded[1] + (loopRem & 0x1F) + 0x1F) >> 5) + (loopRem >> 5) + 1) >> 1) * 40;
                loopRem &= 0x1F;
                cmd->loop_blocks = (padded[1] + loopRem + 0x1F) >> 5;
            } else {
                cmd->loop.first = 0;
                cmd->loop_blocks = 0;
            }
            mainRem = address[0] & 0x3F;
            cmd->main.first = (address[0] >> 6) * 40 + V->data + 0x100;
            cmd->main.second_length = 0;
            cmd->main.first_length = ((((padded[0] + (mainRem & 0x1F) + 0x1F) >> 5) + (mainRem >> 5) + 1) >> 1) * 40;
            cmd->u.book = V->data;
            cmd->main_offset = mainRem;
            mainRem &= 0x1F;
            cmd->main_blocks = (padded[0] + mainRem + 0x1F) >> 5;
            break;
        case 0:
            if (length[1] != 0) {
                loopRem = address[1] & 3;
                cmd->loop.first = (address[1] & 0xFFFC) * 2 + V->data;
                cmd->loop.second_length = 0;
                cmd->loop.first_length = ((padded[1] + loopRem + 3) & 0xFFFC) * 2;
                cmd->u.pcm.loop_count = padded[1];
                cmd->loop_offset = loopRem;
            } else {
                cmd->loop.first = 0;
                cmd->u.pcm.loop_count = 0;
            }
            cmd->main.first = (address[0] & 0xFFFC) * 2 + V->data;
            mainRem = address[0] & 3;
            cmd->main.second_length = 0;
            cmd->main.first_length = ((padded[0] + mainRem + 3) & 0xFFFC) * 2;
            cmd->main_blocks = 0;
            cmd->main_offset = mainRem;
            cmd->u.pcm.main_count = padded[0];
            break;
        }
    }
    cmd->start4E = offset;
    if (mode == -1) {
        cmd->end48 = 0x4000;
    } else if (mode == -2) {
        cmd->end48 = length[0] + mainRem;
        cmd->end4A = length[0] + mainRem - 0x20;
    } else {
        cmd->end48 = length[0] + mainRem;
        if (cmd->loop.first != 0) {
            cmd->end4A = (loopRem + mode) | 0x8000;
        } else {
            cmd->end4A = mainRem + mode;
        }
    }
    V->slot = index;
    func_80011A64(audio, 192, &state);
    if (V->active == 1) {
        V->level[0] = V->target[0] * V->gain;
        cmd->volume[0] = V->level[0] << 16;
        V->level[1] = V->target[1] * V->gain;
        cmd->volume[1] = V->level[1] << 16;
        V->level[2] = V->target[2] * V->gain;
        cmd->volume[2] = V->level[2] << 16;
        V->level[3] = V->target[3] * V->gain;
        cmd->volume[3] = V->level[3] << 16;
        cmd->delta[0] = 0;
        cmd->delta[1] = 0;
        cmd->delta[2] = 0;
        cmd->delta[3] = 0;
    } else {
        cmd->volume[0] = V->level[0] << 16;
        cmd->volume[1] = V->level[1] << 16;
        cmd->volume[2] = V->level[2] << 16;
        cmd->volume[3] = V->level[3] << 16;
        V->level[0] = V->target[0] * V->gain;
        cmd->delta[0] = ((V->level[0] << 16) - cmd->volume[0]) / 192;
        V->level[1] = V->target[1] * V->gain;
        cmd->delta[1] = ((V->level[1] << 16) - cmd->volume[1]) / 192;
        V->level[2] = V->target[2] * V->gain;
        cmd->delta[2] = ((V->level[2] << 16) - cmd->volume[2]) / 192;
        V->level[3] = V->target[3] * V->gain;
        cmd->delta[3] = ((V->level[3] << 16) - cmd->volume[3]) / 192;
    }
    cmd->rate = V->rate;
    cmd->fraction = (V->position - func_8001E688(V->position)) * 65536.0;
    cmd->unknown44 = 0;
    cmd->position4C = padded[0] * (V->rate * 16) + cmd->fraction;
    ((Block_80012D18 *)D_800382D0)[*D_800382D4].count++;
    func_80011D74(audio, 192);
    V->active = state;
}
