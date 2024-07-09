#include "patches.h"
#include "play_patches.h"
#include "z64save.h"
#include "z64horse.h"
#include "overlays/gamestates/ovl_file_choose/z_file_select.h"

#include "misc_funcs.h"
#include "apcommon.h"

extern SavePlayerData sSaveDefaultPlayerData;
extern ItemEquips sSaveDefaultItemEquips;
extern Inventory sSaveDefaultInventory;

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

    SET_WEEKEVENTREG(WEEKEVENTREG_ENTERED_EAST_CLOCK_TOWN);
    SET_WEEKEVENTREG(WEEKEVENTREG_ENTERED_WEST_CLOCK_TOWN);
    SET_WEEKEVENTREG(WEEKEVENTREG_ENTERED_NORTH_CLOCK_TOWN);

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