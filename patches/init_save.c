#include "patches.h"
#include "play_patches.h"
#include "z64save.h"
#include "z64horse.h"
#include "overlays/gamestates/ovl_file_choose/z_file_select.h"

#include "misc_funcs.h"
#include "apcommon.h"

SavePlayerData sSaveDefaultPlayerData = {
    { '\0', '\0', '\0', '\0', '\0', '\0' },             // newf
    0,                                                  // threeDayResetCount
    { 0x3E, 0x3E, 0x3E, 0x3E, 0x3E, 0x3E, 0x3E, 0x3E }, // playerName "        "
    0x30,                                               // healthCapacity
    0x30,                                               // health
    0,                                                  // magicLevel
    MAGIC_NORMAL_METER,                                 // magic
    0,                                                  // rupees
    0,                                                  // swordHealth
    0,                                                  // tatlTimer
    false,                                              // isMagicAcquired
    false,                                              // isDoubleMagicAcquired
    0,                                                  // doubleDefense
    0,                                                  // unk_1F
    0xFF,                                               // unk_20
    0x0000,                                             // owlActivationFlags
    0xFF,                                               // unk_24
    SCENE_SPOT00,                                       // savedSceneId
};

ItemEquips sSaveDefaultItemEquips = {
    {
        { ITEM_SWORD_KOKIRI, ITEM_NONE, ITEM_NONE, ITEM_NONE },
        { ITEM_SWORD_KOKIRI, ITEM_NONE, ITEM_NONE, ITEM_NONE },
        { ITEM_SWORD_KOKIRI, ITEM_NONE, ITEM_NONE, ITEM_NONE },
        { ITEM_FD, ITEM_NONE, ITEM_NONE, ITEM_NONE },
    },
    {
        { SLOT_OCARINA, SLOT_NONE, SLOT_NONE, SLOT_NONE },
        { SLOT_NONE, SLOT_NONE, SLOT_NONE, SLOT_NONE },
        { SLOT_NONE, SLOT_NONE, SLOT_NONE, SLOT_NONE },
        { SLOT_NONE, SLOT_NONE, SLOT_NONE, SLOT_NONE },
    },
    0x11,
};

Inventory sSaveDefaultInventory = {
    // items
    {
        ITEM_NONE, // SLOT_OCARINA
        ITEM_NONE, // SLOT_BOW
        ITEM_NONE, // SLOT_ARROW_FIRE
        ITEM_NONE, // SLOT_ARROW_ICE
        ITEM_NONE, // SLOT_ARROW_LIGHT
        ITEM_NONE, // SLOT_TRADE_DEED
        ITEM_NONE, // SLOT_BOMB
        ITEM_NONE, // SLOT_BOMBCHU
        ITEM_NONE, // SLOT_DEKU_STICK
        ITEM_NONE, // SLOT_DEKU_NUT
        ITEM_NONE, // SLOT_MAGIC_BEANS
        ITEM_NONE, // SLOT_TRADE_KEY_MAMA
        ITEM_NONE, // SLOT_POWDER_KEG
        ITEM_NONE, // SLOT_PICTOGRAPH_BOX
        ITEM_NONE, // SLOT_LENS_OF_TRUTH
        ITEM_NONE, // SLOT_HOOKSHOT
        ITEM_NONE, // SLOT_SWORD_GREAT_FAIRY
        ITEM_NONE, // SLOT_TRADE_COUPLE
        ITEM_NONE, // SLOT_BOTTLE_1
        ITEM_NONE, // SLOT_BOTTLE_2
        ITEM_NONE, // SLOT_BOTTLE_3
        ITEM_NONE, // SLOT_BOTTLE_4
        ITEM_NONE, // SLOT_BOTTLE_5
        ITEM_NONE, // SLOT_BOTTLE_6
        ITEM_NONE, // SLOT_MASK_POSTMAN
        ITEM_NONE, // SLOT_MASK_ALL_NIGHT
        ITEM_NONE, // SLOT_MASK_BLAST
        ITEM_NONE, // SLOT_MASK_STONE
        ITEM_NONE, // SLOT_MASK_GREAT_FAIRY
        ITEM_NONE, // SLOT_MASK_DEKU
        ITEM_NONE, // SLOT_MASK_KEATON
        ITEM_NONE, // SLOT_MASK_BREMEN
        ITEM_NONE, // SLOT_MASK_BUNNY
        ITEM_NONE, // SLOT_MASK_DON_GERO
        ITEM_NONE, // SLOT_MASK_SCENTS
        ITEM_NONE, // SLOT_MASK_GORON
        ITEM_NONE, // SLOT_MASK_ROMANI
        ITEM_NONE, // SLOT_MASK_CIRCUS_LEADER
        ITEM_NONE, // SLOT_MASK_KAFEIS_MASK
        ITEM_NONE, // SLOT_MASK_COUPLE
        ITEM_NONE, // SLOT_MASK_TRUTH
        ITEM_NONE, // SLOT_MASK_ZORA
        ITEM_NONE, // SLOT_MASK_KAMARO
        ITEM_NONE, // SLOT_MASK_GIBDO
        ITEM_NONE, // SLOT_MASK_GARO
        ITEM_NONE, // SLOT_MASK_CAPTAIN
        ITEM_NONE, // SLOT_MASK_GIANT
        ITEM_NONE, // SLOT_MASK_FIERCE_DEITY
    },
    // ammo
    {
        0, // SLOT_OCARINA
        0, // SLOT_BOW
        0, // SLOT_ARROW_FIRE
        0, // SLOT_ARROW_ICE
        0, // SLOT_ARROW_LIGHT
        0, // SLOT_TRADE_DEED
        0, // SLOT_BOMB
        0, // SLOT_BOMBCHU
        0, // SLOT_DEKU_STICK
        0, // SLOT_DEKU_NUT
        0, // SLOT_MAGIC_BEANS
        0, // SLOT_TRADE_KEY_MAMA
        0, // SLOT_POWDER_KEG
        0, // SLOT_PICTOGRAPH_BOX
        0, // SLOT_LENS_OF_TRUTH
        0, // SLOT_HOOKSHOT
        0, // SLOT_SWORD_GREAT_FAIRY
        0, // SLOT_TRADE_COUPLE
        0, // SLOT_BOTTLE_1
        0, // SLOT_BOTTLE_2
        0, // SLOT_BOTTLE_3
        0, // SLOT_BOTTLE_4
        0, // SLOT_BOTTLE_5
        0, // SLOT_BOTTLE_6
    },
    // upgrades
    (0 << 0) |      // UPG_QUIVER
        (0 << 3) |  // UPG_BOMB_BAG
        (0 << 6) |  // UPG_STRENGTH
        (0 << 9) |  // UPG_SCALE
        (0 << 12) | // UPG_WALLET
        (0 << 14) | // UPG_BULLET_BAG
        (1 << 17) | // UPG_DEKU_STICKS
        (1 << 20),  // UPG_DEKU_NUTS
    // questItems
    0,
    // dungeonItems
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    // dungeonKeys
    { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF },
    // defenseHearts
    0,
    // strayFairies
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    // dekuPlaygroundPlayerName
    {
        // "LINK    "
        { 0x15, 0x12, 0x17, 0x14, 0x3E, 0x3E, 0x3E, 0x3E },
        // "LINK    "
        { 0x15, 0x12, 0x17, 0x14, 0x3E, 0x3E, 0x3E, 0x3E },
        // "LINK    "
        { 0x15, 0x12, 0x17, 0x14, 0x3E, 0x3E, 0x3E, 0x3E },
    },
};

void Sram_ClearHighscores(void);
void Sram_GenerateRandomSaveFields(void);
void Sram_ResetSave(void);

void Sram_InitDebugSave(void) {
    Sram_ResetSave();

    Lib_MemCpy(&gSaveContext.save.saveInfo.playerData, &sSaveDefaultPlayerData, sizeof(SavePlayerData));
    Lib_MemCpy(&gSaveContext.save.saveInfo.equips, &sSaveDefaultItemEquips, sizeof(ItemEquips));
    Lib_MemCpy(&gSaveContext.save.saveInfo.inventory, &sSaveDefaultInventory, sizeof(Inventory));
    gSaveContext.save.saveInfo.checksum = 0;
    
    gSaveContext.save.playerForm = PLAYER_FORM_DEKU;

    gSaveContext.save.hasTatl = true;

    gSaveContext.save.saveInfo.horseData.sceneId = SCENE_F01;
    gSaveContext.save.saveInfo.horseData.pos.x = -1420;
    gSaveContext.save.saveInfo.horseData.pos.y = 257;
    gSaveContext.save.saveInfo.horseData.pos.z = -1285;
    gSaveContext.save.saveInfo.horseData.yaw = -0x7554;

    gSaveContext.save.entrance = ENTRANCE(CUTSCENE, 0);
    gSaveContext.save.isFirstCycle = true;

    SET_WEEKEVENTREG(WEEKEVENTREG_15_20);
    SET_WEEKEVENTREG(WEEKEVENTREG_59_04);
    SET_WEEKEVENTREG(WEEKEVENTREG_31_04);

    gSaveContext.nextCutsceneIndex = 0;

    gSaveContext.cycleSceneFlags[SCENE_INSIDETOWER].switch0 = 1;
    gSaveContext.save.saveInfo.permanentSceneFlags[SCENE_INSIDETOWER].switch0 = 1;
    gSaveContext.save.saveInfo.playerData.magicLevel = 0;

    Sram_GenerateRandomSaveFields();
}

void Sram_InitSave(FileSelectState* fileSelect2, SramContext* sramCtx) {
    s32 phi_v0;
    u16 i;
    FileSelectState* fileSelect = fileSelect2;
    s16 maskCount;

    if (gSaveContext.flashSaveAvailable) {
        Sram_InitDebugSave();
        if (fileSelect->buttonIndex == 0) {
            //gSaveContext.save.cutsceneIndex = 0xFFF0;
        }

        for (phi_v0 = 0; phi_v0 < ARRAY_COUNT(gSaveContext.save.saveInfo.playerData.playerName); phi_v0++) {
            gSaveContext.save.saveInfo.playerData.playerName[phi_v0] =
                fileSelect->fileNames[fileSelect->buttonIndex][phi_v0];
        }

        gSaveContext.save.saveInfo.playerData.newf[0] = 'Z';
        gSaveContext.save.saveInfo.playerData.newf[1] = 'E';
        gSaveContext.save.saveInfo.playerData.newf[2] = 'L';
        gSaveContext.save.saveInfo.playerData.newf[3] = 'D';
        gSaveContext.save.saveInfo.playerData.newf[4] = 'A';
        gSaveContext.save.saveInfo.playerData.newf[5] = '3';

        gSaveContext.save.saveInfo.checksum = Sram_CalcChecksum(&gSaveContext.save, sizeof(Save));

        Lib_MemCpy(sramCtx->saveBuf, &gSaveContext.save, sizeof(Save));
        Lib_MemCpy(&sramCtx->saveBuf[0x2000], &gSaveContext.save, sizeof(Save));

        for (i = 0; i < ARRAY_COUNT(gSaveContext.save.saveInfo.playerData.newf); i++) {
            fileSelect->newf[fileSelect->buttonIndex][i] = gSaveContext.save.saveInfo.playerData.newf[i];
        }

        fileSelect->threeDayResetCount[fileSelect->buttonIndex] =
            gSaveContext.save.saveInfo.playerData.threeDayResetCount;

        for (i = 0; i < ARRAY_COUNT(gSaveContext.save.saveInfo.playerData.playerName); i++) {
            fileSelect->fileNames[fileSelect->buttonIndex][i] = gSaveContext.save.saveInfo.playerData.playerName[i];
        }

        fileSelect->healthCapacity[fileSelect->buttonIndex] = gSaveContext.save.saveInfo.playerData.healthCapacity;
        fileSelect->health[fileSelect->buttonIndex] = gSaveContext.save.saveInfo.playerData.health;
        fileSelect->defenseHearts[fileSelect->buttonIndex] = gSaveContext.save.saveInfo.inventory.defenseHearts;
        fileSelect->questItems[fileSelect->buttonIndex] = gSaveContext.save.saveInfo.inventory.questItems;
        fileSelect->time[fileSelect->buttonIndex] = gSaveContext.save.time;
        fileSelect->day[fileSelect->buttonIndex] = gSaveContext.save.day;
        fileSelect->isOwlSave[fileSelect->buttonIndex] = gSaveContext.save.isOwlSave;
        fileSelect->rupees[fileSelect->buttonIndex] = gSaveContext.save.saveInfo.playerData.rupees;
        fileSelect->walletUpgrades[fileSelect->buttonIndex] = CUR_UPG_VALUE(UPG_WALLET);

        for (i = 0, maskCount = 0; i < 24; i++) {
            if (gSaveContext.save.saveInfo.inventory.items[i + 24] != ITEM_NONE) {
                maskCount++;
            }
        }

        fileSelect->maskCount[fileSelect->buttonIndex] = maskCount;
        fileSelect->heartPieceCount[fileSelect->buttonIndex] = GET_QUEST_HEART_PIECE_COUNT;
    }

    gSaveContext.save.time = *((uintptr_t*) 0x801F6AF0);
    gSaveContext.flashSaveAvailable = *((uintptr_t*) 0x801F6AF2);
}