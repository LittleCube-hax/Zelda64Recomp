#include "patches.h"
#include "global.h"
#include "overlays/actors/ovl_En_Bom/z_en_bom.h"

#include "apcommon.h"
#include "misc_funcs.h"

#define SPEED_MODE_CURVED 0.018f

#define LINK_GORON_SHIELDING_LIMB_MAX 0x05

extern LinkAnimationHeader gPlayerAnim_clink_normal_okarina_walk;
extern LinkAnimationHeader gPlayerAnim_clink_normal_okarina_walkB;

extern AnimationHeader gLinkGoronShieldingAnim;

extern FlexSkeletonHeader gLinkGoronShieldingSkel;

extern Input* sPlayerControlInput;

extern s8 sPlayerActionChangeList7[];

extern PlayerAnimationHeader* D_8085BE84[PLAYER_ANIMGROUP_MAX][PLAYER_ANIMTYPE_MAX];

s32 Player_GrabPlayer(PlayState* play, Player* this);
s32 Player_TryCsAction(PlayState* play, Player* this, PlayerCsAction csAction);
void func_8085B384(Player* this, PlayState* play);
s32 Player_InflictDamage(PlayState* play, s32 damage);
void Player_TalkWithPlayer(PlayState* play, Actor* actor);
void func_8085B74C(PlayState* play);
void func_8085B820(PlayState* play, s16 arg1);
PlayerItemAction func_8085B854(PlayState* play, Player* this, ItemId itemId);
s32 func_8085B930(PlayState* play, PlayerAnimationHeader* talkAnim, AnimationMode animMode);

void Player_UpdateCommon(Player* this, PlayState* play, Input* input);
s32 Player_StartFishing(PlayState* play);
void func_8085B170(PlayState* play, Player* this);
s32 func_8083A658(PlayState* play, Player* this);
void Player_InitItemAction(PlayState* play, Player* this, PlayerItemAction itemAction);

void Player_UseItem(PlayState* play, Player* this, ItemId item);

void func_80836988(Player* this, PlayState* play);

void func_808484F0(Player* this);

void func_80838A20(PlayState* play, Player* this);
void func_80839978(PlayState* play, Player* this);
void func_80839A10(PlayState* play, Player* this);

s32 Player_SetAction(PlayState* play, Player* this, PlayerActionFunc actionFunc, s32 arg3);
void Player_Action_80(Player* this, PlayState* play);
void Player_Action_81(Player* this, PlayState* play);
void Player_UseItem(PlayState* play, Player* this, ItemId item);
void Player_StopHorizontalMovement(Player* this);
void Player_AnimationPlayOnce(PlayState* play, Player* this, PlayerAnimationHeader* anim);
void func_8082DE50(PlayState* play, Player* this);
PlayerAnimationHeader* func_8082ED20(Player* this);
void func_80836D8C(Player* this);

s32 Player_TryActionChangeList(PlayState* play, Player* this, s8* actionChangeList, s32 updateUpperBody);
s32 Player_GetMovementSpeedAndYaw(Player* this, f32* outSpeedTarget, s16* outYawTarget, f32 speedMode,
                                  PlayState* play);
void func_80839E74(Player* this, PlayState* play);
s32 func_8083A4A4(Player* this, f32* arg1, s16* arg2, f32 arg3);
void func_8083C8E8(Player* this, PlayState* play);
void func_8083CB04(Player* this, f32 arg1, s16 arg2, f32 arg3, f32 arg4, s16 arg5);

void func_808309CC(PlayState* play, Player* this);

static InitChainEntry sInitChain[] = {
    ICHAIN_F32(targetArrowOffset, 500, ICHAIN_STOP),
};

Vec3s sPlayerSkeletonBaseTransl = { -57, 3377, 0 };

ColliderCylinderInit D_8085C2EC = {
    {
        COLTYPE_HIT5,
        AT_NONE,
        AC_ON | AC_TYPE_ENEMY,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_PLAYER,
        COLSHAPE_CYLINDER,
    },
    {
        ELEMTYPE_UNK1,
        { 0x00000000, 0x00, 0x00 },
        { 0xF7CFFFFF, 0x00, 0x00 },
        TOUCH_NONE | TOUCH_SFX_NORMAL,
        BUMP_ON,
        OCELEM_ON,
    },
    { 12, 60, 0, { 0, 0, 0 } },
};

ColliderCylinderInit D_8085C318 = {
    {
        COLTYPE_METAL,
        AT_ON | AT_TYPE_PLAYER,
        AC_ON | AC_HARD | AC_TYPE_ENEMY,
        OC1_NONE,
        OC2_TYPE_PLAYER,
        COLSHAPE_CYLINDER,
    },
    {
        ELEMTYPE_UNK2,
        { 0x00100000, 0x00, 0x02 },
        { 0xD7CFFFFF, 0x00, 0x00 },
        TOUCH_NONE | TOUCH_SFX_NORMAL,
        BUMP_ON,
        OCELEM_ON,
    },
    { 25, 60, 0, { 0, 0, 0 } },
};

ColliderQuadInit D_8085C344 = {
    {
        COLTYPE_NONE,
        AT_ON | AT_TYPE_PLAYER,
        AC_NONE,
        OC1_NONE,
        OC2_TYPE_PLAYER,
        COLSHAPE_QUAD,
    },
    {
        ELEMTYPE_UNK2,
        { 0x00000000, 0x00, 0x01 },
        { 0xF7CFFFFF, 0x00, 0x00 },
        TOUCH_ON | TOUCH_SFX_NORMAL,
        BUMP_NONE,
        OCELEM_NONE,
    },
    { { { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f } } },
};

ColliderQuadInit D_8085C394 = {
    {
        COLTYPE_METAL,
        AT_ON | AT_TYPE_PLAYER,
        AC_ON | AC_HARD | AC_TYPE_ENEMY,
        OC1_NONE,
        OC2_TYPE_PLAYER,
        COLSHAPE_QUAD,
    },
    {
        ELEMTYPE_UNK2,
        { 0x00100000, 0x00, 0x00 },
        { 0xD7CFFFFF, 0x00, 0x00 },
        TOUCH_ON | TOUCH_SFX_NORMAL,
        BUMP_ON,
        OCELEM_NONE,
    },
    { { { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f } } },
};

void Player_InitCommon(Player* this, PlayState* play, FlexSkeletonHeader* skelHeader) {
    Actor_ProcessInitChain(&this->actor, sInitChain);
    this->currentYaw = this->actor.world.rot.y;

    if ((PLAYER_GET_INITMODE(&this->actor) != PLAYER_INITMODE_TELESCOPE) &&
        ((gSaveContext.respawnFlag != 2) ||
         (gSaveContext.respawn[RESPAWN_MODE_RETURN].playerParams != PLAYER_PARAMS(0xFF, PLAYER_INITMODE_TELESCOPE)))) {
        func_808309CC(play, this);
        SkelAnime_InitPlayer(play, &this->skelAnime, skelHeader, D_8085BE84[PLAYER_ANIMGROUP_wait][this->modelAnimType],
                             1 | 8, this->jointTableBuffer, this->morphTableBuffer, PLAYER_LIMB_MAX);
        this->skelAnime.baseTransl = sPlayerSkeletonBaseTransl;

        SkelAnime_InitPlayer(play, &this->skelAnimeUpper, skelHeader, func_8082ED20(this), 1 | 8,
                             this->jointTableUpperBuffer, this->morphTableUpperBuffer, PLAYER_LIMB_MAX);
        this->skelAnimeUpper.baseTransl = sPlayerSkeletonBaseTransl;

        if (this->transformation == PLAYER_FORM_GORON) {
            SkelAnime_InitFlex(play, &this->unk_2C8, &gLinkGoronShieldingSkel, &gLinkGoronShieldingAnim,
                               this->jointTable, this->morphTable, LINK_GORON_SHIELDING_LIMB_MAX);
        }

        ActorShape_Init(&this->actor.shape, 0.0f, ActorShadow_DrawFeet, this->ageProperties->shadowScale);
    }

    this->subCamId = CAM_ID_NONE;
    Collider_InitAndSetCylinder(play, &this->cylinder, &this->actor, &D_8085C2EC);
    Collider_InitAndSetCylinder(play, &this->shieldCylinder, &this->actor, &D_8085C318);
    Collider_InitAndSetQuad(play, &this->meleeWeaponQuads[0], &this->actor, &D_8085C344);
    Collider_InitAndSetQuad(play, &this->meleeWeaponQuads[1], &this->actor, &D_8085C344);
    Collider_InitAndSetQuad(play, &this->shieldQuad, &this->actor, &D_8085C394);

    if (saveOpened) {
        playing = true;
    }
}

extern u16 sPlayerItemButtons[4];
extern s32 sPlayerHeldItemButtonIsHeldDown;

EquipSlot func_8082FD0C(Player* this, PlayerItemAction itemAction);
PlayerItemAction Player_ItemToItemAction(Player* this, ItemId item);
bool Player_ItemIsInUse(Player* this, ItemId item);
EquipSlot func_8082FDC4(void);

/**
 * Handles the high level item usage and changing process based on the B and C buttons.
 */
void Player_ProcessItemButtons(Player* this, PlayState* play) {
    if (this->stateFlags1 & (PLAYER_STATE1_800 | PLAYER_STATE1_20000000)) {
        return;
    }
    if (this->stateFlags2 & PLAYER_STATE2_2000000) {
        return;
    }
    if (this->stateFlags3 & PLAYER_STATE3_20000000) {
        return;
    }
    if (func_801240DC(this)) {
        return;
    }

    if (this->transformation == PLAYER_FORM_HUMAN) {
        if (this->currentMask != PLAYER_MASK_NONE) {
            PlayerItemAction maskItemAction = GET_IA_FROM_MASK(this->currentMask);
            EquipSlot btn = func_8082FD0C(this, maskItemAction);

            if (btn <= EQUIP_SLOT_NONE) {
                s32 maskIdMinusOne =
                    GET_MASK_FROM_IA(Player_ItemToItemAction(this, GET_CUR_FORM_BTN_ITEM(this->unk_154))) - 1;

                if ((maskIdMinusOne < PLAYER_MASK_TRUTH - 1) || (maskIdMinusOne >= PLAYER_MASK_MAX - 1)) {
                    maskIdMinusOne = this->currentMask - 1;
                }
                Player_UseItem(play, this, Player_MaskIdToItemId(maskIdMinusOne));
                return;
            }

            if ((this->currentMask == PLAYER_MASK_GIANT) && (gSaveContext.save.saveInfo.playerData.magic == 0)) {
                func_80838A20(play, this);
            }

            this->unk_154 = btn;
        }
    }

    if (((this->actor.id == ACTOR_PLAYER) && (this->itemAction >= PLAYER_IA_FISHING_ROD)) &&
        !(((Player_GetHeldBButtonSword(this) == PLAYER_B_SWORD_NONE) || (gSaveContext.jinxTimer == 0)) &&
          (Player_ItemIsInUse(this, (IREG(1) != 0) ? ITEM_FISHING_ROD : Inventory_GetBtnBItem(play)) ||
           Player_ItemIsInUse(this, C_BTN_ITEM(EQUIP_SLOT_C_LEFT)) ||
           Player_ItemIsInUse(this, C_BTN_ITEM(EQUIP_SLOT_C_DOWN)) ||
           Player_ItemIsInUse(this, C_BTN_ITEM(EQUIP_SLOT_C_RIGHT))))) {
        Player_UseItem(play, this, ITEM_NONE);
    } else {
        s32 pad;
        ItemId item;
        EquipSlot i = func_8082FDC4();

        i = ((i >= EQUIP_SLOT_A) && (this->transformation == PLAYER_FORM_FIERCE_DEITY) &&
             (this->heldItemAction != PLAYER_IA_SWORD_TWO_HANDED))
                ? EQUIP_SLOT_B
                : i;

        item = Player_GetItemOnButton(play, this, i);

        if (item >= ITEM_FD) {
            for (i = 0; i < ARRAY_COUNT(sPlayerItemButtons); i++) {
                if (CHECK_BTN_ALL(sPlayerControlInput->cur.button, sPlayerItemButtons[i])) {
                    break;
                }
            }

            item = Player_GetItemOnButton(play, this, i);
            if ((item < ITEM_FD) && (Player_ItemToItemAction(this, item) == this->heldItemAction)) {
                sPlayerHeldItemButtonIsHeldDown = true;
            }
        } else if (item == ITEM_F0) {
            if (this->blastMaskTimer == 0) {
                EnBom* bomb = (EnBom*)Actor_Spawn(&play->actorCtx, play, ACTOR_EN_BOM, this->actor.focus.pos.x,
                                                  this->actor.focus.pos.y, this->actor.focus.pos.z,
                                                  BOMB_EXPLOSIVE_TYPE_BOMB, 0, 0, BOMB_TYPE_BODY);

                if (bomb != NULL) {
                    bomb->timer = 0;
                    //this->blastMaskTimer = 310;
                    this->blastMaskTimer = 0;
                }
            }
        } else if (item == ITEM_F1) {
            func_80839978(play, this);
        } else if (item == ITEM_F2) {
            func_80839A10(play, this);
        } else if ((Player_BButtonSwordFromIA(this, Player_ItemToItemAction(this, item)) != PLAYER_B_SWORD_NONE) &&
                   (gSaveContext.jinxTimer != 0)) {
            if (Message_GetState(&play->msgCtx) == TEXT_STATE_NONE) {
                Message_StartTextbox(play, 0xF7, NULL);
            }
        } else {
            this->heldItemButton = i;
            Player_UseItem(play, this, item);
        }
    }
}

void ZeldaArena_Free(void* ptr);
void func_80831454(Player* this);

void Player_Destroy(Actor* thisx, PlayState* play) {
    Player* this = (Player*)thisx;

    Effect_Destroy(play, this->meleeWeaponEffectIndex[0]);
    Effect_Destroy(play, this->meleeWeaponEffectIndex[1]);
    Effect_Destroy(play, this->meleeWeaponEffectIndex[2]);
    LightContext_RemoveLight(play, &play->lightCtx, this->lightNode);
    Collider_DestroyCylinder(play, &this->cylinder);
    Collider_DestroyCylinder(play, &this->shieldCylinder);
    Collider_DestroyQuad(play, &this->meleeWeaponQuads[0]);
    Collider_DestroyQuad(play, &this->meleeWeaponQuads[1]);
    Collider_DestroyQuad(play, &this->shieldQuad);
    ZeldaArena_Free(this->giObjectSegment);
    ZeldaArena_Free(this->maskObjectSegment);
    Magic_Reset(play);
    func_80831454(this);

    playing = false;
}

//~ s32 func_80847880(PlayState* play, Player* this) {
    //~ if (play->unk_1887C != 0) {
        //~ if (play->sceneId == SCENE_20SICHITAI) {
            //~ Player_SetAction(play, this, Player_Action_80, 0);
            //~ play->unk_1887C = 0;
            //~ this->csAction = PLAYER_CSACTION_NONE;
            //~ return true;
        //~ }

        //~ func_8082DE50(play, this);
        //~ Player_SetAction(play, this, Player_Action_81, 0);
        //~ if (!func_800B7118(this) || Player_IsHoldingHookshot(this)) {
            //~ Player_UseItem(play, this, ITEM_BOW);
        //~ }
        //~ Player_AnimationPlayOnce(play, this, func_8082ED20(this));
        //~ this->csAction = PLAYER_CSACTION_NONE;
        //~ this->stateFlags1 |= PLAYER_STATE1_100000;
        //~ Player_StopHorizontalMovement(this);
        //~ func_80836D8C(this);

        //~ return true;
    //~ }
    //~ return false;
//~ }

//~ void Player_Action_11(Player* this, PlayState* play) {
    //~ this->stateFlags2 |= PLAYER_STATE2_20;

    //~ if (this->linearVelocity < 1.0f) {
        //~ this->skelAnime.animation = &gPlayerAnim_clink_normal_okarina_walk;
    //~ } else {
        //~ this->skelAnime.animation = &gPlayerAnim_clink_normal_okarina_walkB;
    //~ }
    //~ PlayerAnimation_Update(play, &this->skelAnime);

    //~ if (!func_80847880(play, this)/* && (!Player_TryActionChangeList(play, this, sPlayerActionChangeList7, true) ||
                                       //~ (Player_Action_11 == this->actionFunc))*/) {
        //~ f32 speedTarget;
        //~ f32 temp_fv0;
        //~ f32 temp_fv1;
        //~ s16 yawTarget;
        //~ s16 sp30;

        //~ recomp_printf("");
        //~ speedTarget = 0.0f;
        //~ asm("add.s $f2, $0, %0" :: "f"(this->actor.velocity.y));
        //~ speedTarget += 75.0f;
        //~ recomp_printf("");

        //~ if (!CHECK_BTN_ALL(sPlayerControlInput->cur.button, BTN_B)) {
            //~ func_80839E74(this, play);
            //~ return;
        //~ }

        //~ this->linearVelocity = this->unk_B48;
        //~ Player_GetMovementSpeedAndYaw(this, &speedTarget, &yawTarget, SPEED_MODE_CURVED, play);
        //~ sp30 = yawTarget;

        //~ speedTarget += 8.0f;

        //~ if (!func_8083A4A4(this, &speedTarget, &yawTarget, REG(43) / 100.0f)) {
            //~ func_8083CB04(this, speedTarget, yawTarget, REG(19) / 100.0f, 1.5f, 0x3E8);
            //~ func_8083C8E8(this, play);
            //~ if ((this->linearVelocity == 0.0f) && (speedTarget == 0.0f)) {
                //~ this->currentYaw = sp30;
                //~ this->actor.shape.rot.y = this->currentYaw;
            //~ }
        //~ }

        //~ this->unk_B48 = this->linearVelocity;
        //~ temp_fv0 = this->skelAnime.curFrame + 5.0f;
        //~ temp_fv1 = this->skelAnime.animLength / 2.0f;

        //~ // effectively an fmodf
        //~ temp_fv0 -= temp_fv1 * (s32)(temp_fv0 / temp_fv1);
        //~ this->linearVelocity *= Math_CosS(temp_fv0 * 1000.0f) * 0.4f;
    //~ }
//~ }