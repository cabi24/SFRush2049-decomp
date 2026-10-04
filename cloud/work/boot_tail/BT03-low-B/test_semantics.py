#!/usr/bin/env python3
"""Host C89 behavior checks; no native or ROM-execution claim."""
from pathlib import Path
import subprocess
import tempfile

ROOT = Path(__file__).resolve().parents[4]
PACKET = Path(__file__).resolve().parent
HARNESS = {
'80014E00': '''
unsigned char D_8002C604[4];
int main(void) { assert(func_80014E00() == D_8002C604); return 0; }
''',
'80014E10': '''
unsigned short D_80038390;
int main(void) { D_80038390 = 65535; func_80014E10(); assert(D_80038390 == 0); return 0; }
''',
'80015318': '''
static unsigned short received_id, received_count; static void *received_data;
int func_800164D0(unsigned short id, void *data, unsigned short count) {
    received_id = id; received_data = data; received_count = count; return 1;
}
int main(void) {
    unsigned short data[4]; int i;
    for (i = 0; i < 3; ++i) {
        data[0] = i == 2 ? 65535 : i; func_80015318(65535, data);
        assert(received_id == 65535); assert(received_data == data + 2);
        assert(received_count == data[0]);
    }
    return 0;
}
''',
'80015348': '''
static unsigned short received_id;
int func_8001661C(unsigned short id) { received_id = id; return 1; }
int main(void) { func_80015348(65535); assert(received_id == 65535); return 0; }
''',
}
for index, address in enumerate(['80014E64', '80014E90', '80014EBC', '80014EE8']):
    HARNESS[address] = '''
static unsigned short received_id; static void *received_data; static void *result;
void *func_80014E1C(unsigned short id, void *data) {
    received_id = id; received_data = data; return result;
}
int main(void) {
    ResourceOffsets resource; int i;
    for (i = 0; i < 4; ++i) resource.offsets[i] = i * 4;
    result = &resource;
    assert(FUNCTION(65535, &resource) == result);
    assert(received_id == 65535);
    assert(received_data == (unsigned char *)&resource + INDEX * 4);
    result = 0; assert(FUNCTION(0, &resource) == 0); assert(received_id == 0);
    return 0;
}
'''.replace('FUNCTION', 'func_' + address).replace('INDEX', str(index))
with tempfile.TemporaryDirectory(prefix='bt03b-semantics-') as directory:
    for address, harness in HARNESS.items():
        name = 'func_' + address
        source = (ROOT / 'cloud/matches/boot_tail' if address in ('80014E00', '80014E10') else PACKET) / (name + '.c')
        test = Path(directory) / (name + '.c')
        executable = Path(directory) / name
        test.write_text('#include <assert.h>\n' + source.read_text() + '\n' + harness)
        subprocess.run(['cc', '-std=c89', '-pedantic-errors', '-O2', str(test), '-o', str(executable)], check=True)
        subprocess.run([str(executable)], check=True)
        print(name + ': host semantics PASS')
