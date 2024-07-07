#include "patches.h"
#include "global.h"
#include "ultra64.h"

#include "apcommon.h"
#include "misc_funcs.h"

struct DmChar02;

#define FLAGS (ACTOR_FLAG_10 | ACTOR_FLAG_20)

#define THIS ((DmChar02*)thisx)

typedef void (*DmChar02ActionFunc)(struct DmChar02*, PlayState*);

typedef struct DmChar02 {
    /* 0x000 */ Actor actor;
    /* 0x144 */ SkelAnime skelAnime;
    /* 0x188 */ UNK_TYPE1 pad_188[0x108];
    /* 0x290 */ DmChar02ActionFunc actionFunc;
    /* 0x294 */ UNK_TYPE1 pad_294[0x4C];
    /* 0x2E0 */ s16 animIndex;
    /* 0x2E2 */ UNK_TYPE1 pad_2E2[0xE];
    /* 0x2F0 */ u32 unk_2F0;
} DmChar02; // size = 0x2F4

void DmChar02_Init(Actor* thisx, PlayState* play);
void DmChar02_Destroy(Actor* thisx, PlayState* play);
void DmChar02_Update(Actor* thisx, PlayState* play);
void DmChar02_Draw(Actor* thisx, PlayState* play);

void DmChar02_HandleCutscene(DmChar02* this, PlayState* play);
void DmChar02_ChangeAnim(SkelAnime* skelAnime, AnimationInfo* animInfo, u16 animIndex);

ActorInit Dm_Char02_InitVars = {
    /**/ ACTOR_DM_CHAR02,
    /**/ ACTORCAT_ITEMACTION,
    /**/ FLAGS,
    /**/ OBJECT_STK2,
    /**/ sizeof(DmChar02),
    /**/ DmChar02_Init,
    /**/ DmChar02_Destroy,
    /**/ DmChar02_Update,
    /**/ DmChar02_Draw,
};

typedef enum {
    /* 0 */ DMCHAR02_ANIM_HIT_GROUND,
    /* 1 */ DMCHAR02_ANIM_TURN_AROUND,
    /* 2 */ DMCHAR02_ANIM_JUGGLE,
    /* 3 */ DMCHAR02_ANIM_FALL,
    /* 4 */ DMCHAR02_ANIM_MAX
} DmChar02Animation;

extern AnimationHeader gClockTowerOcarinaOfTimeHitGroundAnim;
extern AnimationHeader gClockTowerOcarinaOfTimeTurnAroundAnim;
extern AnimationHeader gClockTowerOcarinaOfTimeJuggleAnim;
extern AnimationHeader gClockTowerOcarinaOfTimeFallAnim;

extern FlexSkeletonHeader gClockTowerOcarinaOfTimeSkel;

AnimationHeader gClockTowerOcarinaOfTimeHitGroundAnim_reloc;
AnimationHeader gClockTowerOcarinaOfTimeTurnAroundAnim_reloc;
AnimationHeader gClockTowerOcarinaOfTimeJuggleAnim_reloc;
AnimationHeader gClockTowerOcarinaOfTimeFallAnim_reloc;

FlexSkeletonHeader gClockTowerOcarinaOfTimeSkel_reloc;

static AnimationInfo sAnimationInfo_reloc[DMCHAR02_ANIM_MAX] = {
    { &gClockTowerOcarinaOfTimeHitGroundAnim_reloc, 1.0f, 0.0f, -1.0f, ANIMMODE_ONCE, 0.0f },  // DMCHAR02_ANIM_HIT_GROUND
    { &gClockTowerOcarinaOfTimeTurnAroundAnim_reloc, 1.0f, 0.0f, -1.0f, ANIMMODE_ONCE, 0.0f }, // DMCHAR02_ANIM_TURN_AROUND
    { &gClockTowerOcarinaOfTimeJuggleAnim_reloc, 1.0f, 0.0f, -1.0f, ANIMMODE_LOOP, 0.0f },     // DMCHAR02_ANIM_JUGGLE
    { &gClockTowerOcarinaOfTimeFallAnim_reloc, 1.0f, 0.0f, -1.0f, ANIMMODE_ONCE, 0.0f },       // DMCHAR02_ANIM_FALL
};

void DmChar02_Init(Actor* thisx, PlayState* play) {
    DmChar02* this = THIS;

    gClockTowerOcarinaOfTimeHitGroundAnim_reloc = *((AnimationHeader*) actor_relocate(&this->actor, gClockTowerOcarinaOfTimeHitGroundAnim));
    gClockTowerOcarinaOfTimeTurnAroundAnim_reloc = *((AnimationHeader*) actor_relocate(&this->actor, gClockTowerOcarinaOfTimeTurnAroundAnim));
    gClockTowerOcarinaOfTimeJuggleAnim_reloc = *((AnimationHeader*) actor_relocate(&this->actor, gClockTowerOcarinaOfTimeJuggleAnim));
    gClockTowerOcarinaOfTimeFallAnim_reloc = *((AnimationHeader*) actor_relocate(&this->actor, gClockTowerOcarinaOfTimeFallAnim));
    
    gClockTowerOcarinaOfTimeSkel_reloc = *((FlexSkeletonHeader*) actor_relocate(&this->actor, gClockTowerOcarinaOfTimeSkel));

    //if (gSaveContext.save.saveInfo.inventory.items[SLOT_OCARINA] == ITEM_NONE) {
    if (!recomp_location_is_checked(GI_OCARINA_OF_TIME)) {
        this->animIndex = DMCHAR02_ANIM_HIT_GROUND;
        this->actor.targetArrowOffset = 3000.0f;
        ActorShape_Init(&this->actor.shape, 0.0f, ActorShadow_DrawCircle, 24.0f);
        SkelAnime_InitFlex(play, &this->skelAnime, &gClockTowerOcarinaOfTimeSkel_reloc, NULL, NULL, NULL, 0);
        DmChar02_ChangeAnim(&this->skelAnime, &sAnimationInfo_reloc[DMCHAR02_ANIM_HIT_GROUND], 0);
        Actor_SetScale(&this->actor, 0.01f);
        this->actionFunc = (DmChar02ActionFunc) actor_relocate(&this->actor, DmChar02_HandleCutscene);
    } else {
        Actor_Kill(&this->actor);
    }
}

void DmChar02_ChangeAnim(SkelAnime* skelAnime, AnimationInfo* animInfo, u16 animIndex) {
    f32 endFrame;

    animInfo += animIndex;

    if (animInfo->frameCount < 0.0f) {
        endFrame = Animation_GetLastFrame(animInfo->animation);
    } else {
        endFrame = animInfo->frameCount;
    }

    Animation_Change(skelAnime, animInfo->animation, animInfo->playSpeed, animInfo->startFrame, endFrame,
                     animInfo->mode, animInfo->morphFrames);
}