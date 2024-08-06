#include "patches.h"
#include "ultra64.h"
#include "z64snap.h"
#include "overlays/effects/ovl_Effect_Ss_Extra/z_eff_ss_extra.h"

#include "apcommon.h"
#include "misc_funcs.h"

struct EnSyatekiMan;

typedef void (*EnSyatekiManActionFunc)(struct EnSyatekiMan*, PlayState*);

#define LOCATION_TOWN_GALLERY_PERFECT 0x07011D

#define BURLY_GUY_LIMB_MAX 0x10

#define SG_MAN_GET_PATH_INDEX(thisx) (((thisx)->params & 0xFF00) >> 8)

// These defines assume that sNormalSwampTargetActorList is used to spawn actors and that the logic of
// EnSyatekiMan_Swamp_RunGame is not modified; in other words, it assumes that each wave consists of five
// Deku Scrubs that the player must shoot and three Guays that the player can either shoot or let escape.
// Once all Deku Scrubs and Guays have been shot or escape, the next wave starts.
#define SG_SWAMP_WAVE_COUNT 4
#define SG_SWAMP_DEKUS_PER_WAVE 5
#define SG_SWAMP_GUAYS_PER_WAVE 3
#define SG_SWAMP_BONUS_DEKU_COUNT 2

// These values are used to modify the shooting gallery man's internal score. They are
// defined in terms of the scores used by EffectSsExtra because EffectSsExtra_Update
// updates the score displayed to the player, and defining them like this will prevent
// these two different scores from becoming desynchronized. If you intend to modify these
// values, then you'll also need to make sure that EffectSsExtra_Spawn in the appropriate
// actor is called with the correct parameter for the index; failing to do this will
// cause the two different score variables to fall out of sync.
#define SG_POINTS_DEKU_NORMAL EXTRA_SCORE_30
#define SG_POINTS_DEKU_BONUS EXTRA_SCORE_100
#define SG_POINTS_GUAY EXTRA_SCORE_60
#define SG_POINTS_WOLFOS EXTRA_SCORE_100

// After getting a perfect score in the Swamp Shooting Gallery, the player is rewarded
// additional points for every second remaining on the minigame timer.
#define SG_BONUS_POINTS_PER_SECOND 10

typedef enum {
    /* 0 */ SG_GAME_STATE_NONE,             // None of the states below apply.
    /* 1 */ SG_GAME_STATE_RUNNING,          // The shooting game is in-progress.
    /* 2 */ SG_GAME_STATE_EXPLAINING_RULES, // For the Town Shooting Gallery, this state is also used for explaining the current high score.
    /* 3 */ SG_GAME_STATE_NOT_PLAYING,      // Either the player said "No" to playing, or they said "Yes" but don't have enough rupees.
    /* 4 */ SG_GAME_STATE_ONE_MORE_GAME,    // The player failed to get a new high score (Town) and/or perfect score (Swamp and Town).
    /* 5 */ SG_GAME_STATE_GIVING_BONUS,     // The player gets bonus points at the end of the Swamp game if they get a perfect score.
    /* 6 */ SG_GAME_STATE_ENDED,            // The player got a new high score and/or perfect score (Town), or the game is over (Swamp).
    /* 7 */ SG_GAME_STATE_MOVING_PLAYER     // The player is automatically moving towards the spot to play the game.
} ShootingGalleryGameState;

typedef enum {
    /*  0 */ SG_OCTO_STATE_APPEARING,    // The Octoroks will begin to appear on the next frame.
    /*  1 */ SG_OCTO_STATE_APPEARED,     // The Octoroks are appearing from underwater or have already appeared and are floating.
    /* 70 */ SG_OCTO_STATE_INITIAL = 70, // The initial state of the game. The Octoroks have not appeared yet.
    /* 80 */ SG_OCTO_STATE_HIDING = 80   // The Octoroks are in the process of hiding underwater.
} ShootingGalleryOctorokState;

typedef enum {
    /* 0 */ SG_PATH_TYPE_GUAY,
    /* 1 */ SG_PATH_TYPE_DEKU_NORMAL,
    /* 2 */ SG_PATH_TYPE_WOLFOS,
    /* 3 */ SG_PATH_TYPE_DEKU_BONUS
} ShootingGalleryPathType;

typedef struct EnSyatekiMan {
    /* 0x000 */ Actor actor;
    /* 0x144 */ SkelAnime skelAnime;
    /* 0x188 */ EnSyatekiManActionFunc actionFunc;
    /* 0x18C */ Path* path;
    /* 0x190 */ s32 octorokFlags;
    /* 0x194 */ s32 swampTargetActorListIndex;
    /* 0x198 */ Vec3s jointTable[BURLY_GUY_LIMB_MAX];
    /* 0x1F8 */ Vec3s morphTable[BURLY_GUY_LIMB_MAX];
    /* 0x258 */ Vec3s headRot;
    /* 0x25E */ Vec3s torsoRot;
    /* 0x264 */ s16 eyeIndex;
    /* 0x266 */ s16 blinkTimer;
    /* 0x268 */ UNK_TYPE1 unk268[0x2];
    /* 0x26A */ s16 shootingGameState;
    /* 0x26C */ union {
                   s16 guayAppearTimer;
                   s16 octorokState;
               };
    /* 0x26E */ union {
                   s16 bonusDekuScrubHitCounter;
                   s16 lastHitOctorokType;
               };
    /* 0x270 */ s16 talkWaitTimer;
    /* 0x272 */ s16 dekuScrubFlags;
    /* 0x274 */ s16 guayFlags;
    /* 0x276 */ s16 wolfosFlags;
    /* 0x278 */ s16 dekuScrubHitCounter;
    /* 0x27A */ s16 guayHitCounter;
    /* 0x27C */ s16 currentWave;
    /* 0x27E */ s16 flagsIndex; // Used for Octoroks in Town and Guays in Swamp
    /* 0x280 */ s16 score;
    /* 0x282 */ s16 talkFlags;
    /* 0x284 */ s16 prevTextId;
} EnSyatekiMan; // size = 0x288

void EnSyatekiMan_Town_GiveReward(EnSyatekiMan* this, PlayState* play);

void EnSyatekiMan_Town_SetupGiveReward(EnSyatekiMan* this, PlayState* play) {
    Player* player = GET_PLAYER(play);

    if (Actor_HasParent(&this->actor, play)) {
        if (this->prevTextId == 0x407) {
            if (!CHECK_WEEKEVENTREG(WEEKEVENTREG_RECEIVED_TOWN_SHOOTING_GALLERY_QUIVER_UPGRADE)) {
                SET_WEEKEVENTREG(WEEKEVENTREG_RECEIVED_TOWN_SHOOTING_GALLERY_QUIVER_UPGRADE);
            }
        }

        if ((this->prevTextId == 0x405) || (this->prevTextId == 0x406)) {
            if (!CHECK_WEEKEVENTREG(WEEKEVENTREG_RECEIVED_TOWN_SHOOTING_GALLERY_HEART_PIECE)) {
                SET_WEEKEVENTREG(WEEKEVENTREG_RECEIVED_TOWN_SHOOTING_GALLERY_HEART_PIECE);
            }
        }

        this->actor.parent = NULL;
        this->actionFunc = EnSyatekiMan_Town_GiveReward;
    } else {
        if (this->score == 50 && !(!recomp_location_is_checked(GI_QUIVER_40) && recomp_location_is_checked(LOCATION_TOWN_GALLERY_PERFECT))) {
            if (recomp_location_is_checked(LOCATION_TOWN_GALLERY_PERFECT)) {
                Actor_OfferGetItem(&this->actor, play, GI_RUPEE_HUGE, 500.0f, 100.0f);
            } else {
                Actor_OfferGetItem(&this->actor, play, GI_HEART_PIECE, 500.0f, 100.0f);
            }
        } else {
            if (recomp_location_is_checked(GI_QUIVER_40)) {
                Actor_OfferGetItem(&this->actor, play, GI_RUPEE_PURPLE, 500.0f, 100.0f);
            } else {
                Actor_OfferGetItem(&this->actor, play, GI_QUIVER_40, 500.0f, 100.0f);
            }
        }

        player->actor.shape.rot.y = -0x8000;
        player->actor.velocity.z = 0.0f;
        player->actor.velocity.x = 0.0f;
        player->actor.world.rot.y = player->actor.shape.rot.y;
    }
}

void EnSyatekiMan_Town_Talk(EnSyatekiMan* this, PlayState* play);

void EnSyatekiMan_Town_EndGame(EnSyatekiMan* this, PlayState* play) {
    if (this->shootingGameState == SG_GAME_STATE_RUNNING) {
        this->octorokFlags = 0;
        if ((this->talkWaitTimer <= 0) && !play->interfaceCtx.perfectLettersOn) {
            Flags_SetAllTreasure(play, this->score);
            this->talkWaitTimer = 15;
            //if ((HS_GET_TOWN_SHOOTING_GALLERY_HIGH_SCORE() < this->score) || (this->score == 50)) {
            if (this->score >= 40) {
                if (this->score == 50) {
                    if (!recomp_location_is_checked(LOCATION_TOWN_GALLERY_PERFECT)) {
                        // No way! That was perfect!
                        Message_StartTextbox(play, 0x405, &this->actor);
                        this->prevTextId = 0x405;
                    } else {
                        // That was perfect!
                        Message_StartTextbox(play, 0x406, &this->actor);
                        this->prevTextId = 0x406;
                    }
                } else {
                    // You got a new record!
                    Message_StartTextbox(play, 0x407, &this->actor);
                    this->prevTextId = 0x407;
                }

                if (this->score > HS_GET_TOWN_SHOOTING_GALLERY_HIGH_SCORE()) {
                    HS_SET_TOWN_SHOOTING_GALLERY_HIGH_SCORE(this->score);
                }
                this->shootingGameState = SG_GAME_STATE_ENDED;
            } else {
                if (CURRENT_DAY != 3) {
                    // You got [score]? Oh, that's too bad...
                    Message_StartTextbox(play, 0x401, &this->actor);
                    this->prevTextId = 0x401;
                } else {
                    // You got [score]? Too bad...
                    Message_StartTextbox(play, 0x403, &this->actor);
                    this->prevTextId = 0x403;
                }

                this->shootingGameState = SG_GAME_STATE_ONE_MORE_GAME;
            }

            this->actionFunc = EnSyatekiMan_Town_Talk;
        } else {
            this->talkWaitTimer--;
        }
    }

    if (this->talkWaitTimer < 5) {
        play->unk_1887C = -10;
    }
}

#define SG_SWAMP_PERFECT_SCORE_WITHOUT_BONUS                                                     \
    (SG_POINTS_DEKU_NORMAL * (SG_SWAMP_DEKUS_PER_WAVE * SG_SWAMP_WAVE_COUNT) +                   \
     SG_POINTS_GUAY * (SG_SWAMP_GUAYS_PER_WAVE * SG_SWAMP_WAVE_COUNT) +                          \
     SG_POINTS_DEKU_BONUS * SG_SWAMP_BONUS_DEKU_COUNT + SG_POINTS_WOLFOS * SG_SWAMP_WAVE_COUNT + \
     SG_POINTS_WOLFOS * (SG_SWAMP_WAVE_COUNT / 2))

#define SG_SWAMP_HEART_PIECE_SCORE (SG_SWAMP_PERFECT_SCORE_WITHOUT_BONUS + (6 * SG_BONUS_POINTS_PER_SECOND))

void EnSyatekiMan_Swamp_GiveReward(EnSyatekiMan* this, PlayState* play);

void EnSyatekiMan_Swamp_SetupGiveReward(EnSyatekiMan* this, PlayState* play) {
    Player* player = GET_PLAYER(play);

    if (Actor_HasParent(&this->actor, play)) {
        if (!CHECK_WEEKEVENTREG(WEEKEVENTREG_RECEIVED_SWAMP_SHOOTING_GALLERY_QUIVER_UPGRADE)) {
            SET_WEEKEVENTREG(WEEKEVENTREG_RECEIVED_SWAMP_SHOOTING_GALLERY_QUIVER_UPGRADE);
        } else if (!CHECK_WEEKEVENTREG(WEEKEVENTREG_RECEIVED_SWAMP_SHOOTING_GALLERY_HEART_PIECE) &&
                   (this->score >= SG_SWAMP_HEART_PIECE_SCORE)) {
            SET_WEEKEVENTREG(WEEKEVENTREG_RECEIVED_SWAMP_SHOOTING_GALLERY_HEART_PIECE);
        }

        this->actor.parent = NULL;
        this->actionFunc = EnSyatekiMan_Swamp_GiveReward;
    } else {
        if ((CUR_UPG_VALUE(UPG_QUIVER) < 3) &&
            !CHECK_WEEKEVENTREG(WEEKEVENTREG_RECEIVED_SWAMP_SHOOTING_GALLERY_QUIVER_UPGRADE)) {
            Actor_OfferGetItem(&this->actor, play, GI_QUIVER_50, 500.0f, 100.0f);
        } else if (this->score < SG_SWAMP_HEART_PIECE_SCORE) {
            Actor_OfferGetItem(&this->actor, play, GI_RUPEE_RED, 500.0f, 100.0f);
        } else if (!CHECK_WEEKEVENTREG(WEEKEVENTREG_RECEIVED_SWAMP_SHOOTING_GALLERY_HEART_PIECE)) {
            Actor_OfferGetItem(&this->actor, play, GI_HEART_PIECE, 500.0f, 100.0f);
        } else {
            Actor_OfferGetItem(&this->actor, play, GI_RUPEE_PURPLE, 500.0f, 100.0f);
        }

        player->actor.shape.rot.y = -0x8000;
        player->actor.velocity.z = 0.0f;
        player->actor.velocity.x = 0.0f;
        player->actor.world.rot.y = player->actor.shape.rot.y;
    }
}