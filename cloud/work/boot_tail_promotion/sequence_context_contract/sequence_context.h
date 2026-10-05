/* Native N64 layout, reconciled from lib_17dc0 consumers and constructors.
 * Unknown ranges are real record storage, not stack/code padding.
 * This research adapter is not yet a production type or a promotion. */
#ifndef BOOT_TAIL_SEQUENCE_CONTEXT_H
#define BOOT_TAIL_SEQUENCE_CONTEXT_H

typedef struct SequenceNode {
    struct SequenceNode *next;
    struct SequenceNode *previous;
    unsigned int identifier;
    unsigned char unknown0C[0x0C];
} SequenceNode;

typedef struct SequenceTimedValue {
    unsigned int time;
    unsigned int value;
} SequenceTimedValue;

typedef struct SequenceContext {
    unsigned int identifier;                  /* 000 */
    unsigned char unknown004[0x10C];
    unsigned int first110;
    unsigned int second114;
    unsigned char unknown118[8];
    unsigned int half120;
    unsigned int rate124;
    unsigned char unknown128[0x400];
    unsigned char channels528[64];
    unsigned char unknown568[0xA00];
    SequenceTimedValue *timingStartF68;
    SequenceTimedValue *currentF6C;
    unsigned int lowF70;
    unsigned int highF74;
    SequenceNode *active;                      /* F78 */
    SequenceNode *pending;                     /* F7C */
    unsigned char unknownF80[0x40];
    unsigned char activeFC0;
    unsigned char inactiveFC1;
    unsigned short valueFC2;
    unsigned char channelFC4;
    unsigned char unknownFC5[0x1B];
    unsigned char valueFE0;
    unsigned char unknownFE1[3];
    unsigned int firstFE4;
    unsigned int secondFE8;
    unsigned short valueFEC;
    unsigned char flagsFEE;
    unsigned char unknownFEF;
    unsigned int pendingFF0;
    unsigned char unknownFF4[4];
} SequenceContext;

extern SequenceNode *D_80043EB0;
extern SequenceContext D_80043EB8[8];
extern SequenceContext *D_8004BE80;
extern unsigned int D_8004BE78;
extern unsigned char D_8004BE7C;
extern unsigned int D_8004F808;

void func_8001729C(SequenceContext *);
void func_8001734C(SequenceContext *);
void func_80017470(SequenceNode *);
void func_80017540(void);
unsigned int func_80017644(unsigned int);
unsigned char func_80017D38(void);
void func_800180A0(void);
int func_80018184(void);
void func_8001824C(void);
void func_80018448(void);
unsigned char func_80018634(void);
void func_8001897C(void);
void func_80018A30(void);
void func_80018AEC(void);
void func_80018C2C(unsigned int);
void func_80018D40(unsigned int);
void func_80019194(unsigned char, unsigned short, unsigned int, unsigned char);
void func_80019A60(unsigned int, unsigned char);
void func_8001B9F8(unsigned char, unsigned short, unsigned char,
                   unsigned char, unsigned int);
int func_8001BDB8(unsigned char);
unsigned int func_800201D0(unsigned int);
#endif
