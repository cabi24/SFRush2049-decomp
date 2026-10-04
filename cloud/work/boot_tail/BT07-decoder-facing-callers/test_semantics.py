#!/usr/bin/env python3
"""Actual-source caller contracts; independent C89 units with external mocks.

The actual source supplies all local declarations/types. The other caller and
opaque decoder are mocked, never linked implementations. O32-compatible layouts
are separately checked with -m32. Native-width execution uses real allocated,
aligned memory and positive capacity. Arithmetic probes establish only caller
semantics, not general malformed-stream, decoder, hardware, or ROM safety.
"""
from hashlib import sha256
import json
import os
from pathlib import Path
import subprocess
import tempfile

PACKET = Path(__file__).resolve().parent
COMMON = r'''
#include <assert.h>
#include <stddef.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
'''
LAYOUT = r'''
#define CHECK(n,e) typedef char layout_##n[(e) ? 1 : -1]
CHECK(byte, __CHAR_BIT__ == 8);
CHECK(word, sizeof(unsigned int) == 4);
CHECK(halfword, sizeof(unsigned short) == 2);
CHECK(pointer, sizeof(void *) == 4);
CHECK(queue, sizeof(OSMesgQueue) == 24);
CHECK(stream, sizeof(StreamState) == 4648);
CHECK(opaque, offsetof(StreamState, opaque_decoder) == 0);
CHECK(callback, offsetof(StreamState, callback) == 400);
CHECK(input, offsetof(StreamState, input) == 404);
CHECK(output, offsetof(StreamState, output) == 408);
CHECK(capacity, offsetof(StreamState, capacity) == 412);
CHECK(input_ring, offsetof(StreamState, input_ring) == 416);
CHECK(input_ring_size, sizeof(((StreamState *)0)->input_ring) == 4096);
CHECK(remaining_blocks, offsetof(StreamState, remaining_blocks) == 4512);
CHECK(header_low, offsetof(StreamState, header_low) == 4514);
CHECK(remaining_input, offsetof(StreamState, remaining_input) == 4516);
CHECK(remaining, offsetof(StreamState, remaining) == 4520);
CHECK(queue_offset, offsetof(StreamState, queue) == 4524);
CHECK(message, offsetof(StreamState, message) == 4548);
CHECK(bit_position, offsetof(StreamState, bit_position) == 4552);
CHECK(consumed_input, offsetof(StreamState, consumed_input) == 4556);
CHECK(received, offsetof(StreamState, received) == 4560);
CHECK(consumed, offsetof(StreamState, consumed) == 4564);
CHECK(available, offsetof(StreamState, available) == 4568);
CHECK(pending, offsetof(StreamState, pending) == 4572);
CHECK(state, offsetof(StreamState, state) == 4573);
CHECK(budget, offsetof(StreamState, budget) == 4574);
CHECK(request_queue, offsetof(StreamState, request_queue) == 4576);
CHECK(request_messages, offsetof(StreamState, request_messages) == 4600);
CHECK(busy, offsetof(StreamState, busy) == 4608);
CHECK(volume, offsetof(StreamState, volume) == 4609);
CHECK(pan, offsetof(StreamState, pan) == 4610);
CHECK(span, offsetof(StreamState, span) == 4611);
CHECK(option, offsetof(StreamState, option) == 4612);
CHECK(rate, offsetof(StreamState, rate) == 4616);
CHECK(handle, offsetof(StreamState, handle) == 4620);
CHECK(buffer, offsetof(StreamState, buffer) == 4624);
CHECK(buffer_count, offsetof(StreamState, buffer_count) == 4628);
CHECK(request_state, offsetof(StreamState, request_state) == 4632);
CHECK(mode, offsetof(StreamState, mode) == 4633);
CHECK(processed, offsetof(StreamState, processed) == 4636);
CHECK(duration, offsetof(StreamState, duration) == 4640);
CHECK(token, offsetof(StreamState, token) == 4644);
'''
DISPATCH = r'''
volatile unsigned char D_8002D480[1];
ServiceHooks D_80038000;
StreamState D_80056230[2];
unsigned int D_800586A0;
static unsigned int cases;
static short *buffers[4];
static int events[32], event_count, current;
static int pumps[2], ready_calls[2], start_calls[2], failure_calls[2];
static int stop_calls[2], release_calls[2], returns[2];
static void *released[2];
static float durations[2];
static void (*pump_mutation[2])(StreamState *);
static void (*ready_mutation[2])(StreamState *);
static void (*start_mutation[2])(StreamState *);
static void (*failure_mutation[2])(StreamState *);
static void (*stop_mutation[2])(StreamState *);
static void (*release_mutation[2])(StreamState *);
static void (*acquire_mutation)(void);
static void event(int value)
{
    assert(event_count < 32); events[event_count++] = value;
}
static int index_of(StreamState *s)
{
    assert(s == &D_80056230[0] || s == &D_80056230[1]);
    return s == &D_80056230[1];
}
void func_80025150(void)
{
    assert(event_count == 0); event(1);
    if (acquire_mutation) acquire_mutation();
}
void func_8002517C(void)
{
    assert(event_count > 0 && events[0] == 1); event(99);
}
void func_80025F74(StreamState *s)
{
    current = index_of(s); pumps[current]++; event(10 + current);
    if (pump_mutation[current]) pump_mutation[current](s);
}
void func_80026328(StreamState *s)
{
    int i;
    i = index_of(s); assert(i == current);
    assert(s->busy == 2 && s->processed == 0 && s->duration == durations[i]);
    ready_calls[i]++; event(20 + i);
    if (ready_mutation[i]) ready_mutation[i](s);
}
int func_8001C580(unsigned char request, short *buffer, unsigned int count,
                  unsigned int rate, unsigned char volume, unsigned char pan,
                  unsigned char span, unsigned char mode,
                  SampleCallback callback, unsigned int context)
{
    StreamState *s;
    assert(context < 2 && (int)context == current);
    s = &D_80056230[context];
    assert(request == s->option && buffer == s->buffer);
    assert(count == s->buffer_count && rate == s->rate);
    assert(volume == s->volume && pan == s->pan && span == s->span && mode == s->mode);
    assert(callback == func_80024FD4);
    start_calls[current]++; event(30 + current);
    if (start_mutation[current]) start_mutation[current](s);
    return returns[current];
}
void func_80026348(StreamState *s)
{
    int i;
    i = index_of(s); assert(i == current && s->handle == -1);
    failure_calls[i]++; event(40 + i);
    /* Explicit helper effect, not an assignment by the caller. */
    s->state = 4;
    if (failure_mutation[i]) failure_mutation[i](s);
}
void func_8001C7F4(int handle)
{
    StreamState *s;
    s = &D_80056230[current]; assert(handle == s->handle);
    stop_calls[current]++; event(50 + current);
    if (stop_mutation[current]) stop_mutation[current](s);
}
int func_80024FD4(short *a, unsigned int b, short *c, unsigned int d, unsigned int e)
{
    (void)a; (void)b; (void)c; (void)d; (void)e;
    assert(0); return 0;
}
static void release_buffer(void *p)
{
    StreamState *s;
    s = &D_80056230[current]; assert(p == s->buffer);
    assert(p == buffers[0] || p == buffers[1] || p == buffers[2] || p == buffers[3]);
    release_calls[current]++; released[current] = p; event(60 + current);
    if (release_mutation[current]) release_mutation[current](s);
}
static void reset(void)
{
    int i;
    cases++; memset(D_80056230, 0, sizeof(D_80056230));
    memset(events, 0, sizeof(events)); event_count = 0; current = -1;
    D_8002D480[0] = 1; D_800586A0 = 0; D_80038000.release = release_buffer;
    acquire_mutation = 0;
    for (i = 0; i < 2; i++) {
        StreamState *s;
        s = &D_80056230[i];
        pumps[i] = ready_calls[i] = start_calls[i] = failure_calls[i] = 0;
        stop_calls[i] = release_calls[i] = 0; released[i] = 0;
        pump_mutation[i] = ready_mutation[i] = start_mutation[i] = 0;
        failure_mutation[i] = stop_mutation[i] = release_mutation[i] = 0;
        returns[i] = 700 + i;
        s->state = 2; s->remaining_blocks = (unsigned short)(123 + i);
        s->rate = 32000U + (unsigned int)i; s->buffer = buffers[i];
        s->buffer_count = 320U + 160U * (unsigned int)i;
        s->volume = (unsigned char)(13 + i); s->pan = (unsigned char)(71 + i);
        s->span = (unsigned char)(127 + i); s->option = (unsigned char)(199 + i);
        s->mode = (unsigned char)(251 + i); s->handle = 900 + i;
        s->processed = 0xFEDCBA98U; s->duration = -3.5f; s->token = 0xCAFEBABEU;
        durations[i] = (float)s->remaining_blocks * 160.0f / (float)s->rate;
    }
}
static void check_events(const int *expected, int count)
{
    assert(event_count == count);
    assert(!memcmp(events, expected, sizeof(int) * (size_t)count));
}
static void dispatch_matrix(void)
{
    static const unsigned char busy_values[] = {0, 1, 2, 3, 4, 255};
    static const int states[] = {-128, -1, 0, 1, 2, 3, 4, 127};
    static const unsigned int flags[] = {0, 1, 2, 3, 0x80000000U, 0xFFFFFFFFU};
    StreamState before[2], expected;
    int b, st, r, f, selected, n, order[8], busy;
    for (selected = 0; selected < 2; selected++)
    for (b = 0; b < 6; b++) for (st = 0; st < 8; st++)
    for (r = 0; r < 6; r++) for (f = 0; f < 6; f++) {
        static const unsigned char requests[] = {0, 1, 2, 3, 4, 255};
        reset(); D_800586A0 = flags[f];
        D_80056230[selected].busy = busy_values[b];
        D_80056230[selected].state = (signed char)states[st];
        D_80056230[selected].request_state = requests[r];
        memcpy(before, D_80056230, sizeof(before)); expected = before[selected];
        n = 0; order[n++] = 1; busy = busy_values[b];
        if (busy) order[n++] = 10 + selected;
        if (busy == 1 && states[st] == 2) {
            expected.duration = durations[selected]; expected.processed = 0;
            expected.busy = 2; expected.handle = returns[selected];
            order[n++] = 20 + selected; order[n++] = 30 + selected;
        } else if (busy == 2 && states[st] == 4) {
            expected.busy = 3; expected.request_state = 4;
        } else if (busy == 3) {
            if (requests[r] == 0) {
                if (!(flags[f] & 1)) order[n++] = 60 + selected;
                expected.busy = 0;
            } else {
                if (requests[r] == 2) {
                    order[n++] = 50 + selected; expected.handle = -1;
                }
                expected.request_state = (unsigned char)(requests[r] - 1);
            }
        }
        order[n++] = 99; func_8002574C(); check_events(order, n);
        assert(!memcmp(&expected, &D_80056230[selected], sizeof(expected)));
        assert(!memcmp(&before[1-selected], &D_80056230[1-selected], sizeof(expected)));
        assert(pumps[selected] == (busy != 0) && !pumps[1-selected]);
    }
}
static void mutate_start_fields(StreamState *s)
{
    s->remaining_blocks = 19; s->rate = 0xFFFFFFFFU; s->buffer = buffers[2 + current];
    s->buffer_count = 640U + 160U * (unsigned int)current; s->option = 254; s->volume = 3;
    s->pan = 41; s->span = 219; s->mode = 137;
}
static void mutate_fail(StreamState *s)
{
    s->buffer = buffers[2 + current]; s->busy = 99;
}
static void mutate_release(StreamState *s) { s->busy = 127; }
static void mutate_stop_zero(StreamState *s) { s->request_state = 0; s->handle = 777; }
static void mutate_stop_high(StreamState *s) { s->request_state = 255; s->handle = 777; }
static void mutate_ownership(StreamState *s) { (void)s; D_800586A0 ^= 1; }
static void mutate_return_handle(StreamState *s) { s->handle = -1; }
static void pump_to_transition(StreamState *s) { s->busy = 2; s->state = 4; }
static void pump_to_start(StreamState *s) { s->busy = 1; s->state = 2; }
static void pump_to_idle(StreamState *s) { s->busy = 0; }
static void acquire_enables_record(void)
{
    D_8002D480[0] = 0; D_80056230[1].busy = 2; D_80056230[1].state = 4;
}
static void extra_dispatch_cases(void)
{
    static const unsigned int rates[] = {1, 8000, 44100, 0x7FFFFFFFU, 0x80000000U, 0xFFFFFFFFU};
    static const unsigned short counts[] = {0, 1, 65535};
    static const int handles[] = {INT_MIN, -2, -1, 0, 1, INT_MAX};
    static const unsigned char enables[] = {1, 2, 128, 255};
    StreamState before[2];
    StreamState *s;
    int i, j, k, selected, order[12], n;
    reset(); D_8002D480[0] = 0; D_80056230[0].busy = 1; D_80056230[1].busy = 3;
    memcpy(before, D_80056230, sizeof(before)); func_8002574C();
    assert(!event_count && !memcmp(before, D_80056230, sizeof(before)));
    reset(); acquire_mutation = acquire_enables_record; func_8002574C();
    assert(D_80056230[1].busy == 3 && D_80056230[1].request_state == 4);
    order[0] = 1; order[1] = 11; order[2] = 99; check_events(order, 3);
    for (i = 0; i < 4; i++) {
        reset(); D_8002D480[0] = enables[i]; D_80056230[0].busy = 1;
        func_8002574C(); assert(start_calls[0] == 1 && D_80056230[0].busy == 2);
    }
    for (selected = 0; selected < 2; selected++) {
        for (i = 0; i < 6; i++) {
            reset(); s = &D_80056230[selected]; s->busy = 1; returns[selected] = handles[i];
            start_mutation[selected] = mutate_return_handle; func_8002574C();
            assert(s->handle == handles[i] && failure_calls[selected] == (handles[i] == -1));
            assert(s->busy == (handles[i] == -1 ? 0 : 2));
        }
        for (i = 0; i < 6; i++) for (j = 0; j < 3; j++) {
            double reference, difference;
            reset(); s = &D_80056230[selected]; s->busy = 1;
            s->rate = rates[i]; s->remaining_blocks = counts[j];
            durations[selected] = (float)counts[j] * 160.0f / (float)rates[i];
            func_8002574C();
            reference = (double)counts[j] * 160.0 / (double)rates[i];
            difference = (double)s->duration - reference;
            if (difference < 0) difference = -difference;
            assert(s->duration >= 0 && difference <= reference * 0.000001 + 1e-12);
            assert(start_calls[selected] == 1 && s->handle == returns[selected]);
        }
        for (i = 0; i < 256; i++) {
            reset(); s = &D_80056230[selected]; s->busy = 3;
            s->request_state = (unsigned char)i; func_8002574C();
            assert(s->busy == (i ? 3 : 0));
            assert(s->request_state == (i ? i - 1 : 0));
            assert(stop_calls[selected] == (i == 2));
            assert(release_calls[selected] == (i == 0));
        }
        for (i = 0; i < 4; i++) for (j = 0; j < 2; j++) {
            reset(); s = &D_80056230[selected]; s->busy = 1; returns[selected] = -1;
            D_800586A0 = (unsigned int)i;
            if (j) start_mutation[selected] = mutate_ownership;
            failure_mutation[selected] = mutate_fail;
            release_mutation[selected] = mutate_release;
            func_8002574C();
            assert(s->handle == -1 && s->state == 4 && !s->busy);
            assert(failure_calls[selected] == 1);
            assert(release_calls[selected] == !((i ^ j) & 1));
            if (release_calls[selected]) assert(released[selected] == buffers[2 + selected]);
            n = 0; order[n++] = 1; order[n++] = 10 + selected;
            order[n++] = 20 + selected; order[n++] = 30 + selected;
            order[n++] = 40 + selected;
            if (!((i ^ j) & 1)) order[n++] = 60 + selected;
            order[n++] = 99; check_events(order, n);
        }
        reset(); s = &D_80056230[selected]; s->busy = 1;
        ready_mutation[selected] = mutate_start_fields; func_8002574C();
        assert(s->duration == durations[selected] && s->handle == returns[selected]);
        assert(start_calls[selected] == 1 && s->rate == 0xFFFFFFFFU);
        reset(); s = &D_80056230[selected]; s->busy = 1;
        pump_mutation[selected] = pump_to_transition; func_8002574C();
        assert(s->busy == 3 && s->request_state == 4 && !start_calls[selected]);
        reset(); s = &D_80056230[selected]; s->busy = 2; s->state = -1;
        pump_mutation[selected] = pump_to_start; func_8002574C();
        assert(s->busy == 2 && start_calls[selected] == 1);
        reset(); s = &D_80056230[selected]; s->busy = 1;
        pump_mutation[selected] = pump_to_idle; func_8002574C();
        assert(!s->busy && !start_calls[selected]);
        for (k = 0; k < 2; k++) {
            reset(); s = &D_80056230[selected]; s->busy = 3; s->request_state = 2;
            stop_mutation[selected] = k ? mutate_stop_high : mutate_stop_zero;
            func_8002574C();
            assert(s->request_state == (k ? 254 : 255) && s->handle == -1);
            assert(stop_calls[selected] == 1 && !release_calls[selected]);
        }
    }
    reset(); D_80056230[0].busy = D_80056230[1].busy = 1;
    ready_mutation[0] = ready_mutation[1] = mutate_start_fields;
    func_8002574C();
    assert(start_calls[0] == 1 && start_calls[1] == 1);
    assert(D_80056230[0].handle == 700 && D_80056230[1].handle == 701);
    order[0] = 1; order[1] = 10; order[2] = 20; order[3] = 30;
    order[4] = 11; order[5] = 21; order[6] = 31; order[7] = 99; check_events(order, 8);
}
int main(void)
{
    int i;
    assert(sizeof(unsigned int) == 4 && sizeof(unsigned short) == 2 && CHAR_BIT == 8);
    for (i = 0; i < 4; i++) { buffers[i] = malloc(16008); assert(buffers[i]); }
    dispatch_matrix(); extra_dispatch_cases();
    for (i = 0; i < 4; i++) free(buffers[i]);
    printf("%u\n", cases); return 0;
}
'''

STREAM = r'''
#define INPUT_BYTES 16384U
#define OUTPUT_BYTES 16008U
#define MAX_CALLS 128
static unsigned int cases;
static StreamState *s;
static unsigned char *input, *output;
static int receives, transfers, decodes, zeros, recv_result;
static int trace[2 * MAX_CALLS + 4], trace_count;
static void *transfer_source, *transfer_destination;
static unsigned int transfer_count;
static void *decode_destination[MAX_CALLS], *zero_destination[MAX_CALLS];
static void *expected_decode[MAX_CALLS], *expected_zero[MAX_CALLS];
static unsigned int scripted_bits[MAX_CALLS];
static int scripted_returns[MAX_CALLS];
static void (*receive_effect)(void);
static void (*transfer_effect)(void);
static void (*decode_effect[MAX_CALLS])(void);
static void (*zero_effect[MAX_CALLS])(void);
static void record(int kind)
{
    assert(trace_count < 2 * MAX_CALLS + 4); trace[trace_count++] = kind;
}
int osRecvMesg(OSMesgQueue *queue, OSMesg *message, int flags)
{
    assert(queue == &s->queue && message == 0 && flags == 0);
    receives++; record(1);
    if (receive_effect) receive_effect();
    return recv_result;
}
static void transfer(void *source, void *destination, unsigned int count, OSMesgQueue *queue)
{
    unsigned char *p;
    assert(!transfers && s->pending == 1 && queue == &s->queue);
    assert(count > 0 && count <= 1024);
    p = source; assert(p >= input && p + count <= input + INPUT_BYTES);
    p = destination;
    assert(p >= (unsigned char *)s->input_ring);
    assert(p + count <= (unsigned char *)s->input_ring + sizeof(s->input_ring));
    transfer_source = source; transfer_destination = destination; transfer_count = count;
    transfers++; record(2);
    if (transfer_effect) transfer_effect();
}
int func_800268D0(StreamState *stream, void *destination)
{
    unsigned char *p;
    int i;
    i = decodes; assert(stream == s && i < MAX_CALLS);
    assert(destination == expected_decode[i]);
    p = destination; assert(p >= output && p + 320 <= output + OUTPUT_BYTES);
    decode_destination[i] = destination; decodes++; record(3);
    /* Recognizable mock output, not an implementation of a decoder algorithm. */
    memset(destination, 0xD3, 320); s->bit_position = scripted_bits[i];
    if (decode_effect[i]) decode_effect[i]();
    return scripted_returns[i];
}
void bzero(void *destination, int count)
{
    unsigned char *p;
    int i;
    i = zeros; assert(i < MAX_CALLS && count == 320);
    assert(destination == expected_zero[i]);
    p = destination; assert(p >= output && p + count <= output + OUTPUT_BYTES);
    zero_destination[i] = destination; zeros++; record(4);
    memset(destination, 0, (size_t)count);
    if (zero_effect[i]) zero_effect[i]();
}
static void reset(void)
{
    int i;
    cases++; memset(s, 0, sizeof(*s)); memset(output, 0xA5, OUTPUT_BYTES);
    receives = transfers = decodes = zeros = trace_count = 0; recv_result = -1;
    transfer_source = transfer_destination = 0; transfer_count = 0;
    receive_effect = transfer_effect = 0;
    s->input = input; s->output = output; s->capacity = 640; s->callback = transfer;
    s->received = 1024; s->state = 2; s->remaining = -12345;
    s->remaining_blocks = 0; s->budget = 0;
    for (i = 0; i < MAX_CALLS; i++) {
        decode_destination[i] = zero_destination[i] = 0;
        expected_decode[i] = expected_zero[i] = output + (size_t)(i < 50 ? i : 0) * 320;
        scripted_bits[i] = 791U; scripted_returns[i] = -77;
        decode_effect[i] = zero_effect[i] = 0;
    }
}
static void run(void) { func_80025F74(s); }
static void unchanged_output(void)
{
    unsigned int i;
    for (i = 0; i < OUTPUT_BYTES; i++) assert(output[i] == 0xA5);
}
static void received_mutation(void)
{
    s->received = 1024; s->remaining_input = 513; s->budget = 0;
    s->remaining_blocks = 17; s->header_low = 19; s->bit_position = 321;
}
static void receive_cases(void)
{
    static const int pending_values[] = {-128, -1, 1, 127};
    static const int results[] = {INT_MIN, -2, -1, 0, 1, INT_MAX};
    static const unsigned int remaining_values[] = {0, 1, 255, 256, 257, 511, 512, 0xFFFFFFU};
    static const unsigned int positions[] = {1024, 4096, 0xFFFFFC00U};
    static const unsigned int headers[] = {0, 1, 0xFFFF, 0x10000, 0x80008000U, 0xFFFFFFFFU};
    StreamState before, expected;
    int p, r, n, j;
    reset(); s->state = 0; s->pending = 0;
    before = *s; run(); assert(!receives && !memcmp(&before, s, sizeof(before)));
    for (p = 0; p < 4; p++) for (r = 0; r < 6; r++) for (n = 0; n < 8; n++) {
        reset(); s->state = 0; s->received = 0; s->pending = (signed char)pending_values[p];
        s->remaining_blocks = 7; s->header_low = 9; s->remaining_input = 55;
        s->bit_position = 24; s->input_ring[1] = 0xFEDCBA98U;
        s->input_ring[2] = 0xAB000000U | remaining_values[n]; recv_result = results[r];
        expected = *s;
        if (results[r] >= 0) {
            expected.remaining_blocks = 0xFEDC; expected.header_low = 0xBA98;
            expected.remaining_input = remaining_values[n] > 256 ? remaining_values[n] - 256 : 0;
            expected.bit_position = 96; expected.received = 1024; expected.pending = 0;
        }
        run(); assert(receives == 1 && !transfers && !decodes && !zeros);
        assert(!memcmp(&expected, s, sizeof(expected))); unchanged_output();
    }
    for (j = 0; j < 6; j++) {
        reset(); s->state = 0; s->pending = 1; s->received = 0; recv_result = 0;
        s->input_ring[1] = headers[j]; run();
        assert(s->remaining_blocks == (headers[j] >> 16));
        assert(s->header_low == (headers[j] & 65535U));
        assert(s->received == 1024 && s->bit_position == 96 && !s->remaining_input);
    }
    for (j = 0; j < 3; j++) for (n = 0; n < 8; n++) {
        reset(); s->state = 0; s->pending = 1; s->received = positions[j];
        s->remaining_blocks = 65535; s->header_low = 123; s->bit_position = 987;
        s->remaining_input = remaining_values[n]; s->input_ring[1] = 0x01020304U;
        s->input_ring[2] = 0xFFFFFFFFU; recv_result = 0; expected = *s;
        expected.received += 1024;
        expected.remaining_input = remaining_values[n] > 256 ? remaining_values[n] - 256 : 0;
        expected.pending = 0; run();
        assert(!memcmp(&expected, s, sizeof(expected)) && receives == 1 && !transfers);
    }
    reset(); s->state = 0; s->pending = 1; s->received = 0;
    receive_effect = received_mutation; recv_result = 0; run();
    assert(s->received == 2048 && s->remaining_input == 257 && !s->pending);
    assert(s->remaining_blocks == 17 && s->header_low == 19 && s->bit_position == 321);
    assert(!transfers && !decodes && !zeros);
}
static void transfer_cases(void)
{
    static const int states[] = {-128, -1, 0, 1, 2, 3, 4, 127};
    static const unsigned int positions[] = {0, 1024, 2048, 3072, 4096, 8192};
    static const unsigned int remaining_values[] = {0, 1, 255, 256, 257, 0xFFFFFFFFU};
    static const unsigned int occupancy[] = {0, 3072, 3073, 4096};
    static const unsigned int unaligned[] = {7, 1023, 1031, 2055, 3079, 4095, 4103};
    StreamState expected;
    unsigned int bytes;
    int st, p, n, o, pending, eligible;
    for (st = 0; st < 8; st++) for (p = 0; p < 6; p++) for (n = 0; n < 6; n++)
    for (o = 0; o < 4; o++) for (pending = 0; pending < 2; pending++) {
        reset(); s->state = (signed char)states[st]; s->received = positions[p];
        s->consumed_input = positions[p] - occupancy[o];
        s->remaining_input = remaining_values[n]; s->pending = (signed char)pending;
        expected = *s;
        bytes = positions[p] == 0 ? 1024 : remaining_values[n] > 256 ? 1024 : remaining_values[n] * 4;
        eligible = !pending && states[st] >= 1 && states[st] <= 3 && occupancy[o] <= 3072 && bytes != 0;
        if (eligible) expected.pending = 1;
        run(); assert(receives == pending && transfers == eligible && !decodes && !zeros);
        assert(!memcmp(&expected, s, sizeof(expected)));
        if (eligible) {
            assert(transfer_source == input + positions[p]);
            assert(transfer_destination == (unsigned char *)s->input_ring + (positions[p] % 4096));
            assert(transfer_count == bytes);
        }
    }
    for (p = 0; p < 7; p++) {
        reset(); s->state = 2; s->received = unaligned[p]; s->consumed_input = unaligned[p];
        s->remaining_input = 1; run();
        assert(transfers == 1 && transfer_source == input + unaligned[p] && transfer_count == 4);
        assert(transfer_destination == (unsigned char *)s->input_ring + ((unaligned[p] % 4096) & ~7U));
    }
    reset(); s->received = 32; s->consumed_input = 0xFFFFFFF8U; s->remaining_input = 1; run();
    assert(transfers == 1 && transfer_source == input + 32 && transfer_count == 4);
    reset(); s->received = 1024; s->consumed_input = 1025; s->remaining_input = 1; run();
    assert(!transfers && !s->pending);
}
static void decode_predicates(void)
{
    static const int states[] = {-128, -1, 0, 1, 2, 3, 4, 127};
    static const unsigned short blocks[] = {0, 1, 65535};
    static const unsigned int occupied[] = {0, 160, 320, 480, 640};
    static const unsigned int input_left[] = {0, 36, 37, 38};
    static const int budgets[] = {-128, -1, 0, 1};
    StreamState expected;
    unsigned int offset;
    int st, b, o, left, budget, decode, zero;
    for (st = 0; st < 8; st++) for (b = 0; b < 3; b++) for (o = 0; o < 5; o++)
    for (left = 0; left < 4; left++) for (budget = 0; budget < 4; budget++) {
        reset(); s->state = (signed char)states[st]; s->remaining_blocks = blocks[b];
        s->pending = 1; s->available = occupied[o]; s->budget = (signed char)budgets[budget];
        s->received = 4096; s->consumed_input = 4096 - input_left[left]; expected = *s;
        decode = occupied[o] < 480 && (states[st] == 1 || blocks[b] > 0)
                 && input_left[left] >= 37 && budgets[budget] > 0;
        zero = !decode && occupied[o] < 480 && (states[st] == 3 || states[st] == 4)
               && blocks[b] == 0 && budgets[budget] > 0;
        offset = (occupied[o] % 640) * 2;
        expected_decode[0] = expected_zero[0] = output + offset;
        if (decode) {
            expected.bit_position = 791; expected.consumed_input = 96;
            expected.available += 160;
            expected.remaining_blocks = (unsigned short)(blocks[b] - 1);
            if (blocks[b] == 1) expected.remaining = 640;
        } else if (zero) expected.available += 160;
        if (states[st] == 1 && expected.available >= 320) expected.state = 2;
        run(); assert(receives == 1 && !transfers && decodes == decode && zeros == zero);
        assert(!memcmp(&expected, s, sizeof(expected)));
        if (!decode && !zero) unchanged_output();
        else {
            unsigned int i;
            for (i = 0; i < OUTPUT_BYTES; i++)
                assert(output[i] == ((i >= offset && i < offset + 320) ? (decode ? 0xD3 : 0) : 0xA5));
        }
    }
}
static void budget_and_fill_cases(void)
{
    static const unsigned int bits[] = {0, 1, 7, 8, 31, 32, 255, 256, 0xFFFFFFFFU};
    unsigned int capacity, n, i, available;
    int budget, state;
    for (budget = 1; budget <= 127; budget++) {
        reset(); s->pending = 1; s->capacity = 8000; s->budget = (signed char)budget;
        s->remaining_blocks = 512;
        for (i = 0; i < MAX_CALLS; i++) scripted_bits[i] = 0;
        n = budget < 49 ? (unsigned int)budget : 49;
        run(); assert(decodes == (int)n && !zeros && s->available == n * 160);
        assert(s->remaining_blocks == 512 - n && s->remaining == -12345);
        assert(s->budget == budget && s->consumed_input == 0);
    }
    for (capacity = 320; capacity <= 8000; capacity += 160)
    for (state = 3; state <= 4; state++) for (budget = 1; budget <= 3; budget++) {
        reset(); s->pending = 1; s->capacity = capacity; s->state = (signed char)state;
        s->budget = (signed char)(budget == 3 ? 127 : budget);
        n = capacity / 160 - 1;
        if (n > (unsigned int)s->budget) n = (unsigned int)s->budget;
        run(); assert(!decodes && zeros == (int)n && s->available == n * 160);
        assert(s->state == state && s->remaining == -12345 && s->budget == (budget == 3 ? 127 : budget));
        for (i = 0; i < OUTPUT_BYTES; i++) assert(output[i] == (i < n * 320 ? 0 : 0xA5));
    }
    for (i = 0; i < 9; i++) {
        reset(); s->pending = 1; s->budget = 1; s->remaining_blocks = 2;
        scripted_bits[0] = bits[i]; run();
        assert(decodes == 1 && s->consumed_input == ((bits[i] / 8U) & ~3U));
        assert(s->available == 160 && s->remaining_blocks == 1);
    }
    /* The opaque decoder is called at the end, then start, of one output ring. */
    reset(); s->pending = 1; s->budget = 3; s->remaining_blocks = 3;
    s->available = s->consumed = 480;
    expected_decode[0] = output + 960; expected_decode[1] = output; expected_decode[2] = output + 320;
    run(); assert(decodes == 3 && !zeros && s->available == 960 && s->remaining == 640);
    for (i = 0; i < OUTPUT_BYTES; i++)
        assert(output[i] == ((i < 640 || (i >= 960 && i < 1280)) ? 0xD3 : 0xA5));
    /* Zero filling visits the end and then the start of one allocated ring. */
    reset(); s->pending = 1; s->state = 4; s->budget = 3;
    s->available = s->consumed = 480;
    expected_zero[0] = output + 960; expected_zero[1] = output; expected_zero[2] = output + 320;
    run(); assert(zeros == 3 && s->available == 960 && !decodes);
    for (i = 0; i < OUTPUT_BYTES; i++)
        assert(output[i] == ((i < 640 || (i >= 960 && i < 1280)) ? 0 : 0xA5));
    /* Unsigned add/subtract crosses UINT_MAX, with destinations inside the allocation. */
    for (state = 1; state <= 4; state += 3) {
        reset(); s->pending = 1; s->state = (signed char)state; s->budget = 1;
        s->available = 0xFFFFFFA0U; s->consumed = s->available - 160U;
        s->remaining_blocks = (unsigned short)(state == 1 ? 2 : 0);
        expected_decode[0] = expected_zero[0] = output + 320;
        run(); assert(s->available == 64 && s->available - s->consumed == 320);
        assert(decodes == (state == 1) && zeros == (state == 4));
        assert(s->state == (state == 1 ? 2 : 4));
    }
    /* Four-sample truncation in the output destination expression. */
    for (available = 163; available <= 1283; available += 1120) {
        reset(); s->pending = 1; s->budget = 1; s->remaining_blocks = 2;
        s->available = s->consumed = available;
        expected_decode[0] = output + ((available % 640) & ~3U) * 2;
        run(); assert(decodes == 1 && s->available == available + 160);
    }
    reset(); s->pending = 1; s->budget = 1; s->remaining_blocks = 2;
    s->received = 32; s->consumed_input = 0xFFFFFFF8U; run();
    assert(decodes == 1 && s->consumed_input == 96);
}
static void transfer_opens_decode(void)
{
    s->received = 1024; s->remaining_blocks = 2; s->budget = 0;
    s->state = 2; s->pending = 0;
}
static void receive_changes_budget(void) { s->budget = 0; }
static void decode_reload_fields(void)
{
    s->available = 160; s->consumed_input = 999; s->remaining_blocks = 1;
    s->capacity = 800; s->bit_position = 1647; s->received = 240;
    s->budget = 127;
}
static void decode_underflows_blocks(void)
{
    s->remaining_blocks = 0; s->bit_position = 1647; s->received = 240;
}
static void decode_next_destination(void)
{
    s->available = 640; s->consumed = 640; s->capacity = 960;
    s->remaining_blocks = 5; s->bit_position = 135;
}
static void decode_last_block(void) { s->remaining_blocks = 1; s->bit_position = 295; }
static void zero_reload_fields(void)
{
    s->available = 160; s->capacity = 800; s->state = 0; s->remaining_blocks = 7;
}
static void call_mutation_cases(void)
{
    reset(); s->state = 1; s->received = 0; s->budget = 2;
    transfer_effect = transfer_opens_decode; run();
    assert(!receives && transfers == 1 && decodes == 2 && !zeros);
    assert(trace_count == 3 && trace[0] == 2 && trace[1] == 3 && trace[2] == 3);
    assert(!s->pending && s->budget == 0 && s->available == 320 && !s->remaining_blocks && s->remaining == 640);
    reset(); s->pending = 1; s->budget = 1; s->remaining_blocks = 1;
    recv_result = 0; receive_effect = receive_changes_budget; run();
    assert(receives == 1 && decodes == 1 && !transfers && s->budget == 0);
    assert(trace_count == 2 && trace[0] == 1 && trace[1] == 3);
    reset(); s->pending = 1; s->budget = 2; s->remaining_blocks = 9;
    decode_effect[0] = decode_reload_fields; run();
    assert(decodes == 1 && !zeros && s->available == 320 && s->consumed_input == 204);
    assert(!s->remaining_blocks && s->remaining == 800 && s->capacity == 800 && s->budget == 127);
    reset(); s->pending = 1; s->state = 3; s->budget = 2; s->remaining_blocks = 9;
    decode_effect[0] = decode_underflows_blocks; run();
    assert(decodes == 1 && !zeros && s->remaining_blocks == 65535 && s->remaining == -12345);
    assert(s->consumed_input == 204 && s->received - s->consumed_input == 36);
    reset(); s->pending = 1; s->budget = 2; s->remaining_blocks = 9;
    decode_effect[0] = decode_next_destination; decode_effect[1] = decode_last_block;
    expected_decode[1] = output + 1600; scripted_returns[0] = INT_MIN; scripted_returns[1] = INT_MAX;
    run(); assert(decodes == 2 && !zeros && s->available == 960 && s->consumed_input == 36);
    assert(s->remaining == 960 && !s->remaining_blocks && s->capacity == 960 && s->consumed == 640);
    reset(); s->pending = 1; s->state = 3; s->budget = 2;
    zero_effect[0] = zero_reload_fields; expected_zero[1] = output + 640; run();
    assert(zeros == 2 && !decodes && s->available == 480 && s->remaining_blocks == 7 && s->state == 0);
    /* Last decode and padding share one signed per-call budget. */
    reset(); s->pending = 1; s->state = 3; s->budget = 3; s->remaining_blocks = 1;
    expected_zero[0] = output + 320; expected_zero[1] = output + 640; run();
    assert(decodes == 1 && zeros == 2 && s->available == 480 && s->remaining == 640);
    assert(trace_count == 4 && trace[0] == 1 && trace[1] == 3 && trace[2] == 4 && trace[3] == 4);
}
static void readiness_cases(void)
{
    static const unsigned int capacities[] = {320, 480, 640, 8000};
    int c, delta;
    unsigned int occupancy;
    for (c = 0; c < 4; c++) for (delta = -1; delta <= 1; delta++) {
        if (capacities[c] == 320 && delta == -1) continue;
        reset(); s->state = 1; s->pending = 1; s->capacity = capacities[c];
        occupancy = (unsigned int)((int)capacities[c] - 320 + delta);
        s->available = occupancy; run();
        assert(!decodes && !zeros && s->state == (delta >= 0 ? 2 : 1));
    }
    reset(); s->state = 1; s->pending = 1; s->available = 64; s->consumed = 0xFFFFFF00U;
    run(); assert(s->state == 2 && !decodes && !zeros);
}
int main(void)
{
    assert(sizeof(unsigned int) == 4 && sizeof(unsigned short) == 2 && CHAR_BIT == 8);
    s = malloc(sizeof(*s)); input = malloc(INPUT_BYTES); output = malloc(OUTPUT_BYTES);
    assert(s && input && output); memset(input, 0x6A, INPUT_BYTES);
    receive_cases(); transfer_cases(); decode_predicates(); budget_and_fill_cases();
    call_mutation_cases(); readiness_cases();
    free(output); free(input); free(s);
    printf("%u\n", cases); return 0;
}
'''
COVERAGE = {
    '8002574C': [
        'enable/acquire/release ordering, two-record traversal, inactive and unknown-busy no-ops',
        'busy/state/request-state matrix, exhaustive byte countdown, ownership flag bit masking',
        'all ten start inputs, typed five-input callback, and actual contexts 0/1',
        'start success/failure, post-helper fields/buffer reloads, and release ordering',
        'pump-mutated dispatch, acquire-mutated record, stop-mutated byte narrowing and handle overwrite',
        'finite positive unsigned rates through UINT_MAX and halfword counts through 65535',
    ],
    '80025F74': [
        'signed receive results/pending values, first/later header paths, low-24-bit masks and saturation',
        'state/pending/3072-byte transfer gates, initial/later sizes, modulo-4096 alignment and queue identity',
        'signed state/budget, 36/37-byte input bound, capacity-minus-160 bound, halfword decrement',
        'decoder-mutated counters/capacity/blocks, bit-position rounding, destination and field reloads',
        'budget capture across receive/transfer/decode, ignored decoder return, shared zero-fill budget',
        'zero-fill size/content/locations, ring rollover, helper mutation, final capacity-minus-320 threshold',
        'unsigned counter rollover and alignment arithmetic with real allocated storage',
    ],
}


def checked(command, **kwargs):
    return subprocess.run(command, check=True, capture_output=True, text=True,
                          timeout=60, **kwargs)


def find_source(address):
    filename = 'func_' + address + '.c'
    # The exact dispatch body may live in cloud/matches after promotion from
    # packet research. Standalone recovery copies remain runnable beside us.
    if address == '8002574C' and len(PACKET.parents) > 3:
        matched = PACKET.parents[3] / 'cloud' / 'matches' / 'boot_tail' / filename
        if matched.is_file():
            return matched
    source = PACKET / filename
    if not source.is_file():
        raise FileNotFoundError('actual source unavailable: ' + str(source))
    return source


def run():
    results = []
    compiler = os.environ.get('CC', 'cc')
    compiler_version = checked([compiler, '--version']).stdout.splitlines()[0]
    flags = ['-std=c89', '-pedantic', '-Wall', '-Wextra', '-Werror', '-fno-builtin']
    modes = [
        ('c89-O2', ['-O2']),
        ('c89-asan-ubsan-O1', ['-O1', '-fsanitize=address,undefined',
                              '-fno-sanitize-recover=all', '-fno-omit-frame-pointer',
                              '-fno-pie', '-no-pie']),
    ]
    with tempfile.TemporaryDirectory(prefix='bt07-decoder-callers-host-') as temporary:
        temporary = Path(temporary)
        for address, body in [('8002574C', DISPATCH), ('80025F74', STREAM)]:
            source = find_source(address)
            source_hash = sha256(source.read_bytes()).hexdigest()
            # Include current source, not a rewritten body or copied type view.
            include = '#include ' + json.dumps(str(source)) + '\n'
            layout = temporary / (address + '-layout.c')
            extra = ''
            if address == '8002574C':
                extra = ('CHECK(hooks, sizeof(ServiceHooks) == 32);\n'
                         'CHECK(release, offsetof(ServiceHooks, release) == 28);\n')
            layout.write_text('#include <stddef.h>\n' + include + LAYOUT + extra)
            checked([compiler, '-m32', *flags, '-ffreestanding', '-fsyntax-only', str(layout)])
            harness = temporary / (address + '.c')
            harness.write_text(COMMON + include + body)
            counts = []
            for mode, mode_flags in modes:
                executable = temporary / (address + '-' + mode)
                checked([compiler, *flags, *mode_flags, str(harness), '-o', str(executable)])
                environment = dict(os.environ, ASAN_OPTIONS='detect_leaks=0:halt_on_error=1',
                                   UBSAN_OPTIONS='halt_on_error=1:print_stacktrace=1')
                execution = checked([str(executable)], env=environment)
                counts.append(int(execution.stdout.strip()))
            if counts[0] != counts[1]:
                raise AssertionError('different case counts between compilation modes')
            if sha256(source.read_bytes()).hexdigest() != source_hash:
                raise AssertionError('source changed during verification; rerun the final snapshot')
            results.append({'name': 'func_' + address, 'source_sha256': source_hash,
                            'result': 'PASS', '32bit_c89_layout': 'PASS',
                            'cases_per_mode': counts[0], 'modes': [mode for mode, _ in modes],
                            'coverage': COVERAGE[address]})
    return {'result': 'PASS', 'compiler': compiler_version,
            'cases_per_mode': sum(result['cases_per_mode'] for result in results),
            'results': results,
            'limits': [
                'Actual-source caller host semantics only; no native/ROM execution or matching claim.',
                'External helpers, including decoder, are scripted mocks; no decoder algorithm/safety claim.',
                '32-bit layout compiled separately; execution uses native-width pointers and allocated objects.',
                'Positive sufficient capacity and valid aligned allocations; arithmetic probes access valid storage.',
                'No arbitrary malformed-input, zero-capacity, invalid-float, concurrency, or hardware-safety claim.',
                'ASan/UBSan enabled; LeakSanitizer disabled for the host execution environment.',
            ]}


if __name__ == '__main__':
    try:
        print(json.dumps(run(), indent=2))
    except subprocess.CalledProcessError as error:
        print(error.stdout or '')
        print(error.stderr or '')
        raise
