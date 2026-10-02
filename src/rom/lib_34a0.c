/*-
 * Copyright (c) 1990 The Regents of the University of California.
 * All rights reserved.
 *
 * This code is derived from software contributed to Berkeley by
 * Chris Torek.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *	This product includes software developed by the University of
 *	California, Berkeley and its contributors.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */
/* GENERATED ROM-aligned TU — segment 0x34a0 (rom/lib_34a0)
 * layout map f54dc176ce4855b86d3262eb2d2e0a66fa10aa93d8447be4d46e417ab6c9dc75; regenerate via `pipeline.layout convert`.
 * Slots are GLOBAL_ASM passthroughs until promoted; do not hand-edit
 * passthrough lines. */
#include "static_debug_context.h"

#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_34a0/modf.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_34a0/modff.s")
/* PROMOTED 2026-10-02 — __isinf
 * Source:   cloud/work/static_debug_consolidation/acceptance_sources/__isinf.c (in-repo, locked)
 * Flags:    -g1 -O1 -mips2 -G 0 -non_shared -Wab,-r4300_mul
 * Evidence: lock:cloud/work/static_debug_consolidation/acceptance_sources/__isinf.c:__isinf (score0)
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
int __isinf(register double x) { DoubleUnion v; v.value=x; if(v.bits.exponent==2047) { v.bits.exponent=0; return v.value != 0.0; } return 0; }

/* PROMOTED 2026-10-02 — __isnan
 * Source:   cloud/work/static_debug_consolidation/acceptance_sources/__isnan.c (in-repo, locked)
 * Flags:    -g1 -O1 -mips2 -G 0 -non_shared -Wab,-r4300_mul
 * Evidence: lock:cloud/work/static_debug_consolidation/acceptance_sources/__isnan.c:__isnan (score0)
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
int __isnan(register double x) { DoubleUnion v; v.value=x; if(v.bits.exponent==2047) { v.bits.exponent=0; return v.value == 0.0; } return 0; }

#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_34a0/fcvt.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_34a0/__ecvt_internal.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_34a0/__round_helper.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_34a0/__write_exponent.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_34a0/sprintf.s")
