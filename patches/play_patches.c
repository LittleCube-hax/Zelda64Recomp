#include "play_patches.h"
#include "z64debug_display.h"
#include "input.h"

#include "misc_funcs.h"
#include "apcommon.h"

extern Input D_801F6C18;

s16 sExtraItemBases[] = {
    ITEM_DEKU_STICK, // ITEM_DEKU_STICKS_5
    ITEM_DEKU_STICK, // ITEM_DEKU_STICKS_10
    ITEM_DEKU_NUT,   // ITEM_DEKU_NUTS_5
    ITEM_DEKU_NUT,   // ITEM_DEKU_NUTS_10
    ITEM_BOMB,       // ITEM_BOMBS_5
    ITEM_BOMB,       // ITEM_BOMBS_10
    ITEM_BOMB,       // ITEM_BOMBS_20
    ITEM_BOMB,       // ITEM_BOMBS_30
    ITEM_BOW,        // ITEM_ARROWS_10
    ITEM_BOW,        // ITEM_ARROWS_30
    ITEM_BOW,        // ITEM_ARROWS_40
    ITEM_BOMBCHU,    // ITEM_ARROWS_50 !@bug this data is missing an ITEM_BOW, offsetting the rest by 1
    ITEM_BOMBCHU,    // ITEM_BOMBCHUS_20
    ITEM_BOMBCHU,    // ITEM_BOMBCHUS_10
    ITEM_BOMBCHU,    // ITEM_BOMBCHUS_1
    ITEM_DEKU_STICK, // ITEM_BOMBCHUS_5
    ITEM_DEKU_STICK, // ITEM_DEKU_STICK_UPGRADE_20
    ITEM_DEKU_NUT,   // ITEM_DEKU_STICK_UPGRADE_30
    ITEM_DEKU_NUT,   // ITEM_DEKU_NUT_UPGRADE_30
};

s16 sAmmoRefillCounts[] = { 5, 10, 20, 30 }; // Sticks, nuts, bombs
s16 sArrowRefillCounts[] = { 10, 30, 40, 50 };
s16 sBombchuRefillCounts[] = { 20, 10, 1, 5 };
s16 sRupeeRefillCounts[] = { 1, 5, 10, 20, 50, 100, 200 };

Vec3f D_8085D41C = { 0.0f, 0.0f, -30.0f };

PlayState* gPlay;
u32 old_items_size = 0;
bool playing = false;

void Interface_StartBottleTimer(s16 seconds, s16 timerId);
void Player_TranslateAndRotateY(Player* this, Vec3f* translation, Vec3f* src, Vec3f* dst);
void Player_DetachHeldActor(PlayState* play, Player* this);
void Player_UpdateCommon(Player* this, PlayState* play, Input* input);

void controls_play_update(PlayState* play) {
    gSaveContext.options.zTargetSetting = recomp_get_targeting_mode();
}

u8 apItemGive(u32 item) {
    PlayState* play = gPlay;
    Player* player = GET_PLAYER(play);
    u8 i;
    u8 temp;
    u8 slot;

    switch (item & 0xFF00) {
        case 0xFE00:
            switch (item & 0xFF) {
                case 0x7F:
                    SET_WEEKEVENTREG(WEEKEVENTREG_08_80);
                    break;
            }
            break;

        case 0xFD00:
            gSaveContext.save.saveInfo.playerData.isMagicAcquired = true;
            gSaveContext.magicFillTarget = MAGIC_NORMAL_METER;
            break;
    }

    slot = SLOT(item);
    if (item >= ITEM_DEKU_STICKS_5) {
        slot = SLOT(sExtraItemBases[item - ITEM_DEKU_STICKS_5]);
    }

    if (item == ITEM_SKULL_TOKEN) {
        //! @bug: Sets QUEST_QUIVER instead of QUEST_SKULL_TOKEN
        // Setting `QUEST_SKULL_TOKEN` will result in misplaced digits on the pause menu - Quest Status page.
        SET_QUEST_ITEM(item - ITEM_SKULL_TOKEN + QUEST_QUIVER);
        Inventory_IncrementSkullTokenCount(play->sceneId);
        return ITEM_NONE;

    } else if (item == ITEM_DEED_LAND) {
        INV_CONTENT(ITEM_DEED_LAND) = ITEM_DEED_LAND;
        return ITEM_NONE;

    } else if (item == ITEM_TINGLE_MAP) {
        return ITEM_NONE;

    } else if (item == ITEM_BOMBERS_NOTEBOOK) {
        SET_QUEST_ITEM(QUEST_BOMBERS_NOTEBOOK);
        return ITEM_NONE;

    } else if ((item == ITEM_HEART_PIECE_2) || (item == ITEM_HEART_PIECE)) {
        INCREMENT_QUEST_HEART_PIECE_COUNT;
        if (EQ_MAX_QUEST_HEART_PIECE_COUNT) {
            RESET_HEART_PIECE_COUNT;
            gSaveContext.save.saveInfo.playerData.healthCapacity += 0x10;
            gSaveContext.save.saveInfo.playerData.health += 0x10;
        }
        return ITEM_NONE;

    } else if (item == ITEM_HEART_CONTAINER) {
        gSaveContext.save.saveInfo.playerData.healthCapacity += 0x10;
        gSaveContext.save.saveInfo.playerData.health += 0x10;
        return ITEM_NONE;

    } else if ((item >= ITEM_SONG_SONATA) && (item <= ITEM_SONG_LULLABY_INTRO)) {
        SET_QUEST_ITEM(item - ITEM_SONG_SONATA + QUEST_SONG_SONATA);
        return ITEM_NONE;

    } else if ((item >= ITEM_SWORD_KOKIRI) && (item <= ITEM_SWORD_GILDED)) {
        SET_EQUIP_VALUE(EQUIP_TYPE_SWORD, item - ITEM_SWORD_KOKIRI + EQUIP_VALUE_SWORD_KOKIRI);
        CUR_FORM_EQUIP(EQUIP_SLOT_B) = item;
        Interface_LoadItemIconImpl(play, EQUIP_SLOT_B);
        if (item == ITEM_SWORD_RAZOR) {
            gSaveContext.save.saveInfo.playerData.swordHealth = 100;
        }
        return ITEM_NONE;

    } else if ((item >= ITEM_SHIELD_HERO) && (item <= ITEM_SHIELD_MIRROR)) {
        if (GET_CUR_EQUIP_VALUE(EQUIP_TYPE_SHIELD) != (u16)(item - ITEM_SHIELD_HERO + EQUIP_VALUE_SHIELD_HERO)) {
            SET_EQUIP_VALUE(EQUIP_TYPE_SHIELD, item - ITEM_SHIELD_HERO + EQUIP_VALUE_SHIELD_HERO);
            Player_SetEquipmentData(play, player);
            return ITEM_NONE;
        }
        return item;

    } else if ((item == ITEM_KEY_BOSS) || (item == ITEM_COMPASS) || (item == ITEM_DUNGEON_MAP)) {
        SET_DUNGEON_ITEM(item - ITEM_KEY_BOSS, gSaveContext.mapIndex);
        return ITEM_NONE;

    } else if (item == ITEM_KEY_SMALL) {
        if (DUNGEON_KEY_COUNT(gSaveContext.mapIndex) < 0) {
            DUNGEON_KEY_COUNT(gSaveContext.mapIndex) = 1;
            return ITEM_NONE;
        } else {
            DUNGEON_KEY_COUNT(gSaveContext.mapIndex)++;
            return ITEM_NONE;
        }

    } else if ((item == ITEM_QUIVER_30) || (item == ITEM_BOW)) {
        if (CUR_UPG_VALUE(UPG_QUIVER) == 0) {
            Inventory_ChangeUpgrade(UPG_QUIVER, 1);
            INV_CONTENT(ITEM_BOW) = ITEM_BOW;
            AMMO(ITEM_BOW) = CAPACITY(UPG_QUIVER, 1);
            return ITEM_NONE;
        } else {
            AMMO(ITEM_BOW)++;
            if (AMMO(ITEM_BOW) > (s8)CUR_CAPACITY(UPG_QUIVER)) {
                AMMO(ITEM_BOW) = CUR_CAPACITY(UPG_QUIVER);
            }
        }

    } else if (item == ITEM_QUIVER_40) {
        Inventory_ChangeUpgrade(UPG_QUIVER, 2);
        INV_CONTENT(ITEM_BOW) = ITEM_BOW;
        AMMO(ITEM_BOW) = CAPACITY(UPG_QUIVER, 2);
        return ITEM_NONE;

    } else if (item == ITEM_QUIVER_50) {
        Inventory_ChangeUpgrade(UPG_QUIVER, 3);
        INV_CONTENT(ITEM_BOW) = ITEM_BOW;
        AMMO(ITEM_BOW) = CAPACITY(UPG_QUIVER, 3);
        return ITEM_NONE;

    } else if (item == ITEM_BOMB_BAG_20) {
        if (CUR_UPG_VALUE(UPG_BOMB_BAG) == 0) {
            Inventory_ChangeUpgrade(UPG_BOMB_BAG, 1);
            INV_CONTENT(ITEM_BOMB) = ITEM_BOMB;
            AMMO(ITEM_BOMB) = CAPACITY(UPG_BOMB_BAG, 1);
            return ITEM_NONE;

        } else {
            AMMO(ITEM_BOMB)++;
            if (AMMO(ITEM_BOMB) > CUR_CAPACITY(UPG_BOMB_BAG)) {
                AMMO(ITEM_BOMB) = CUR_CAPACITY(UPG_BOMB_BAG);
            }
        }

    } else if (item == ITEM_BOMB_BAG_30) {
        Inventory_ChangeUpgrade(UPG_BOMB_BAG, 2);
        INV_CONTENT(ITEM_BOMB) = ITEM_BOMB;
        AMMO(ITEM_BOMB) = CAPACITY(UPG_BOMB_BAG, 2);
        return ITEM_NONE;

    } else if (item == ITEM_BOMB_BAG_40) {
        Inventory_ChangeUpgrade(UPG_BOMB_BAG, 3);
        INV_CONTENT(ITEM_BOMB) = ITEM_BOMB;
        AMMO(ITEM_BOMB) = CAPACITY(UPG_BOMB_BAG, 3);
        return ITEM_NONE;

    } else if (item == ITEM_WALLET_ADULT) {
        Inventory_ChangeUpgrade(UPG_WALLET, 1);
        return ITEM_NONE;

    } else if (item == ITEM_WALLET_GIANT) {
        Inventory_ChangeUpgrade(UPG_WALLET, 2);
        return ITEM_NONE;

    } else if (item == ITEM_DEKU_STICK_UPGRADE_20) {
        if (INV_CONTENT(ITEM_DEKU_STICK) != ITEM_DEKU_STICK) {
            INV_CONTENT(ITEM_DEKU_STICK) = ITEM_DEKU_STICK;
        }
        Inventory_ChangeUpgrade(UPG_DEKU_STICKS, 2);
        AMMO(ITEM_DEKU_STICK) = CAPACITY(UPG_DEKU_STICKS, 2);
        return ITEM_NONE;

    } else if (item == ITEM_DEKU_STICK_UPGRADE_30) {
        if (INV_CONTENT(ITEM_DEKU_STICK) != ITEM_DEKU_STICK) {
            INV_CONTENT(ITEM_DEKU_STICK) = ITEM_DEKU_STICK;
        }
        Inventory_ChangeUpgrade(UPG_DEKU_STICKS, 3);
        AMMO(ITEM_DEKU_STICK) = CAPACITY(UPG_DEKU_STICKS, 3);
        return ITEM_NONE;

    } else if (item == ITEM_DEKU_NUT_UPGRADE_30) {
        if (INV_CONTENT(ITEM_DEKU_NUT) != ITEM_DEKU_NUT) {
            INV_CONTENT(ITEM_DEKU_NUT) = ITEM_DEKU_NUT;
        }
        Inventory_ChangeUpgrade(UPG_DEKU_NUTS, 2);
        AMMO(ITEM_DEKU_NUT) = CAPACITY(UPG_DEKU_NUTS, 2);
        return ITEM_NONE;

    } else if (item == ITEM_DEKU_NUT_UPGRADE_40) {
        if (INV_CONTENT(ITEM_DEKU_NUT) != ITEM_DEKU_NUT) {
            INV_CONTENT(ITEM_DEKU_NUT) = ITEM_DEKU_NUT;
        }
        Inventory_ChangeUpgrade(UPG_DEKU_NUTS, 3);
        AMMO(ITEM_DEKU_NUT) = CAPACITY(UPG_DEKU_NUTS, 3);
        return ITEM_NONE;

    } else if (item == ITEM_DEKU_STICK) {
        if (INV_CONTENT(ITEM_DEKU_STICK) != ITEM_DEKU_STICK) {
            Inventory_ChangeUpgrade(UPG_DEKU_STICKS, 1);
            AMMO(ITEM_DEKU_STICK) = 1;
        } else {
            AMMO(ITEM_DEKU_STICK)++;
            if (AMMO(ITEM_DEKU_STICK) > CUR_CAPACITY(UPG_DEKU_STICKS)) {
                AMMO(ITEM_DEKU_STICK) = CUR_CAPACITY(UPG_DEKU_STICKS);
            }
        }

    } else if ((item == ITEM_DEKU_STICKS_5) || (item == ITEM_DEKU_STICKS_10)) {
        if (INV_CONTENT(ITEM_DEKU_STICK) != ITEM_DEKU_STICK) {
            Inventory_ChangeUpgrade(UPG_DEKU_STICKS, 1);
            AMMO(ITEM_DEKU_STICK) = sAmmoRefillCounts[item - ITEM_DEKU_STICKS_5];
        } else {
            AMMO(ITEM_DEKU_STICK) += sAmmoRefillCounts[item - ITEM_DEKU_STICKS_5];
            if (AMMO(ITEM_DEKU_STICK) > CUR_CAPACITY(UPG_DEKU_STICKS)) {
                AMMO(ITEM_DEKU_STICK) = CUR_CAPACITY(UPG_DEKU_STICKS);
            }
        }

        item = ITEM_DEKU_STICK;

    } else if (item == ITEM_DEKU_NUT) {
        if (INV_CONTENT(ITEM_DEKU_NUT) != ITEM_DEKU_NUT) {
            Inventory_ChangeUpgrade(UPG_DEKU_NUTS, 1);
            AMMO(ITEM_DEKU_NUT) = 1;
        } else {
            AMMO(ITEM_DEKU_NUT)++;
            if (AMMO(ITEM_DEKU_NUT) > CUR_CAPACITY(UPG_DEKU_NUTS)) {
                AMMO(ITEM_DEKU_NUT) = CUR_CAPACITY(UPG_DEKU_NUTS);
            }
        }

    } else if ((item == ITEM_DEKU_NUTS_5) || (item == ITEM_DEKU_NUTS_10)) {
        if (INV_CONTENT(ITEM_DEKU_NUT) != ITEM_DEKU_NUT) {
            Inventory_ChangeUpgrade(UPG_DEKU_NUTS, 1);
            AMMO(ITEM_DEKU_NUT) += sAmmoRefillCounts[item - ITEM_DEKU_NUTS_5];
        } else {
            AMMO(ITEM_DEKU_NUT) += sAmmoRefillCounts[item - ITEM_DEKU_NUTS_5];
            if (AMMO(ITEM_DEKU_NUT) > CUR_CAPACITY(UPG_DEKU_NUTS)) {
                AMMO(ITEM_DEKU_NUT) = CUR_CAPACITY(UPG_DEKU_NUTS);
            }
        }
        item = ITEM_DEKU_NUT;

    } else if (item == ITEM_POWDER_KEG) {
        if (INV_CONTENT(ITEM_POWDER_KEG) != ITEM_POWDER_KEG) {
            INV_CONTENT(ITEM_POWDER_KEG) = ITEM_POWDER_KEG;
        }

        AMMO(ITEM_POWDER_KEG) = 1;
        return ITEM_NONE;

    } else if (item == ITEM_BOMB) {
        if ((AMMO(ITEM_BOMB) += 1) > CUR_CAPACITY(UPG_BOMB_BAG)) {
            AMMO(ITEM_BOMB) = CUR_CAPACITY(UPG_BOMB_BAG);
        }
        return ITEM_NONE;

    } else if ((item >= ITEM_BOMBS_5) && (item <= ITEM_BOMBS_30)) {
        if (gSaveContext.save.saveInfo.inventory.items[SLOT_BOMB] != ITEM_BOMB) {
            INV_CONTENT(ITEM_BOMB) = ITEM_BOMB;
            AMMO(ITEM_BOMB) += sAmmoRefillCounts[item - ITEM_BOMBS_5];
            return ITEM_NONE;
        }

        if ((AMMO(ITEM_BOMB) += sAmmoRefillCounts[item - ITEM_BOMBS_5]) > CUR_CAPACITY(UPG_BOMB_BAG)) {
            AMMO(ITEM_BOMB) = CUR_CAPACITY(UPG_BOMB_BAG);
        }
        return ITEM_NONE;

    } else if (item == ITEM_BOMBCHU) {
        if (INV_CONTENT(ITEM_BOMBCHU) != ITEM_BOMBCHU) {
            INV_CONTENT(ITEM_BOMBCHU) = ITEM_BOMBCHU;
            AMMO(ITEM_BOMBCHU) = 10;
            return ITEM_NONE;
        }
        if ((AMMO(ITEM_BOMBCHU) += 10) > CUR_CAPACITY(UPG_BOMB_BAG)) {
            AMMO(ITEM_BOMBCHU) = CUR_CAPACITY(UPG_BOMB_BAG);
        }
        return ITEM_NONE;

    } else if ((item >= ITEM_BOMBCHUS_20) && (item <= ITEM_BOMBCHUS_5)) {
        if (gSaveContext.save.saveInfo.inventory.items[SLOT_BOMBCHU] != ITEM_BOMBCHU) {
            INV_CONTENT(ITEM_BOMBCHU) = ITEM_BOMBCHU;
            AMMO(ITEM_BOMBCHU) += sBombchuRefillCounts[item - ITEM_BOMBCHUS_20];

            if (AMMO(ITEM_BOMBCHU) > CUR_CAPACITY(UPG_BOMB_BAG)) {
                AMMO(ITEM_BOMBCHU) = CUR_CAPACITY(UPG_BOMB_BAG);
            }
            return ITEM_NONE;
        }

        if ((AMMO(ITEM_BOMBCHU) += sBombchuRefillCounts[item - ITEM_BOMBCHUS_20]) > CUR_CAPACITY(UPG_BOMB_BAG)) {
            AMMO(ITEM_BOMBCHU) = CUR_CAPACITY(UPG_BOMB_BAG);
        }
        return ITEM_NONE;

    } else if ((item >= ITEM_ARROWS_10) && (item <= ITEM_ARROWS_50)) {
        AMMO(ITEM_BOW) += sArrowRefillCounts[item - ITEM_ARROWS_10];

        if ((AMMO(ITEM_BOW) >= CUR_CAPACITY(UPG_QUIVER)) || (AMMO(ITEM_BOW) < 0)) {
            AMMO(ITEM_BOW) = CUR_CAPACITY(UPG_QUIVER);
        }
        return ITEM_BOW;

    } else if (item == ITEM_OCARINA_OF_TIME) {
        INV_CONTENT(ITEM_OCARINA_OF_TIME) = ITEM_OCARINA_OF_TIME;
        return ITEM_NONE;

    } else if (item == ITEM_MAGIC_BEANS) {
        if (INV_CONTENT(ITEM_MAGIC_BEANS) == ITEM_NONE) {
            INV_CONTENT(item) = item;
            AMMO(ITEM_MAGIC_BEANS) = 1;
        } else if (AMMO(ITEM_MAGIC_BEANS) < 20) {
            AMMO(ITEM_MAGIC_BEANS)++;
        } else {
            AMMO(ITEM_MAGIC_BEANS) = 20;
        }
        return ITEM_NONE;

    } else if ((item >= ITEM_REMAINS_ODOLWA) && (item <= ITEM_REMAINS_TWINMOLD)) {
        SET_QUEST_ITEM(item - ITEM_REMAINS_ODOLWA + QUEST_REMAINS_ODOLWA);
        return ITEM_NONE;

    } else if (item == ITEM_RECOVERY_HEART) {
        Health_ChangeBy(play, 0x10);
        return item;

    } else if (item == ITEM_MAGIC_JAR_SMALL) {
        Magic_Add(play, MAGIC_NORMAL_METER / 2);
        if (!CHECK_WEEKEVENTREG(WEEKEVENTREG_12_80)) {
            SET_WEEKEVENTREG(WEEKEVENTREG_12_80);
            return ITEM_NONE;
        }
        return item;

    } else if (item == ITEM_MAGIC_JAR_BIG) {
        Magic_Add(play, MAGIC_NORMAL_METER);
        if (!CHECK_WEEKEVENTREG(WEEKEVENTREG_12_80)) {
            SET_WEEKEVENTREG(WEEKEVENTREG_12_80);
            return ITEM_NONE;
        }
        return item;

    } else if ((item >= ITEM_RUPEE_GREEN) && (item <= ITEM_RUPEE_HUGE)) {
        Rupees_ChangeBy(sRupeeRefillCounts[item - ITEM_RUPEE_GREEN]);
        return ITEM_NONE;

    } else if (item == ITEM_LONGSHOT) {
        slot = SLOT(item);

        for (i = BOTTLE_FIRST; i < BOTTLE_MAX; i++) {
            if (gSaveContext.save.saveInfo.inventory.items[slot + i] == ITEM_NONE) {
                gSaveContext.save.saveInfo.inventory.items[slot + i] = ITEM_POTION_RED;
                return ITEM_NONE;
            }
        }
        return item;

    } else if ((item == ITEM_MILK_BOTTLE) || (item == ITEM_POE) || (item == ITEM_GOLD_DUST) || (item == ITEM_CHATEAU) ||
               (item == ITEM_HYLIAN_LOACH)) {
        slot = SLOT(item);

        for (i = BOTTLE_FIRST; i < BOTTLE_MAX; i++) {
            if (gSaveContext.save.saveInfo.inventory.items[slot + i] == ITEM_NONE) {
                gSaveContext.save.saveInfo.inventory.items[slot + i] = item;
                return ITEM_NONE;
            }
        }
        return item;

    } else if (item == ITEM_BOTTLE) {
        slot = SLOT(item);

        for (i = BOTTLE_FIRST; i < BOTTLE_MAX; i++) {
            if (gSaveContext.save.saveInfo.inventory.items[slot + i] == ITEM_NONE) {
                gSaveContext.save.saveInfo.inventory.items[slot + i] = item;
                return ITEM_NONE;
            }
        }
        return item;

    } else if (((item >= ITEM_POTION_RED) && (item <= ITEM_OBABA_DRINK)) || (item == ITEM_CHATEAU_2) ||
               (item == ITEM_MILK) || (item == ITEM_GOLD_DUST_2) || (item == ITEM_HYLIAN_LOACH_2) ||
               (item == ITEM_SEAHORSE_CAUGHT)) {
        slot = SLOT(item);

        if ((item != ITEM_MILK_BOTTLE) && (item != ITEM_MILK_HALF)) {
            if (item == ITEM_CHATEAU_2) {
                item = ITEM_CHATEAU;

            } else if (item == ITEM_MILK) {
                item = ITEM_MILK_BOTTLE;

            } else if (item == ITEM_GOLD_DUST_2) {
                item = ITEM_GOLD_DUST;

            } else if (item == ITEM_HYLIAN_LOACH_2) {
                item = ITEM_HYLIAN_LOACH;

            } else if (item == ITEM_SEAHORSE_CAUGHT) {
                item = ITEM_SEAHORSE;
            }
            slot = SLOT(item);

            for (i = BOTTLE_FIRST; i < BOTTLE_MAX; i++) {
                if (gSaveContext.save.saveInfo.inventory.items[slot + i] == ITEM_BOTTLE) {
                    if (item == ITEM_HOT_SPRING_WATER) {
                        Interface_StartBottleTimer(60, i);
                    }

                    if ((slot + i) == C_SLOT_EQUIP(0, EQUIP_SLOT_C_LEFT)) {
                        BUTTON_ITEM_EQUIP(0, EQUIP_SLOT_C_LEFT) = item;
                        Interface_LoadItemIconImpl(play, EQUIP_SLOT_C_LEFT);
                        gSaveContext.buttonStatus[EQUIP_SLOT_C_LEFT] = BTN_ENABLED;
                    } else if ((slot + i) == C_SLOT_EQUIP(0, EQUIP_SLOT_C_DOWN)) {
                        BUTTON_ITEM_EQUIP(0, EQUIP_SLOT_C_DOWN) = item;
                        Interface_LoadItemIconImpl(play, EQUIP_SLOT_C_DOWN);
                        gSaveContext.buttonStatus[EQUIP_SLOT_C_DOWN] = BTN_ENABLED;
                    } else if ((slot + i) == C_SLOT_EQUIP(0, EQUIP_SLOT_C_RIGHT)) {
                        BUTTON_ITEM_EQUIP(0, EQUIP_SLOT_C_RIGHT) = item;
                        Interface_LoadItemIconImpl(play, EQUIP_SLOT_C_RIGHT);
                        gSaveContext.buttonStatus[EQUIP_SLOT_C_RIGHT] = BTN_ENABLED;
                    }

                    gSaveContext.save.saveInfo.inventory.items[slot + i] = item;
                    return ITEM_NONE;
                }
            }
        } else {
            for (i = BOTTLE_FIRST; i < BOTTLE_MAX; i++) {
                if (gSaveContext.save.saveInfo.inventory.items[slot + i] == ITEM_NONE) {
                    gSaveContext.save.saveInfo.inventory.items[slot + i] = item;
                    return ITEM_NONE;
                }
            }
        }

    } else if ((item >= ITEM_MOONS_TEAR) && (item <= ITEM_MASK_GIANT)) {
        temp = INV_CONTENT(item);
        INV_CONTENT(item) = item;
        if ((item >= ITEM_MOONS_TEAR) && (item <= ITEM_PENDANT_OF_MEMORIES) && (temp != ITEM_NONE)) {
            for (i = EQUIP_SLOT_C_LEFT; i <= EQUIP_SLOT_C_RIGHT; i++) {
                if (temp == GET_CUR_FORM_BTN_ITEM(i)) {
                    SET_CUR_FORM_BTN_ITEM(i, item);
                    Interface_LoadItemIconImpl(play, i);
                    return ITEM_NONE;
                }
            }
        }
        return ITEM_NONE;
    }

    temp = gSaveContext.save.saveInfo.inventory.items[slot];
    INV_CONTENT(item) = item;
    return temp;
}

s32 func_8082DA90(PlayState* play) {
    return (play->transitionTrigger != TRANS_TRIGGER_OFF) || (play->transitionMode != TRANS_MODE_OFF);
}

void Player_Update(Actor* thisx, PlayState* play) {
    static Vec3f sDogSpawnPos;
    Player* this = (Player*)thisx;
    s32 dogParams;
    s32 pad;
    Input input;
    s32 pad2;
    u32 new_items_size;

    if (playing) {
        new_items_size = recomp_get_items_size();

        if (new_items_size > old_items_size) {
            u32 i;
            for (i = old_items_size; i < new_items_size; ++i) {
                apItemGive(recomp_get_item(i));
            }

            old_items_size = new_items_size;
        }
    }

    this->stateFlags3 &= ~PLAYER_STATE3_10;

    // This block is a leftover dog-following mechanic from OoT
    if (gSaveContext.dogParams < 0) {
        if (Object_GetSlot(&play->objectCtx, OBJECT_DOG) < 0) {
            gSaveContext.dogParams = 0;
        } else {
            Actor* dog;

            gSaveContext.dogParams &= (u16)~0x8000;
            Player_TranslateAndRotateY(this, &this->actor.world.pos, &D_8085D41C, &sDogSpawnPos);

            dogParams = gSaveContext.dogParams;

            dog = Actor_Spawn(&play->actorCtx, play, ACTOR_EN_DG, sDogSpawnPos.x, sDogSpawnPos.y, sDogSpawnPos.z, 0,
                              this->actor.shape.rot.y, 0, dogParams | 0x8000);
            if (dog != NULL) {
                dog->room = -1;
            }
        }
    }

    if ((this->interactRangeActor != NULL) && (this->interactRangeActor->update == NULL)) {
        this->interactRangeActor = NULL;
    }

    if ((this->heldActor != NULL) && (this->heldActor->update == NULL)) {
        Player_DetachHeldActor(play, this);
    }

    if ((play->actorCtx.isOverrideInputOn != 0) && (this == GET_PLAYER(play))) {
        input = play->actorCtx.overrideInput;
    } else if ((this->csAction == PLAYER_CSACTION_5) ||
               (this->stateFlags1 & (PLAYER_STATE1_20 | PLAYER_STATE1_20000000)) || (this != GET_PLAYER(play)) ||
               func_8082DA90(play) || (gSaveContext.save.saveInfo.playerData.health == 0)) {
        bzero(&input, sizeof(Input));
        this->fallStartHeight = this->actor.world.pos.y;
    } else {
        input = *CONTROLLER1(&play->state);
        if (this->unk_B5E != 0) {
            input.cur.button &= ~(BTN_CUP | BTN_B | BTN_A);
            input.press.button &= ~(BTN_CUP | BTN_B | BTN_A);
        }
    }

    Player_UpdateCommon(this, play, &input);
    play->actorCtx.isOverrideInputOn = 0;
    bzero(&play->actorCtx.overrideInput, sizeof(Input));

    MREG(52) = this->actor.world.pos.x;
    MREG(53) = this->actor.world.pos.y;
    MREG(54) = this->actor.world.pos.z;
    MREG(55) = this->actor.world.rot.y;
}

// @recomp Patched to add hooks for various added functionality.
void Play_Main(GameState* thisx) {
    static Input* prevInput = NULL;
    PlayState* this = (PlayState*)thisx;

    gPlay = this;

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
