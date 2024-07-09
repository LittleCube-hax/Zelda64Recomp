#include "patches.h"
#include "play_patches.h"

void Message_OpenText(PlayState* play, u16 textId);

void Message_StartTextbox(PlayState* play, u16 textId, Actor* actor) {
    MessageContext* msgCtx = &play->msgCtx;

    msgCtx->ocarinaAction = 0xFFFF;

    switch (textId) {
        case 0xB3:
            textId = 0x20D4;
            break;
        case 0xB0:
            textId = 0x20D9;
            break;
    }

    Message_OpenText(play, textId);
    msgCtx->talkActor = actor;
    msgCtx->msgMode = MSGMODE_TEXT_START;
    msgCtx->stateTimer = 0;
    msgCtx->textDelayTimer = 0;
    play->msgCtx.ocarinaMode = OCARINA_MODE_NONE;
}