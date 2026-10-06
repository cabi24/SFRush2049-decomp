/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * func_800A3424(handle, active) (historical label) -- set the "in use" byte of a
 * Controller Pak file slot.  handle->object gives the port (+16) and file (+17)
 * (as in func_800A1A60); the pak record is D_80144030[port] (772 bytes: enabled
 * byte +1, flags +9/+10, 16 file slots of 40 bytes at +132).  When the value
 * changes it is stored; a set marks the port as having active files (+10); a clear
 * scans the 16 slots and, if none is still active, clears +10 and, unless +9 is
 * set, the port's enabled byte.  Caller: playgame_state_change.  N64 only.
 * Logic from cloud/work/game_C91 (46/57).
 *
 * Shaping:
 *  - the pak table is declared volatile (it is shared with the SI/rumble thread;
 *    dma_wait_complete reads fields of the same records as volatile).  This gives
 *    retail's `beqzl …; b exit` scan loop and keeps the +10 store before the +9 load;
 *  - no local for handle->object (a local takes v0 and swaps the colouring).
 */
typedef unsigned char u8;
typedef signed char s8;
typedef int s32;

typedef struct Object { u8 opaque[16]; u8 port, file; } Object;
typedef struct Handle { Object *object; } Handle;
typedef struct PakFile40 { s8 active; u8 opaque1[39]; } PakFile40;
typedef struct Pak772 {
    u8 index;
    s8 enabled;
    u8 opaque2[7];
    s8 retained;
    s8 files_active;
    u8 opaque11[121];
    PakFile40 files[16];
} Pak772;

extern volatile Pak772 D_80144030[];

void func_800A3424(Handle *handle, s32 active)
{
    s32 port;
    s32 i;

    if (handle != 0) {
        port = handle->object->port;
        if (active != D_80144030[port].files[handle->object->file].active) {
            D_80144030[port].files[handle->object->file].active = active;
            if (active) {
                D_80144030[port].files_active = 1;
            } else {
                for (i = 0; i < 16; i++) {
                    if (D_80144030[port].files[i].active) {
                        break;
                    }
                }
                if (i >= 16) {
                    D_80144030[port].files_active = 0;
                    if (D_80144030[port].retained == 0) {
                        D_80144030[port].enabled = 0;
                    }
                }
            }
        }
    }
}
