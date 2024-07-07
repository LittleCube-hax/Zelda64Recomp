#include "patches.h"
#include "global.h"

#include "misc_funcs.h"
#include "apcommon.h"

typedef enum {
    OPA0,
    XLU0,
    OPA01,
    OPA0XLU1,
    XLU01
} ObjectType;

ObjectType sGetObjectType[] = {
    OPA0XLU1,
    OPA0,
    OPA01,
    OPA0XLU1,
    XLU01,
    XLU01,
    XLU01,
    XLU01,
    XLU0,
    OPA0XLU1,
    OPA0XLU1,
    OPA0XLU1,
    OPA0XLU1,
    OPA01,
    OPA0XLU1,
    OPA01,
    OPA0XLU1,
    OPA0,
    XLU01,
    XLU0,
    OPA01,  // ?
    OPA01,  // ?
    OPA01,  // ?
    OPA01,  // ?
    OPA01,  // ?
    OPA01,  // ?
    OPA0,
    OPA0,
    OPA0,
    OPA0,
    OPA0,
    OPA0,
    OPA01,  // ?
    OPA01,  // ?
    OPA0XLU1,
    OPA0,
    OPA0,
    OPA0,
    OPA0,
    OPA0XLU1,
    OPA01,
    OPA01,
    OPA0XLU1,
    OPA0XLU1,
    OPA0XLU1,
    OPA01,
    OPA01,
    OPA0XLU1,
    OPA0XLU1,  // ?
    OPA0XLU1,  // ?
    OPA0XLU1,  // ?
    OPA0XLU1,
    OPA0,
    XLU0,
    OPA0XLU1,
    OPA0,
    OPA0,
    OPA0XLU1,
    OPA0XLU1,
    OPA0XLU1,  // ?
    OPA01,
    OPA01,
    OPA0XLU1,
    OPA0XLU1,
    OPA01,
    OPA01,
    OPA0XLU1,
    OPA01,
    OPA0XLU1,
    OPA01,
    OPA0,
    OPA0XLU1,
    OPA0XLU1,
    OPA0XLU1,
    OPA0XLU1,
    OPA0XLU1,
    OPA0XLU1,
    OPA0XLU1,
    OPA0XLU1,
    OPA0XLU1,
    OPA0XLU1,
    OPA0XLU1,
    OPA0XLU1,
    OPA0XLU1,
    OPA0XLU1,
    OPA01,
    OPA0XLU1,
    OPA0XLU1,
    OPA0XLU1,
    OPA0XLU1,
    OPA01,
    OPA0XLU1,
    OPA0,
    OPA0XLU1,
    OPA01,
    OPA0XLU1,
    OPA0XLU1,
    OPA0XLU1,
    OPA0XLU1,
    OPA0,
    OPA0,
    OPA0,
    OPA01,
    OPA01,
    OPA0XLU1,
    OPA0XLU1,
    OPA0XLU1,
    OPA0XLU1,
    OPA0XLU1,
    OPA0XLU1,
    OPA0XLU1,
    OPA0XLU1,
    OPA0XLU1,
    OPA0XLU1,
    OPA01,
    OPA0XLU1,
    OPA0XLU1,
    OPA01
};

static DmaRequest objectDmaRequest;
static OSMesg objectLoadMsg;

bool loadObject(PlayState* play, void* objectSegment, OSMesgQueue* objectLoadQueue, s16 objectId) {
    s32 objectSlot = Object_GetSlot(&play->objectCtx, objectId);

    if (Object_IsLoaded(&play->objectCtx, objectSlot)) {
        return false;
    }

    if (objectId != OBJECT_UNSET_0) {
        osCreateMesgQueue(objectLoadQueue, &objectLoadMsg, 1);
        DmaMgr_SendRequestImpl(&objectDmaRequest, objectSegment, gObjectTable[objectId].vromStart,
                               gObjectTable[objectId].vromEnd - gObjectTable[objectId].vromStart, 0,
                               objectLoadQueue, NULL);
    }

    return true;
}

void GetItem_DrawDynamic(PlayState* play, void* objectSegment, s16 drawId) {
    OPEN_DISPS(play->state.gfxCtx);

    u32 prevSegment = gSegments[6];
    gSegments[6] = OS_K0_TO_PHYSICAL(objectSegment);

    switch (sGetObjectType[drawId]) {
        case OPA0:
            gSPSegment(POLY_OPA_DISP++, 0x06, objectSegment);
            break;
        case XLU0:
            gSPSegment(POLY_XLU_DISP++, 0x06, objectSegment);
            break;
        case OPA01:
            gSPSegment(POLY_OPA_DISP++, 0x06, objectSegment);
            gSPSegment(POLY_OPA_DISP++, 0x06, objectSegment);
            break;
        case XLU01:
            gSPSegment(POLY_XLU_DISP++, 0x06, objectSegment);
            gSPSegment(POLY_XLU_DISP++, 0x06, objectSegment);
            break;
        case OPA0XLU1:
            gSPSegment(POLY_OPA_DISP++, 0x06, objectSegment);
            gSPSegment(POLY_XLU_DISP++, 0x06, objectSegment);
            break;
    }

    GetItem_Draw(play, drawId);

    gSegments[6] = prevSegment;

    CLOSE_DISPS(play->state.gfxCtx);
}