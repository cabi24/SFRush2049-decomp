#ifndef BOOT_TAIL_SAMPLE_CONTRACT_H
#define BOOT_TAIL_SAMPLE_CONTRACT_H

/* Native shared object contracts for lib_16320 and lib_1cf90. These are
 * object fields, not stack-layout devices. Native offsets are checked by
 * sample_buffer_contract/verify.py; host pointer sizes may differ. */
typedef struct SampleRecord {
    unsigned short identifier;
    unsigned short references;
    unsigned int offset;
    void *data;
    unsigned char descriptor[16];
} SampleRecord;
typedef struct RegisteredSamples {
    SampleRecord *records;
    unsigned char *base;
    unsigned short count;
    unsigned short unknown0A;
} RegisteredSamples;

typedef int (*SampleCallback)(short *, unsigned int, short *, unsigned int,
                              unsigned int);
typedef struct SampleBuffer {
    unsigned char mode;
    unsigned char unknown01[3];
    SampleCallback callback;
    short *buffer;
    unsigned int samples;
    unsigned int position;
    unsigned int context;
} SampleBuffer;

/* 1D084 and 1D578 used only the first 56 bytes. 1DDE0 and the native
 * constructor 1D1F4 establish the remaining fields of the same object. */
typedef struct StateNode {
    struct StateNode *next;
    struct StateNode *previous;
    unsigned int flags08;
    unsigned char unknown0C[40];
    unsigned int identifier34;
    unsigned int group;
    unsigned short sound_id;
    unsigned short counter;
    float fade;
} StateNode;
typedef StateNode StatePrefix;

/* Separate listener-list prefix, not an emitter or integer-valued head. */
typedef struct LinkNode {
    struct LinkNode *next;
    struct LinkNode *previous;
} LinkNode;

#endif
