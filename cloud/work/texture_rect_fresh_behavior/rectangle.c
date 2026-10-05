/* A clean, behavior-first formulation. Not a matching-source claim.
 * sdk_context.h is generated from pinned SDK headers by verify_behavior.py.
 * Ordinary valid domain: disjoint globals and aligned packet storage, no races.
 * Unsigned arithmetic deliberately models the native 32-bit modular operations.
 */
#include "sdk_context.h"

extern Gfx *D_80149438;
extern int D_8012E608, D_8012E60C, D_8012E610;
extern int D_8012E668, D_8012E674, D_8014A248;

void func_80087110(int x, int y, int right, int bottom, int s, int t)
{
    unsigned int flags = D_8012E608;
    unsigned int tex_s = s;
    unsigned int tex_t = t;
    unsigned int height;
    unsigned int command_right;
    unsigned int command_bottom;
    unsigned int ds = 4096;
    unsigned int dt = 1024;
    unsigned int t_phase = 0;

    if (x < D_8012E60C) {
        if (!(flags & 4))
            tex_s += (unsigned int)D_8012E60C - (unsigned int)x;
        x = D_8012E60C;
    }
    if (y < D_8012E668) {
        if (!(flags & 8))
            tex_t += (unsigned int)D_8012E668 - (unsigned int)y;
        y = D_8012E668;
    }
    if (right > D_8012E610) {
        if (flags & 4)
            tex_s += (unsigned int)right - (unsigned int)D_8012E610;
        right = D_8012E610;
    }
    if (bottom > D_8012E674) {
        if (flags & 8)
            tex_t += (unsigned int)bottom - (unsigned int)D_8012E674;
        bottom = D_8012E674;
    }
    if (right < x || bottom < y)
        return;

    height = (unsigned int)bottom - (unsigned int)y;
    if (flags & 4)
        tex_s += (unsigned int)right - (unsigned int)x;
    if (flags & 8)
        tex_t += height;

    command_right = right;
    command_bottom = bottom;
    if (D_8014A248 != 0) {
        ds = 1024;
        ++command_right;
        ++command_bottom;
        if (flags & 0x8000) {
            command_bottom += height + 1;
            dt = 512;
            if (flags & 8)
                t_phase = 16;
        }
    }
    if (flags & 4)
        ds = 0U - ds;
    if (flags & 8)
        dt = 0U - dt;

    gSPTextureRectangle(D_80149438++, (unsigned int)x << 2,
                        (unsigned int)y << 2, command_right << 2,
                        command_bottom << 2, 0, tex_s << 5,
                        (tex_t << 5) + t_phase, ds, dt);
}
