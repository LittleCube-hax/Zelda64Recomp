#include "patches.h"
#include "overlays/actors/ovl_Obj_Moon_Stone/z_obj_moon_stone.h"
#include "ultra64.h"

#include "apcommon.h"
#include "misc_funcs.h"

#define THIS ((ObjMoonStone*)thisx)

typedef void (*ActorFunc)(struct Actor* this, struct PlayState* play);

static s8 trueGI;

static bool objectLoading;
static bool objectLoaded;
static DmaRequest objectDmaRequest;
static OSMesgQueue objectLoadQueue;
static OSMesg objectLoadMsg;
static void* objectSegment;

extern AnimatedMaterial gGiMoonsTearTexAnim[];
extern Gfx gGiMoonsTearItemDL[];
extern Gfx gGiMoonsTearGlowDL[];

void* ZeldaArena_Malloc(size_t size);
void func_80C0662C(ObjMoonStone* this);
void func_80C0673C(ObjMoonStone* this);
void func_80C0685C(ObjMoonStone* this);

void loadObject(PlayState* play, s16 objectId) {
    if (objectId != OBJECT_UNSET_0) {
        osCreateMesgQueue(&objectLoadQueue, &objectLoadMsg, 1);
        DmaMgr_SendRequestImpl(&objectDmaRequest, objectSegment, gObjectTable[objectId].vromStart,
                               gObjectTable[objectId].vromEnd - gObjectTable[objectId].vromStart, 0,
                               &objectLoadQueue, NULL);
    }
}

void ObjMoonStone_Init(Actor* thisx, PlayState* play) {
    ObjMoonStone* this = THIS;

    trueGI = apGetItemId();
    objectSegment = ZeldaArena_Malloc(0x2000);
    objectLoading = false;
    objectLoaded = false;

    Actor_SetScale(&this->actor, 0.3f);
    this->unk194 = (this->actor.params & 0xF000) >> 0xC;
    this->actor.targetMode = TARGET_MODE_0;
    this->actor.shape.yOffset = 25.0f;
    this->actor.focus.pos.y += 10.0f;
    if (this->unk194 == 0) {
        this->actor.colChkInfo.health = 0;
        this->actor.flags |= (ACTOR_FLAG_TARGETABLE | ACTOR_FLAG_FRIENDLY);
        func_80C0662C(this);
    } else if (!CHECK_WEEKEVENTREG(WEEKEVENTREG_74_40)) {
        if (CHECK_WEEKEVENTREG(WEEKEVENTREG_74_80)) {
            Actor_Spawn(&play->actorCtx, play, 1, this->actor.world.pos.x, this->actor.world.pos.y,
                        this->actor.world.pos.z, 0, 0, 0, -1);
        }
        this->actor.flags &= ~ACTOR_FLAG_TARGETABLE;
        func_80C0673C(this);
    } else {
        Actor_Kill(&this->actor);
    }
}

void ObjMoonStone_Update(Actor* thisx, PlayState* play) {
    ObjMoonStone* this = THIS;
    Player* player = GET_PLAYER(play);

    if (!objectLoading) {
        loadObject(play, OBJECT_GI_RESERVE_C_00);
        objectLoading = true;
    } else if (!objectLoaded && osRecvMesg(&objectLoadQueue, NULL, OS_MESG_NOBLOCK) == 0) {
        objectLoaded = true;
    }

    if (!(player->stateFlags1 & (PLAYER_STATE1_2 | PLAYER_STATE1_80 | PLAYER_STATE1_200 | PLAYER_STATE1_10000000))) {
        this->actionFunc(this, play);
    }
}

void ObjMoonStone_Draw(Actor* thisx, PlayState* play) {
    GraphicsContext* gfxCtx = play->state.gfxCtx;

    OPEN_DISPS(play->state.gfxCtx);

    u32 prevSegment = gSegments[6];
    gSegments[6] = OS_K0_TO_PHYSICAL(objectSegment);

    gSPSegment(POLY_OPA_DISP++, 0x06, objectSegment);
    gSPSegment(POLY_XLU_DISP++, 0x06, objectSegment);

    if (objectLoaded) {
        GetItem_Draw(play, GID_LETTER_TO_KAFEI);
    }

    gSegments[6] = prevSegment;

    /*Gfx_SetupDL25_Opa(play->state.gfxCtx);
    Gfx_SetupDL25_Xlu(play->state.gfxCtx);
    AnimatedMat_Draw(play, Lib_SegmentedToVirtual(gGiMoonsTearTexAnim));
    gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(POLY_OPA_DISP++, gGiMoonsTearItemDL);
    Matrix_ReplaceRotation(&play->billboardMtxF);
    gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(POLY_XLU_DISP++, gGiMoonsTearGlowDL);*/

    CLOSE_DISPS(play->state.gfxCtx);
}

void func_80C06768(ObjMoonStone* this, PlayState* play) {
    if (CHECK_WEEKEVENTREG(WEEKEVENTREG_74_80)) {
        if (this->actor.draw == NULL) {
            this->actor.draw = ObjMoonStone_Draw;
            Actor_Spawn(&play->actorCtx, play, 1, this->actor.world.pos.x, this->actor.world.pos.y,
                        this->actor.world.pos.z, 0, 0, 0, -1);
        }
    }
    if (this->actor.draw) {
        if (Actor_HasParent(&this->actor, play)) {
            this->actor.parent = NULL;
            this->actor.draw = NULL;
            func_80C0685C(this);
        } else if (this->actor.xzDistToPlayer < 25.0f) {
            Actor_OfferGetItem(&this->actor, play, GI_MOONS_TEAR, 100.0f, 30.0f);
            //Actor_OfferGetItem(&this->actor, play, ITEM_AP, 100.0f, 30.0f);
        }
    }
}