/* Genuine MBOX visibility/accessor and ZOID texture-setter source boundaries.
 * Native 0x44-byte object view; unused areas describe actual record offsets.
 * Three already-locked native routines; no new accepted-byte claim.
 * Later source/DWARF provenance and limitations are in README.md. */
typedef signed char s8; typedef unsigned char u8; typedef signed short s16; typedef unsigned short u16; typedef int s32; typedef unsigned int u32;
/* Native stride/used offsets, not the later-platform instance layout. */
typedef struct Obj { u32 flags; u8 pad04[0x12]; s16 child,next; u8 pad1A[6]; void *texture[4]; u8 tail30[0x14]; } Obj;
extern Obj D_8012E700[];
int MBOX_GetChild(int); int MBOX_GetNext(int); unsigned int ZOID_ObjectFlags(s16); void ZOID_SetObjectFlags(int,unsigned int);
void model_transform_setup(int,int,int); void model_data_load(int,int,int);
void model_data_load(int dlidx, int mode, int camlink)
{
    unsigned int flags;
    int tmpidx;
    if (mode == 2) {
        flags = ZOID_ObjectFlags(dlidx);
        flags |= camlink << 8;
        ZOID_SetObjectFlags(dlidx, flags);
        tmpidx = MBOX_GetChild(dlidx);
        if (tmpidx != -1) {
            model_data_load(tmpidx, 3, camlink);
        }
    } else if (mode == 0) {
        flags = ZOID_ObjectFlags(dlidx);
        ZOID_SetObjectFlags(dlidx, flags | 0x80000000);
    } else if (mode == 1) {
        flags = ZOID_ObjectFlags(dlidx);
        flags |= camlink << 8;
        ZOID_SetObjectFlags(dlidx, flags);
    } else if (mode == 3) {
        do {
            flags = ZOID_ObjectFlags(dlidx);
            flags |= camlink << 8;
            ZOID_SetObjectFlags(dlidx, flags);
            tmpidx = MBOX_GetChild(dlidx);
            if (tmpidx != -1) {
                model_data_load(tmpidx, 3, camlink);
            }
            dlidx = MBOX_GetNext(dlidx);
        } while (dlidx != -1);
    }
}

void model_transform_setup(int dlidx, int mode, int camlink)
{
    unsigned int flags;
    int tmpidx;
    if (mode == 1) {
        flags = ZOID_ObjectFlags(dlidx);
        flags &= ~((camlink << 8) | 0x80000000);
        ZOID_SetObjectFlags(dlidx, flags);
        tmpidx = MBOX_GetChild(dlidx);
        if (tmpidx != -1) {
            model_transform_setup(tmpidx, 2, camlink);
        }
    } else if (mode == 0) {
        flags = ZOID_ObjectFlags(dlidx);
        flags &= ~((camlink << 8) | 0x80000000);
        ZOID_SetObjectFlags(dlidx, flags);
    } else if (mode == 0) {
        /* unreachable: tests mode == 0 a second time */
        flags = ZOID_ObjectFlags(dlidx);
        flags &= ~0x80000000;
        flags |= camlink << 8;
        ZOID_SetObjectFlags(dlidx, flags);
    } else if (mode == 2) {
        do {
            flags = ZOID_ObjectFlags(dlidx);
            flags &= ~((camlink << 8) | 0x80000000);
            ZOID_SetObjectFlags(dlidx, flags);
            tmpidx = MBOX_GetChild(dlidx);
            if (tmpidx != -1) {
                model_transform_setup(tmpidx, 2, camlink);
            }
            dlidx = MBOX_GetNext(dlidx);
        } while (dlidx != -1);
    }
}

int MBOX_GetChild(int dlidx)
{
    return D_8012E700[dlidx].child;
}

int MBOX_GetNext(int dlidx)
{
    return D_8012E700[dlidx].next;
}

unsigned int ZOID_ObjectFlags(signed short objnum)
{
    return D_8012E700[objnum].flags;
}

void ZOID_SetObjectFlags(int objnum, unsigned int flags)
{
    D_8012E700[objnum].flags = flags;
}

typedef struct TexDef TexDef;
void func_8008D870(s16 objnum, TexDef *tex, int lod) {
 Obj *obj = &D_8012E700[objnum];
 if(lod<0) { obj->texture[0]=tex;obj->texture[1]=tex;obj->texture[2]=tex;obj->texture[3]=tex; }
 else obj->texture[lod]=tex;
}
