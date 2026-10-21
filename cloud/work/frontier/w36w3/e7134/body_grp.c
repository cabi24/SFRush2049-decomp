/* func_800E7134 (0x800E7134, 676 B): w36w3 (wave 36) -- read the four controller records into the shared pad
 * state, under the SI lock (queue D_801497A8). N64-only, no arcade or MAT3 ancestor. Initialises the controller
 * subsystem first if needed (func_800E7038). Per controller: a disconnected pad has its raw stick zeroed and its
 * stick calibration reset to 70; a connected one copies buttons and stick from its record (D_80156CF0, 16 B,
 * filled by func_800E73D8 from osContGetReadData), turns the stick into d-pad bits when the player's
 * stick-as-dpad flag (D_80149B74, set by player_mode_set) is on, resets the calibration on button bit 0x40000 and
 * grows the per-axis calibration (up to 127) whenever the stick exceeds it. Then buttons (D_8013FED0), newly
 * pressed buttons (D_801403C0), the held/pressed accumulators and the scaled stick floats (D_80140620, via
 * func_800C9590) are updated.
 * Shaping: the shared pad state is read through volatile views (record, raw stick, calibration, held and
 * pressed-accumulator arrays): retail keeps every one of those loads and stores in source order; the stick
 * value passed to func_800C9590 is read into a local first (rawX/rawY: retail loads it before the calibration);
 * pressed is computed in two statements (retail xor/and into one register). */
typedef struct {
    s8 connected;       /* 0x0 */
    u8 pad1[3];
    u32 buttons;        /* 0x4 */
    s8 stick[2];        /* 0x8 */
    u8 padA[6];
} ContRec16;
#define gContRec      ((volatile ContRec16 *) &D_80156CF0)
#define gHeldV        ((volatile s32 *) &D_80149B30)
#define gPrevPressedV ((volatile s32 *) &D_80149B10)
#define gButtons      ((s32 *) &D_8013FED0)
#define gPressed      ((s32 *) &D_801403C0)
#define gStickF       ((f32 (*)[2]) &D_80140620)

void func_800E7134(void)
{
    s32 i;
    u32 buttons;
    u32 pressed;
    s32 rawX;
    s32 rawY;

    if (D_80111954 == 0) {
        func_800E7038();
    }
    osRecvMesg((OSMesgQueue *) &D_801497A8, NULL, 1);
    for (i = 0; i < 4; i++) {
        buttons = 0;
        if (gContRec[i].connected == 0) {
            gStickRaw[i][0] = 0;
            gStickRaw[i][1] = 0;
            gStickCal[i][0] = 70;
            gStickCal[i][1] = 70;
        } else {
            buttons = gContRec[i].buttons;
            gStickRaw[i][0] = gContRec[i].stick[0];
            gStickRaw[i][1] = gContRec[i].stick[1];
            if (D_80149B74[i]) {
                if (gStickRaw[i][0] < -56) {
                    buttons |= 0x1000;
                }
                if (gStickRaw[i][0] > 56) {
                    buttons |= 0x2000;
                }
                if (gStickRaw[i][1] < -56) {
                    buttons |= 0x800;
                }
                if (gStickRaw[i][1] > 56) {
                    buttons |= 0x400;
                }
            }
            if (buttons & 0x40000) {
                gStickCal[i][0] = 70;
                gStickCal[i][1] = 70;
            }
            if ((gStickCal[i][0] < gStickRaw[i][0] || gStickRaw[i][0] < -gStickCal[i][0]) && gStickCal[i][0] < 127) {
                gStickCal[i][0] += 1;
            }
            if ((gStickCal[i][1] < gStickRaw[i][1] || gStickRaw[i][1] < -gStickCal[i][1]) && gStickCal[i][1] < 127) {
                gStickCal[i][1] += 1;
            }
        }
        pressed = gHeldV[i] ^ buttons;
        pressed = pressed & buttons;
        gPrevPressedV[i] |= pressed;
        gHeldV[i] = buttons;
        gPressed[i] = pressed;
        gButtons[i] = buttons;
        rawX = gStickRaw[i][0];
        gStickF[i][0] = func_800C9590(1.0f, gStickCal[i][0], rawX);
        rawY = gStickRaw[i][1];
        gStickF[i][1] = func_800C9590(1.0f, gStickCal[i][1], rawY);
    }
    osSendMesg((OSMesgQueue *) &D_801497A8, NULL, 0);
}
