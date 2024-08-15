#include "patches.h"
#include "overlays/actors/ovl_Obj_Moon_Stone/z_obj_moon_stone.h"
#include "ultra64.h"

#include "apcommon.h"
#include "misc_funcs.h"

#define THIS ((ObjMoonStone*)thisx)

#define LOCATION_MOONS_TEAR GI_MOONS_TEAR

typedef void (*ActorFunc)(struct Actor* this, struct PlayState* play);

s16 moonsTearTrueGI;

static bool objectStatic;
static bool objectLoading;
static bool objectLoaded;
static OSMesgQueue objectLoadQueue;
static void* objectSegment;

extern AnimatedMaterial gGiMoonsTearTexAnim[];
extern Gfx gGiMoonsTearItemDL[];
extern Gfx gGiMoonsTearGlowDL[];

void* ZeldaArena_Malloc(size_t size);
void func_80C0662C(ObjMoonStone* this);
void func_80C0673C(ObjMoonStone* this);
void func_80C0685C(ObjMoonStone* this);

void ObjMoonStone_Init(Actor* thisx, PlayState* play) {
    ObjMoonStone* this = THIS;

    moonsTearTrueGI = apGetItemId(GI_MOONS_TEAR);
    objectSegment = ZeldaArena_Malloc(0x2000);
    objectStatic = false;
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

    s16 objectSlot = Object_GetSlot(&play->objectCtx, getObjectId(moonsTearTrueGI));

    if (isAP(moonsTearTrueGI)) {
        objectStatic = true;
        objectLoaded = true;
    } else if (!objectLoaded && !objectLoading && Object_IsLoaded(&play->objectCtx, objectSlot)) {
        this->actor.objectSlot = objectSlot;
        Actor_SetObjectDependency(play, &this->actor);
        objectStatic = true;
        objectLoaded = true;
    } else if (!objectLoading && !objectLoaded) {
        loadObject(play, &objectSegment, &objectLoadQueue, getObjectId(moonsTearTrueGI));
        objectLoading = true;
    } else if (osRecvMesg(&objectLoadQueue, NULL, OS_MESG_NOBLOCK) == 0) {
        objectLoading = false;
        objectLoaded = true;
    }

    if (!(player->stateFlags1 & (PLAYER_STATE1_2 | PLAYER_STATE1_80 | PLAYER_STATE1_200 | PLAYER_STATE1_10000000))) {
        this->actionFunc(this, play);
    }
}

void ObjMoonStone_Draw(Actor* thisx, PlayState* play) {
    GraphicsContext* gfxCtx = play->state.gfxCtx;

    OPEN_DISPS(play->state.gfxCtx);

    if (objectLoaded) {
        if (objectStatic) {
            GetItem_Draw(play, getGid(moonsTearTrueGI));
        } else {
            GetItem_DrawDynamic(play, objectSegment, getGid(moonsTearTrueGI));
        }
    }

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
        }
    }
}