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
    u8 *car;
    u8 *state;
    if (D_8011EAE0) {
        car = (u8 *)D_80144030;
        state = (u8 *)D_80149440;
        do {
            if ((state[2] & 1) && !(state[2] & 2)) {
                if (!car[2]) car[2] = 1;
            } else {
                if (car[2]) {car[2] = 0; car[3] = 1;}
            }
            car += 772;
            state += 4;
            if ((state[2] & 1) && !(state[2] & 2)) {
                if (!car[2]) car[2] = 1;
            } else {
                if (car[2]) {car[2] = 0; car[3] = 1;}
            }
            car += 772;
            state += 4;
        } while (car != (u8 *)D_80144C40);
    }
}
