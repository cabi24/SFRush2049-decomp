extern s32 DPC_BUFBUSY_REG;
extern s32 DPC_CLOCK_REG;
extern s32 DPC_PIPEBUSY_REG;
extern s32 DPC_TMEM_REG;

void func_800099B0(void *arg0) {
    void *temp_a0;

    arg0->unk0 = (s32) DPC_CLOCK_REG;
    temp_a0 = arg0 + 0xC;
    temp_a0->unk-8 = (s32) DPC_BUFBUSY_REG;
    temp_a0->unk-4 = (s32) DPC_PIPEBUSY_REG;
    arg0->unkC = (s32) DPC_TMEM_REG;
}
