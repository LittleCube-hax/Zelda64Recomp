#include "patches.h"
#include "global.h"

#include "apcommon.h"
#include "misc_funcs.h"

#define SPEED_MODE_CURVED 0.018f

extern LinkAnimationHeader gPlayerAnim_clink_normal_okarina_walk;
extern LinkAnimationHeader gPlayerAnim_clink_normal_okarina_walkB;

extern Input* sPlayerControlInput;

extern s8 sPlayerActionChangeList7[];

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