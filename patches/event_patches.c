#include "patches.h"

u8 func_800FE5D0(struct PlayState* play);

void Environment_UpdateTimeBasedSequence(PlayState* play) {
    s32 pad;

    //! FAKE:
    if (gSaveContext.sceneLayer) {}

    if ((play->csCtx.state == CS_STATE_IDLE) && !(play->actorCtx.flags & ACTORCTX_FLAG_TELESCOPE_ON)) {
        switch (play->envCtx.timeSeqState) {
            case TIMESEQ_DAY_BGM:
                break;

            case TIMESEQ_FADE_DAY_BGM:
                if (CURRENT_TIME > CLOCK_TIME(17, 10)) {
                    SEQCMD_STOP_SEQUENCE(SEQ_PLAYER_BGM_MAIN, 240);
                    play->envCtx.timeSeqState++;
                }
                break;

            case TIMESEQ_NIGHT_BEGIN_SFX:
                if (CURRENT_TIME >= CLOCK_TIME(18, 0)) {
                    play->envCtx.timeSeqState++;
                }
                break;

            case TIMESEQ_EARLY_NIGHT_CRITTERS:
                if (play->envCtx.precipitation[PRECIP_RAIN_CUR] < 9) {
                    Audio_PlayAmbience(play->sequenceCtx.ambienceId);
                    Audio_SetAmbienceChannelIO(AMBIENCE_CHANNEL_CRITTER_0, 1, 1);
                }
                play->envCtx.timeSeqState++;
                break;

            case TIMESEQ_NIGHT_DELAY:
                if (CURRENT_TIME >= CLOCK_TIME(19, 0)) {
                    play->envCtx.timeSeqState++;
                }
                break;

            case TIMESEQ_NIGHT_CRITTERS:
                Audio_SetAmbienceChannelIO(AMBIENCE_CHANNEL_CRITTER_0, 1, 0);
                Audio_SetAmbienceChannelIO(AMBIENCE_CHANNEL_CRITTER_1 << 4 | AMBIENCE_CHANNEL_CRITTER_3, 1, 1);
                play->envCtx.timeSeqState++;
                break;

            case TIMESEQ_DAY_BEGIN_SFX:
                if ((CURRENT_TIME < CLOCK_TIME(19, 0)) && (CURRENT_TIME >= CLOCK_TIME(5, 0))) {
                    play->envCtx.timeSeqState++;
                }
                break;

            case TIMESEQ_MORNING_CRITTERS:
                Audio_SetAmbienceChannelIO(AMBIENCE_CHANNEL_CRITTER_1 << 4 | AMBIENCE_CHANNEL_CRITTER_3, 1, 0);
                Audio_SetAmbienceChannelIO(AMBIENCE_CHANNEL_CRITTER_4 << 4 | AMBIENCE_CHANNEL_CRITTER_5, 1, 1);
                play->envCtx.timeSeqState++;
                break;

            case TIMESEQ_DAY_DELAY:
                break;

            default:
                break;
        }
    }

    // @recomp Don't play final hours until it's actually past midnight
    if ((play->envCtx.timeSeqState != TIMESEQ_REQUEST) && (((void)0, gSaveContext.save.day) == 3) &&
        (CURRENT_TIME < CLOCK_TIME(6, 0)) && !func_800FE5D0(play) && (play->transitionTrigger == TRANS_TRIGGER_OFF) &&
        (play->transitionMode == TRANS_MODE_OFF) && (play->csCtx.state == CS_STATE_IDLE) &&
        ((play->sceneId != SCENE_00KEIKOKU) || (((void)0, gSaveContext.sceneLayer) != 1)) &&
        (CutsceneManager_GetCurrentCsId() == CS_ID_NONE) &&
        (AudioSeq_GetActiveSeqId(SEQ_PLAYER_BGM_MAIN) != NA_BGM_FINAL_HOURS) &&
        (AudioSeq_GetActiveSeqId(SEQ_PLAYER_BGM_MAIN) != NA_BGM_SONG_OF_SOARING) &&
        (CURRENT_TIME > CLOCK_TIME(0, 0))) {
        SEQCMD_STOP_SEQUENCE(SEQ_PLAYER_AMBIENCE, 0);
        Audio_PlaySceneSequence(NA_BGM_FINAL_HOURS, 3 - 1);
    }
}