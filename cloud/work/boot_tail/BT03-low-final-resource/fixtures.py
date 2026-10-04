"""Synthetic twelve-byte insertion and bucket-removal states and list model."""
import struct
COUNT = {0x80015A0C: 0x8003CE18, 0x80016998: 0x8003DA20}
TABLE = {0x80015A0C: 0x8003CE20, 0x80016998: 0x8003E228}
CAPACITY = {0x80015A0C: 256, 0x80016998: 2048}
STRIDE = {0x80015A0C: 12, 0x80016998: 8}
RANGES, PAYLOAD = 0x8003DA28, 0x600000
NEW_PAYLOAD = PAYLOAD + 4 * 2048
PROFILES = [(0, []), (1, []), (2, []), (5, []), (17, []), (255, []), (256, []),
            (0, []), (0, [(0, 2)]), (0, [(0, 3), (2, 2), (511, 3)]),
            (0, [(3, 5), (8, 3), (2, 4)]),
            (0, [(0, 32)] + [(i, 63) for i in range(1, 33)])]


def initial(function, profile):
    flat = function == 0x80015A0C
    rows = [[PAYLOAD + 4 * i, 0xA500 + i % 256, 0xB000 + i, 0xCAFE, 0xD000 + i] if flat else
            [PAYLOAD + 4 * i, 0xA500 + i % 256, 0xCAFE] for i in range(CAPACITY[function])]
    ranges = [[0, 0] for _ in range(512)]
    n, groups = PROFILES[profile]
    if flat:
        for i in range(n):
            rows[i][1] = 2 * i + 2
            rows[i][3] = 65535 if i % 3 == 0 else 100 + i
    else:
        n = 0
        for group, length in groups:
            ranges[group] = [length, n]
            for i in range(length):
                step = 2 if profile == 11 and group == 0 else 1
                rows[n + i][1:] = [group * 64 + i * step, [1, 0, 2, 65535][i % 4]]
            n += length
    return {'count': n, 'rows': rows, 'ranges': ranges}


def encode(function, state):
    fmt = '>IHHHH' if function == 0x80015A0C else '>IHH'
    regions = {COUNT[function]: struct.pack('>I', state['count']), TABLE[function]: b''.join(struct.pack(fmt, *row) for row in state['rows'])}
    if function == 0x80016998:
        regions[RANGES] = b''.join(struct.pack('>HH', *row) for row in state['ranges'])
    return regions


def fingerprint(regions):
    h = 2166136261
    for base in sorted(regions):
        for byte in regions[base]:
            h = ((h ^ byte) * 16777619) & 0xFFFFFFFF
    return h


def hook(function, state, key, mode, entering):
    if entering and mode == 1:
        state['count'] = 0
        state['ranges'] = [[0, 0] for _ in range(512)]
    if not entering and mode == 2:
        for row in state['rows'][:state['count']]:
            if row[1] == key:
                row[3 if function == 0x80015A0C else 2] = 500
                break


def model(function, profile, key, mode, parameter):
    state = initial(function, profile)
    hook(function, state, key, mode, True)
    rows, ranges = state['rows'], state['ranges']
    if function == 0x80015A0C:
        pos = 0
        while pos < state['count'] and rows[pos][1] < key:
            pos += 1
        if pos < state['count'] and rows[pos][1] == key:
            rows[pos][3] = (rows[pos][3] + 1) & 65535
            hook(function, state, key, mode, False)
            return 0, state
        if state['count'] == 256:
            hook(function, state, key, mode, False)
            return 0, state
        tail = rows[pos][4]
        rows[pos + 1:state['count'] + 1] = [list(row) for row in rows[pos:state['count']]]
        rows[pos] = [NEW_PAYLOAD, key, parameter, 1, tail]
        state['count'] += 1
        result = 1
    else:
        group = ranges[key >> 6]
        first = group[1]
        offset = next((i for i in range(group[0]) if rows[first + i][1] == key), group[0])
        if offset < group[0]:
            pos = first + offset
            rows[pos][2] = (rows[pos][2] - 1) & 65535
            if rows[pos][2] == 0:
                rows[pos:state['count'] - 1] = [list(row) for row in rows[pos + 1:state['count']]]
                for other in ranges:
                    if first < other[1]:
                        other[1] = (other[1] - 1) & 65535
                group[0] = (group[0] - 1) & 65535
                state['count'] -= 1
        result = 0
    hook(function, state, key, mode, False)
    return result, state


def cases(function):
    result = []
    for profile in (range(7) if function == 0x80015A0C else range(7, 12)):
        if function == 0x80015A0C:
            n = PROFILES[profile][0]
            keys = sorted({0, 1, 2, 3, max(0, 2 * n - 1), 2 * n, 2 * n + 1, 65534, 65535})
            parameters = (0, 1, 0x1234, 65535)
        else:
            keys = [0, 1, 2, 3, 4, 62, 63, 64, 65, 126, 127, 128, 129, 130, 131, 192, 196, 197, 512, 515, 32704, 32706, 32766, 32767]
            parameters = (0,)
        for key in keys:
            for mode in range(3):
                for parameter in parameters:
                    result.append((profile, key, mode, parameter))
    return result


def native_hook(function, key, mode):
    def apply(destination, memory, calls):
        if destination == 0x80014594:
            assert calls == 0
            if mode == 1:
                memory(COUNT[function], 4, 0)
                if function == 0x80016998:
                    for i in range(512):
                        memory(RANGES + 4 * i, 4, 0)
        else:
            assert destination == 0x800145DC and calls == 1
            if mode == 2:
                count = memory(COUNT[function], 4)
                for i in range(count):
                    entry = TABLE[function] + STRIDE[function] * i
                    if memory(entry + 4, 2) == key:
                        memory(entry + (8 if function == 0x80015A0C else 6), 2, 500)
                        break
    return apply
