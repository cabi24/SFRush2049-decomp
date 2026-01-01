/**
 * render.h - 3D rendering pipeline for Rush 2049 N64
 *
 * Based on arcade game/visuals.c rendering functions and N64 F3DEX2 microcode.
 * Handles model rendering, display list construction, and visual effects.
 *
 * Key N64 functions (from game code analysis):
 * - render_object @ 0x80099BFC - 10KB, main 3D model render
 * - render_large @ 0x80087A08 - 10KB, large render function
 * - render_scene @ 0x800A04C4 - 2.7KB, viewport/camera setup
 * - render_helper @ 0x800B65B4 - rendering utility
 *
 * Uses F3DEX2 microcode:
 * - gSPMatrix, gSPVertex, gSP2Triangles for geometry
 * - gDPSetCombineMode, gDPSetRenderMode for materials
 * - gSPDisplayList for nested display lists
 */

#ifndef RENDER_H
#define RENDER_H

#include "types.h"
#include "game/visuals.h"

/* ======================= DEFINES ======================== */

/* Render modes (arcade dis.h compatibility) */
#define RENDER_NORMAL       0       /* Standard opaque rendering */
#define RENDER_TRANSLUCENT  1       /* Alpha-blended rendering */
#define RENDER_ADDITIVE     2       /* Additive blending (sparks, explosions) */
#define RENDER_SHADOW       3       /* Shadow projection rendering */

/* Object visibility flags (arcade dis.h) */
#define OBJ_FLAG_VISIBLE    0x0001  /* Object is visible */
#define OBJ_FLAG_HIDDEN     0x0002  /* Object is hidden */
#define OBJ_FLAG_SORT_ALPHA 0x0004  /* Sort for alpha blending */
#define OBJ_FLAG_CAR_BODY   0x0008  /* Part of car body (for reflections) */
#define OBJ_FLAG_CAR_WINDOW 0x0010  /* Car window (special reflection) */
#define OBJ_FLAG_CAMERA_FACE 0x0020 /* Always face camera (billboards) */

/* Display list buffer sizes */
#define GFX_POOL_SIZE       0x10000 /* 64KB display list pool */
#define GFX_STACK_SIZE      0x0400  /* Matrix stack entries */
#define GFX_VTX_CACHE_SIZE  32      /* Vertex cache size for F3DEX2 */

/* Render priority (for sorting) */
#define RENDER_PRI_BACKGROUND   0   /* Sky, far terrain */
#define RENDER_PRI_TERRAIN      1   /* Track surface */
#define RENDER_PRI_OBJECTS      2   /* Track objects, buildings */
#define RENDER_PRI_CARS         3   /* Car bodies */
#define RENDER_PRI_EFFECTS      4   /* Smoke, sparks, etc. */
#define RENDER_PRI_HUD          5   /* HUD overlay */

/* F3DEX2 geometry mode flags (libultra compatibility) */
#ifndef G_ZBUFFER
#define G_ZBUFFER           0x00000001
#define G_SHADE             0x00000004
#define G_CULL_FRONT        0x00000200
#define G_CULL_BACK         0x00000400
#define G_CULL_BOTH         (G_CULL_FRONT | G_CULL_BACK)
#define G_FOG               0x00010000
#define G_LIGHTING          0x00020000
#define G_TEXTURE_GEN       0x00040000
#define G_TEXTURE_GEN_LINEAR 0x00080000
#define G_SHADING_SMOOTH    0x00200000
#endif

/* ======================= TYPEDEFS ======================== */

/**
 * RenderObject - Renderable 3D object instance
 * Tracks an object's position, orientation, and render state.
 */
typedef struct RenderObject {
    s32     objnum;             /* Object definition index (-1 if unused) */
    s32     parent;             /* Parent object index (-1 if root) */
    f32     pos[3];             /* World position (x, y, z) */
    f32     orient[3][3];       /* 3x3 rotation matrix */
    f32     scale[3];           /* Scale factors (x, y, z) */
    u32     flags;              /* OBJ_FLAG_* bitfield */
    u8      alpha;              /* Alpha/translucency (0-255) */
    u8      priority;           /* Render priority */
    s16     sortOffset;         /* Z-sort offset for ordering */
} RenderObject;

/**
 * RenderState - Current rendering context
 */
typedef struct RenderState {
    Gfx     *dlHead;            /* Display list head */
    Gfx     *dlEnd;             /* Display list end */
    Gfx     *dlCurrent;         /* Current position in display list */
    Mtx     *mtxStack;          /* Matrix stack */
    s32     mtxDepth;           /* Current matrix stack depth */
    f32     viewMatrix[4][4];   /* View matrix */
    f32     projMatrix[4][4];   /* Projection matrix */
    u32     geomMode;           /* Current geometry mode flags */
    u32     renderMode;         /* Current render mode */
    s32     fogNear;            /* Fog near distance */
    s32     fogFar;             /* Fog far distance */
    u8      fogColor[4];        /* Fog color (RGBA) */
} RenderState;

/**
 * RenderVertex - Vertex data for rendering
 */
typedef struct RenderVertex {
    s16     x, y, z;            /* Position */
    s16     s, t;               /* Texture coordinates */
    u8      r, g, b, a;         /* Vertex color/alpha */
    s8      nx, ny, nz;         /* Normal (for lighting) */
    u8      pad;                /* Padding */
} RenderVertex;

/**
 * TextureInfo - Texture descriptor
 */
typedef struct TextureInfo {
    u32     address;            /* ROM/RDRAM address */
    u16     width;              /* Width in pixels */
    u16     height;             /* Height in pixels */
    u8      format;             /* Texture format (RGBA, CI, IA, etc.) */
    u8      size;               /* Texel size (4, 8, 16, 32 bits) */
    u8      wrapS;              /* S-axis wrap mode */
    u8      wrapT;              /* T-axis wrap mode */
} TextureInfo;

/* ======================= GLOBAL VARIABLES ======================== */

/* Current render state */
extern RenderState gRenderState;

/* Display list pointers (from gfx.c) */
extern Gfx *gDisplayListHead;
extern Gfx *gDisplayListEnd;
extern u32 gDisplayListSize;

/* Camera position (for culling, sorting) */
extern f32 gCamPos[3];
extern f32 gCamDir[3];
extern f32 gCamUp[3];

/* View frustum planes (for culling) */
extern f32 gFrustumPlanes[6][4];

/* Object list for track objects (arcade gObjList) */
extern s32 gObjList[];

/* ======================= FUNCTION DECLARATIONS ======================== */

/* ---- Initialization ---- */

/**
 * render_init - Initialize rendering subsystem
 * Sets up display lists, matrix stack, and default render state.
 */
void render_init(void);

/**
 * render_frame_start - Begin a new frame
 * Resets display list and prepares for rendering.
 */
void render_frame_start(void);

/**
 * render_frame_end - End current frame
 * Finalizes display list and submits to RSP.
 */
void render_frame_end(void);

/* ---- Display List Management (from gfx.c) ---- */

/**
 * gfx_init_dl - Initialize display list buffer
 * @param start Pointer to start of display list buffer
 * @param end Pointer to end of display list buffer
 */
void gfx_init_dl(Gfx *start, Gfx *end);

/**
 * gfx_alloc_dl - Allocate space in display list
 * @param size Number of Gfx commands to allocate
 * @return Pointer to allocated display list space
 */
Gfx *gfx_alloc_dl(u32 size);

/* ---- Scene Setup ---- */

/**
 * render_set_viewport - Configure viewport
 * @param x Left edge
 * @param y Top edge
 * @param width Viewport width
 * @param height Viewport height
 */
void render_set_viewport(s32 x, s32 y, s32 width, s32 height);

/**
 * render_set_projection - Set projection matrix
 * @param fovy Field of view (Y axis, degrees)
 * @param aspect Aspect ratio (width/height)
 * @param near Near clip plane
 * @param far Far clip plane
 */
void render_set_projection(f32 fovy, f32 aspect, f32 near, f32 far);

/**
 * render_set_camera - Set view matrix from camera parameters
 * @param eye Camera position
 * @param target Look-at target
 * @param up Up vector
 */
void render_set_camera(f32 eye[3], f32 target[3], f32 up[3]);

/**
 * render_set_fog - Configure fog parameters
 * @param near Fog start distance
 * @param far Fog end distance (fully fogged)
 * @param r Red component
 * @param g Green component
 * @param b Blue component
 */
void render_set_fog(s32 near, s32 far, u8 r, u8 g, u8 b);

/* ---- Object Rendering (arcade visuals.c compatibility) ---- */

/**
 * render_object - Render a 3D object
 * Address: 0x80099BFC (10KB function)
 *
 * Main model rendering function. Handles:
 * - Matrix setup from object transform
 * - Texture/material binding
 * - Geometry submission to RSP
 *
 * @param obj Object to render
 */
void render_object(RenderObject *obj);

/**
 * render_scene_large - Render large scene geometry
 * Address: 0x80087A08 (10KB function)
 *
 * Handles track/world rendering with LOD and culling.
 */
void render_scene_large(void);

/**
 * render_scene - Set up scene for rendering
 * Address: 0x800A04C4 (2.7KB function)
 *
 * Configures viewport, projection, and view matrices.
 */
void render_scene(void);

/**
 * render_util_common - Common rendering utility
 * Address: 0x800B65B4 (called 65 times)
 *
 * Shared rendering operations.
 */
void render_util_common(void);

/* ---- Object Visibility (arcade MBOX functions) ---- */

/**
 * MBOX_ShowObject - Show an object and optionally children
 * Based on arcade: MBOX_ShowObject()
 *
 * @param objnum Object index
 * @param mode SHOW_ONLY or SHOW_EACHCHILD
 */
void MBOX_ShowObject(s32 objnum, s32 mode);

/**
 * MBOX_HideObject - Hide an object and optionally children
 * Based on arcade: MBOX_HideObject()
 *
 * @param objnum Object index
 * @param mode HIDE_ONLY or HIDE_EACHCHILD
 */
void MBOX_HideObject(s32 objnum, s32 mode);

/**
 * MBOX_SetObjectFlags - Set object attribute flags
 * Based on arcade: ZOID_SetObjectFlags()
 *
 * @param objnum Object index
 * @param flags OBJ_FLAG_* bitfield
 */
void MBOX_SetObjectFlags(s32 objnum, u32 flags);

/**
 * MBOX_ObjectFlags - Get object attribute flags
 * Based on arcade: ZOID_ObjectFlags()
 *
 * @param objnum Object index
 * @return Current flags
 */
u32 MBOX_ObjectFlags(s32 objnum);

/**
 * MBOX_SetObjectDef - Change object's model definition
 * Based on arcade: ZOID_SetObjectDef()
 *
 * @param objnum Object instance index
 * @param defnum Model definition index
 */
void MBOX_SetObjectDef(s32 objnum, s32 defnum);

/**
 * MBOX_FindObject - Find object by name
 * Based on arcade: MBOX_FindObject()
 *
 * @param name Object name string
 * @return Object index, or -1 if not found
 */
s32 MBOX_FindObject(const char *name);

/**
 * MBOX_SetSortOffset - Set Z-sort offset for object
 * Based on arcade: ZOID_SetSortOffset()
 *
 * @param objnum Object index
 * @param offset Sort offset (negative = draw later)
 */
void MBOX_SetSortOffset(s32 objnum, f32 offset);

/* ---- Texture Management ---- */

/**
 * MBOX_FindTexture - Find texture by name
 * Based on arcade: MBOX_FindTexture()
 *
 * @param name Texture name string
 * @param out_index Output texture index
 * @return 0 on success, -1 on failure
 */
s32 MBOX_FindTexture(const char *name, s32 *out_index);

/**
 * ZOID_GetRomTex - Get ROM texture pointer
 * Based on arcade: ZOID_GetRomTex()
 *
 * @param texIndex Texture index
 * @return Texture data pointer
 */
void *ZOID_GetRomTex(s32 texIndex);

/**
 * ZOID_SetReflect - Set reflection texture on object
 * Based on arcade: ZOID_SetReflect()
 *
 * @param objnum Object index
 * @param texture Texture data pointer
 * @param speed Reflection animation speed
 */
void ZOID_SetReflect(s32 objnum, void *texture, f32 speed);

/* ---- Matrix Operations ---- */

/**
 * render_push_matrix - Push current matrix onto stack
 */
void render_push_matrix(void);

/**
 * render_pop_matrix - Pop matrix from stack
 */
void render_pop_matrix(void);

/**
 * render_translate - Apply translation to current matrix
 * @param x X translation
 * @param y Y translation
 * @param z Z translation
 */
void render_translate(f32 x, f32 y, f32 z);

/**
 * render_rotate - Apply rotation to current matrix
 * @param angle Rotation angle (degrees)
 * @param x X axis component
 * @param y Y axis component
 * @param z Z axis component
 */
void render_rotate(f32 angle, f32 x, f32 y, f32 z);

/**
 * render_scale - Apply scale to current matrix
 * @param x X scale
 * @param y Y scale
 * @param z Z scale
 */
void render_scale(f32 x, f32 y, f32 z);

/* ---- Geometry Mode ---- */

/**
 * render_set_geometry_mode - Set RSP geometry mode flags
 * @param flags G_ZBUFFER, G_LIGHTING, etc.
 */
void render_set_geometry_mode(u32 flags);

/**
 * render_clear_geometry_mode - Clear RSP geometry mode flags
 * @param flags Flags to clear
 */
void render_clear_geometry_mode(u32 flags);

/* ---- Culling ---- */

/**
 * render_cull_sphere - Test if sphere is visible
 * @param center Sphere center
 * @param radius Sphere radius
 * @return 1 if visible, 0 if culled
 */
s32 render_cull_sphere(f32 center[3], f32 radius);

/**
 * render_cull_box - Test if bounding box is visible
 * @param min Box minimum corner
 * @param max Box maximum corner
 * @return 1 if visible, 0 if culled
 */
s32 render_cull_box(f32 min[3], f32 max[3]);

/* ---- Debug Rendering ---- */

/**
 * render_debug_line - Draw a debug line
 * @param start Line start point
 * @param end Line end point
 * @param r Red component
 * @param g Green component
 * @param b Blue component
 */
void render_debug_line(f32 start[3], f32 end[3], u8 r, u8 g, u8 b);

/**
 * render_debug_box - Draw a debug bounding box
 * @param min Box minimum corner
 * @param max Box maximum corner
 * @param r Red component
 * @param g Green component
 * @param b Blue component
 */
void render_debug_box(f32 min[3], f32 max[3], u8 r, u8 g, u8 b);

#endif /* RENDER_H */
