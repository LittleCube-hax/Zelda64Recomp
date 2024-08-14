#include "play_patches.h"
#include "z64debug_display.h"
#include "input.h"

#include "misc_funcs.h"
#include "apcommon.h"

extern Input D_801F6C18;

extern s16 sExtraItemBases[];

extern s16 sAmmoRefillCounts[]; // Sticks, nuts, bombs
extern s16 sArrowRefillCounts[];
extern s16 sBombchuRefillCounts[];
extern s16 sRupeeRefillCounts[];

s8 giToItemId[GI_MAX] = {
    0x00,
    0x84,
    0x85,
    0x86,
    0x87,
    0x88,
    0x89,
    0x8A,
    0x5A,
    0x5B,
    0x83,
    0x83,
    0x7B,
    0x6F,
    0x79,
    0x7A,
    0x83,
    0xFF,
    0x83,
    0x83,
    0x06,
    0x8F,
    0x90,
    0x91,
    0x92,
    0x08,
    0x98,
    0x56,
    0x57,
    0x58,
    0x93,
    0x94,
    0x95,
    0x95,
    0x01,
    0x54,
    0x55,
    0x02,
    0x03,
    0x04,
    0x09,
    0x8D,
    0x8E,
    0x9D,
    0x9D,
    0x9E,
    0x97,
    0x9B,
    0x9B,
    0x9C,
    0x51,
    0x52,
    0x0C,
    0x0A,
    0x99,
    0x4D,
    0x4E,
    0x4F,
    0x9A,
    0x10,
    0x78,
    0x74,
    0x76,
    0x75,
    0xFF,
    0x0F,
    0x0E,
    0x0D,
    0x0D,
    0x83,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0x00,
    0xFF,
    0xFF,
    0xFF,
    0x6D,
    0xFF,
    0x6E,
    0xFF,
    0xFF,
    0x5D,
    0x5E,
    0x5F,
    0x60,
    0x11,
    0x12,
    0x13,
    0x14,
    0x15,
    0x16,
    0x16,
    0x18,
    0x19,
    0x1A,
    0x1B,
    0x1C,
    0x12,
    0x1E,
    0x1F,
    0x20,
    0x21,
    0x22,
    0x23,
    0xFF,
    0x12,
    0x24,
    0x25,
    0x12,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0x32,
    0x33,
    0x34,
    0x35,
    0x44,
    0x49,
    0x38,
    0x39,
    0x3A,
    0x3B,
    0x3C,
    0x3D,
    0x3E,
    0x3F,
    0x40,
    0x41,
    0x42,
    0x43,
    0x36,
    0x45,
    0x46,
    0x47,
    0x48,
    0x37,
    0xFF,
    0x9F,
    0xA0,
    0xA1,
    0xA2,
    0xA3,
    0x28,
    0x29,
    0x2A,
    0x2B,
    0x2C,
    0x10,
    0x4D,
    0x4E,
    0x4F,
    0x51,
    0x2D,
    0x2E,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0x12,
    0x2F,
    0x30,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0x31,
    0x31,
    0x31,
    0x31,
    0x31,
    0x31
};

PlayState* gPlay;
u32 old_items_size = 0;
bool playing = false;
bool initItems = false;

void Interface_StartBottleTimer(s16 seconds, s16 timerId);

void controls_play_update(PlayState* play) {
    gSaveContext.options.zTargetSetting = recomp_get_targeting_mode();
}

// @recomp Patched to add hooks for various added functionality.
void Play_Main(GameState* thisx) {
    static Input* prevInput = NULL;
    PlayState* this = (PlayState*)thisx;
    u32 new_items_size;
    u32 i;
    u8* save_ptr;

    gPlay = this;

    if (playing) {
        new_items_size = recomp_get_items_size();

        if (!initItems) {
            u8 new_bow_level = recomp_has_item(GI_QUIVER_30);
            u8 new_bomb_level = recomp_has_item(GI_BOMB_BAG_20);
            u8 new_wallet_level = recomp_has_item(GI_WALLET_ADULT);
            u8 new_sword_level = recomp_has_item(GI_SWORD_KOKIRI);

            u8 bottle_count_new = recomp_has_item(GI_BOTTLE) + recomp_has_item(GI_POTION_RED_BOTTLE) + recomp_has_item(GI_CHATEAU_BOTTLE);
            u8 bottle_count = 0;

            SET_EQUIP_VALUE(EQUIP_TYPE_SWORD, EQUIP_VALUE_SWORD_NONE);
            if (CUR_FORM == 0) {
                CUR_FORM_EQUIP(EQUIP_SLOT_B) = ITEM_NONE;
            } else {
                BUTTON_ITEM_EQUIP(0, EQUIP_SLOT_B) = ITEM_NONE;
            }
            Interface_LoadItemIconImpl(this, EQUIP_SLOT_B);

            new_bow_level -= CUR_UPG_VALUE(UPG_QUIVER);
            for (i = 0; i < new_bow_level; ++i) {
                apItemGive(GI_QUIVER_30);
            }

            new_bomb_level -= CUR_UPG_VALUE(UPG_BOMB_BAG);
            for (i = 0; i < new_bomb_level; ++i) {
                apItemGive(GI_BOMB_BAG_20);
            }

            new_wallet_level -= CUR_UPG_VALUE(UPG_WALLET);
            for (i = 0; i < new_wallet_level; ++i) {
                apItemGive(GI_WALLET_ADULT);
            }

            new_sword_level -= GET_CUR_EQUIP_VALUE(EQUIP_TYPE_SWORD);
            for (i = 0; i < new_sword_level; ++i) {
                apItemGive(GI_SWORD_KOKIRI);
            }

            for (i = SLOT_BOTTLE_1; i <= SLOT_BOTTLE_6; ++i) {
                if ((gSaveContext.save.saveInfo.inventory.items[i] >= ITEM_POTION_RED && gSaveContext.save.saveInfo.inventory.items[i] <= ITEM_OBABA_DRINK) || gSaveContext.save.saveInfo.inventory.items[i] == ITEM_BOTTLE) {
                    bottle_count += 1;
                }
            }
            for (i = bottle_count; i < bottle_count_new; ++i) {
                apItemGive(GI_BOTTLE);
            }

            if (recomp_has_item(GI_BOMBCHUS_1) || recomp_has_item(GI_BOMBCHUS_5) || recomp_has_item(GI_BOMBCHUS_10) || recomp_has_item(GI_BOMBCHUS_20)) {
                apItemGive(GI_BOMBCHUS_20);
            }

            for (i = old_items_size; i < new_items_size; ++i) {
                u32 item_id = recomp_get_item(i);
                u8 gi = item_id & 0xFF;
                bool is_gi = (item_id & 0xFF0000) == 0;
                if (is_gi) {
                    if ((gi == GI_BOMBCHUS_1 || gi == GI_BOMBCHUS_5 || gi == GI_BOMBCHUS_10 || gi == GI_BOMBCHUS_20) && INV_HAS(ITEM_BOMBCHU)) {
                        continue;
                    }
                    switch (gi) {
                        case GI_QUIVER_30:
                        case GI_BOMB_BAG_20:
                        case GI_WALLET_ADULT:
                        case GI_SWORD_KOKIRI:
                        case GI_POTION_RED_BOTTLE:
                        case GI_CHATEAU_BOTTLE:
                            continue;
                    }
                }
                apItemGive(item_id);
            }

            old_items_size = new_items_size;
            initItems = true;
        }

        if (new_items_size > old_items_size) {
            u32 i;

            for (i = old_items_size; i < new_items_size; ++i) {
                apItemGive(recomp_get_item(i));
            }

            old_items_size = new_items_size;
        }
    }

    // @recomp
    debug_play_update(this);
    controls_play_update(this);
    analog_cam_pre_play_update(this);
    matrix_play_update(this);
    
    // @recomp avoid unused variable warning
    (void)prevInput;

    prevInput = CONTROLLER1(&this->state);
    DebugDisplay_Init();

    {
        GraphicsContext* gfxCtx = this->state.gfxCtx;

        if (1) {
            this->state.gfxCtx = NULL;
        }
        camera_pre_play_update(this);
        Play_Update(this);
        camera_post_play_update(this);
        analog_cam_post_play_update(this);
        autosave_post_play_update(this);
        this->state.gfxCtx = gfxCtx;
    }

    {
        Input input = *CONTROLLER1(&this->state);

        if (1) {
            *CONTROLLER1(&this->state) = D_801F6C18;
        }
        Play_Draw(this);
        *CONTROLLER1(&this->state) = input;
    }

    CutsceneManager_Update();
    CutsceneManager_ClearWaiting();
}

// @recomp Patched to add load a hook for loading rooms.
s32 Room_HandleLoadCallbacks(PlayState* play, RoomContext* roomCtx) {
    if (roomCtx->status == 1) {
        if (osRecvMesg(&roomCtx->loadQueue, NULL, OS_MESG_NOBLOCK) == 0) {
            roomCtx->status = 0;
            roomCtx->curRoom.segment = roomCtx->activeRoomVram;
            gSegments[3] = OS_K0_TO_PHYSICAL(roomCtx->activeRoomVram);

            // @recomp Call the room load hook.
            room_load_hook(play, &roomCtx->curRoom);

            Scene_ExecuteCommands(play, roomCtx->curRoom.segment);
            func_80123140(play, GET_PLAYER(play));
            Actor_SpawnTransitionActors(play, &play->actorCtx);

            if (((play->sceneId != SCENE_IKANA) || (roomCtx->curRoom.num != 1)) && (play->sceneId != SCENE_IKNINSIDE)) {
                play->envCtx.lightSettingOverride = LIGHT_SETTING_OVERRIDE_NONE;
                play->envCtx.lightBlendOverride = LIGHT_BLEND_OVERRIDE_NONE;
            }
            func_800FEAB0();
            if (Environment_GetStormState(play) == STORM_STATE_OFF) {
                Environment_StopStormNatureAmbience(play);
            }
        } else {
            return 0;
        }
    }

    return 1;
}
