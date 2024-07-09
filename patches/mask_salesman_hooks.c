#include "patches.h"
#include "global.h"
#include "ultra64.h"

#include "apcommon.h"
#include "misc_funcs.h"

struct EnOsn;

#define FLAGS (ACTOR_FLAG_TARGETABLE | ACTOR_FLAG_FRIENDLY | ACTOR_FLAG_10)

#define THIS ((EnOsn*)thisx)

typedef void (*EnOsnActionFunc)(struct EnOsn*, PlayState*);

#define ENOSN_GET_TYPE(this) ((thisx)->params & 3)
#define OSN_STATE_SPECIAL_CONVERSTATION (1 << 0)

typedef enum {
    /* 0 */ OSN_TYPE_CHOOSE, // Will choose between Idle and Cutscene
    /* 1 */ OSN_TYPE_LIE_FACE_DOWN,
    /* 2 */ OSN_TYPE_LIE_FACE_UP,
    /* 3 */ OSN_TYPE_CUTSCENE
} OsnType;

typedef struct EnOsn {
    /* 0x000 */ Actor actor;
    /* 0x144 */ ColliderCylinder collider;
    /* 0x190 */ SkelAnime skelAnime;
    /* 0x1D4 */ EnOsnActionFunc actionFunc;
    /* 0x1D8 */ Vec3s headRot;
    /* 0x1DE */ Vec3s torsoRot;
    /* 0x1E4 */ UNK_TYPE1 unk_1E4[0x6];
    /* 0x1EA */ u16 stateFlags;
    /* 0x1EC */ u8 animIndex;
    /* 0x1ED */ u8 cueId;
    /* 0x1EE */ s16 csId;
    /* 0x1F0 */ u8 shouldRotateHead;
    /* 0x1F1 */ UNK_TYPE1 unk_1F1[0x3];
    /* 0x1F4 */ u16 textId;
    /* 0x1F6 */ s32 maskTextFlags;
    /* 0x1FA */ u8 alpha;
} EnOsn; // size = 0x1FB

typedef enum {
    /*  0 */ OSN_ANIM_IDLE,
    /*  1 */ OSN_ANIM_ARMS_OUT,
    /*  2 */ OSN_ANIM_BOWING,
    /*  3 */ OSN_ANIM_REMINISCE,
    /*  4 */ OSN_ANIM_HANDS_CLASPED,
    /*  5 */ OSN_ANIM_BELIEVE,
    /*  6 */ OSN_ANIM_THINK,
    /*  7 */ OSN_ANIM_SHAKE_HEAD,
    /*  8 */ OSN_ANIM_ORGAN_TALK,
    /*  9 */ OSN_ANIM_ORGAN_PLAY,
    /* 10 */ OSN_ANIM_SHAKE,
    /* 11 */ OSN_ANIM_CHOKE,
    /* 12 */ OSN_ANIM_DESPAIR,
    /* 13 */ OSN_ANIM_FAST_BOWS,
    /* 14 */ OSN_ANIM_HAND_OUT,
    /* 15 */ OSN_ANIM_LYING_DOWN_FACE_UP,
    /* 16 */ OSN_ANIM_LYING_DOWN_FACE_DOWN,
    /* 17 */ OSN_ANIM_MASK_LOOK_AT,
    /* 18 */ OSN_ANIM_TURN_AROUND_START,
    /* 19 */ OSN_ANIM_TURN_AROUND_LOOP,
    /* 20 */ OSN_ANIM_WALK_AWAY,
    /* 21 */ OSN_ANIM_MASK_LOOK_FROM_START,
    /* 22 */ OSN_ANIM_MASK_LOOK_FROM_LOOP,
    /* 23 */ OSN_ANIM_HAND_OUT_2,    // Exact same as OSN_ANIM_HAND_OUT
    /* 24 */ OSN_ANIM_WALK_AWAY_END, // Only the last frame of OSN_ANIM_WALK_AWAY
    /* 25 */ OSN_ANIM_MAX
} OsnAnimation;

static ColliderCylinderInit sCylinderInit = {
    {
        COLTYPE_NONE,
        AT_NONE,
        AC_NONE,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_1,
        COLSHAPE_CYLINDER,
    },
    {
        ELEMTYPE_UNK0,
        { 0x00000000, 0x00, 0x00 },
        { 0xF7CFFFFF, 0x00, 0x00 },
        TOUCH_NONE | TOUCH_SFX_NORMAL,
        BUMP_ON,
        OCELEM_ON,
    },
    { 30, 40, 0, { 0, 0, 0 } },
};

static CollisionCheckInfoInit2 sColChkInfoInit = { 0, 0, 0, 0, MASS_IMMOVABLE };

static DamageTable sDamageTable = {
    /* Deku Nut       */ DMG_ENTRY(0, 0x0),
    /* Deku Stick     */ DMG_ENTRY(0, 0x0),
    /* Horse trample  */ DMG_ENTRY(0, 0x0),
    /* Explosives     */ DMG_ENTRY(0, 0x0),
    /* Zora boomerang */ DMG_ENTRY(0, 0x0),
    /* Normal arrow   */ DMG_ENTRY(0, 0x0),
    /* UNK_DMG_0x06   */ DMG_ENTRY(0, 0x0),
    /* Hookshot       */ DMG_ENTRY(0, 0x0),
    /* Goron punch    */ DMG_ENTRY(0, 0x0),
    /* Sword          */ DMG_ENTRY(0, 0x0),
    /* Goron pound    */ DMG_ENTRY(0, 0x0),
    /* Fire arrow     */ DMG_ENTRY(0, 0x0),
    /* Ice arrow      */ DMG_ENTRY(0, 0x0),
    /* Light arrow    */ DMG_ENTRY(0, 0x0),
    /* Goron spikes   */ DMG_ENTRY(0, 0x0),
    /* Deku spin      */ DMG_ENTRY(0, 0x0),
    /* Deku bubble    */ DMG_ENTRY(0, 0x0),
    /* Deku launch    */ DMG_ENTRY(0, 0x0),
    /* UNK_DMG_0x12   */ DMG_ENTRY(0, 0x0),
    /* Zora barrier   */ DMG_ENTRY(0, 0x0),
    /* Normal shield  */ DMG_ENTRY(0, 0x0),
    /* Light ray      */ DMG_ENTRY(0, 0x0),
    /* Thrown object  */ DMG_ENTRY(0, 0x0),
    /* Zora punch     */ DMG_ENTRY(0, 0x0),
    /* Spin attack    */ DMG_ENTRY(0, 0x0),
    /* Sword beam     */ DMG_ENTRY(0, 0x0),
    /* Normal Roll    */ DMG_ENTRY(0, 0x0),
    /* UNK_DMG_0x1B   */ DMG_ENTRY(0, 0x0),
    /* UNK_DMG_0x1C   */ DMG_ENTRY(0, 0x0),
    /* Unblockable    */ DMG_ENTRY(0, 0x0),
    /* UNK_DMG_0x1E   */ DMG_ENTRY(0, 0x0),
    /* Powder Keg     */ DMG_ENTRY(0, 0x0),
};

static InitChainEntry sInitChain[] = {
    ICHAIN_U8(targetMode, TARGET_MODE_0, ICHAIN_STOP),
};

extern AnimationInfo sHappyMaskSalesmanAnimationInfo[];
extern FlexSkeletonHeader gHappyMaskSalesmanSkel;
extern AnimationHeader gHappyMaskSalesmanIdleAnim;

s32 Actor_ProcessTalkRequest(Actor* actor, GameState* gameState);

void EnOsn_DoNothing(EnOsn* this, PlayState* play);
void EnOsn_StartCutscene(EnOsn* this, PlayState* play);
s32 EnOsn_GetInitialText(EnOsn* this, PlayState* play);
void EnOsn_Talk(EnOsn* this, PlayState* play);
void EnOsn_InitCutscene(EnOsn* this);
void EnOsn_HandleCsAction(EnOsn* this, PlayState* play);
void EnOsn_ChooseAction(EnOsn* this, PlayState* play);

static bool shouldSetForm = false;
static u32 prevForm;
/*
void EnOsn_Init(Actor* thisx, PlayState* play) {
    s32 pad;
    EnOsn* this = THIS;

    if (shouldSetForm) {
        gSaveContext.save.playerForm = prevForm;
    }

    Actor_ProcessInitChain(&this->actor, sInitChain);
    ActorShape_Init(&this->actor.shape, 0.0f, ActorShadow_DrawCircle, 20.0f);
    SkelAnime_InitFlex(play, &this->skelAnime, &gHappyMaskSalesmanSkel, &gHappyMaskSalesmanIdleAnim, NULL, NULL, 0);
    Collider_InitCylinder(play, &this->collider);
    Collider_SetCylinder(play, &this->collider, &this->actor, &sCylinderInit);
    CollisionCheck_SetInfo2(&this->actor.colChkInfo, &sDamageTable, &sColChkInfoInit);
    this->alpha = 255;

    switch (ENOSN_GET_TYPE(&this->actor)) {
        case OSN_TYPE_CHOOSE:
            if (((gSaveContext.save.entrance == ENTRANCE(CLOCK_TOWER_INTERIOR, 2)) ||
                 (gSaveContext.save.entrance == ENTRANCE(CLOCK_TOWER_INTERIOR, 3))) ||
                (gSaveContext.save.entrance == ENTRANCE(CLOCK_TOWER_INTERIOR, 6))) {
                this->stateFlags |= OSN_STATE_SPECIAL_CONVERSTATION;
            }
            this->shouldRotateHead = true;
            if (play->sceneId == SCENE_INSIDETOWER) {
                if ((gSaveContext.save.entrance == ENTRANCE(CLOCK_TOWER_INTERIOR, 2)) ||
                    (gSaveContext.save.entrance == ENTRANCE(CLOCK_TOWER_INTERIOR, 6))) {
                    this->actionFunc = EnOsn_HandleCsAction;
                } else if (gSaveContext.save.entrance == ENTRANCE(CLOCK_TOWER_INTERIOR, 3)) {
                    EnOsn_InitCutscene(this);
                    this->actionFunc = EnOsn_StartCutscene;
                } else {
                    EnOsn_ChooseAction(this, play);
                }
            } else {
                EnOsn_ChooseAction(this, play);
            }
            break;

        case OSN_TYPE_LIE_FACE_DOWN:
            this->animIndex = OSN_ANIM_LYING_DOWN_FACE_UP;
            Actor_ChangeAnimationByInfo(&this->skelAnime, sHappyMaskSalesmanAnimationInfo, this->animIndex);
            this->actionFunc = EnOsn_DoNothing;
            break;

        case OSN_TYPE_LIE_FACE_UP:
            this->animIndex = OSN_ANIM_LYING_DOWN_FACE_DOWN;
            Actor_ChangeAnimationByInfo(&this->skelAnime, sHappyMaskSalesmanAnimationInfo, this->animIndex);
            this->actionFunc = EnOsn_DoNothing;
            break;

        case OSN_TYPE_CUTSCENE:
            this->actor.flags &= ~ACTOR_FLAG_TARGETABLE;
            this->actionFunc = EnOsn_HandleCsAction;
            break;

        default:
            Actor_Kill(&this->actor);
            break;
    }
}*/

void EnOsn_Idle(EnOsn* this, PlayState* play) {
    s16 yaw = this->actor.yawTowardsPlayer - this->actor.shape.rot.y;

    //if ((gSaveContext.save.saveInfo.inventory.items[SLOT_OCARINA] != ITEM_NONE) &&
    //    !CHECK_QUEST_ITEM(QUEST_SONG_HEALING)) {
    if ((gSaveContext.save.saveInfo.inventory.items[SLOT_OCARINA] != ITEM_NONE) &&
        (!recomp_location_is_checked(0x040068) || !recomp_location_is_checked(GI_MASK_DEKU))) {
        recomp_printf("xz distance: %f, y distance: %f\n", this->actor.xzDistToPlayer, this->actor.playerHeightRel);
        if ((this->actor.xzDistToPlayer <= 60.0f) && (fabsf(this->actor.playerHeightRel) <= fabsf(80.0f))) {
            //shouldSetForm = true;
            //prevForm = gSaveContext.save.playerForm;
            //this->actionFunc = EnOsn_StartCutscene;
            Audio_PlayFanfare(NA_BGM_GET_SMALL_ITEM);
            recomp_send_location(0x040068);
            recomp_send_location(GI_MASK_DEKU);
        } else if (((this->actor.xzDistToPlayer < 100.0f) || this->actor.isLockedOn) && (yaw < 0x4000) &&
                   (yaw > -0x4000)) {
            Actor_OfferTalkNearColChkInfoCylinder(&this->actor, play);
            this->actor.textId = 0xFFFF;
        }
    }/* else if (Actor_ProcessTalkRequest(&this->actor, &play->state)) {
        this->textId = EnOsn_GetInitialText(this, play);
        Message_StartTextbox(play, this->textId, &this->actor);
        this->actionFunc = EnOsn_Talk;
    } else if (((this->actor.xzDistToPlayer < 100.0f) || this->actor.isLockedOn) && (yaw < 0x4000) && (yaw > -0x4000)) {
        Actor_OfferTalkNearColChkInfoCylinder(&this->actor, play);
    }*/
}
/*
void EnOsn_Destroy(Actor* thisx, PlayState* play) {
    EnOsn* this = THIS;

    shouldSetForm = false;

    SkelAnime_Free(&this->skelAnime, play);
    Collider_DestroyCylinder(play, &this->collider);
}*/