/* Native host behavioral harness; byte layout checks use the pinned MIPS compiler. */
#include <assert.h>
#include <string.h>
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef struct MacroState { u8 unknown00[0x30]; u32 field30; u8 unknown34[0x2C]; u32 field60; } MacroState;
typedef struct MacroCommand { u32 word[2]; } MacroCommand;
struct MacroState_80021BF0;
struct MacroState_800225FC;
struct MacroState_80022678;
struct MacroState_80023520;
extern u8 func_80021BF0(struct MacroState_80021BF0 *, MacroCommand *);
extern u8 func_800225FC(struct MacroState_800225FC *, MacroCommand *);
extern u8 func_80022678(struct MacroState_80022678 *, MacroCommand *);
extern u8 func_80023520(struct MacroState_80023520 *, MacroCommand *);
static void *expected_state;
static MacroCommand *expected_command;
static int dispatch_calls, audio_calls, active, stop_calls, control_calls;
static u32 seen_control;
static u8 seen_channel, seen_set, seen_note;
u8 func_80021BC0(MacroState *state, MacroCommand *command) {
    assert((void *)state == expected_state && command == expected_command);
    ++stop_calls;
    return 0xA7;
}
u8 func_8002193C(MacroState *state, MacroCommand *command) {
    assert((void *)state == expected_state && command == expected_command);
    assert(command->word[0] == 4);
    ++dispatch_calls;
    return 0xA9;
}
int func_80021764(MacroState *state) {
    assert((void *)state == expected_state);
    return active;
}
void func_80020F4C(u8 channel, u8 set, u8 note) {
    ++audio_calls; seen_channel = channel; seen_set = set; seen_note = note;
}
u8 func_800233B0(MacroState *state, MacroCommand *command, u32 control) {
    assert((void *)state == expected_state && command == expected_command);
    ++control_calls; seen_control = control;
    return 0xAB;
}
static void set16(u8 *state, unsigned offset, u16 value) { memcpy(state + offset, &value, 2); }
static u16 get16(u8 *state, unsigned offset) { u16 value; memcpy(&value, state + offset, 2); return value; }
int main(void) {
    static union { u32 words[64]; u8 bytes[256]; } state;
    static MacroCommand command;
    unsigned i, j, k, a;
    const int steps[] = {-128, -1, 0, 1, 127};
    const u16 notes[] = {0, 127, 256, 0xFFFF};
    int expected;
    expected_state = state.bytes; expected_command = &command;
    for (i=0; i<4; ++i) {
        memset(&state, 0, sizeof(state));
        command.word[0] = (i & 1) << 8;
        state.words[2] = (i & 2) ? 0x81234567U : 0;
        state.words[3] = 0xFDECBA98U;
        stop_calls = 0;
        if (i != 3) {
            assert(func_80021BF0((struct MacroState_80021BF0 *)state.bytes, &command) == 0xA7);
            assert(stop_calls == 1 && state.words[0] == 0 && state.words[1] == 0);
        } else {
            assert(func_80021BF0((struct MacroState_80021BF0 *)state.bytes, &command) == 0);
            assert(stop_calls == 0 && state.words[0] == state.words[2] && state.words[1] == state.words[3]);
        }
    }
    for (a=0; a<2; ++a) for (i=0; i<4; ++i) {
        memset(&state, 0, sizeof(state));
        state.bytes[0x4A]=7; state.bytes[0x4B]=9;
        active=a; dispatch_calls=audio_calls=0;
        command.word[0]=(0x80U+i)<<16 | (0xFEU-i)<<8;
        assert(func_800225FC((struct MacroState_800225FC *)state.bytes, &command)==0xA9);
        assert(get16(state.bytes,0x50)==((0xFEU-i)&127));
        assert(state.bytes[0xC0]==0x80U+i);
        assert(dispatch_calls==1 && audio_calls==(int)a);
        if (a) assert(seen_channel==7 && seen_set==9 && seen_note==((0xFEU-i)&127));
    }
    for (a=0; a<2; ++a) for (i=0; i<2; ++i) for (j=0; j<4; ++j) for (k=0; k<5; ++k) {
        memset(&state,0,sizeof(state));
        state.bytes[0x4A]=13; state.bytes[0x4B]=17;
        set16(state.bytes,0x4E,notes[j]); set16(state.bytes,0x50,notes[3-j]);
        active=a; dispatch_calls=audio_calls=0;
        command.word[0]=(i<<24) | (0xFAU<<16) | ((u32)(u8)steps[k]<<8);
        expected=(u16)((i ? notes[j] : notes[3-j])+steps[k]);
        if ((short)expected<0) expected=0; else if (expected>127) expected=127;
        assert(func_80022678((struct MacroState_80022678 *)state.bytes,&command)==0xA9);
        assert(get16(state.bytes,0x50)==expected && state.bytes[0xC0]==0xFA);
        assert(dispatch_calls==1 && audio_calls==(int)a);
        if (a) assert(seen_channel==13 && seen_set==17 && seen_note==expected);
    }
    for (i=0; i<4; ++i) {
        state.words[0x30/4]=0xFEDCBA98U ^ i;
        control_calls=0;
        assert(func_80023520((struct MacroState_80023520 *)state.bytes,&command)==0xAB);
        assert(control_calls==1 && seen_control==state.words[0x30/4]);
    }
    return 0;
}
