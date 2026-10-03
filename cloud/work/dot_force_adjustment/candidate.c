/* Research reconstruction of func_800E1AA0; offset-view names are hypotheses. */
typedef struct ForceConfig {
    unsigned char unknown_0[36];
    float scale;
} ForceConfig;
typedef struct ForceState {
    unsigned char unknown_0[4];
    ForceConfig *config;
    unsigned char unknown_8[56];
    float direction;
    unsigned char unknown_68[4];
    float height;
    unsigned char unknown_76[4];
    float velocity;
    unsigned char unknown_84[236];
    float force;
    unsigned char unknown_324[656];
    float blend;
    unsigned char unknown_984[24];
    float speed;
    unsigned char unknown_1012[440];
    float bias;
    unsigned char unknown_1456[92];
    int mode_a;
    int mode_b;
    unsigned char unknown_1556[268];
    float magnitude;
    unsigned char unknown_1828[166];
    short direct;
    unsigned char unknown_1996[8];
    unsigned int flags;
} ForceState;
extern float D_801243C0;
void func_800E1AA0(ForceState *state)
{
    float factor, speed, direction;
    if (!(state->flags & 0x10)) {
        if (state->mode_a != 8 || state->mode_b != 8) {
            factor = state->bias;
            factor += state->blend * 0.5f;
            if (factor > 1.0f) factor = 1.0f;
            speed = state->speed;
            if (speed > 100.0f)
                state->force -= state->magnitude * D_801243C0 * factor;
            else
                state->force -= state->magnitude * speed * 120.0f * factor;
        }
        if (!(state->height < 0.0f)) {
            if (state->direct != 0) {
                direction = state->direction;
                if (direction * state->velocity > 0.0f)
                    state->force -= direction * 100.0f;
            } else {
                direction = state->direction;
                if (direction * state->velocity > 0.0f && state->blend < 0.5f)
                    state->force -= direction * 100.0f * state->config->scale;
            }
        }
    }
}
