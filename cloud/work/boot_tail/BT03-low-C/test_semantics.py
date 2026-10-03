#!/usr/bin/env python3
"""Host C89 checks of actual sources; packed views are exercised unaligned."""
from pathlib import Path
import subprocess
import tempfile

ROOT = Path(__file__).resolve().parents[4]
PACKET = Path(__file__).resolve().parent
HARNESS = {
'80015574': '''
int main(void) { assert(func_80015574(0, 0, 0, 0) == 0);
    assert(func_80015574(1, 0xFFFFFFFFU, 0x80000000U, 19) == 0); return 0; }
''',
'800156E8': '''
static unsigned short got_group, got_item; static unsigned int got_context, got_option;
static unsigned char got_flag; static int result;
int func_8001558C(unsigned short group, unsigned short item, unsigned int context,
                  unsigned int option, unsigned char flag) {
    got_group = group; got_item = item; got_context = context; got_option = option;
    got_flag = flag; return result;
}
int main(void) {
    result = -1; assert(func_800156E8(65535, 32768, 123, 456) == -1);
    assert(got_group == 65535); assert(got_item == 32768);
    assert(got_context == 123); assert(got_option == 456); assert(got_flag == 0);
    result = 37; assert(func_800156E8(0, 1, 0, 0) == 37); return 0;
}
''',
'80016CE0': '''
int main(void) {
    unsigned short a, b; a = 65535; b = 0;
    assert(func_80016CE0(&a, &b) == 65535);
    assert(func_80016CE0(&b, &a) == -65535);
    assert(func_80016CE0(&a, &a) == 0); return 0;
}
''',
}
for address in ['80016BF8', '80016E40', '80016F58', '80017018']:
    typename = 'PackedLeadingKey' if address == '80017018' else 'PackedKey'
    offset = 0 if address == '80017018' else 4
    HARNESS[address] = '''
struct Holder { unsigned char lead; TYPE value; };
union AlignedHolder { double align; struct Holder holder; };
int main(void) {
    union AlignedHolder a, b; unsigned int i, j;
    unsigned short values[5] = {0, 1, 32767, 32768, 65535};
    assert(offsetof(TYPE, key) == OFFSET);
    assert(offsetof(struct Holder, value) == 1);
    for (i = 0; i < 5; ++i) for (j = 0; j < 5; ++j) {
        a.holder.value.key = values[i]; b.holder.value.key = values[j];
        assert(FUNCTION(&a.holder.value, &b.holder.value) == (int)values[i] - (int)values[j]);
    }
    return 0;
}
'''.replace('TYPE', typename).replace('OFFSET', str(offset)).replace('FUNCTION', 'func_' + address)
with tempfile.TemporaryDirectory(prefix='bt03c-semantics-') as directory:
    for address, harness in HARNESS.items():
        name = 'func_' + address
        source = (PACKET if address == '800156E8' else ROOT / 'cloud/matches/boot_tail') / (name + '.c')
        test = Path(directory) / (name + '.c')
        executable = Path(directory) / name
        test.write_text('#include <assert.h>\n#include <stddef.h>\n' + source.read_text() + '\n' + harness)
        subprocess.run(['cc', '-std=c89', '-pedantic-errors', '-O2', str(test), '-o', str(executable)], check=True)
        subprocess.run([str(executable)], check=True)
        print(name + ': host semantics PASS')
