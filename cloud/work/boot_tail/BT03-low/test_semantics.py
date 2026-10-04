#!/usr/bin/env python3
"""Host C89 checks of packet A's documented behavior; not MIPS/ROM proof."""
from pathlib import Path
import subprocess
import tempfile

ROOT = Path(__file__).resolve().parents[4]
PACKET = Path(__file__).resolve().parent
MATCHES = ROOT / 'cloud/matches/boot_tail'
HARNESS = {
    '80014624': '''
struct MessageQueue { int identity; };
MessageQueue D_80038368;
static int calls;
int osRecvMesg(MessageQueue *q, void **out, int blocking) {
    assert(q == &D_80038368); assert(out == 0); assert(blocking == 1);
    ++calls; return -1;
}
int main(void) { func_80014624(); assert(calls == 1); return 0; }
''',
    '800146AC': 'int main(void) { assert(func_800146AC() == 0); return 0; }',
    '800149BC': '''
static unsigned short received;
void func_80011C84(unsigned short value) { received = value; }
int main(void) { func_800149BC(0x1234ABCDU); assert(received == 0xABCD); return 0; }
''',
    '800149DC': '''
AudioSlot slots[3]; AudioSlot *D_80038294 = slots;
int main(void) {
    memset(slots, 0xA5, sizeof(slots)); func_800149DC(1);
    assert(sizeof(AudioSlot) == 104); assert(slots[1].pending == 0);
    assert(slots[1].active == 0xA5); assert(slots[0].pending == 0xA5);
    assert(slots[2].pending == 0xA5); return 0;
}
''',
    '80014A04': 'int main(void) { func_80014A04(123); return 0; }',
    '80014BB0': '''
AudioSlot slots[3]; AudioSlot *D_80038294 = slots;
int main(void) {
    memset(slots, 0xA5, sizeof(slots)); func_80014BB0(1);
    assert(sizeof(AudioSlot) == 104); assert(slots[1].active == 0);
    assert(slots[1].pending == 0xA5); assert(slots[0].active == 0xA5);
    assert(slots[2].active == 0xA5); return 0;
}
''',
    '80014C18': '''
AudioSlot slots[3]; AudioSlot *D_80038294 = slots;
int main(void) {
    assert(sizeof(AudioSlot) == 104); assert(offsetof(AudioSlot, position) == 20);
    slots[1].position = 0xFEDCBA98U; assert(func_80014C18(1) == 0xFEDCBA98U);
    return 0;
}
''',
    '80014C40': '''
static void *received; static int received_size;
void osWritebackDCache(void *p, int n) { received = p; received_size = n; }
int main(void) {
    char buffer[64]; func_80014C40(buffer, 12);
    assert(received == buffer); assert(received_size == 24); return 0;
}
''',
    '80014D1C': '''
unsigned char D_80038291;
int main(void) { D_80038291 = 99; func_80014D1C(); assert(D_80038291 == 1); return 0; }
''',
    '8001467C': '''
AudioSlot slots[3]; AudioSlot *D_80038294 = slots;
int main(void) {
    int i; assert(sizeof(AudioSlot) == 104);
    for (i = 0; i < 256; ++i) { slots[1].active = i; assert(func_8001467C(1) == (i != 0)); }
    return 0;
}
''',
}
with tempfile.TemporaryDirectory(prefix='bt03-semantics-') as directory:
    for address, harness in HARNESS.items():
        name = 'func_' + address
        source = (PACKET if address == '8001467C' else MATCHES) / (name + '.c')
        test = Path(directory) / (name + '.c')
        executable = Path(directory) / name
        test.write_text('#include <assert.h>\n#include <stddef.h>\n#include <string.h>\n' + source.read_text() + '\n' + harness)
        subprocess.run(['cc', '-std=c89', '-pedantic-errors', '-O2', str(test), '-o', str(executable)], check=True)
        subprocess.run([str(executable)], check=True)
        print(name + ': host semantics PASS')
