/* osAiSetNextBuffer TU additions (context_check refuses it: object has .data) */
#define AI_DRAM_ADDR_REG 0xA4500000
#define AI_LEN_REG 0xA4500004
extern s32 __osAiDeviceBusy(void);
