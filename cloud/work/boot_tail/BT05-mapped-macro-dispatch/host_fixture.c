/* Test-only external contracts; not part of any scored candidate. */
#include <stdio.h>
#include <stdlib.h>
#include "nonmatch/func_80023BDC.c"
struct VoiceState { int marker; };
static VoiceState voice;
static MacroCommand command;
static unsigned int replacement0, replacement1;
static int mutate_at, left_value, right_value, read_count, write_count;
static unsigned int selectors[4], destination[2];
static int output_value;

s16 func_80023AD4(VoiceState *state, u8 controller, u8 index)
{
    int value;
    if (state != &voice || read_count >= 2) abort();
    selectors[2*read_count] = controller;
    selectors[2*read_count+1] = index;
    value = read_count == 0 ? left_value : right_value;
    ++read_count;
    if (mutate_at == read_count) {
        command.word0 = replacement0;
        command.word1 = replacement1;
    }
    return (s16)value;
}

void func_80023B50(VoiceState *state, u8 controller, u8 index, s16 value)
{
    if (state != &voice || write_count != 0) abort();
    ++write_count;
    destination[0] = controller;
    destination[1] = index;
    output_value = value;
}

int main(void)
{
    unsigned int operation;
    int result;
    while (scanf("%u %u %u %d %d %d %u %u", &operation,
                 &command.word0, &command.word1, &left_value, &right_value,
                 &mutate_at, &replacement0, &replacement1) == 8) {
        if (operation > 4 || left_value < -32768 || left_value > 32767 ||
            right_value < -32768 || right_value > 32767) abort();
        read_count = write_count = 0;
        selectors[0] = selectors[1] = selectors[2] = selectors[3] = 0;
        result = func_80023BDC(&voice, &command, (u8)operation);
        printf("%d %d %d %u %u %u %u %u %u %d %u %u\n", result,
               read_count, write_count, selectors[0], selectors[1],
               selectors[2], selectors[3], destination[0], destination[1],
               output_value, command.word0, command.word1);
    }
    return 0;
}
