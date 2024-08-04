#include "patches.h"
#include "play_patches.h"

void Message_OpenText(PlayState* play, u16 textId);

void Message_StartTextbox(PlayState* play, u16 textId, Actor* actor) {
    MessageContext* msgCtx = &play->msgCtx;
    msgCtx->ocarinaAction = 0xFFFF;
    Message_OpenText(play, textId);
    msgCtx->talkActor = actor;
    msgCtx->msgMode = MSGMODE_TEXT_START;
    msgCtx->stateTimer = 0;
    msgCtx->textDelayTimer = 0;
    play->msgCtx.ocarinaMode = OCARINA_MODE_NONE;
}

#undef SEGMENT_ROM_START
#define SEGMENT_ROM_START(segment) (_ ## segment ## SegmentRomStart)

extern u8 D_801C6A70;
extern s32 sCharTexSize;
extern s32 sCharTexScale;
extern s32 D_801F6B08;

static unsigned char ap_msg[128] = "You got an\x01 AP item\x00!\xbf";
static unsigned char sf_msg[128] = "You got a\x05 Stray Fairy\x00!\xbf";
static unsigned char sot_msg[128] = "You got the\x05 Song of Time\x00!\xbf";
static unsigned char soh_msg[128] = "You got the\x05 Song of Healing\x00!\xbf";
static unsigned char es_msg[128] = "You got \x05 Epona's Song\x00!\xbf";
static unsigned char sost_msg[128] = "You got the\x05 Song of Storms\x00!\xbf";
static unsigned char sos_msg[128] = "You got the\x05 Song of Soaring\x00!\xbf";
static unsigned char soa_msg[128] = "You got the\x05 Sonata of Awakening\x00!\xbf";
static unsigned char gl_msg[128] = "You got the\x05 Goron's Lullaby\x00!\xbf";
static unsigned char nwbn_msg[128] = "You got the\x05 New Wave Bossa Nova\x00!\xbf";
static unsigned char eoe_msg[128] = "You got the\x05 Elegy of Emptiness\x00!\xbf";
static unsigned char oto_msg[128] = "You got the\x05 Oath to Order\x00!\xbf";

void Message_FindMessage(PlayState* play, u16 textId);

void Message_OpenText(PlayState* play, u16 textId) {
    MessageContext* msgCtx = &play->msgCtx;
    Font* font = &msgCtx->font;
    Player* player = GET_PLAYER(play);
    f32 var_fv0;
    uintptr_t i;
    u32 ffcount = 0;
    unsigned char* msg = NULL;

    if (play->msgCtx.msgMode == MSGMODE_NONE) {
        gSaveContext.prevHudVisibility = gSaveContext.hudVisibility;
    }

    if (textId == 0xFF) {
        func_80115844(play, DO_ACTION_STOP);
        play->msgCtx.hudVisibility = gSaveContext.hudVisibility;
        Interface_SetHudVisibility(HUD_VISIBILITY_A_B_C);
        gSaveContext.save.unk_06 = 20;
    } else if ((textId == 0x579) || (textId == 0x8D8)) {
        Interface_SetHudVisibility(HUD_VISIBILITY_A_HEARTS_MAGIC_WITH_OVERWRITE);
    } else if (((textId == 0x28) || (textId == 0x29) || (textId == 0x2A)) &&
               (player->transformation == PLAYER_FORM_DEKU)) {
        //! FAKE:
        if (msgCtx) {}
        textId = 0xC9;
    } else if (textId == 0x11) {
        if (gSaveContext.save.saveInfo.inventory.strayFairies[((void)0, gSaveContext.dungeonIndex)] == 0xF) {
            textId = 0xF3;
        }
    } else if ((textId == 0x92) && (play->sceneId == SCENE_KOEPONARACE)) {
        textId = 0xCD;
    }

    if ((textId == 0xC) && (GET_QUEST_HEART_PIECE_COUNT != 0)) {
        textId = GET_QUEST_HEART_PIECE_COUNT;
        textId += 0xC4;
    }

    msgCtx->currentTextId = textId;

    if ((msgCtx->currentTextId == 0xC) || (msgCtx->currentTextId == 0xD) || (msgCtx->currentTextId == 0xC5) ||
        (msgCtx->currentTextId == 0xC6) || (msgCtx->currentTextId == 0xC7)) {
        Interface_SetHudVisibility(HUD_VISIBILITY_A_HEARTS_MAGIC_WITH_OVERWRITE);
    }

    msgCtx->messageHasSetSfx = false;
    D_801C6A70 = 0;
    msgCtx->textboxSkipped = false;
    msgCtx->textIsCredits = false;
    var_fv0 = 1.0f;

    if (play->pauseCtx.bombersNotebookOpen) {
        if (gSaveContext.options.language == LANGUAGE_JPN) {
            msgCtx->textCharScale = 1.4f;
            msgCtx->unk11FFC = 0x1E;
            msgCtx->unk11FF8 = 0x32;
            var_fv0 = 1.4;
        } else {
            msgCtx->textCharScale = 1.4f;
            msgCtx->unk11FFC = 0x16;
            msgCtx->unk11FF8 = 0x32;
            var_fv0 = 1.4;
        }
    } else if (textId >= 0x4E20) {
        msgCtx->textIsCredits = true;
        msgCtx->textCharScale = 0.85f;
        msgCtx->unk11FFC = 6;
        msgCtx->unk11FF8 = 0x14;
    } else if (gSaveContext.options.language == LANGUAGE_JPN) {
        msgCtx->textCharScale = 0.88f;
        msgCtx->unk11FFC = 0x12;
        msgCtx->unk11FF8 = 0x32;
    } else {
        msgCtx->textCharScale = 0.75f;
        msgCtx->unk11FFC = 0xC;
        msgCtx->unk11FF8 = 0x41;
    }

    sCharTexSize = msgCtx->textCharScale * 16.0f;
    sCharTexScale = 1024.0f / msgCtx->textCharScale;
    D_801F6B08 = 1024.0f / var_fv0;

    if (msgCtx->textIsCredits) {
        Message_FindCreditsMessage(play, textId);
        msgCtx->msgLength = font->messageEnd;
        DmaMgr_SendRequest0(&font->msgBuf, (uintptr_t) &SEGMENT_ROM_START(staff_message_data_static)[font->messageStart],
                            font->messageEnd);
    } else if (gSaveContext.options.language == LANGUAGE_JPN) {
        Message_FindMessage(play, textId);
        msgCtx->msgLength = font->messageEnd;
        DmaMgr_SendRequest0(&font->msgBuf, (uintptr_t) &SEGMENT_ROM_START(message_data_static)[font->messageStart],
                            font->messageEnd);
    } else {
        Message_FindMessageNES(play, textId);
        msgCtx->msgLength = font->messageEnd;
        DmaMgr_SendRequest0(&font->msgBuf, (uintptr_t) &SEGMENT_ROM_START(message_data_static)[font->messageStart],
                            font->messageEnd);
    }

    msgCtx->choiceNum = 0;
    msgCtx->textUnskippable = false;
    msgCtx->textboxEndType = TEXTBOX_ENDTYPE_00;
    msgCtx->textDrawPos = 0;
    msgCtx->msgBufPos = 0;
    msgCtx->decodedTextLen = 0;

    switch (textId) {
        case 0xA2:
            msg = sost_msg;
            break;
        case 0xA3:
            msg = sos_msg;
            break;
        case 0xA5:
            msg = es_msg;
            break;
        case 0xA7:
            msg = oto_msg;
            break;
        case 0xA8:
            msg = eoe_msg;
            break;
        case 0xAC:
            msg = nwbn_msg;
            break;
        case 0xAD:
            msg = gl_msg;
            break;
        case 0xAE:
            msg = soa_msg;
            break;
        case 0xAF:
            msg = soh_msg;
            break;
        case 0xB0:
            msg = sot_msg;
            break;
        case 0xB2:
            msg = sf_msg;
            break;
        case 0xB3:
            msg = ap_msg;
            break;
    }

    if (msg != NULL) {
        font->msgBuf.schar[0] = 0x02;
        font->msgBuf.schar[1] = 0x00;
        font->msgBuf.schar[2] = 0xFE;
        for (i = 0; i < 64; ++i) {
            font->msgBuf.schar[i + 11] = msg[i];
            if (msg[i] == 0xBF) {
                break;
            }
        }
    }

    msgCtx->unk11F08 = font->msgBuf.schar[msgCtx->msgBufPos] << 8;
    msgCtx->unk11F08 |= font->msgBuf.schar[msgCtx->msgBufPos + 1];

    msgCtx->unk11F18 = (msgCtx->unk11F08 & 0xF000) >> 0xC;
    msgCtx->textBoxType = (msgCtx->unk11F08 & 0xF00) >> 8;
    msgCtx->textBoxPos = (msgCtx->unk11F08 & 0xF0) >> 4;
    msgCtx->unk11F0C = msgCtx->unk11F08 & 0xF;

    if ((msgCtx->unk11F0C == 1) || (msgCtx->unk11F0C == 3)) {
        //msgCtx->textUnskippable = true;
    }
    msgCtx->itemId = 0xFE;

    if ((msgCtx->textBoxType == TEXTBOX_TYPE_5) || (msgCtx->textBoxType == TEXTBOX_TYPE_D) ||
        (play->pauseCtx.bombersNotebookOpen)) {
        msgCtx->unk120CE = msgCtx->unk120D0 = msgCtx->unk120D2 = 0;
    } else {
        msgCtx->unk120CE = msgCtx->unk120D0 = msgCtx->unk120D2 = 0xFF;
    }

    msgCtx->choiceIndex = 0;

    if (msgCtx->unk11F0C != 3) {
        msgCtx->textColorAlpha = 0xFF;
    } else {
        msgCtx->textColorAlpha = 0;
    }

    if (textId == 0x1467) {
        Message_BombersNotebookQueueEvent(play, BOMBERS_NOTEBOOK_EVENT_MET_KAFEI);
    }

    if (textId == 0x2955) {
        Message_BombersNotebookQueueEvent(play, BOMBERS_NOTEBOOK_EVENT_MET_ANJU);
        Message_BombersNotebookQueueEvent(play, BOMBERS_NOTEBOOK_EVENT_MET_KAFEI);
        Message_BombersNotebookQueueEvent(play, BOMBERS_NOTEBOOK_EVENT_RECEIVED_COUPLES_MASK);
    }
}