/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef unsigned char u8;
struct VoiceState;
extern void func_80020DA8(u8, struct VoiceState *, struct VoiceState *);
void func_8001B744(struct VoiceState *destination, struct VoiceState *source)
{
    func_80020DA8(7, destination, source);
    func_80020DA8(10, destination, source);
    func_80020DA8(91, destination, source);
    func_80020DA8(128, destination, source);
    func_80020DA8(132, destination, source);
}
