#include "patches.h"
#include "global.h"
#include "ultra64.h"

#include "apcommon.h"
#include "misc_funcs.h"

struct DmStk;

typedef void (*DmStkActionFunc)(struct DmStk*, PlayState*);

#define ACTORCTX_FLAG_1 (1 << 1)

#define DM_STK_GET_TYPE(thisx) ((thisx)->params)

#define SKULL_KID_LIMB_MAX 0x16

typedef enum {
    /*  0 */ SK_ANIM_SHAKE_HEAD,
    /*  1 */ SK_ANIM_WALK,
    /*  2 */ SK_ANIM_LOOK_AROUND, // Not used in the final game
    /*  3 */ SK_ANIM_IDLE,
    /*  4 */ SK_ANIM_UNUSED_IDLE, // Unused duplicate of SK_ANIM_IDLE
    /*  5 */ SK_ANIM_BENT_OVER_HEAD_TWITCH,
    /*  6 */ SK_ANIM_GLITCHY_HORSE_RIDE_START, // Not used in the final game
    /*  7 */ SK_ANIM_RECLINING_FLOAT,
    /*  8 */ SK_ANIM_CURSE_START,
    /*  9 */ SK_ANIM_CURSE_LOOP,
    /* 10 */ SK_ANIM_LAUGH_START,
    /* 11 */ SK_ANIM_LAUGH_LOOP,
    /* 12 */ SK_ANIM_HIDE_OCARINA_START,
    /* 13 */ SK_ANIM_HIDE_OCARINA_LOOP,
    /* 14 */ SK_ANIM_PICK_UP_OCARINA_AND_START_PLAYING,
    /* 15 */ SK_ANIM_PLAY_OCARINA_AND_LAUGH,
    /* 16 */ SK_ANIM_PICK_UP_OCARINA, // Not used in the final game
    /* 17 */ SK_ANIM_LOOK_AT_OCARINA, // Not used in the final game
    /* 18 */ SK_ANIM_KICK_OVER_LINK,
    /* 19 */ SK_ANIM_SEARCH_LINK,
    /* 20 */ SK_ANIM_UNUSED_KICK_OVER_LINK, // Unused duplicate of SK_ANIM_KICK_OVER_LINK
    /* 21 */ SK_ANIM_UNUSED_SEARCH_LINK,    // Unused duplicate of SK_ANIM_SEARCH_LINK
    /* 22 */ SK_ANIM_HORSE_RIDE_START,
    /* 23 */ SK_ANIM_HORSE_RIDE_LOOP,
    /* 24 */ SK_ANIM_RAISE_MASK_START,
    /* 25 */ SK_ANIM_RAISE_MASK_LOOP,
    /* 26 */ SK_ANIM_LOWER_MASK,
    /* 27 */ SK_ANIM_JUMP_WHILE_HIDING_OCARINA,
    /* 28 */ SK_ANIM_HORSE_RIDE_AND_ROTATE, // Used when Link gets thrown off his horse
    /* 29 */ SK_ANIM_PLAY_OCARINA_WHILE_FLOATING,
    /* 30 */ SK_ANIM_FLOATING_TURN_AROUND,
    /* 31 */ SK_ANIM_OCARINA_JUGGLE,
    /* 32 */ SK_ANIM_CALL_DOWN_MOON_START,
    /* 33 */ SK_ANIM_CALL_DOWN_MOON_LOOP,
    /* 34 */ SK_ANIM_SMACK_FAIRY_START,
    /* 35 */ SK_ANIM_SMACK_FAIRY_LOOP,
    /* 36 */ SK_ANIM_HIT_BY_BUBBLE,
    /* 37 */ SK_ANIM_DROP_OCARINA,
    /* 38 */ SK_ANIM_FLOATING_ARMS_CROSSED,
    /* 39 */ SK_ANIM_DEFLECT_ATTACK,
    /* 40 */ SK_ANIM_TELESCOPE_LOOK_UP_START,
    /* 41 */ SK_ANIM_TELESCOPE_LOOK_UP_LOOP,
    /* 42 */ SK_ANIM_SURPRISE_START,
    /* 43 */ SK_ANIM_SURPRISE_LOOP,
    /* 44 */ SK_ANIM_LOOK_AROUND_FOR_GIANTS_START,
    /* 45 */ SK_ANIM_LOOK_AROUND_FOR_GIANTS_LOOP,
    /* 46 */ SK_ANIM_HOLD_HEAD_AND_SHAKE_START,
    /* 47 */ SK_ANIM_HOLD_HEAD_AND_SHAKE_LOOP,
    /* 48 */ SK_ANIM_HOLD_HEAD_AND_SCREAM_START,
    /* 49 */ SK_ANIM_HOLD_HEAD_AND_SCREAM_LOOP,
    /* 50 */ SK_ANIM_HUDDLE_WITH_FAIRIES,
    /* 51 */ SK_ANIM_SEARCH_MASK_SALESMAN,
    /* 52 */ SK_ANIM_HOLD_UP_MASK_START,
    /* 53 */ SK_ANIM_HOLD_UP_MASK_LOOP,
    /* 54 */ SK_ANIM_SHIVER,
    /* 55 */ SK_ANIM_DRAW,
    /* 56 */ SK_ANIM_BENT_OVER_LOOK_UP,
    /* 57 */ SK_ANIM_SPANK,
    /* 58 */ SK_ANIM_HIP_SHAKE_AND_JUMP,
    /* 59 */ SK_ANIM_PLAY_FLUTE,
    /* 60 */ SK_ANIM_CARTWHEEL,
    /* 61 */ SK_ANIM_LIE_FLAT,
    /* 62 */ SK_ANIM_DANGLE_FROM_MASK_START,
    /* 63 */ SK_ANIM_DANGLE_FROM_MASK_LOOP,
    /* 64 */ SK_ANIM_DROPPED_FROM_MASK,
    /* 65 */ SK_ANIM_LOOK_UP_AT_GIANTS,
    /* 66 */ SK_ANIM_ASHAMED_START,
    /* 67 */ SK_ANIM_ASHAMED_LOOP,
    /* 68 */ SK_ANIM_LOOK_LEFT_START,
    /* 69 */ SK_ANIM_LOOK_LEFT_LOOP,
    /* 70 */ SK_ANIM_SNIFF,
    /* 71 */ SK_ANIM_LAUGH_AFTER_SNIFF,
    /* 72 */ SK_ANIM_LAUGH_AFTER_MAX
} SkullKidAnimation;

typedef enum {
    /* 0 */ DM_STK_TYPE_SKULL_KID,
    /* 1 */ DM_STK_TYPE_MAJORAS_MASK
} DmStkType;

typedef struct DmStk {
    /* 0x000 */ Actor actor;
    /* 0x144 */ SkelAnime skelAnime;
    /* 0x188 */ Vec3s jointTable[SKULL_KID_LIMB_MAX]; // Not used, since it's allocated dynamically instead.
    /* 0x20C */ Vec3s morphTable[SKULL_KID_LIMB_MAX]; // Not used, since it's allocated dynamically instead.
    /* 0x290 */ DmStkActionFunc actionFunc;
    /* 0x294 */ ColliderCylinder collider;
    /* 0x2E0 */ s16 animIndex;
    /* 0x2E4 */ s32 alpha;
    /* 0x2E8 */ u32 fogR;
    /* 0x2EC */ u32 fogG;
    /* 0x2F0 */ u32 fogB;
    /* 0x2F4 */ s32 fogA;
    /* 0x2F8 */ s32 fogN;
    /* 0x2FC */ s32 fogF;
    /* 0x300 */ f32 fogScale;
    /* 0x304 */ Vec3f headPos; // set but never used
    /* 0x310 */ Vec3f oathToOrderCutsceneVoicePos;
    /* 0x31C */ UNK_TYPE1 unk31C[0xC];
    /* 0x328 */ u16 tatlMessageTimer;
    /* 0x32A */ u16 bobPhase;
    /* 0x32C */ u8 maskType;
    /* 0x32D */ u8 handType;
    /* 0x32E */ u8 fadeInState;
    /* 0x32F */ u8 fadeOutState;
    /* 0x330 */ s32 fadeOutTimer;
    /* 0x334 */ u8 cueId;
    /* 0x335 */ u8 hasBeenHit; // set but never used
    /* 0x336 */ s8 objectStkObjectSlot;
    /* 0x337 */ s8 objectStk2ObjectSlot;
    /* 0x338 */ s8 objectStk3ObjectSlot;
    /* 0x339 */ u8 deflectCount;
    /* 0x33A */ u8 dekuPipesCutsceneState;
    /* 0x33B */ u8 shouldDraw;
} DmStk; // size = 0x33C

typedef enum {
    /* 0 */ SK_MASK_TYPE_NO_MASK,
    /* 1 */ SK_MASK_TYPE_NORMAL,
    /* 2 */ SK_MASK_TYPE_GLOWING_EYES,
    /* 3 */ SK_MASK_TYPE_FADING_OUT,
    /* 9 */ SK_MASK_TYPE_RAISED = 9
} SkullKidMaskType;

typedef enum {
    /* 0 */ SK_HAND_TYPE_HOLDING_LINK_MASK_AND_FLUTE,
    /* 1 */ SK_HAND_TYPE_HOLDING_LINK_MASK,
    /* 2 */ SK_HAND_TYPE_HOLDING_MAJORAS_MASK,
    /* 3 */ SK_HAND_TYPE_HOLDING_OCARINA,
    /* 4 */ SK_HAND_TYPE_JUGGLING_OR_DROPPING_OCARINA,
    /* 5 */ SK_HAND_TYPE_HOLDING_MAJORAS_MASK_AND_FLUTE,
    /* 6 */ SK_HAND_TYPE_HOLDING_FLUTE,
    /* 9 */ SK_HAND_TYPE_DEFAULT = 9
} SkullKidHandType;

typedef enum {
    /* 0 */ SK_FADE_IN_STATE_NONE,
    /* 1 */ SK_FADE_IN_STATE_START,
    /* 2 */ SK_FADE_IN_STATE_INCREASE_FOG,
    /* 3 */ SK_FADE_IN_STATE_INCREASE_ALPHA
} SkullKidFadeInState;

typedef enum {
    /* 0 */ SK_FADE_OUT_STATE_NONE,
    /* 1 */ SK_FADE_OUT_STATE_FADING_OUT
} SkullKidFadeOutState;

typedef enum {
    /* 0 */ SK_DEKU_PIPES_CS_STATE_NOT_READY,
    /* 1 */ SK_DEKU_PIPES_CS_STATE_READY,
    /* 2 */ SK_DEKU_PIPES_CS_STATE_PLAYER_USED_OCARINA,
    /* 3 */ SK_DEKU_PIPES_CS_STATE_START,
    /* 4 */ SK_DEKU_PIPES_CS_STATE_END
} SkullKidDekuPipesCutsceneState;

bool skull_kid_has_ocarina;

void DmStk_ClockTower_WaitForIntroCutsceneVersion1ToEnd(DmStk* this, PlayState* play);
void DmStk_ClockTower_StartIntroCutsceneVersion2(DmStk* this, PlayState* play);
void DmStk_ClockTower_WaitForIntroCutsceneVersion2ToEnd(DmStk* this, PlayState* play);
void DmStk_ClockTower_WaitForDropOcarinaCutsceneToEnd(DmStk* this, PlayState* play);
void DmStk_ClockTower_Idle(DmStk* this, PlayState* play);

void DmStk_UpdateCollision(DmStk* this, PlayState* play) {
    s32 pad;

    Collider_UpdateCylinder(&this->actor, &this->collider);
    CollisionCheck_SetAC(play, &play->colChkCtx, &this->collider.base);
    CollisionCheck_SetOC(play, &play->colChkCtx, &this->collider.base);

    if (play->sceneId == SCENE_OKUJOU && !recomp_location_is_checked(GI_OCARINA_OF_TIME)) {
        this->handType = SK_HAND_TYPE_JUGGLING_OR_DROPPING_OCARINA;
        INV_CONTENT(ITEM_OCARINA_OF_TIME) = ITEM_NONE;
    }
}

/*void DmStk_Update(Actor* thisx, PlayState* play) {
    DmStk* this = THIS;

    if (DM_STK_GET_TYPE(&this->actor) != DM_STK_TYPE_MAJORAS_MASK) {
        if (this->animIndex == SK_ANIM_CALL_DOWN_MOON_LOOP) {
            Actor_SetFocus(&this->actor, 40.0f);
        } else {
            Actor_SetFocus(&this->actor, 6.0f);
        }

        DmStk_LoadObjectForAnimation(this, play);

        if (this->animIndex != SK_ANIM_LIE_FLAT) {
            SkelAnime_Update(&this->skelAnime);
        }

        this->alpha = this->alpha;

        this->actionFunc(this, play);

        if (play->sceneId == SCENE_OKUJOU) {
            DmStk_UpdateCollision(this, play);
        }

        DmStk_HandleCutscene(this, play);
        DmStk_PlaySfxForCutscenes(this, play);

        // This handles the cutscene where the player takes out the Deku Pipes for the first time.
        switch (this->dekuPipesCutsceneState) {
            case SK_DEKU_PIPES_CS_STATE_READY:
                if (func_800B8718(&this->actor, &play->state)) {
                    this->dekuPipesCutsceneState = SK_DEKU_PIPES_CS_STATE_PLAYER_USED_OCARINA;
                } else {
                    func_800B874C(&this->actor, play, this->actor.xzDistToPlayer, fabsf(this->actor.playerHeightRel));
                }
                break;

            case SK_DEKU_PIPES_CS_STATE_PLAYER_USED_OCARINA:
                if (CutsceneManager_IsNext(16)) {
                    this->dekuPipesCutsceneState = SK_DEKU_PIPES_CS_STATE_START;
                    CutsceneManager_Start(16, &this->actor);
                    this->actionFunc = DmStk_ClockTower_Idle;
                } else {
                    CutsceneManager_Queue(16);
                }
                break;

            case SK_DEKU_PIPES_CS_STATE_START:
                if (play->csCtx.state == CS_STATE_IDLE) {
                    this->dekuPipesCutsceneState = SK_DEKU_PIPES_CS_STATE_END;
                }
                break;

            default:
                break;
        }

        // This code is responsible for making in-game time pass while using the telescope in the Astral Observatory.
        // Skull Kid is always loaded in the scene, even if he isn't visible, hence why time always passes.
        if ((play->actorCtx.flags & ACTORCTX_FLAG_1) && (play->msgCtx.msgMode != MSGMODE_NONE) &&
            (play->msgCtx.currentTextId == 0x5E6) && !FrameAdvance_IsEnabled(&play->state) &&
            (play->transitionTrigger == TRANS_TRIGGER_OFF) && (CutsceneManager_GetCurrentCsId() == CS_ID_NONE) &&
            (play->csCtx.state == CS_STATE_IDLE)) {
            gSaveContext.save.time = ((void)0, gSaveContext.save.time) + (u16)R_TIME_SPEED;
            if (R_TIME_SPEED != 0) {
                gSaveContext.save.time =
                    ((void)0, gSaveContext.save.time) + (u16)((void)0, gSaveContext.save.timeSpeedOffset);
            }
        }
    }

    if ((play->sceneId == SCENE_00KEIKOKU) && (gSaveContext.sceneLayer == 3) && (play->csCtx.scriptIndex > 0)) {
        play->envCtx.skyboxConfig = 15;
        play->envCtx.changeSkyboxNextConfig = 15;
    }
}*/

void DmStk_ClockTower_StartIntroCutsceneVersion1(DmStk* this, PlayState* play) {
    if (recomp_location_is_checked(0x00004C)) {
        skull_kid_has_ocarina = INV_CONTENT(ITEM_OCARINA_OF_TIME) == ITEM_NONE;
        DmStk_ClockTower_StartIntroCutsceneVersion2(this, play);
        return;
    }
    if (CutsceneManager_IsNext(9)) {
        CutsceneManager_Start(9, &this->actor);
        if (play->csCtx.state == CS_STATE_IDLE) {
            if (!skull_kid_has_ocarina) {
                INV_CONTENT(ITEM_OCARINA_OF_TIME) = ITEM_OCARINA_OF_TIME;
            }
            this->actionFunc = (DmStkActionFunc) actor_relocate(&this->actor, DmStk_ClockTower_WaitForIntroCutsceneVersion1ToEnd);
        }
    } else {
        CutsceneManager_Queue(9);
    }
}

void DmStk_ClockTower_StartIntroCutsceneVersion2(DmStk* this, PlayState* play) {
    if (!recomp_location_is_checked(0x00004C)) {
        skull_kid_has_ocarina = INV_CONTENT(ITEM_OCARINA_OF_TIME) == ITEM_NONE;
        DmStk_ClockTower_StartIntroCutsceneVersion1(this, play);
        return;
    }
    if (CutsceneManager_IsNext(11)) {
        CutsceneManager_Start(11, &this->actor);
        this->actionFunc = (DmStkActionFunc) actor_relocate(&this->actor, DmStk_ClockTower_WaitForIntroCutsceneVersion2ToEnd);
    } else {
        CutsceneManager_Queue(11);
    }
}

void DmStk_ClockTower_StartDropOcarinaCutscene(DmStk* this, PlayState* play) {
    if (CutsceneManager_IsNext(10)) {
        Actor_PlaySfx(&this->actor, NA_SE_EN_STALKIDS_DAMAGE);
        CutsceneManager_Start(10, &this->actor);
        this->actor.shape.rot.x = 0;
        this->actor.world.rot.x = this->actor.shape.rot.x;
        this->actor.shape.rot.y = this->actor.shape.rot.x;
        this->actor.world.rot.y = this->actor.shape.rot.x;
        this->actionFunc = DmStk_ClockTower_WaitForDropOcarinaCutsceneToEnd;
    } else {
        if (!skull_kid_has_ocarina) {
            INV_CONTENT(ITEM_OCARINA_OF_TIME) = ITEM_NONE;
        }
        CutsceneManager_Queue(10);
    }
}

void DmStk_ClockTower_WaitForDropOcarinaCutsceneToEnd(DmStk* this, PlayState* play) {
    if ((play->csCtx.state != CS_STATE_IDLE) && (play->csCtx.curFrame > 20)) {
        if (!skull_kid_has_ocarina) {
            INV_CONTENT(ITEM_OCARINA_OF_TIME) = ITEM_OCARINA_OF_TIME;
        }
        this->actionFunc = (DmStkActionFunc) actor_relocate(&this->actor, DmStk_ClockTower_Idle);
    }
}