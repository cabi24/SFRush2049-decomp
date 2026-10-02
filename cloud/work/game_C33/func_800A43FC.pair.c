/* flags: -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef signed char s8;
typedef unsigned char u8;
typedef struct Car {u8 pad0[2]; s8 enabled; s8 changed; u8 pad4[768];} Car;
typedef struct State {u8 pad0[2]; u8 flags; u8 pad3;} State;
extern s8 D_8011EAE0;
extern Car D_80144030[4];
extern State D_80149440[4];
extern Car D_80144C40[];
void func_800A43FC(void) {
    Car *car;
    State *state;
    if (D_8011EAE0) {
        car = D_80144030;
        state = D_80149440;
        do {
            if ((state->flags & 1) && !(state->flags & 2)) {
                if (!car->enabled) car->enabled = 1;
            } else {
                if (car->enabled) {car->enabled = 0; car->changed = 1;}
            }
            car++;
            state++;
            if ((state->flags & 1) && !(state->flags & 2)) {
                if (!car->enabled) car->enabled = 1;
            } else {
                if (car->enabled) {car->enabled = 0; car->changed = 1;}
            }
            car++;
            state++;
        } while (car != D_80144C40);
    }
}
