typedef struct {
    s8 connected;       /* 0x0 */
    u8 pad1[3];
    u32 buttons;        /* 0x4 */
    s8 stick[2];        /* 0x8 */
    u8 padA[6];
} ContRec16;
#define gContRec   ((ContRec16 *) &D_80156CF0)
#define gButtons   ((u32 *) &D_8013FED0)
#define gPressed   ((u32 *) &D_801403C0)
#define gStickF    ((f32 (*)[2]) &D_80140620)

void func_800E7134(void)
{
    s32 i;
    u32 buttons;
    u32 pressed;

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
                gStickCal[i][0]++;
            }
            if ((gStickCal[i][1] < gStickRaw[i][1] || gStickRaw[i][1] < -gStickCal[i][1]) && gStickCal[i][1] < 127) {
                gStickCal[i][1]++;
            }
        }
        gButtons[i] = buttons;
        pressed = (gHeld[i] ^ buttons) & buttons;
        gPrevPressed[i] |= pressed;
        gHeld[i] = buttons;
        gPressed[i] = pressed;
        gStickF[i][0] = func_800C9590(1.0f, gStickCal[i][0], gStickRaw[i][0]);
        gStickF[i][1] = func_800C9590(1.0f, gStickCal[i][1], gStickRaw[i][1]);
    }
    osSendMesg((OSMesgQueue *) &D_801497A8, NULL, 0);
}
