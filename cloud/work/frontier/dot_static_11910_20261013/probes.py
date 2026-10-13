"""Bounded source/storage hypotheses; none is an accepted matching source."""

def replace(text,old,new,count=1):
    if text.count(old)!=count:
        raise ValueError(('probe anchor count',old,count,text.count(old)))
    return text.replace(old,new)


def variants(source):
    union=replace(source,'extern AudioTask D_800382F8;','typedef union { AudioTask t; long long align; } Task;\nextern Task D_800382F8;')
    union=replace(union,'D_800382F8.','D_800382F8.t.',10)
    union=replace(union,'&D_800382F8)','&D_800382F8.t)')
    volatile=replace(source,'extern AudioTask D_800382F8;','extern volatile AudioTask D_800382F8;')
    volatile=replace(volatile,'void (*D_80038000)(AudioTask *)','void (*D_80038000)(volatile AudioTask *)')
    pointer=replace(source,'    if (count','    AudioTask *task = &D_800382F8;\n    if (count')
    pointer=replace(pointer,'D_800382F8.','task->',10)
    pointer=replace(pointer,'D_80038000(&D_800382F8)','D_80038000(task)')
    return {'baseline':source,
            'defined_task':replace(source,'extern AudioTask D_800382F8;','AudioTask D_800382F8;'),
            'aligned_union':union,'volatile_task':volatile,'explicit_pointer':pointer}
