"""Apply named edit sets to a base source and score each with obj71 (trace71 toolkit)."""
import subprocess, sys, re
from pathlib import Path
D = Path("cloud/work/frontier/z12d18")

EDITS = {
 'r_excess': [('    unsigned int excess;\n', '    register unsigned int excess;\n')],
 'r_frames': [('    unsigned int frames;\n', '    register unsigned int frames;\n')],
 'i_frames': [('    unsigned int frames;\n', '    int frames;\n')],
 'f_twostep': [('    frames = ((unsigned int)(V->rate * 192 + 0xFFF) >> 12) + 6;\n', '    frames = (unsigned int)(V->rate * 192 + 0xFFF) >> 12;\n    frames += 6;\n')],

 'x_init0': [('    unsigned int excess;\n', '    unsigned int excess = 0;\n')],
 'x_twostep': [('        } else if (V->loop_start >= position) {', '        } else if (excess = frames - end, excess += position, V->loop_start >= position) {'), ('        } else if (V->loop_length < (excess = frames - end + position) || excess >= position - V->loop_start) {', '        } else if (V->loop_length < excess || excess >= position - V->loop_start) {')],
 'x_twostep_b': [('        } else if (V->loop_length < (excess = frames - end + position) || excess >= position - V->loop_start) {', '        } else if (excess = frames - end, V->loop_length < (excess += position) || excess >= position - V->loop_start) {')],

 'x_int': [('    unsigned int excess;\n', '    int excess;\n')],

 't_u44': [('    cmd->unknown44 = 0;\n    cmd->position4C = padded[0] * (V->rate * 16) + cmd->fraction;', '    cmd->position4C = padded[0] * (V->rate * 16) + cmd->fraction;\n    cmd->unknown44 = 0;')],

 'a_loop1': [('                cmd->loop.first = (address[1] >> 6) * 40 + V->data + 0x100;\n                cmd->loop.second_length = 0;\n                cmd->loop_offset = loopRem;\n                cmd->loop.first_length = ((((padded[1] + (loopRem & 0x1F) + 0x1F) >> 5) + loopRem / 32 + 1) >> 1) * 40;', '                cmd->loop.first = (address[1] >> 6) * 40 + V->data + 0x100;\n                cmd->loop.first_length = ((((padded[1] + (loopRem & 0x1F) + 0x1F) >> 5) + loopRem / 32 + 1) >> 1) * 40;\n                cmd->loop.second_length = 0;\n                cmd->loop_offset = loopRem;')],
 'a_loop2': [('                cmd->loop.first = (address[1] >> 6) * 40 + V->data + 0x100;\n                cmd->loop.second_length = 0;\n                cmd->loop_offset = loopRem;\n                cmd->loop.first_length = ((((padded[1] + (loopRem & 0x1F) + 0x1F) >> 5) + loopRem / 32 + 1) >> 1) * 40;', '                cmd->loop.first = (address[1] >> 6) * 40 + V->data + 0x100;\n                cmd->loop_offset = loopRem;\n                cmd->loop.second_length = 0;\n                cmd->loop.first_length = ((((padded[1] + (loopRem & 0x1F) + 0x1F) >> 5) + loopRem / 32 + 1) >> 1) * 40;')],
 'a_main1': [('            cmd->main.first = (address[0] >> 6) * 40 + V->data + 0x100;\n            cmd->main.second_length = 0;\n            cmd->main.first_length = ((((padded[0] + (mainRem & 0x1F) + 0x1F) >> 5) + mainRem / 32 + 1) >> 1) * 40;', '            cmd->main.first = (address[0] >> 6) * 40 + V->data + 0x100;\n            cmd->main.first_length = ((((padded[0] + (mainRem & 0x1F) + 0x1F) >> 5) + mainRem / 32 + 1) >> 1) * 40;\n            cmd->main.second_length = 0;')],

 'probe_u32': [('    unsigned short loopRem;\n', '    unsigned short loopRem;\n    unsigned int probe;\n')],
 'probe_u16': [('    unsigned short loopRem;\n', '    unsigned short loopRem;\n    unsigned short probe;\n')],

 'x_stmt': [('        } else if (V->loop_start >= position) {', '        } else {\n            excess = frames - end + position;\n            if (V->loop_start >= position) {'), ('        } else if (V->loop_length < (excess = frames - end + position) || excess >= position - V->loop_start) {', '        } else if (V->loop_length < excess || excess >= position - V->loop_start) {'), ('            mode = 0;\n        }\n    } else {\n        mode = -1;', '            mode = 0;\n        }\n        }\n    } else {\n        mode = -1;')],
 'x_flip': [('        } else if (V->loop_length < (excess = frames - end + position) || excess >= position - V->loop_start) {', '        } else if ((excess = frames - end + position) > V->loop_length || excess >= position - V->loop_start) {')],

 'b3a': [('            offset = position - V->loop_start;\n            address[0] = V->loop_start;\n            length[1] = 0;\n            mode = 0;\n            length[0] = V->loop_length;', '            offset = position - V->loop_start;\n            address[0] = V->loop_start;\n            length[0] = V->loop_length;\n            length[1] = 0;\n            mode = 0;')],
 'b3c': [('            offset = position - V->loop_start;\n            address[0] = V->loop_start;\n            length[1] = 0;\n            mode = 0;\n            length[0] = V->loop_length;', '            offset = position - V->loop_start;\n            address[0] = V->loop_start;\n            mode = 0;\n            length[1] = 0;\n            length[0] = V->loop_length;')],
 'b3d': [('            offset = position - V->loop_start;\n            address[0] = V->loop_start;\n            length[1] = 0;\n            mode = 0;\n            length[0] = V->loop_length;', '            offset = position - V->loop_start;\n            address[0] = V->loop_start;\n            length[0] = V->loop_length;\n            mode = 0;\n            length[1] = 0;')],
 'b4a': [('            offset = 0;\n            address[0] = position;\n            length[0] = end - position;\n            length[1] = excess - 6;\n            mode = 0;\n            address[1] = V->loop_start;', '            offset = 0;\n            address[0] = position;\n            length[0] = end - position;\n            address[1] = V->loop_start;\n            length[1] = excess - 6;\n            mode = 0;')],
 'b4b': [('            offset = 0;\n            address[0] = position;\n            length[0] = end - position;\n            length[1] = excess - 6;\n            mode = 0;\n            address[1] = V->loop_start;', '            offset = 0;\n            address[0] = position;\n            length[0] = end - position;\n            mode = 0;\n            address[1] = V->loop_start;\n            length[1] = excess - 6;')],
 'e1a': [('            address[0] = position;\n            offset = 0;\n            length[1] = 0;\n            length[0] = V->length - position + 0x20;\n            mode = -2;\n            state = 0;', '            address[0] = position;\n            offset = 0;\n            length[0] = V->length - position + 0x20;\n            length[1] = 0;\n            mode = -2;\n            state = 0;')],
 'e1b': [('            address[0] = position;\n            offset = 0;\n            length[1] = 0;\n            length[0] = V->length - position + 0x20;\n            mode = -2;\n            state = 0;', '            address[0] = position;\n            offset = 0;\n            length[1] = 0;\n            length[0] = V->length - position + 0x20;\n            state = 0;\n            mode = -2;')],
 'd1': [('    } else {\n        mode = -1;\n        if (position + frames >= V->length) {', '    } else {\n        offset = 0;\n        mode = -1;\n        if (position + frames >= V->length) {'), ('            address[0] = position;\n            offset = 0;\n            length[0] = frames - 6;\n            length[1] = 0;\n        }', '            address[0] = position;\n            length[0] = frames - 6;\n            length[1] = 0;\n        }')],

 "b2_order": [("""            address[0] = position;
            length[1] = 0;
            length[0] = V->loop_start - position + V->loop_length;
            offset = 0;
            mode = V->loop_start - position;""",
"""            address[0] = position;
            length[0] = V->loop_start - position + V->loop_length;
            length[1] = 0;
            offset = 0;
            mode = V->loop_start - position;""")],
 "b3_offset_first": [("""            address[0] = V->loop_start;
            offset = position - V->loop_start;
            length[1] = 0;
            mode = 0;
            length[0] = V->loop_length;""",
"""            offset = position - V->loop_start;
            address[0] = V->loop_start;
            length[1] = 0;
            mode = 0;
            length[0] = V->loop_length;""")],
 "b4_offset_first": [("""            address[0] = position;
            length[0] = end - position;
            length[1] = excess - 6;
            mode = 0;
            address[1] = V->loop_start;
            offset = 0;""",
"""            offset = 0;
            address[0] = position;
            length[0] = end - position;
            length[1] = excess - 6;
            mode = 0;
            address[1] = V->loop_start;""")],
}

def score(path):
    out = subprocess.run(["sh", "cloud/work/frontier/tools/trace71/obj71.sh", str(path), "func_80012D18"],
                         capture_output=True, text=True, env={**__import__("os").environ, "SCR": "rush2049/scratch/frontier/z12d18"})
    m = re.search(r"rows: want (\d+) got (\d+); differing rows (\d+)", out.stdout + out.stderr)
    return m.groups() if m else ("?", "?", (out.stdout + out.stderr)[-300:])

def apply(src, names):
    for n in names:
        for a, b in EDITS[n]:
            assert src.count(a) == 1, (n, a[:50]); src = src.replace(a, b)
    return src

if __name__ == "__main__":
    base = Path(sys.argv[1]).read_text()
    for spec in sys.argv[2:]:
        names = [n for n in spec.split("+") if n]
        out = D / ("var_" + (spec.replace("+", "_") or "base") + ".c")
        out.write_text(apply(base, names))
        print(f"{spec or 'base':40s} {score(out)}", flush=True)
