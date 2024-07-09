#include "patches.h"
#include "global.h"

#include "misc_funcs.h"
#include "apcommon.h"

void GetItem_DrawBombchu(PlayState* play, s16 drawId);
void GetItem_DrawPoes(PlayState* play, s16 drawId);
void GetItem_DrawFairyBottle(PlayState* play, s16 drawId);
void GetItem_DrawSkullToken(PlayState* play, s16 drawId);
void GetItem_DrawCompass(PlayState* play, s16 drawId);
void GetItem_DrawPotion(PlayState* play, s16 drawId);
void GetItem_DrawGoronSword(PlayState* play, s16 drawId);
void GetItem_DrawDekuNuts(PlayState* play, s16 drawId);
void GetItem_DrawRecoveryHeart(PlayState* play, s16 drawId);
void GetItem_DrawFish(PlayState* play, s16 drawId);
void GetItem_DrawOpa0(PlayState* play, s16 drawId);
void GetItem_DrawOpa0Xlu1(PlayState* play, s16 drawId);
void GetItem_DrawOpa01(PlayState* play, s16 drawId);
void GetItem_DrawXlu01(PlayState* play, s16 drawId);
void GetItem_DrawSeahorse(PlayState* play, s16 drawId);
void GetItem_DrawFairyContainer(PlayState* play, s16 drawId);
void GetItem_DrawMoonsTear(PlayState* play, s16 drawId);
void GetItem_DrawMagicArrow(PlayState* play, s16 drawId);
void GetItem_DrawUpgrades(PlayState* play, s16 drawId);
void GetItem_DrawRupee(PlayState* play, s16 drawId);
void GetItem_DrawSmallRupee(PlayState* play, s16 drawId);
void GetItem_DrawWallet(PlayState* play, s16 drawId);
void GetItem_DrawRemains(PlayState* play, s16 drawId);

typedef struct {
    /* 0x0 */ void (*drawFunc)(PlayState*, s16);
    /* 0x4 */ void* drawResources[8]; // Either display lists (Gfx*) or matrices (Mtx*)
} DrawItemTableEntry;                 // size = 0x24
/*
static DrawItemTableEntry sDrawItemTable[] = {
    // GID_BOTTLE, OBJECT_GI_BOTTLE
    { GetItem_DrawOpa0Xlu1, { gGiEmptyBottleCorkDL, gGiEmptyBottleGlassDL } },
    // GID_KEY_SMALL, OBJECT_GI_KEY
    { GetItem_DrawOpa0, { gGiSmallKeyDL } },
    // GID_MASK_KAMARO, OBJECT_GI_MASK17
    { GetItem_DrawOpa01, { gGiKamaroMaskDL, gGiKamaroMaskEmptyDL } },
    // GID_MASK_COUPLE, OBJECT_GI_MASK13
    { GetItem_DrawOpa0Xlu1, { gGiCouplesMaskFullDL, gGiCouplesMaskHalfDL } },
    // GID_04, OBJECT_GI_MELODY
    { GetItem_DrawXlu01, { gGiSerenadeColorDL, gGiSongNoteDL } },
    // GID_05, OBJECT_GI_MELODY
    { GetItem_DrawXlu01, { gGiRequiemColorDL, gGiSongNoteDL } },
    // GID_06, OBJECT_GI_MELODY
    { GetItem_DrawXlu01, { gGiNocturneColorDL, gGiSongNoteDL } },
    // GID_07, OBJECT_GI_MELODY
    { GetItem_DrawXlu01, { gGiPreludeColorDL, gGiSongNoteDL } },
    // GID_RECOVERY_HEART, OBJECT_GI_HEART
    { GetItem_DrawRecoveryHeart, { gGiRecoveryHeartDL } },
    // GID_KEY_BOSS, OBJECT_GI_KEY_BOSS
    { GetItem_DrawOpa0Xlu1, { gGiBossKeyDL, gGiBossKeyGemDL } },
    // GID_COMPASS, OBJECT_GI_COMPASS
    { GetItem_DrawCompass, { gGiCompassDL, gGiCompassGlassDL } },
    // GID_BOMBERS_NOTEBOOK, OBJECT_GI_SCHEDULE
    { GetItem_DrawOpa0Xlu1, { gGiBombersNotebookEmptyDL, gGiBombersNotebookDL } },
    // GID_MASK_STONE, OBJECT_GI_STONEMASK
    { GetItem_DrawOpa0Xlu1, { gGiStoneMaskEmptyDL, gGiStoneMaskDL } },
    // GID_MASK_KAFEIS_MASK, OBJECT_GI_MASK05
    { GetItem_DrawOpa01, { gGiKafeiMaskDL, gGiKafeiMaskEmptyDL } },
    // GID_MASK_CIRCUS_LEADER, OBJECT_GI_MASK11
    { GetItem_DrawOpa0Xlu1, { gGiCircusLeaderMaskFaceDL, gGiCircusLeaderMaskEyebrowsDL } },
    // GID_MASK_BREMEN, OBJECT_GI_MASK20
    { GetItem_DrawOpa01, { gGiBremenMaskDL, gGiBremenMaskEmptyDL } },
    // GID_MASK_ALL_NIGHT, OBJECT_GI_MASK06
    { GetItem_DrawOpa0Xlu1, { gGiAllNightMaskEyesDL, gGiAllNightMaskFaceDL } },
    // GID_DEKU_NUTS, OBJECT_GI_NUTS
    { GetItem_DrawDekuNuts, { gGiNutDL } },
    // GID_HEART_CONTAINER, OBJECT_GI_HEARTS
    { GetItem_DrawXlu01, { gGiHeartBorderDL, gGiHeartContainerDL } },
    // GID_HEART_PIECE, OBJECT_GI_HEARTS
    { GetItem_DrawXlu01, { gGiHeartBorderDL, gGiHeartPieceDL } },
    // GID_QUIVER_30, OBJECT_GI_ARROWCASE
    { GetItem_DrawUpgrades, { gGiQuiverInnerDL, gGiQuiver30InnerColorDL, gGiQuiver30OuterColorDL, gGiQuiverOuterDL } },
    // GID_QUIVER_40, OBJECT_GI_ARROWCASE
    { GetItem_DrawUpgrades, { gGiQuiverInnerDL, gGiQuiver40InnerColorDL, gGiQuiver40OuterColorDL, gGiQuiverOuterDL } },
    // GID_QUIVER_50, OBJECT_GI_ARROWCASE
    { GetItem_DrawUpgrades, { gGiQuiverInnerDL, gGiQuiver50InnerColorDL, gGiQuiver50OuterColorDL, gGiQuiverOuterDL } },
    // GID_BOMB_BAG_20, OBJECT_GI_BOMBPOUCH
    { GetItem_DrawUpgrades, { gGiBombBagDL, gGiBombBag20BagColorDL, gGiBombBag20RingColorDL, gGiBombBagRingDL } },
    // GID_BOMB_BAG_30, OBJECT_GI_BOMBPOUCH
    { GetItem_DrawUpgrades, { gGiBombBagDL, gGiBombBag30BagColorDL, gGiBombBag30RingColorDL, gGiBombBagRingDL } },
    // GID_BOMB_BAG_40, OBJECT_GI_BOMBPOUCH
    { GetItem_DrawUpgrades, { gGiBombBagDL, gGiBombBag40BagColorDL, gGiBombBag40RingColorDL, gGiBombBagRingDL } },
    // GID_DEKU_STICK, OBJECT_GI_STICK
    { GetItem_DrawOpa0, { gGiStickDL } },
    // GID_DUNGEON_MAP, OBJECT_GI_MAP
    { GetItem_DrawOpa0, { gGiDungeonMapDL } },
    // GID_MAGIC_JAR_SMALL, OBJECT_GI_MAGICPOT
    { GetItem_DrawOpa0, { gGiMagicJarSmallDL } },
    // GID_MAGIC_JAR_BIG, OBJECT_GI_MAGICPOT
    { GetItem_DrawOpa0, { gGiMagicJarLargeDL } },
    // GID_BOMB, OBJECT_GI_BOMB_1
    { GetItem_DrawOpa0, { gGiBombDL } },
    // GID_STONE_OF_AGONY, OBJECT_GI_MAP
    { GetItem_DrawOpa0, { gGiStoneOfAgonyDL } },
    // GID_WALLET_ADULT, OBJECT_GI_PURSE
    { GetItem_DrawWallet,
      { gGiWalletDL, gGiAdultWalletColorDL, gGiAdultWalletRupeeOuterColorDL, gGiWalletRupeeOuterDL,
        gGiAdultWalletStringColorDL, gGiWalletStringDL, gGiAdultWalletRupeeInnerColorDL, gGiWalletRupeeInnerDL } },
    // GID_WALLET_GIANT, OBJECT_GI_PURSE
    { GetItem_DrawWallet,
      { gGiWalletDL, gGiGiantsWalletColorDL, gGiGiantsWalletRupeeOuterColorDL, gGiWalletRupeeOuterDL,
        gGiGiantsWalletStringColorDL, gGiWalletStringDL, gGiGiantsWalletRupeeInnerColorDL, gGiWalletRupeeInnerDL } },
    // GID_MASK_DON_GERO, OBJECT_GI_MASK16
    { GetItem_DrawOpa0Xlu1, { gGiDonGeroMaskFaceDL, gGiDonGeroMaskBodyDL } },
    // GID_ARROWS_SMALL, OBJECT_GI_ARROW
    { GetItem_DrawOpa0, { gGiArrowSmallDL } },
    // GID_ARROWS_MEDIUM, OBJECT_GI_ARROW
    { GetItem_DrawOpa0, { gGiArrowMediumDL } },
    // GID_ARROWS_LARGE, OBJECT_GI_ARROW
    { GetItem_DrawOpa0, { gGiArrowLargeDL } },
    // GID_BOMBCHU, OBJECT_GI_BOMB_2
    { GetItem_DrawBombchu, { gGiBombchuDL } },
    // GID_SHIELD_HERO, OBJECT_GI_SHIELD_2
    { GetItem_DrawOpa0Xlu1, { gGiHerosShieldEmblemDL, gGiHerosShieldDL } },
    // GID_HOOKSHOT, OBJECT_GI_HOOKSHOT
    { GetItem_DrawOpa01, { gGiHookshotEmptyDL, gGiHookshotDL } },
    // GID_29, OBJECT_GI_HOOKSHOT
    { GetItem_DrawOpa01, { gGiHookshotEmptyDL, gGiHookshotDL } },
    // GID_OCARINA, OBJECT_GI_OCARINA
    { GetItem_DrawOpa0Xlu1, { gGiOcarinaOfTimeDL, gGiOcarinaOfTimeHolesDL } },
    // GID_MILK, OBJECT_GI_MILK
    { GetItem_DrawOpa0Xlu1, { gGiMilkBottleContentsDL, gGiMilkBottleGlassDL } },
    // GID_MASK_KEATON, OBJECT_GI_KI_TAN_MASK
    { GetItem_DrawOpa0Xlu1, { gGiKeatonMaskDL, gGiKeatonMaskEyesDL } },
    // GID_TINGLE_MAP, OBJECT_GI_FIELDMAP
    { GetItem_DrawOpa01, { gGiTingleMapDL, gGiTingleMapEmptyDL } },
    // GID_BOW, OBJECT_GI_BOW
    { GetItem_DrawOpa01, { gGiBowHandleDL, gGiBowStringDL } },
    // GID_LENS, OBJECT_GI_GLASSES
    { GetItem_DrawOpa0Xlu1, { gGiLensDL, gGiLensGlassDL } },
    // GID_POTION_GREEN, OBJECT_GI_LIQUID
    { GetItem_DrawPotion,
      { gGiPotionContainerPotDL, gGiPotionContainerGreenPotColorDL, gGiPotionContainerGreenLiquidColorDL,
        gGiPotionContainerLiquidDL, gGiPotionContainerGreenPatternColorDL, gGiPotionContainerPatternDL } },
    // GID_POTION_RED, OBJECT_GI_LIQUID
    { GetItem_DrawPotion,
      { gGiPotionContainerPotDL, gGiPotionContainerRedPotColorDL, gGiPotionContainerRedLiquidColorDL,
        gGiPotionContainerLiquidDL, gGiPotionContainerRedPatternColorDL, gGiPotionContainerPatternDL } },
    // GID_POTION_BLUE, OBJECT_GI_LIQUID
    { GetItem_DrawPotion,
      { gGiPotionContainerPotDL, gGiPotionContainerBluePotColorDL, gGiPotionContainerBlueLiquidColorDL,
        gGiPotionContainerLiquidDL, gGiPotionContainerBluePatternColorDL, gGiPotionContainerPatternDL } },
    // GID_SHIELD_MIRROR, OBJECT_GI_SHIELD_3
    { GetItem_DrawOpa0Xlu1, { gGiMirrorShieldEmptyDL, gGiMirrorShieldDL } },
    // GID_MAGIC_BEANS, OBJECT_GI_BEAN
    { GetItem_DrawOpa0, { gGiBeanDL } },
    // GID_FISH, OBJECT_GI_FISH
    { GetItem_DrawFish, { gGiFishContainerDL } },
    // GID_LETTER_MAMA, OBJECT_GI_RESERVE_B_01
    { GetItem_DrawOpa0Xlu1, { gGiLetterToMamaEnvelopeLetterDL, gGiLetterToMamaInscriptionsDL } },
    // GID_37
    { GetItem_DrawOpa0, { NULL } },
    // GID_SWORD_BGS, OBJECT_GI_LONGSWORD
    { GetItem_DrawGoronSword, { gGiBiggoronSwordDL } },
    // GID_MASK_SUN, OBJECT_GI_MSSA
    { GetItem_DrawOpa0Xlu1, { gGiSunMaskFaceDL, gGiSunMaskEyesSidePatternsDL } },
    // GID_MASK_BLAST, OBJECT_GI_MASK21
    { GetItem_DrawOpa0Xlu1, { gGiBlastMaskEmptyDL, gGiBlastMaskDL } },
    // GID_FAIRY, OBJECT_GI_BOTTLE_04
    { GetItem_DrawFairyContainer,
      { gGiFairyBottleEmptyDL, gGiFairyBottleGlassCorkDL, gGiFairyBottleContentsDL, &gGiFairyBottleBillboardRotMtx } },
    // GID_MASK_SCENTS, OBJECT_GI_MASK22
    { GetItem_DrawOpa01, { gGiMaskOfScentsFaceDL, gGiMaskOfScentsTeethDL } },
    // GID_MASK_CAPTAIN, OBJECT_GI_MASK18
    { GetItem_DrawOpa01, { gGiCaptainsHatBodyDL, gGiCaptainsHatFaceDL } },
    // GID_MASK_BUNNY, OBJECT_GI_RABIT_MASK
    { GetItem_DrawOpa0Xlu1, { gGiBunnyHoodDL, gGiBunnyHoodEyesDL } },
    // GID_MASK_TRUTH, OBJECT_GI_TRUTH_MASK
    { GetItem_DrawOpa0Xlu1, { gGiMaskOfTruthDL, gGiMaskOfTruthAccentsDL } },
    // GID_DEED_SWAMP, OBJECT_GI_RESERVE01
    { GetItem_DrawOpa01, { gGiTitleDeedEmptyDL, gGiTitleDeedSwampColorDL } },
    // GID_DEED_MOUNTAIN, OBJECT_GI_RESERVE01
    { GetItem_DrawOpa01, { gGiTitleDeedEmptyDL, gGiTitleDeedMountainColorDL } },
    // GID_RUPEE_HUGE, OBJECT_GI_RUPY
    { GetItem_DrawRupee, { gGiRupeeInnerDL, gGiGoldRupeeInnerColorDL, gGiRupeeOuterDL, gGiGoldRupeeOuterColorDL } },
    // GID_DEED_OCEAN, OBJECT_GI_RESERVE01
    { GetItem_DrawOpa01, { gGiTitleDeedEmptyDL, gGiTitleDeedOceanColorDL } },
    // GID_MASK_GORON, OBJECT_GI_GOLONMASK
    { GetItem_DrawOpa0Xlu1, { gGiGoronMaskEmptyDL, gGiGoronMaskDL } },
    // GID_MASK_ZORA, OBJECT_GI_ZORAMASK
    { GetItem_DrawOpa01, { gGiZoraMaskEmptyDL, gGiZoraMaskDL } },
    // GID_46
    { GetItem_DrawBombchu, { NULL } },
    // GID_ARROW_FIRE, OBJECT_GI_M_ARROW
    { GetItem_DrawMagicArrow, { gGiMagicArrowAmmoDL, gGiMagicArrowFireColorDL, gGiMagicArrowGlowDL } },
    // GID_ARROW_ICE, OBJECT_GI_M_ARROW
    { GetItem_DrawMagicArrow, { gGiMagicArrowAmmoDL, gGiMagicArrowIceColorDL, gGiMagicArrowGlowDL } },
    // GID_ARROW_LIGHT, OBJECT_GI_M_ARROW
    { GetItem_DrawMagicArrow, { gGiMagicArrowAmmoDL, gGiMagicArrowLightColorDL, gGiMagicArrowGlowDL } },
    // GID_SKULL_TOKEN, OBJECT_GI_SUTARU
    { GetItem_DrawSkullToken, { gGiSkulltulaTokenDL, gGiSkulltulaTokenFlameDL } },
    // GID_BUG, OBJECT_GI_INSECT
    { GetItem_DrawOpa0Xlu1, { gGiBugContainerContentsDL, gGiBugContainerGlassDL } },
    // GID_4C
    { GetItem_DrawOpa0Xlu1, { NULL } },
    // GID_POE, OBJECT_GI_GHOST
    { GetItem_DrawPoes,
      { gGiPoeContainerLidDL, gGiPoeContainerGlassDL, gGiPoeContainerContentsDL, gGiPoeContainerPoeColorDL } },
    // GID_FAIRY_2, OBJECT_GI_SOUL
    { GetItem_DrawFairyBottle, { gGiFairyContainerBaseCapDL, gGiFairyContainerGlassDL, gGiFairyContainerContentsDL } },
    // GID_RUPEE_GREEN, OBJECT_GI_RUPY
    { GetItem_DrawSmallRupee,
      { gGiRupeeInnerDL, gGiGreenRupeeInnerColorDL, gGiRupeeOuterDL, gGiGreenRupeeOuterColorDL } },
    // GID_RUPEE_BLUE, OBJECT_GI_RUPY
    { GetItem_DrawSmallRupee,
      { gGiRupeeInnerDL, gGiBlueRupeeInnerColorDL, gGiRupeeOuterDL, gGiBlueRupeeOuterColorDL } },
    // GID_RUPEE_RED, OBJECT_GI_RUPY
    { GetItem_DrawSmallRupee, { gGiRupeeInnerDL, gGiRedRupeeInnerColorDL, gGiRupeeOuterDL, gGiRedRupeeOuterColorDL } },
    // GID_BIG_POE, OBJECT_GI_GHOST
    { GetItem_DrawPoes,
      { gGiPoeContainerLidDL, gGiPoeContainerGlassDL, gGiPoeContainerContentsDL, gGiPoeContainerBigPoeColorDL } },
    // GID_RUPEE_PURPLE, OBJECT_GI_RUPY
    { GetItem_DrawRupee, { gGiRupeeInnerDL, gGiPurpleRupeeInnerColorDL, gGiRupeeOuterDL, gGiPurpleRupeeOuterColorDL } },
    // GID_RUPEE_SILVER, OBJECT_GI_RUPY
    { GetItem_DrawRupee, { gGiRupeeInnerDL, gGiSilverRupeeInnerColorDL, gGiRupeeOuterDL, gGiSilverRupeeOuterColorDL } },
    // GID_SWORD_KOKIRI, OBJECT_GI_SWORD_1
    { GetItem_DrawOpa01, { gGiKokiriSwordBladeHiltDL, gGiKokiriSwordGuardDL } },
    // GID_SKULL_TOKEN_2, OBJECT_ST
    { GetItem_DrawSkullToken, { gSkulltulaTokenDL, gSkulltulaTokenFlameDL } },
    // GID_57, OBJECT_GI_BOTTLE_RED
    { GetItem_DrawOpa0Xlu1, { gGiRedPotionBottleEmptyDL, gGiRedPotionBottleDL } },
    // GID_ZORA_EGG, OBJECT_GI_BOTTLE_15
    { GetItem_DrawOpa0Xlu1, { gGiZoraEggBottleContentsDL, gGiZoraEggBottleGlassAndCorkDL } },
    // GID_MOONS_TEAR, OBJECT_GI_RESERVE00
    { GetItem_DrawMoonsTear, { gGiMoonsTearItemDL, gGiMoonsTearGlowDL } },
    // GID_DEED_LAND, OBJECT_GI_RESERVE01
    { GetItem_DrawOpa01, { gGiTitleDeedEmptyDL, gGiTitleDeedLandColorDL } },
    // GID_MASK_DEKU, OBJECT_GI_NUTSMASK
    { GetItem_DrawOpa0Xlu1, { gGiDekuMaskEmptyDL, gGiDekuMaskDL } },
    // GID_REMAINS_ODOLWA, OBJECT_BSMASK
    { GetItem_DrawRemains, { gRemainsOdolwaDL, gRemainsOdolwaDL } },
    // GID_POWDER_KEG, OBJECT_GI_BIGBOMB
    { GetItem_DrawOpa0Xlu1, { gGiPowderKegBarrelDL, gGiPowderKegGoronSkullAndFuseDL } },
    // GID_GOLD_DUST, OBJECT_GI_GOLD_DUST
    { GetItem_DrawOpa01, { gGiGoldDustPowderDL, gGiGoldDustPowderEmptyDL } },
    // GID_SEAHORSE, OBJECT_GI_BOTTLE_16
    { GetItem_DrawOpa0Xlu1, { gGiSeahorseBottleEmptyDL, gGiSeahorseBottleGlassAndCorkDL } },
    // GID_60, OBJECT_GI_BOTTLE_22
    { GetItem_DrawOpa0Xlu1, { gGiHylianLoachBottleContentsDL, gGiHylianLoachBottleGlassCorkWaterDL } },
    // GID_HYLIAN_LOACH, OBJECT_GI_LOACH
    { GetItem_DrawOpa0Xlu1, { gGiHylianLoachEmptyDL, gGiHylianLoachDL } },
    // GID_SEAHORSE_CAUGHT, OBJECT_GI_SEAHORSE
    { GetItem_DrawSeahorse, { gGiSeahorseEmptyDL, gGiSeahorseBodyDL, gGiSeahorseGlowDL, &gGiSeahorseBillboardRotMtx } },
    // GID_REMAINS_GOHT, OBJECT_BSMASK
    { GetItem_DrawRemains, { gRemainsGohtDL, gRemainsGohtDL } },
    // GID_REMAINS_GYORG, OBJECT_BSMASK
    { GetItem_DrawRemains, { gRemainsGyorgDL, gRemainsGyorgDL } },
    // GID_REMAINS_TWINMOLD, OBJECT_BSMASK
    { GetItem_DrawRemains, { gRemainsTwinmoldDL, gRemainsTwinmoldDL } },
    // GID_SWORD_RAZOR, OBJECT_GI_SWORD_2
    { GetItem_DrawOpa01, { gGiRazorSwordDL, gGiRazorSwordEmptyDL } },
    // GID_SWORD_GILDED, OBJECT_GI_SWORD_3
    { GetItem_DrawOpa01, { gGiGildedSwordDL, gGiGildedSwordEmptyDL } },
    // GID_SWORD_GREAT_FAIRY, OBJECT_GI_SWORD_4
    { GetItem_DrawOpa0Xlu1, { gGiGreatFairysSwordBladeDL, gGiGreatFairysSwordHiltEmblemDL } },
    // GID_MASK_GARO, OBJECT_GI_MASK09
    { GetItem_DrawOpa0Xlu1, { gGiGarosMaskCloakDL, gGiGarosMaskFaceDL } },
    // GID_MASK_GREAT_FAIRY, OBJECT_GI_MASK14
    { GetItem_DrawOpa0Xlu1, { gGiGreatFairyMaskFaceDL, gGiGreatFairyMaskLeavesDL } },
    // GID_MASK_GIBDO, OBJECT_GI_MASK15
    { GetItem_DrawOpa0Xlu1, { gGiGibdoMaskEmptyDL, gGiGibdoMaskDL } },
    // GID_ROOM_KEY, OBJECT_GI_RESERVE_B_00
    { GetItem_DrawOpa0Xlu1, { gGiRoomKeyEmptyDL, gGiRoomKeyDL } },
    // GID_LETTER_TO_KAFEI, OBJECT_GI_RESERVE_C_00
    { GetItem_DrawOpa0Xlu1, { gGiLetterToKafeiEnvelopeLetterDL, gGiLetterToKafeiInscriptionsDL } },
    // GID_PENDANT_OF_MEMORIES, OBJECT_GI_RESERVE_C_01
    { GetItem_DrawOpa0Xlu1, { gGiPendantOfMemoriesEmptyDL, gGiPendantOfMemoriesDL } },
    // GID_MUSHROOM, OBJECT_GI_MAGICMUSHROOM
    { GetItem_DrawOpa0Xlu1, { gGiMagicMushroomEmptyDL, gGiMagicMushroomDL } },
    // GID_MASK_ROMANI, OBJECT_GI_MASK10
    { GetItem_DrawOpa0Xlu1, { gGiRomaniMaskCapDL, gGiRomaniMaskNoseEyeDL } },
    // GID_MASK_POSTMAN, OBJECT_GI_MASK12
    { GetItem_DrawOpa0Xlu1, { gGiPostmanHatCapDL, gGiPostmanHatBunnyLogoDL } },
    // GID_MASK_GIANT, OBJECT_GI_MASK23
    { GetItem_DrawOpa01, { gGiGiantMaskEmptyDL, gGiGiantMaskDL } },
    // GID_CHATEAU, OBJECT_GI_BOTTLE_21
    { GetItem_DrawOpa0Xlu1, { gGiChateauRomaniBottleEmptyDL, gGiChateauRomaniBottleDL } },
    // GID_PICTOGRAPH_BOX, OBJECT_GI_CAMERA
    { GetItem_DrawOpa0Xlu1, { gGiPictoBoxFrameDL, gGiPictoBoxBodyAndLensDL } },
    // GID_MASK_FIERCE_DEITY, OBJECT_GI_MASK03
    { GetItem_DrawOpa01, { gGiFierceDeityMaskFaceDL, gGiFierceDeityMaskHairAndHatDL } },
};*/

typedef enum {
    OPA0,
    XLU0,
    OPA01,
    OPA0XLU1,
    XLU01
} ObjectType;

ObjectType sGetObjectType[] = {
    OPA0XLU1,
    OPA0,
    OPA01,
    OPA0XLU1,
    XLU01,
    XLU01,
    XLU01,
    XLU01,
    XLU0,
    OPA0XLU1,
    OPA0XLU1,
    OPA0XLU1,
    OPA0XLU1,
    OPA01,
    OPA0XLU1,
    OPA01,
    OPA0XLU1,
    OPA0,
    XLU01,
    XLU0,
    OPA01,  // ?
    OPA01,  // ?
    OPA01,  // ?
    OPA01,  // ?
    OPA01,  // ?
    OPA01,  // ?
    OPA0,
    OPA0,
    OPA0,
    OPA0,
    OPA0,
    OPA0,
    OPA01,  // ?
    OPA01,  // ?
    OPA0XLU1,
    OPA0,
    OPA0,
    OPA0,
    OPA0,
    OPA0XLU1,
    OPA01,
    OPA01,
    OPA0XLU1,
    OPA0XLU1,
    OPA0XLU1,
    OPA01,
    OPA01,
    OPA0XLU1,
    OPA0XLU1,  // ?
    OPA0XLU1,  // ?
    OPA0XLU1,  // ?
    OPA0XLU1,
    OPA0,
    XLU0,
    OPA0XLU1,
    OPA0,
    OPA0,
    OPA0XLU1,
    OPA0XLU1,
    OPA0XLU1,  // ?
    OPA01,
    OPA01,
    OPA0XLU1,
    OPA0XLU1,
    OPA01,
    OPA01,
    OPA0XLU1,
    OPA01,
    OPA0XLU1,
    OPA01,
    OPA0,
    OPA0XLU1,
    OPA0XLU1,
    OPA0XLU1,
    OPA0XLU1,
    OPA0XLU1,
    OPA0XLU1,
    OPA0XLU1,
    OPA0XLU1,
    OPA0XLU1,
    OPA0XLU1,
    OPA0XLU1,
    OPA0XLU1,
    OPA0XLU1,
    OPA0XLU1,
    OPA01,
    OPA0XLU1,
    OPA0XLU1,
    OPA0XLU1,
    OPA0XLU1,
    OPA01,
    OPA0XLU1,
    OPA0,
    OPA0XLU1,
    OPA01,
    OPA0XLU1,
    OPA0XLU1,
    OPA0XLU1,
    OPA0XLU1,
    OPA0,
    OPA0,
    OPA0,
    OPA01,
    OPA01,
    OPA0XLU1,
    OPA0XLU1,
    OPA0XLU1,
    OPA0XLU1,
    OPA0XLU1,
    OPA0XLU1,
    OPA0XLU1,
    OPA0XLU1,
    OPA0XLU1,
    OPA0XLU1,
    OPA01,
    OPA0XLU1,
    OPA0XLU1,
    OPA01
};

static DmaRequest objectDmaRequest;
static OSMesg objectLoadMsg;

bool loadObject(PlayState* play, void* objectSegment, OSMesgQueue* objectLoadQueue, s16 objectId) {
    s32 objectSlot = Object_GetSlot(&play->objectCtx, objectId);

    if (Object_IsLoaded(&play->objectCtx, objectSlot)) {
        return false;
    }

    if (objectId != OBJECT_UNSET_0) {
        osCreateMesgQueue(objectLoadQueue, &objectLoadMsg, 1);
        DmaMgr_SendRequestImpl(&objectDmaRequest, objectSegment, gObjectTable[objectId].vromStart,
                               gObjectTable[objectId].vromEnd - gObjectTable[objectId].vromStart, 0,
                               objectLoadQueue, NULL);
    }

    return true;
}

/**
 * Draw "Get Item" Model
 * Calls the corresponding draw function for the given draw ID
 */
/*void GetItem_Draw(PlayState* play, s16 drawId) {
    sDrawItemTable[drawId].drawFunc(play, drawId);
}*/

void GetItem_DrawDynamic(PlayState* play, void* objectSegment, s16 drawId) {
    OPEN_DISPS(play->state.gfxCtx);

    u32 prevSegment = gSegments[6];
    gSegments[6] = OS_K0_TO_PHYSICAL(objectSegment);

    switch (sGetObjectType[drawId]) {
        case OPA0:
            gSPSegment(POLY_OPA_DISP++, 0x06, objectSegment);
            break;
        case XLU0:
            gSPSegment(POLY_XLU_DISP++, 0x06, objectSegment);
            break;
        case OPA01:
            gSPSegment(POLY_OPA_DISP++, 0x06, objectSegment);
            gSPSegment(POLY_OPA_DISP++, 0x06, objectSegment);
            break;
        case XLU01:
            gSPSegment(POLY_XLU_DISP++, 0x06, objectSegment);
            gSPSegment(POLY_XLU_DISP++, 0x06, objectSegment);
            break;
        case OPA0XLU1:
            gSPSegment(POLY_OPA_DISP++, 0x06, objectSegment);
            gSPSegment(POLY_XLU_DISP++, 0x06, objectSegment);
            break;
    }

    GetItem_Draw(play, drawId);

    gSegments[6] = prevSegment;

    CLOSE_DISPS(play->state.gfxCtx);
}