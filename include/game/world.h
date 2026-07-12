/**
 * world.h - World/Object management for Rush 2049 N64
 *
 * Handles loading world data and creating display list objects.
 * Based on arcade source: reference/repos/rushtherock/game/world.h
 */

#ifndef WORLD_H
#define WORLD_H

#include "types.h"

/* World data buffer size */
#define WORLD_BSIZE         0x20000     /* 128KB for world data */

/* World data access macros */
#define WORLD_DATA          WorldData
#define WORLD_HDRS          (WORLD_DATA + sizeof(S32))
#define NWORLDS             (*((S32 *)WORLD_DATA))

/* Special effects flags (from arcade) */
#define SFX_FRONTFACE       0x1         /* Object uses front-face culling */
#define SFX_ANIM_TEXTURE    0x2         /* Object has animated textures */

/* Display list attribute flags */
#define ZATR_SORTALPHA      0x1000      /* Sort translucent objects */

/* World header structure - index entry for finding worlds */
typedef struct WorldHeader {
    U32     woff;               /* Offset from world data start */
    char    wname[16];          /* World name string */
} WorldHeader;

/* World object structure - node in world hierarchy */
typedef struct WorldObject {
    char    name[16];           /* Object name */
    F32     matrix[3][3];       /* 3x3 rotation matrix */
    F32     objpos[3];          /* Position in parent's coordinate frame */
    S32     dlflags;            /* Display list flags */
    S16     next;               /* Next sibling index (-1 if none) */
    S16     child;              /* First child index (-1 if none) */
    S16     sfx1;               /* Special effects flag 1 */
    S16     sfx2;               /* Special effects flag 2 */
} WorldObject;

/* Arcade-compatible typedefs */
typedef WorldHeader WORLDHDR;
typedef WorldObject WOBJ;

/* Function pointer types (from arcade) */
typedef int (*FW_FUNCT)(const void *, const void *);
typedef int (*WD_FUNCT)(S16 init);

/* Global world data */
extern char WorldData[WORLD_BSIZE];     /* World data buffer */
extern WOBJ *WorldBase;                 /* Base node for current world */
extern WOBJ *Current;                   /* Current world object being processed */
extern S16 Worlddlidx;                  /* Display list index for world */

/* World loading */
void world_init(void);
void world_reset(void);
void world_shutdown(void);

/**
 * LoadWorld - Load and instantiate world objects
 *
 * Loads the "TEMP" world from WorldData and creates display list objects
 * for all nodes in the hierarchy. Also initializes road surface data.
 */
void LoadWorld(void);

/**
 * FindWorld - Find a world by name
 *
 * Searches the world table for a world with the given name using binary search.
 * Fatal error if world not found.
 *
 * @param wstr World name to find
 * @return Pointer to world header
 */
WORLDHDR *FindWorld(const char *wstr);

/**
 * NewWorldObject - Create display list objects for world hierarchy
 *
 * Recursively processes world object tree, creating display list objects
 * for each node. Links children to parents via display list indices.
 *
 * @param Wnode Current world object node
 * @param Parent Parent display list index (-1 for root)
 */
void NewWorldObject(WOBJ *Wnode, S16 Parent);

/* World query functions */
s32 world_get_object_count(void);
WOBJ *world_get_object(s32 index);
WOBJ *world_find_object(const char *name);

/* World state */
s32 world_is_loaded(void);

#endif /* WORLD_H */
