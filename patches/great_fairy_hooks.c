#include "patches.h"
#include "ultra64.h"

#include "apcommon.h"
#include "misc_funcs.h"

struct EnDyExtra;

typedef void (*EnDyExtraActionFunc)(struct EnDyExtra*, PlayState*);

typedef struct EnDyExtra {
    /* 0x000 */ Actor actor;
    /* 0x144 */ EnDyExtraActionFunc actionFunc;
    /* 0x148 */ s16 type;
    /* 0x14A */ s16 trigger;
    /* 0x14C */ s16 timer;
    /* 0x150 */ f32 alphaScale;
    /* 0x154 */ Vec3f scale; // not used, leftover from OoT
    /* 0x160 */ Vec3f initPos; // set and not used
} EnDyExtra; // size = 0x16C

struct BgDyYoseizo;

typedef void (*BgDyYoseizoActionFunc)(struct BgDyYoseizo*, PlayState*);

#define GREAT_FAIRY_GET_SWITCHFLAG(thisx) (((thisx)->params & 0xFE00) >> 9)
#define GREAT_FAIRY_GET_TYPE(thisx) ((thisx)->params & 0xF)
#define GREAT_FAIRY_ROTZ(thisx) ((thisx)->home.rot.z)

#define GREAT_FAIRY_LIMB_MAX 0x1C

typedef enum GreatFairyType {
    /* 0 */ GREAT_FAIRY_TYPE_MAGIC,
    /* 1 */ GREAT_FAIRY_TYPE_POWER,
    /* 2 */ GREAT_FAIRY_TYPE_WISDOM,
    /* 3 */ GREAT_FAIRY_TYPE_COURAGE,
    /* 4 */ GREAT_FAIRY_TYPE_KINDNESS
} GreatFairyType;

#define BG_DY_YOSEIZO_EFFECT_COUNT 200

typedef struct BgDyYoseizoEffect {
    /* 0x00 */ u8 alive; // drawn if 1, respawn if 0
    /* 0x04 */ Vec3f pos;
    /* 0x10 */ Vec3f velocity;
    /* 0x1C */ Vec3f accel;
    /* 0x28 */ Color_RGB8 primColor;
    /* 0x2B */ Color_RGB8 envColor;
    /* 0x2E */ s16 alpha;
    /* 0x30 */ f32 scale;
    /* 0x34 */ s16 timer; // lifetime
    /* 0x36 */ s16 type; // 0 is general radiance, else is directed towards Player
    /* 0x38 */ s16 pitch;
    /* 0x3A */ s16 yaw;
    /* 0x3C */ s16 roll; // all three are f32 in OoT
} BgDyYoseizoEffect; // size = 0x40

typedef struct BgDyYoseizo {
    /* 0x0000 */ Actor actor;
    /* 0x0144 */ BgDyYoseizoActionFunc actionFunc;
    /* 0x0148 */ SkelAnime skelAnime;
    /* 0x018C */ Vec3s jointTable[GREAT_FAIRY_LIMB_MAX];
    /* 0x0234 */ Vec3s morphTable[GREAT_FAIRY_LIMB_MAX];
    /* 0x02DC */ Vec3s headRot;
    /* 0x02DC */ Vec3s torsoRot;
    /* 0x02E8 */ EnDyExtra* beam;
    /* 0x02EC */ f32 unk2EC; // unused
    /* 0x02F0 */ f32 unk2F0;
    /* 0x02F4 */ f32 unk2F4;
    /* 0x02F8 */ union {
        s16 unk2F8;
        s16 csId; // used on BgDyYoseizo_TrainPlayer
    };
    /* 0x02FA */ s16 eyeIndex;
    /* 0x02FC */ s16 mouthIndex;
    /* 0x02FE */ s16 blinkTimer;
    /* 0x0300 */ s16 timer;
    /* 0x0302 */ u16 unk302;
    /* 0x0304 */ BgDyYoseizoEffect effects[BG_DY_YOSEIZO_EFFECT_COUNT];
} BgDyYoseizo; // size = 0x3504

void BgDyYoseizo_Bob(BgDyYoseizo* this, PlayState* play);
s32 SkelAnime_Update(SkelAnime* skelAnime);
extern void func_80A0B290(BgDyYoseizo* this, PlayState* play);

void func_80A0B35C(BgDyYoseizo* this, PlayState* play) {
    BgDyYoseizo_Bob(this, play);
    SkelAnime_Update(&this->skelAnime);

    if (this->timer == 60) {
        if (!Flags_GetSwitch(play, GREAT_FAIRY_GET_SWITCHFLAG(&this->actor))) {
            switch (GREAT_FAIRY_GET_TYPE(&this->actor)) {
                case GREAT_FAIRY_TYPE_MAGIC:
                    /*if (gSaveContext.save.saveInfo.playerData.isMagicAcquired != true) {
                        gSaveContext.save.saveInfo.playerData.isMagicAcquired = true;
                        gSaveContext.magicFillTarget = MAGIC_NORMAL_METER;
                    }*/
                    recomp_send_location(0x00FC00);
                    break;

                case GREAT_FAIRY_TYPE_WISDOM:
                    if (gSaveContext.save.saveInfo.playerData.isDoubleMagicAcquired != true) {
                        gSaveContext.save.saveInfo.playerData.isDoubleMagicAcquired = true;
                        gSaveContext.magicFillTarget = MAGIC_DOUBLE_METER;
                        gSaveContext.save.saveInfo.playerData.magicLevel = 0;
                    }
                    break;

                case GREAT_FAIRY_TYPE_COURAGE:
                    if (gSaveContext.save.saveInfo.playerData.doubleDefense != true) {
                        gSaveContext.save.saveInfo.playerData.doubleDefense = true;
                    }
                    break;

                default:
                    break;
            }
        }
        Interface_SetHudVisibility(9);
    }

    if ((this->timer < 50) && (GREAT_FAIRY_GET_TYPE(&this->actor) == GREAT_FAIRY_TYPE_COURAGE)) {
        if (gSaveContext.save.saveInfo.inventory.defenseHearts < 20) {
            gSaveContext.save.saveInfo.inventory.defenseHearts++;
        }
    }

    if (this->timer == 50) {
        gSaveContext.healthAccumulator = 0x140;
        Magic_Add(play, MAGIC_FILL_TO_CAPACITY);
    }

    if (this->timer == 0) {
        this->beam->trigger = true;
        //this->actionFunc = func_80A0B290;
        this->actionFunc = (BgDyYoseizoActionFunc) actor_relocate(&this->actor, func_80A0B290);
    }
}