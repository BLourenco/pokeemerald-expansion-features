#include "global.h"
#include "malloc.h"
#include "battle.h"
#include "pokemon.h"
#include "battle_controllers.h"
#include "battle_interface.h"
#include "battle_z_move.h"
#include "graphics.h"
#include "sprite.h"
#include "window.h"
#include "string_util.h"
#include "text.h"
#include "sound.h"
#include "decompress.h"
#include "task.h"
#include "util.h"
#include "gpu_regs.h"
#include "battle_message.h"
#include "pokedex.h"
#include "palette.h"
#include "international_string_util.h"
#include "safari_zone.h"
#include "battle_anim.h"
#include "data.h"
#include "pokemon_summary_screen.h"
#include "strings.h"
#include "battle_debug.h"
#include "item.h"
#include "item_icon.h"
#include "item_use.h"
#include "test_runner.h"
#include "constants/battle_anim.h"
#include "constants/rgb.h"
#include "constants/songs.h"
#include "constants/items.h"

enum
{   // Corresponds to gHealthboxElementsGfxTable (and the tables after it) in graphics.c
    // These are indexes into the tables, which are filled with 8x8 square pixel data.
    HEALTHBOX_GFX_0, //hp bar [black section]
    HEALTHBOX_GFX_1, //hp bar "H"
    HEALTHBOX_GFX_2, //hp bar "P"
    HEALTHBOX_GFX_HP_BAR_GREEN, //hp bar [0 pixels]
    HEALTHBOX_GFX_4,  //hp bar [1 pixels]
    HEALTHBOX_GFX_5,  //hp bar [2 pixels]
    HEALTHBOX_GFX_6,  //hp bar [3 pixels]
    HEALTHBOX_GFX_7,  //hp bar [4 pixels]
    HEALTHBOX_GFX_8,  //hp bar [5 pixels]
    HEALTHBOX_GFX_9,  //hp bar [6 pixels]
    HEALTHBOX_GFX_10, //hp bar [7 pixels]
    HEALTHBOX_GFX_11, //hp bar [8 pixels]
    HEALTHBOX_GFX_12, //exp bar [0 pixels]
    HEALTHBOX_GFX_13, //exp bar [1 pixels]
    HEALTHBOX_GFX_14, //exp bar [2 pixels]
    HEALTHBOX_GFX_15, //exp bar [3 pixels]
    HEALTHBOX_GFX_16, //exp bar [4 pixels]
    HEALTHBOX_GFX_17, //exp bar [5 pixels]
    HEALTHBOX_GFX_18, //exp bar [6 pixels]
    HEALTHBOX_GFX_19, //exp bar [7 pixels]
    HEALTHBOX_GFX_20, //exp bar [8 pixels]
    HEALTHBOX_GFX_STATUS_PSN_BATTLER0,  //status psn "(P"
    HEALTHBOX_GFX_22,                   //status psn "SN"
    //HEALTHBOX_GFX_23,                   //status psn "|)""
    HEALTHBOX_GFX_STATUS_PRZ_BATTLER0,  //status prz
    HEALTHBOX_GFX_25,
    //HEALTHBOX_GFX_26,
    HEALTHBOX_GFX_STATUS_SLP_BATTLER0,  //status slp
    HEALTHBOX_GFX_28,
    //HEALTHBOX_GFX_29,
    HEALTHBOX_GFX_STATUS_FRZ_BATTLER0,  //status frz
    HEALTHBOX_GFX_31,
    //HEALTHBOX_GFX_32,
    HEALTHBOX_GFX_STATUS_BRN_BATTLER0,  //status brn
    HEALTHBOX_GFX_34,
    //HEALTHBOX_GFX_35,
    HEALTHBOX_GFX_36, //misc [Black section]
    HEALTHBOX_GFX_37, //misc [Black section]
    HEALTHBOX_GFX_38, //misc [Black section]
    HEALTHBOX_GFX_39, //misc [Blank Health Window?]
    HEALTHBOX_GFX_40, //misc [Blank Health Window?]
    HEALTHBOX_GFX_41, //misc [Blank Health Window?]
    HEALTHBOX_GFX_42, //misc [Blank Health Window?]
    HEALTHBOX_GFX_43, //misc [Top of Health Window?]
    HEALTHBOX_GFX_44, //misc [Top of Health Window?]
    HEALTHBOX_GFX_45, //misc [Top of Health Window?]
    HEALTHBOX_GFX_46, //misc [Blank Health Window?]
    HEALTHBOX_GFX_HP_BAR_YELLOW, //hp bar yellow [0 pixels]
    HEALTHBOX_GFX_48, //hp bar yellow [1 pixels]
    HEALTHBOX_GFX_49, //hp bar yellow [2 pixels]
    HEALTHBOX_GFX_50, //hp bar yellow [3 pixels]
    HEALTHBOX_GFX_51, //hp bar yellow [4 pixels]
    HEALTHBOX_GFX_52, //hp bar yellow [5 pixels]
    HEALTHBOX_GFX_53, //hp bar yellow [6 pixels]
    HEALTHBOX_GFX_54, //hp bar yellow [7 pixels]
    HEALTHBOX_GFX_55, //hp bar yellow [8 pixels]
    HEALTHBOX_GFX_HP_BAR_RED,  //hp bar red [0 pixels]
    HEALTHBOX_GFX_57, //hp bar red [1 pixels]
    HEALTHBOX_GFX_58, //hp bar red [2 pixels]
    HEALTHBOX_GFX_59, //hp bar red [3 pixels]
    HEALTHBOX_GFX_60, //hp bar red [4 pixels]
    HEALTHBOX_GFX_61, //hp bar red [5 pixels]
    HEALTHBOX_GFX_62, //hp bar red [6 pixels]
    HEALTHBOX_GFX_63, //hp bar red [7 pixels]
    HEALTHBOX_GFX_64, //hp bar red [8 pixels]
    HEALTHBOX_GFX_65, //hp bar frame end
    HEALTHBOX_GFX_STATUS_BALL, // Full
    HEALTHBOX_GFX_STATUS_BALL_EMPTY,
    HEALTHBOX_GFX_STATUS_BALL_FAINTED,
    HEALTHBOX_GFX_STATUS_BALL_STATUSED,
    HEALTHBOX_GFX_STATUS_BALL_CAUGHT,
    HEALTHBOX_GFX_STATUS_PSN_BATTLER1, //status2 "PSN"
    HEALTHBOX_GFX_72,
    //HEALTHBOX_GFX_73,
    HEALTHBOX_GFX_STATUS_PRZ_BATTLER1, //status2 "PRZ"
    HEALTHBOX_GFX_75,
    //HEALTHBOX_GFX_76,
    HEALTHBOX_GFX_STATUS_SLP_BATTLER1, //status2 "SLP"
    HEALTHBOX_GFX_78,
    //HEALTHBOX_GFX_79,
    HEALTHBOX_GFX_STATUS_FRZ_BATTLER1, //status2 "FRZ"
    HEALTHBOX_GFX_81,
    //HEALTHBOX_GFX_82,
    HEALTHBOX_GFX_STATUS_BRN_BATTLER1, //status2 "BRN"
    HEALTHBOX_GFX_84,
    //HEALTHBOX_GFX_85,
    HEALTHBOX_GFX_STATUS_PSN_BATTLER2, //status3 "PSN"
    HEALTHBOX_GFX_87,
    //HEALTHBOX_GFX_88,
    HEALTHBOX_GFX_STATUS_PRZ_BATTLER2, //status3 "PRZ"
    HEALTHBOX_GFX_90,
    //HEALTHBOX_GFX_91,
    HEALTHBOX_GFX_STATUS_SLP_BATTLER2, //status3 "SLP"
    HEALTHBOX_GFX_93,
    //HEALTHBOX_GFX_94,
    HEALTHBOX_GFX_STATUS_FRZ_BATTLER2, //status3 "FRZ"
    HEALTHBOX_GFX_96,
    //HEALTHBOX_GFX_97,
    HEALTHBOX_GFX_STATUS_BRN_BATTLER2, //status3 "BRN"
    HEALTHBOX_GFX_99,
    //HEALTHBOX_GFX_100,
    HEALTHBOX_GFX_STATUS_PSN_BATTLER3, //status4 "PSN"
    HEALTHBOX_GFX_102,
    //HEALTHBOX_GFX_103,
    HEALTHBOX_GFX_STATUS_PRZ_BATTLER3, //status4 "PRZ"
    HEALTHBOX_GFX_105,
    //HEALTHBOX_GFX_106,
    HEALTHBOX_GFX_STATUS_SLP_BATTLER3, //status4 "SLP"
    HEALTHBOX_GFX_108,
    //HEALTHBOX_GFX_109,
    HEALTHBOX_GFX_STATUS_FRZ_BATTLER3, //status4 "FRZ"
    HEALTHBOX_GFX_111,
    //HEALTHBOX_GFX_112,
    HEALTHBOX_GFX_STATUS_BRN_BATTLER3, //status4 "BRN"
    HEALTHBOX_GFX_114,
    //HEALTHBOX_GFX_115,
    HEALTHBOX_GFX_FRAME_END,
    HEALTHBOX_GFX_FRAME_END_BAR,
};

enum
{   // Corresponds to gBattleInterface_IndicatorsGfxTable in graphics.c
    // These are indexes into the tables, which are filled with 8x8 square pixel data.
    INDICATOR_GFX_BLANK,
    INDICATOR_GFX_BLANK_2,
    INDICATOR_GFX_CAUGHT,
    INDICATOR_GFX_CAUGHT_2,
    INDICATOR_GFX_MEGA,
    INDICATOR_GFX_MEGA_2,
    INDICATOR_GFX_ALPHA,
    INDICATOR_GFX_ALPHA_2,
    INDICATOR_GFX_OMEGA,
    INDICATOR_GFX_OMEGA_2,
    INDICATOR_GFX_DYNAMAX,
    INDICATOR_GFX_DYNAMAX_2,
    INDICATOR_GFX_TERASTAL,
    INDICATOR_GFX_TERASTAL_2
};

enum
{   // Corresponds to gBattleInterface_StatusGfxTable in graphics.c
    // These are indexes into the tables, which are filled with 8x8 square pixel data.
    STATUS_GFX_BLANK,
    STATUS_GFX_BLANK_2,
    STATUS_GFX_PARALYZE,
    STATUS_GFX_PARALYZE_2,
    STATUS_GFX_BURN,
    STATUS_GFX_BURN_2,
    STATUS_GFX_POISON,
    STATUS_GFX_POISON_2,
    STATUS_GFX_BADLY_POISON,
    STATUS_GFX_BADLY_POISON_2,
    STATUS_GFX_FROZEN,
    STATUS_GFX_FROZEN_2,
    STATUS_GFX_FROSTBITE,
    STATUS_GFX_FROSTBITE_2,
    STATUS_GFX_SLEEP,
    STATUS_GFX_SLEEP_2,
    STATUS_GFX_DROWSY,
    STATUS_GFX_DROWSY_2
};

static const u8 *GetHealthboxElementGfxPtr(u8);
static const u8 *GetIndicatorGfxPtr(u8);
static const u8 *GetStatusGfxPtr(u8);
static u8 *AddTextPrinterAndCreateWindowOnHealthbox(const u8 *, u32, u32, u8, u32, u32, u32, u32 *);

static void RemoveWindowOnHealthbox(u32 windowId);
static void UpdateHpTextInHealthboxInDoubles(u32 healthboxSpriteId, u32 maxOrCurrent, s16 currHp, s16 maxHp);

static void TextIntoHealthboxObject(void *, u8 *, s32);
static void SafariTextIntoHealthboxObject(void *, u8 *, u32);
static void HpTextIntoHealthboxObject(void *, u8 *, u32);
static void FillHealthboxObject(void *, u32, u32);

static void Task_HidePartyStatusSummary_BattleStart_1(u8);
static void Task_HidePartyStatusSummary_BattleStart_2(u8);
static void Task_HidePartyStatusSummary_DuringBattle(u8);

static void SpriteCB_HealthBoxOther(struct Sprite *);
static void SpriteCB_HealthBar(struct Sprite *);
static void SpriteCB_StatusSummaryBar_Enter(struct Sprite *);
static void SpriteCB_StatusSummaryBar_Exit(struct Sprite *);
static void SpriteCB_StatusSummaryBalls_Enter(struct Sprite *);
static void SpriteCB_StatusSummaryBalls_Exit(struct Sprite *);
static void SpriteCB_StatusSummaryBalls_OnSwitchout(struct Sprite *);

static void SpriteCb_MegaTrigger(struct Sprite *);
static void Indicator_SetVisibility(u32 healthboxId, bool32 invisible);
static void Indicator_CreateSprite(u32 battlerId, u32 healthboxSpriteId);
static void Indicator_UpdateOamPriorities(u32 healthboxId, u32 oamPriority);
static void SpriteCb_Indicator(struct Sprite *);

static void TypeSymbols_CreateSprites(u32 battlerId, u32 healthboxSpriteId);
static void TypeSymbols_SetVisibilities(u32 healthboxId, bool32 invisible);
static void TypeSymbols_UpdateOamPriorities(u32 healthboxId, u32 oamPriority);
static void SpriteCb_TypeSymbols(struct Sprite *);
void UpdateTypeSymbols(u32 battlerId, u32 healthboxSpriteId);

static void Status_CreateSprite(u32 battlerId, u32 healthboxSpriteId);
static void Status_SetVisibility(u32 healthboxId, bool32 invisible);
static void Status_UpdateOamPriorities(u32 healthboxId, u32 oamPriority);
static void SpriteCb_Status(struct Sprite *);
static void UpdateStatusIconInHealthbox(u8);

static s32 CalcNewBarValue(s32, s32, s32, s32 *, u8, u16);
static u8 GetScaledExpFraction(s32, s32, s32, u8);
static void MoveBattleBarGraphically(u8, u8);
static u8 CalcBarFilledPixels(s32, s32, s32, s32 *, u8 *, u8);

static void SpriteCb_AbilityPopUp(struct Sprite *);
static void Task_FreeAbilityPopUpGfx(u8);
static void Task_FreeAbilityPopUpOpponentGfx(u8);
static void Task_FreeAbilityPopUpPartnerGfx(u8);

static void SpriteCB_LastUsedBall(struct Sprite *);
static void SpriteCB_LastUsedBallWin(struct Sprite *);

static const struct OamData sOamData_64x32 =
{
    .y = 0,
    .affineMode = ST_OAM_AFFINE_OFF,
    .objMode = ST_OAM_OBJ_NORMAL,
    .mosaic = FALSE,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(64x32),
    .x = 0,
    .matrixNum = 0,
    .size = SPRITE_SIZE(64x32),
    .tileNum = 0,
    .priority = 1,
    .paletteNum = 0,
    .affineParam = 0,
};

static const struct SpriteTemplate sHealthboxPlayerSpriteTemplates[2] =
{
    {
        .tileTag = TAG_HEALTHBOX_PLAYER1_TILE,
        .paletteTag = TAG_HEALTHBOX_PAL,
        .oam = &sOamData_64x32,
        .anims = gDummySpriteAnimTable,
        .images = NULL,
        .affineAnims = gDummySpriteAffineAnimTable,
        .callback = SpriteCallbackDummy
    },
    {
        .tileTag = TAG_HEALTHBOX_PLAYER2_TILE,
        .paletteTag = TAG_HEALTHBOX_PAL,
        .oam = &sOamData_64x32,
        .anims = gDummySpriteAnimTable,
        .images = NULL,
        .affineAnims = gDummySpriteAffineAnimTable,
        .callback = SpriteCallbackDummy
    }
};

static const struct SpriteTemplate sHealthboxOpponentSpriteTemplates[2] =
{
    {
        .tileTag = TAG_HEALTHBOX_OPPONENT1_TILE,
        .paletteTag = TAG_HEALTHBOX_PAL,
        .oam = &sOamData_64x32,
        .anims = gDummySpriteAnimTable,
        .images = NULL,
        .affineAnims = gDummySpriteAffineAnimTable,
        .callback = SpriteCallbackDummy
    },
    {
        .tileTag = TAG_HEALTHBOX_OPPONENT2_TILE,
        .paletteTag = TAG_HEALTHBOX_PAL,
        .oam = &sOamData_64x32,
        .anims = gDummySpriteAnimTable,
        .images = NULL,
        .affineAnims = gDummySpriteAffineAnimTable,
        .callback = SpriteCallbackDummy
    }
};

static const struct SpriteTemplate sHealthboxPartnerSpriteTemplates[2] =
{
    {
        .tileTag = TAG_HEALTHBOX_PARTNER1_TILE,
        .paletteTag = TAG_HEALTHBOX_PAL,
        .oam = &sOamData_64x32,
        .anims = gDummySpriteAnimTable,
        .images = NULL,
        .affineAnims = gDummySpriteAffineAnimTable,
        .callback = SpriteCallbackDummy
    },
    {
        .tileTag = TAG_HEALTHBOX_PARTNER2_TILE,
        .paletteTag = TAG_HEALTHBOX_PAL,
        .oam = &sOamData_64x32,
        .anims = gDummySpriteAnimTable,
        .images = NULL,
        .affineAnims = gDummySpriteAffineAnimTable,
        .callback = SpriteCallbackDummy
    }
};

static const struct SpriteTemplate sHealthboxSafariSpriteTemplate =
{
    .tileTag = TAG_HEALTHBOX_SAFARI_TILE,
    .paletteTag = TAG_HEALTHBOX_PAL,
    .oam = &sOamData_64x32,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCallbackDummy
};

static const struct OamData sOamData_Healthbar =
{
    .y = 0,
    .affineMode = ST_OAM_AFFINE_OFF,
    .objMode = ST_OAM_OBJ_NORMAL,
    .mosaic = FALSE,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(32x8),
    .x = 0,
    .matrixNum = 0,
    .size = SPRITE_SIZE(32x8),
    .tileNum = 0,
    .priority = 1,
    .paletteNum = 0,
    .affineParam = 0,
};

static const struct SpriteTemplate sHealthbarSpriteTemplates[MAX_BATTLERS_COUNT] =
{
    {
        .tileTag = TAG_HEALTHBAR_PLAYER1_TILE,
        .paletteTag = TAG_HEALTHBAR_PAL,
        .oam = &sOamData_Healthbar,
        .anims = gDummySpriteAnimTable,
        .images = NULL,
        .affineAnims = gDummySpriteAffineAnimTable,
        .callback = SpriteCB_HealthBar
    },
    {
        .tileTag = TAG_HEALTHBAR_OPPONENT1_TILE,
        .paletteTag = TAG_HEALTHBAR_PAL,
        .oam = &sOamData_Healthbar,
        .anims = gDummySpriteAnimTable,
        .images = NULL,
        .affineAnims = gDummySpriteAffineAnimTable,
        .callback = SpriteCB_HealthBar
    },
    {
        .tileTag = TAG_HEALTHBAR_PLAYER2_TILE,
        .paletteTag = TAG_HEALTHBAR_PAL,
        .oam = &sOamData_Healthbar,
        .anims = gDummySpriteAnimTable,
        .images = NULL,
        .affineAnims = gDummySpriteAffineAnimTable,
        .callback = SpriteCB_HealthBar
    },
    {
        .tileTag = TAG_HEALTHBAR_OPPONENT2_TILE,
        .paletteTag = TAG_HEALTHBAR_PAL,
        .oam = &sOamData_Healthbar,
        .anims = gDummySpriteAnimTable,
        .images = NULL,
        .affineAnims = gDummySpriteAffineAnimTable,
        .callback = SpriteCB_HealthBar
    }
};

/*  v-- Origin
[0  +  ][1     ]   64x8
*/
static const struct Subsprite sHealthBar_Subsprites_Player[] =
{
    {
        .x = -8,
        .y = 1,
        .shape = SPRITE_SHAPE(32x8),
        .size = SPRITE_SIZE(32x8),
        .tileOffset = 0,
        .priority = 1
    },
    {
        .x = 24,
        .y = 1,
        .shape = SPRITE_SHAPE(32x8),
        .size = SPRITE_SIZE(32x8),
        .tileOffset = 4,
        .priority = 1
    }
};

/*       v-- Origin
[]  [0  +  ][1     ]   8x8 + 64x8
2^ ^--- Note 8px space
*/
static const struct Subsprite sHealthBar_Subsprites_Opponent[] =
{
    {
        .x = 4,
        .y = 1,
        .shape = SPRITE_SHAPE(32x8),
        .size = SPRITE_SIZE(32x8),
        .tileOffset = 0,
        .priority = 1
    },
    {
        .x = 36,
        .y = 1,
        .shape = SPRITE_SHAPE(32x8),
        .size = SPRITE_SIZE(32x8),
        .tileOffset = 4,
        .priority = 1
    },
    {
        .x = -12,
        .y = 1,
        .shape = SPRITE_SHAPE(8x8),
        .size = SPRITE_SIZE(8x8),
        .tileOffset = 8,
        .priority = 1
    }
};

static const struct SubspriteTable sHealthBar_SubspriteTables[] =
{
    [B_SIDE_PLAYER]   = {ARRAY_COUNT(sHealthBar_Subsprites_Player), sHealthBar_Subsprites_Player},
    [B_SIDE_OPPONENT] = {ARRAY_COUNT(sHealthBar_Subsprites_Opponent), sHealthBar_Subsprites_Opponent}
};
/*                      v-- Origin
[0     ][1     ][2     ][3     ]   128x8
*/
static const struct Subsprite sStatusSummaryBar_Subsprites_Enter[] =
{
    {
        .x = 32 * -3,
        .y = 0,
        .shape = SPRITE_SHAPE(32x8),
        .size = SPRITE_SIZE(32x8),
        .tileOffset = 0,
        .priority = 1
    },
    {
        .x = 32 * -2,
        .y = 0,
        .shape = SPRITE_SHAPE(32x8),
        .size = SPRITE_SIZE(32x8),
        .tileOffset = 4,
        .priority = 1
    },
    {
        .x = 32 * -1,
        .y = 0,
        .shape = SPRITE_SHAPE(32x8),
        .size = SPRITE_SIZE(32x8),
        .tileOffset = 8,
        .priority = 1
    },
    {
        .x = 0,
        .y = 0,
        .shape = SPRITE_SHAPE(32x8),
        .size = SPRITE_SIZE(32x8),
        .tileOffset = 12,
        .priority = 1
    }
};

/*                      v-- Origin
[0     ][1     ][2     ][3     ][4     ][5     ]   192x8
                 ^-- uses same tiles --^
*/
static const struct Subsprite sStatusSummaryBar_Subsprites_Exit[] =
{
    {
        .x = 32 * -3,
        .y = 0,
        .shape = SPRITE_SHAPE(32x8),
        .size = SPRITE_SIZE(32x8),
        .tileOffset = 0,
        .priority = 1
    },
    {
        .x = 32 * -2,
        .y = 0,
        .shape = SPRITE_SHAPE(32x8),
        .size = SPRITE_SIZE(32x8),
        .tileOffset = 4,
        .priority = 1
    },
    {
        .x = 32 * -1,
        .y = 0,
        .shape = SPRITE_SHAPE(32x8),
        .size = SPRITE_SIZE(32x8),
        .tileOffset = 8,
        .priority = 1
    },
    {
        .x = 32 * 0,
        .y = 0,
        .shape = SPRITE_SHAPE(32x8),
        .size = SPRITE_SIZE(32x8),
        .tileOffset = 8,
        .priority = 1
    },
    {
        .x = 32 * 1,
        .y = 0,
        .shape = SPRITE_SHAPE(32x8),
        .size = SPRITE_SIZE(32x8),
        .tileOffset = 8,
        .priority = 1
    },
    {
        .x = 32 * 2,
        .y = 0,
        .shape = SPRITE_SHAPE(32x8),
        .size = SPRITE_SIZE(32x8),
        .tileOffset = 12,
        .priority = 1
    }
};

static const struct SubspriteTable sStatusSummaryBar_SubspriteTable_Enter[] =
{
    {ARRAY_COUNT(sStatusSummaryBar_Subsprites_Enter), sStatusSummaryBar_Subsprites_Enter}
};

static const struct SubspriteTable sStatusSummaryBar_SubspriteTable_Exit[] =
{
    {ARRAY_COUNT(sStatusSummaryBar_Subsprites_Exit), sStatusSummaryBar_Subsprites_Exit}
};

static const struct CompressedSpriteSheet sStatusSummaryBarSpriteSheet =
{
    gBattleInterface_BallStatusBarGfx, 0x200, TAG_STATUS_SUMMARY_BAR_TILE
};

static const struct SpritePalette sStatusSummaryBarSpritePal =
{
    gBattleInterface_BallStatusBarPal, TAG_STATUS_SUMMARY_BAR_PAL
};

static const struct SpritePalette sStatusSummaryBallsSpritePal =
{
    gBattleInterface_BallDisplayPal, TAG_STATUS_SUMMARY_BALLS_PAL
};

static const struct SpriteSheet sStatusSummaryBallsSpriteSheet =
{
    &gHealthboxElementsGfxTable[HEALTHBOX_GFX_STATUS_BALL], 0x80, TAG_STATUS_SUMMARY_BALLS_TILE
};

static const struct OamData sOamData_StatusSummaryBalls =
{
    .y = 0,
    .affineMode = ST_OAM_AFFINE_OFF,
    .objMode = ST_OAM_OBJ_NORMAL,
    .mosaic = FALSE,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(8x8),
    .x = 0,
    .matrixNum = 0,
    .size = SPRITE_SIZE(8x8),
    .tileNum = 0,
    .priority = 1,
    .paletteNum = 0,
    .affineParam = 0,
};

static const struct SpriteTemplate sStatusSummaryBarSpriteTemplates[2] =
{
    { // Player
        .tileTag = TAG_STATUS_SUMMARY_BAR_TILE,
        .paletteTag = TAG_STATUS_SUMMARY_BAR_PAL,
        .oam = &sOamData_64x32,
        .anims = gDummySpriteAnimTable,
        .images = NULL,
        .affineAnims = gDummySpriteAffineAnimTable,
        .callback = SpriteCB_StatusSummaryBar_Enter
    },
    { // Opponent
        .tileTag = TAG_STATUS_SUMMARY_BAR_TILE,
        .paletteTag = TAG_STATUS_SUMMARY_BAR_PAL,
        .oam = &sOamData_64x32,
        .anims = gDummySpriteAnimTable,
        .images = NULL,
        .affineAnims = gDummySpriteAffineAnimTable,
        .callback = SpriteCB_StatusSummaryBar_Enter
    }
};

static const struct SpriteTemplate sStatusSummaryBallsSpriteTemplates[2] =
{
    {
        .tileTag = TAG_STATUS_SUMMARY_BALLS_TILE,
        .paletteTag = TAG_STATUS_SUMMARY_BALLS_PAL,
        .oam = &sOamData_StatusSummaryBalls,
        .anims = gDummySpriteAnimTable,
        .images = NULL,
        .affineAnims = gDummySpriteAffineAnimTable,
        .callback = SpriteCB_StatusSummaryBalls_Enter
    },
    {
        .tileTag = TAG_STATUS_SUMMARY_BALLS_TILE,
        .paletteTag = TAG_STATUS_SUMMARY_BALLS_PAL,
        .oam = &sOamData_StatusSummaryBalls,
        .anims = gDummySpriteAnimTable,
        .images = NULL,
        .affineAnims = gDummySpriteAffineAnimTable,
        .callback = SpriteCB_StatusSummaryBalls_Enter
    }
};

static const u8 sEmptyWhiteText_GrayHighlight[] = __("{COLOR WHITE}{HIGHLIGHT DARK_GRAY}              ");
static const u8 sEmptyWhiteText_TransparentHighlight[] = __("{COLOR WHITE}{HIGHLIGHT TRANSPARENT}              ");

enum
{
    PAL_STATUS_PSN,
    PAL_STATUS_PAR,
    PAL_STATUS_SLP,
    PAL_STATUS_FRZ,
    PAL_STATUS_BRN
};

static const u16 sStatusIconColors[] =
{
    [PAL_STATUS_PSN] = RGB(24, 12, 24),
    [PAL_STATUS_PAR] = RGB(23, 23, 3),
    [PAL_STATUS_SLP] = RGB(20, 20, 17),
    [PAL_STATUS_FRZ] = RGB(17, 22, 28),
    [PAL_STATUS_BRN] = RGB(28, 14, 10),
};

static const struct WindowTemplate sHealthboxWindowTemplate = {
    .bg = 0,
    .tilemapLeft = 0,
    .tilemapTop = 0,
    .width = 8,
    .height = 2,
    .paletteNum = 0,
    .baseBlock = 0
};

const struct CompressedSpriteSheet gSpriteSheet_EnemyShadow =
{
    .data = gEnemyMonShadow_Gfx, .size = 0x80, .tag = TAG_SHADOW_GFX
};

static const struct OamData sOamData_EnemyShadow =
{
    .y = 0,
    .affineMode = ST_OAM_AFFINE_OFF,
    .objMode = ST_OAM_OBJ_NORMAL,
    .mosaic = FALSE,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(32x8),
    .x = 0,
    .matrixNum = 0,
    .size = SPRITE_SIZE(32x8),
    .tileNum = 0,
    .priority = 3,
    .paletteNum = 0,
    .affineParam = 0
};

const struct SpriteTemplate gSpriteTemplate_EnemyShadow =
{
    .tileTag = TAG_SHADOW_GFX,
    .paletteTag = TAG_SHADOW_PAL,
    .oam = &sOamData_EnemyShadow,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCB_SetInvisible
};

static const u8 ALIGNED(4) sMegaTriggerGfx[] = INCBIN_U8("graphics/battle_interface/mega_trigger.4bpp");
static const u16 sMegaTriggerPal[] = INCBIN_U16("graphics/battle_interface/mega_trigger.gbapal");

static const struct SpriteSheet sSpriteSheet_MegaTrigger =
{
    sMegaTriggerGfx, sizeof(sMegaTriggerGfx), TAG_MEGA_TRIGGER_TILE
};
static const struct SpritePalette sSpritePalette_MegaTrigger =
{
    sMegaTriggerPal, TAG_MEGA_TRIGGER_PAL
};

static const struct OamData sOamData_MegaTrigger =
{
    .y = 0,
    .affineMode = 0,
    .objMode = 0,
    .mosaic = 0,
    .bpp = 0,
    .shape = ST_OAM_SQUARE,
    .x = 0,
    .matrixNum = 0,
    .size = 2,
    .tileNum = 0,
    .priority = 1,
    .paletteNum = 0,
    .affineParam = 0,
};

static const union AnimCmd sSpriteAnim_MegaTriggerOff[] =
{
    ANIMCMD_FRAME(0, 0),
    ANIMCMD_END
};

static const union AnimCmd sSpriteAnim_MegaTriggerOn[] =
{
    ANIMCMD_FRAME(16, 0),
    ANIMCMD_END
};

static const union AnimCmd *const sSpriteAnimTable_MegaTrigger[] =
{
    sSpriteAnim_MegaTriggerOff,
    sSpriteAnim_MegaTriggerOn,
};

static const struct SpriteTemplate sSpriteTemplate_MegaTrigger =
{
    .tileTag = TAG_MEGA_TRIGGER_TILE,
    .paletteTag = TAG_MEGA_TRIGGER_PAL,
    .oam = &sOamData_MegaTrigger,
    .anims = sSpriteAnimTable_MegaTrigger,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCb_MegaTrigger
};

static const struct OamData sOamData_MoveTypeSymbols =
{
    .y = 0,
    .affineMode = ST_OAM_AFFINE_OFF,
    .objMode = ST_OAM_OBJ_NORMAL,
    .mosaic = FALSE,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(8x8),
    .x = 0,
    .matrixNum = 0,
    .size = SPRITE_SIZE(8x8),
    .tileNum = 0,
    .priority = 1,
    .paletteNum = 0,
    .affineParam = 0,
};
static const union AnimCmd sSpriteAnim_TypeNormalSymbol[] = {
    ANIMCMD_FRAME(TYPE_NORMAL * 1, 0, FALSE, FALSE),
    ANIMCMD_END
};
static const union AnimCmd sSpriteAnim_TypeFightingSymbol[] = {
    ANIMCMD_FRAME(TYPE_FIGHTING * 1, 0, FALSE, FALSE),
    ANIMCMD_END
};
static const union AnimCmd sSpriteAnim_TypeFlyingSymbol[] = {
    ANIMCMD_FRAME(TYPE_FLYING * 1, 0, FALSE, FALSE),
    ANIMCMD_END
};
static const union AnimCmd sSpriteAnim_TypePoisonSymbol[] = {
    ANIMCMD_FRAME(TYPE_POISON * 1, 0, FALSE, FALSE),
    ANIMCMD_END
};
static const union AnimCmd sSpriteAnim_TypeGroundSymbol[] = {
    ANIMCMD_FRAME(TYPE_GROUND * 1, 0, FALSE, FALSE),
    ANIMCMD_END
};
static const union AnimCmd sSpriteAnim_TypeRockSymbol[] = {
    ANIMCMD_FRAME(TYPE_ROCK * 1, 0, FALSE, FALSE),
    ANIMCMD_END
};
static const union AnimCmd sSpriteAnim_TypeBugSymbol[] = {
    ANIMCMD_FRAME(TYPE_BUG * 1, 0, FALSE, FALSE),
    ANIMCMD_END
};
static const union AnimCmd sSpriteAnim_TypeGhostSymbol[] = {
    ANIMCMD_FRAME(TYPE_GHOST * 1, 0, FALSE, FALSE),
    ANIMCMD_END
};
static const union AnimCmd sSpriteAnim_TypeSteelSymbol[] = {
    ANIMCMD_FRAME(TYPE_STEEL * 1, 0, FALSE, FALSE),
    ANIMCMD_END
};
static const union AnimCmd sSpriteAnim_TypeMysterySymbol[] = {
    ANIMCMD_FRAME(TYPE_MYSTERY * 1, 0, FALSE, FALSE),
    ANIMCMD_END
};
static const union AnimCmd sSpriteAnim_TypeFireSymbol[] = {
    ANIMCMD_FRAME(TYPE_FIRE * 1, 0, FALSE, FALSE),
    ANIMCMD_END
};
static const union AnimCmd sSpriteAnim_TypeWaterSymbol[] = {
    ANIMCMD_FRAME(TYPE_WATER * 1, 0, FALSE, FALSE),
    ANIMCMD_END
};
static const union AnimCmd sSpriteAnim_TypeGrassSymbol[] = {
    ANIMCMD_FRAME(TYPE_GRASS * 1, 0, FALSE, FALSE),
    ANIMCMD_END
};
static const union AnimCmd sSpriteAnim_TypeElectricSymbol[] = {
    ANIMCMD_FRAME(TYPE_ELECTRIC * 1, 0, FALSE, FALSE),
    ANIMCMD_END
};
static const union AnimCmd sSpriteAnim_TypePsychicSymbol[] = {
    ANIMCMD_FRAME(TYPE_PSYCHIC * 1, 0, FALSE, FALSE),
    ANIMCMD_END
};
static const union AnimCmd sSpriteAnim_TypeIceSymbol[] = {
    ANIMCMD_FRAME(TYPE_ICE * 1, 0, FALSE, FALSE),
    ANIMCMD_END
};
static const union AnimCmd sSpriteAnim_TypeDragonSymbol[] = {
    ANIMCMD_FRAME(TYPE_DRAGON * 1, 0, FALSE, FALSE),
    ANIMCMD_END
};
static const union AnimCmd sSpriteAnim_TypeDarkSymbol[] = {
    ANIMCMD_FRAME(TYPE_DARK * 1, 0, FALSE, FALSE),
    ANIMCMD_END
};
static const union AnimCmd sSpriteAnim_TypeFairySymbol[] = {
    ANIMCMD_FRAME(TYPE_FAIRY * 1, 0, FALSE, FALSE),
    ANIMCMD_END
};
static const union AnimCmd *const sSpriteAnimTable_MoveTypeSymbols[NUMBER_OF_MON_TYPES] = {
    sSpriteAnim_TypeNormalSymbol,
    sSpriteAnim_TypeFightingSymbol,
    sSpriteAnim_TypeFlyingSymbol,
    sSpriteAnim_TypePoisonSymbol,
    sSpriteAnim_TypeGroundSymbol,
    sSpriteAnim_TypeRockSymbol,
    sSpriteAnim_TypeBugSymbol,
    sSpriteAnim_TypeGhostSymbol,
    sSpriteAnim_TypeSteelSymbol,
    sSpriteAnim_TypeMysterySymbol,
    sSpriteAnim_TypeFireSymbol,
    sSpriteAnim_TypeWaterSymbol,
    sSpriteAnim_TypeGrassSymbol,
    sSpriteAnim_TypeElectricSymbol,
    sSpriteAnim_TypePsychicSymbol,
    sSpriteAnim_TypeIceSymbol,
    sSpriteAnim_TypeDragonSymbol,
    sSpriteAnim_TypeDarkSymbol,
    sSpriteAnim_TypeFairySymbol,
};

static const struct CompressedSpriteSheet sSpriteSheet_MoveTypeSymbols =
{
    .data = gMoveTypeSymbols_Gfx,
    .size = (NUMBER_OF_MON_TYPES) * 32,
    .tag = 30002
};
static const struct SpriteTemplate sSpriteTemplate_MoveTypeSymbols =
{
    .tileTag = 30002,
    .paletteTag = 30002,
    .oam = &sOamData_MoveTypeSymbols,
    .anims = sSpriteAnimTable_MoveTypeSymbols,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCb_TypeSymbols
};

// Because the healthbox is too large to fit into one sprite, it is divided into two sprites.
// healthboxLeft  or healthboxMain  is the left part that is used as the 'main' sprite.
// healthboxRight or healthboxOther is the right part of the healthbox.
// There's also the third sprite under name of healthbarSprite that refers to the healthbar visible on the healtbox.

// data fields for healthboxMain
// oam.affineParam holds healthboxRight spriteId
#define hMain_IndicatorId           data[3]
#define hMain_StatusId              data[4]
#define hMain_HealthBarSpriteId     data[5]
#define hMain_Battler               data[6]
#define hMain_Data7                 data[7]

// data fields for healthboxRight
#define hOther_HealthBoxSpriteId    data[5]

// data fields for healthbar
#define hBar_HealthBoxSpriteId      data[5]
#define hBar_Data6                  data[6]

static void InitLastUsedBallAssets(void)
{
    gBattleStruct->ballSpriteIds[0] = MAX_SPRITES;
    gBattleStruct->ballSpriteIds[1] = MAX_SPRITES;
}

// This function is here to cover a specific case - one player's mon in a 2 vs 1 double battle. In this scenario - display singles layout.
// The same goes for a 2 vs 1 where opponent has only one pokemon.
u32 WhichBattleCoords(u32 battlerId) // 0 - singles, 1 - doubles
{
    if (GetBattlerPosition(battlerId) == B_POSITION_PLAYER_LEFT
        && gPlayerPartyCount == 1
        && !(gBattleTypeFlags & BATTLE_TYPE_MULTI))
        return 0;
    else if (GetBattlerPosition(battlerId) == B_POSITION_OPPONENT_LEFT
             && gEnemyPartyCount == 1
             && !(gBattleTypeFlags & BATTLE_TYPE_TWO_OPPONENTS))
        return 0;
    else
        return IsDoubleBattle();
}

static u8 GetBattlerRelation(u8 battlerId)
{
    if (gBattleTypeFlags & BATTLE_TYPE_MULTI
            && GetBattlerPosition(battlerId) == 2)
        return RELATION_PARTNER;
    else if (GetBattlerSide(battlerId) == B_SIDE_PLAYER)
        return RELATION_PLAYER;
    else
        return RELATION_OPPONENT;
}

u8 CreateBattlerHealthboxSprites(u8 battlerId)
{
    s16 data6 = 0;
    u8 healthboxLeftSpriteId, healthboxRightSpriteId;
    u8 healthbarSpriteId;
    struct Sprite *healthBarSpritePtr;

    if (WhichBattleCoords(battlerId) == 0) // Singles
    {
        if (GetBattlerSide(battlerId) == B_SIDE_PLAYER)
        {
            healthboxLeftSpriteId = CreateSprite(&sHealthboxPlayerSpriteTemplates[0], DISPLAY_WIDTH, DISPLAY_HEIGHT, 1);
            healthboxRightSpriteId = CreateSpriteAtEnd(&sHealthboxPlayerSpriteTemplates[0], DISPLAY_WIDTH, DISPLAY_HEIGHT, 1);

            gSprites[healthboxLeftSpriteId].oam.shape = ST_OAM_SQUARE;

            gSprites[healthboxRightSpriteId].oam.shape = ST_OAM_SQUARE;
            gSprites[healthboxRightSpriteId].oam.tileNum += 64;
        }
        else
        {
            healthboxLeftSpriteId = CreateSprite(&sHealthboxOpponentSpriteTemplates[0], DISPLAY_WIDTH, DISPLAY_HEIGHT, 1);
            healthboxRightSpriteId = CreateSpriteAtEnd(&sHealthboxOpponentSpriteTemplates[0], DISPLAY_WIDTH, DISPLAY_HEIGHT, 1);

            gSprites[healthboxRightSpriteId].oam.tileNum += 32;

            data6 = 2;
        }
        gSprites[healthboxLeftSpriteId].oam.affineParam = healthboxRightSpriteId;

        gSprites[healthboxRightSpriteId].hOther_HealthBoxSpriteId = healthboxLeftSpriteId;
        gSprites[healthboxRightSpriteId].callback = SpriteCB_HealthBoxOther;
    }
    else // Doubles / Multi
    {
        if (GetBattlerRelation(battlerId) == RELATION_PARTNER)
        {
            healthboxLeftSpriteId = CreateSprite(&sHealthboxPartnerSpriteTemplates[GetBattlerPosition(battlerId) / 2], DISPLAY_WIDTH, DISPLAY_HEIGHT, 1);
            healthboxRightSpriteId = CreateSpriteAtEnd(&sHealthboxPartnerSpriteTemplates[GetBattlerPosition(battlerId) / 2], DISPLAY_WIDTH, DISPLAY_HEIGHT, 1);

            gSprites[healthboxLeftSpriteId].oam.affineParam = healthboxRightSpriteId;

            gSprites[healthboxRightSpriteId].hOther_HealthBoxSpriteId = healthboxLeftSpriteId;
            gSprites[healthboxRightSpriteId].oam.tileNum += 32;
            gSprites[healthboxRightSpriteId].callback = SpriteCB_HealthBoxOther;

            data6 = 1;
        }
        else if (GetBattlerRelation(battlerId) == RELATION_PLAYER)
        {
            healthboxLeftSpriteId = CreateSprite(&sHealthboxPlayerSpriteTemplates[GetBattlerPosition(battlerId) / 2], DISPLAY_WIDTH, DISPLAY_HEIGHT, 1);
            healthboxRightSpriteId = CreateSpriteAtEnd(&sHealthboxPlayerSpriteTemplates[GetBattlerPosition(battlerId) / 2], DISPLAY_WIDTH, DISPLAY_HEIGHT, 1);

            gSprites[healthboxLeftSpriteId].oam.affineParam = healthboxRightSpriteId;

            gSprites[healthboxRightSpriteId].hOther_HealthBoxSpriteId = healthboxLeftSpriteId;
            gSprites[healthboxRightSpriteId].oam.tileNum += 32;
            gSprites[healthboxRightSpriteId].callback = SpriteCB_HealthBoxOther;

            data6 = 1;
        }
        else
        {
            healthboxLeftSpriteId = CreateSprite(&sHealthboxOpponentSpriteTemplates[GetBattlerPosition(battlerId) / 2], DISPLAY_WIDTH, DISPLAY_HEIGHT, 1);
            healthboxRightSpriteId = CreateSpriteAtEnd(&sHealthboxOpponentSpriteTemplates[GetBattlerPosition(battlerId) / 2], DISPLAY_WIDTH, DISPLAY_HEIGHT, 1);

            gSprites[healthboxLeftSpriteId].oam.affineParam = healthboxRightSpriteId;

            gSprites[healthboxRightSpriteId].hOther_HealthBoxSpriteId = healthboxLeftSpriteId;
            gSprites[healthboxRightSpriteId].oam.tileNum += 32;
            gSprites[healthboxRightSpriteId].callback = SpriteCB_HealthBoxOther;

            data6 = 2;
        }
    }

    healthbarSpriteId = CreateSpriteAtEnd(&sHealthbarSpriteTemplates[gBattlerPositions[battlerId]], 140, 60, 0);
    healthBarSpritePtr = &gSprites[healthbarSpriteId];
    SetSubspriteTables(healthBarSpritePtr, &sHealthBar_SubspriteTables[GetBattlerSide(battlerId)]);
    healthBarSpritePtr->subspriteMode = SUBSPRITES_IGNORE_PRIORITY;
    healthBarSpritePtr->oam.priority = 1;

    CpuCopy32(GetHealthboxElementGfxPtr(HEALTHBOX_GFX_1), (void *)(OBJ_VRAM0 + healthBarSpritePtr->oam.tileNum * TILE_SIZE_4BPP), 64);

    gSprites[healthboxLeftSpriteId].hMain_HealthBarSpriteId = healthbarSpriteId;
    gSprites[healthboxLeftSpriteId].hMain_Battler = battlerId;
    gSprites[healthboxLeftSpriteId].invisible = TRUE;

    gSprites[healthboxRightSpriteId].invisible = TRUE;

    healthBarSpritePtr->hBar_HealthBoxSpriteId = healthboxLeftSpriteId;
    healthBarSpritePtr->hBar_Data6 = data6;
    healthBarSpritePtr->invisible = TRUE;
    
    TypeSymbols_CreateSprites(battlerId, healthboxLeftSpriteId);
    Indicator_CreateSprite(battlerId, healthboxLeftSpriteId);
    Status_CreateSprite(battlerId, healthboxLeftSpriteId);

    gBattleStruct->ballSpriteIds[0] = MAX_SPRITES;
    gBattleStruct->ballSpriteIds[1] = MAX_SPRITES;

    return healthboxLeftSpriteId;
}

u8 CreateSafariPlayerHealthboxSprites(void)
{
    u8 healthboxLeftSpriteId, healthboxRightSpriteId;

    healthboxLeftSpriteId = CreateSprite(&sHealthboxSafariSpriteTemplate, DISPLAY_WIDTH, DISPLAY_HEIGHT, 1);
    healthboxRightSpriteId = CreateSpriteAtEnd(&sHealthboxSafariSpriteTemplate, DISPLAY_WIDTH, DISPLAY_HEIGHT, 1);

    gSprites[healthboxLeftSpriteId].oam.shape = ST_OAM_SQUARE;
    gSprites[healthboxRightSpriteId].oam.shape = ST_OAM_SQUARE;

    gSprites[healthboxRightSpriteId].oam.tileNum += 64;

    gSprites[healthboxLeftSpriteId].oam.affineParam = healthboxRightSpriteId;
    gSprites[healthboxRightSpriteId].hOther_HealthBoxSpriteId = healthboxLeftSpriteId;

    gSprites[healthboxRightSpriteId].callback = SpriteCB_HealthBoxOther;

    return healthboxLeftSpriteId;
}

static const u8 *GetHealthboxElementGfxPtr(u8 elementId)
{
    return gHealthboxElementsGfxTable[elementId];
}

// Syncs the position of healthbar accordingly with the healthbox.
static void SpriteCB_HealthBar(struct Sprite *sprite)
{
    u8 healthboxSpriteId = sprite->hBar_HealthBoxSpriteId;

    switch (sprite->hBar_Data6)
    {
    case 0:
        sprite->x = gSprites[healthboxSpriteId].x + 26;
        sprite->y = gSprites[healthboxSpriteId].y;
        break;
    case 1:
        sprite->x = gSprites[healthboxSpriteId].x + 26;
        sprite->y = gSprites[healthboxSpriteId].y;
        break;
    case 2:
    default:
        sprite->x = gSprites[healthboxSpriteId].x - 10;
        sprite->y = gSprites[healthboxSpriteId].y;
        break;
    }

    sprite->x2 = gSprites[healthboxSpriteId].x2;
    sprite->y2 = gSprites[healthboxSpriteId].y2;
}

static void SpriteCB_HealthBoxOther(struct Sprite *sprite)
{
    u8 healthboxMainSpriteId = sprite->hOther_HealthBoxSpriteId;

    sprite->x = gSprites[healthboxMainSpriteId].x + 64;
    sprite->y = gSprites[healthboxMainSpriteId].y;

    sprite->x2 = gSprites[healthboxMainSpriteId].x2;
    sprite->y2 = gSprites[healthboxMainSpriteId].y2;
}

void SetBattleBarStruct(u8 battlerId, u8 healthboxSpriteId, s32 maxVal, s32 oldVal, s32 receivedValue)
{
    gBattleSpritesDataPtr->battleBars[battlerId].healthboxSpriteId = healthboxSpriteId;
    gBattleSpritesDataPtr->battleBars[battlerId].maxValue = maxVal;
    gBattleSpritesDataPtr->battleBars[battlerId].oldValue = oldVal;
    gBattleSpritesDataPtr->battleBars[battlerId].receivedValue = receivedValue;
    gBattleSpritesDataPtr->battleBars[battlerId].currValue = -32768;
}

void SetHealthboxSpriteInvisible(u8 healthboxSpriteId)
{
    gSprites[healthboxSpriteId].invisible = TRUE;
    gSprites[gSprites[healthboxSpriteId].hMain_HealthBarSpriteId].invisible = TRUE;
    gSprites[gSprites[healthboxSpriteId].oam.affineParam].invisible = TRUE;
    Indicator_SetVisibility(healthboxSpriteId, TRUE);
    TypeSymbols_SetVisibilities(healthboxSpriteId, TRUE);
    Status_SetVisibility(healthboxSpriteId, TRUE);
}

void SetHealthboxSpriteVisible(u8 healthboxSpriteId)
{
    gSprites[healthboxSpriteId].invisible = FALSE;
    gSprites[gSprites[healthboxSpriteId].hMain_HealthBarSpriteId].invisible = FALSE;
    gSprites[gSprites[healthboxSpriteId].oam.affineParam].invisible = FALSE;
    Indicator_SetVisibility(healthboxSpriteId, FALSE);
    TypeSymbols_SetVisibilities(healthboxSpriteId, FALSE);
    Status_SetVisibility(healthboxSpriteId, FALSE);
}

static void UpdateSpritePos(u8 spriteId, s16 x, s16 y)
{
    gSprites[spriteId].x = x;
    gSprites[spriteId].y = y;
}

void DummyBattleInterfaceFunc(u8 healthboxSpriteId, bool8 isDoubleBattleBattlerOnly)
{

}

static void TryToggleHealboxVisibility(u32 priority, u32 healthboxLeftSpriteId, u32 healthboxRightSpriteId, u32 healthbarSpriteId)
{
    bool32 invisible = FALSE;

    if (priority == 0)  // start of anim -> make invisible
        invisible = TRUE;
    else if (priority == 1) // end of anim -> make visible
        invisible = FALSE;

    TypeSymbols_ReloadPalette(); // Fix any palettes overwritten by attack anim palettes. (ex. Relic Song)
    gSprites[healthboxLeftSpriteId].invisible = invisible;
    gSprites[healthboxRightSpriteId].invisible = invisible;
    gSprites[healthbarSpriteId].invisible = invisible;
    Indicator_SetVisibility(healthboxLeftSpriteId, invisible);
    TypeSymbols_SetVisibilities(healthboxLeftSpriteId, invisible);
    Status_SetVisibility(healthboxLeftSpriteId, invisible);
}

void UpdateOamPriorityInAllHealthboxes(u8 priority, bool32 hideHPBoxes)
{
    s32 i;

    for (i = 0; i < gBattlersCount; i++)
    {
        u8 healthboxLeftSpriteId = gHealthboxSpriteIds[i];
        u8 healthboxRightSpriteId = gSprites[gHealthboxSpriteIds[i]].oam.affineParam;
        u8 healthbarSpriteId = gSprites[gHealthboxSpriteIds[i]].hMain_HealthBarSpriteId;

        gSprites[healthboxLeftSpriteId].oam.priority = priority;
        gSprites[healthboxRightSpriteId].oam.priority = priority;
        gSprites[healthbarSpriteId].oam.priority = priority;

        TypeSymbols_UpdateOamPriorities(healthboxLeftSpriteId, priority);
        Indicator_UpdateOamPriorities(healthboxLeftSpriteId, priority);
        Status_UpdateOamPriorities(healthboxLeftSpriteId, priority);

    #if B_HIDE_HEALTHBOX_IN_ANIMS
        if (hideHPBoxes && IsBattlerAlive(i))
            TryToggleHealboxVisibility(priority, healthboxLeftSpriteId, healthboxRightSpriteId, healthbarSpriteId);
    #endif
    }
}

void GetBattlerHealthboxCoords(u8 battler, s16 *x, s16 *y)
{
    *x = 0, *y = 0;

    if (!WhichBattleCoords(battler))
    {
        if (GetBattlerSide(battler) != B_SIDE_PLAYER)
            *x = 25, *y = 30;
        else
            *x = 156, *y = 88;
    }
    else
    {
        switch (GetBattlerPosition(battler))
        {
        case B_POSITION_PLAYER_LEFT:
            *x = 156, *y = 74;
            break;
        case B_POSITION_PLAYER_RIGHT:
            *x = 164, *y = 99;
            break;
        case B_POSITION_OPPONENT_LEFT:
            *x = 25, *y = 12;
            break;
        case B_POSITION_OPPONENT_RIGHT:
            *x = 17, *y = 37;
            break;
        }
    }
}

void InitBattlerHealthboxCoords(u8 battler)
{
    s16 x, y;

    GetBattlerHealthboxCoords(battler, &x, &y);
    UpdateSpritePos(gHealthboxSpriteIds[battler], x, y);
}

static void UpdateLvlInHealthbox(u8 healthboxSpriteId, struct Pokemon *mon)
{
    u32 windowId, spriteTileNum;
    u8 *windowTileData;
    u8 text[16];
    u32 xPos;
    u8 *objVram;
    u8 battler = gSprites[healthboxSpriteId].hMain_Battler;
    u8 gender, lvl;
    u8 appendPoint;

    lvl = GetMonData(mon, MON_DATA_LEVEL);

     // Use gender of illusion mon
    struct Pokemon *illusionMon = GetIllusionMonPtr(gSprites[healthboxSpriteId].hMain_Battler);
    if (illusionMon != NULL)
        mon = illusionMon; 

    gender = GetMonGender(mon);

    switch (gender)
    {
    default:
        StringCopy(gDisplayedStringBattle, gText_HealthboxGender_None);
        appendPoint = 12;
        break;
    case MON_MALE:
        StringCopy(gDisplayedStringBattle, gText_HealthboxGender_Male);
        appendPoint = 11;
        break;
    case MON_FEMALE:
        StringCopy(gDisplayedStringBattle, gText_HealthboxGender_Female);
        appendPoint = 11;
        break;
    }

    objVram = ConvertIntToDecimalStringN(gDisplayedStringBattle + appendPoint, lvl, STR_CONV_MODE_LEFT_ALIGN, 3);
    xPos = 5 * (3 - (objVram - (gDisplayedStringBattle + appendPoint)));

    windowTileData = AddTextPrinterAndCreateWindowOnHealthbox(gDisplayedStringBattle, xPos, 2, FONT_SMALL_OUTLINED, 2, 1, 0, &windowId); // Gender/Lv.### FG colors are set in the string itself.
    spriteTileNum = gSprites[healthboxSpriteId].oam.tileNum * TILE_SIZE_4BPP;

    if (GetBattlerSide(battler) == B_SIDE_PLAYER)
    {
        objVram = (void *)(OBJ_VRAM0);
        if (!WhichBattleCoords(battler))
            objVram += spriteTileNum + 0x820;
        else
            objVram += spriteTileNum + 0x420;
    }
    else
    {
        objVram = (void *)(OBJ_VRAM0);
        objVram += spriteTileNum + 96 + 0x400;
    }
    TextIntoHealthboxObject(objVram, windowTileData, 4);
    RemoveWindowOnHealthbox(windowId);
}

static void PrintHpOnHealthbox(u32 spriteId, s16 currHp, s16 maxHp, u8 x, u8 y, u8 font, u32 textColor, u32 shadeColor, u32 bgColor, u32 rightTile, u32 leftTile)
{
    u8 *windowTileData;
    u32 windowId, tilesCount, healthboxTileNum;
    u8 text[28], *txtPtr;
    void *objVram = (void *)(OBJ_VRAM0) + gSprites[spriteId].oam.tileNum * TILE_SIZE_4BPP;

    // To fit 4 digit HP values we need to modify a bit the way hp is printed on Healthbox.
    // 6 chars can fit on the right healthbox, the rest goes to the left one
    txtPtr = ConvertIntToDecimalStringN(text, currHp, STR_CONV_MODE_RIGHT_ALIGN, 4);
    *txtPtr++ = CHAR_SLASH;
    txtPtr = ConvertIntToDecimalStringN(txtPtr, maxHp, STR_CONV_MODE_LEFT_ALIGN, 4);
    // Print last 6 chars on the right window
    windowTileData = AddTextPrinterAndCreateWindowOnHealthbox(txtPtr - 8, x, y, font, textColor, shadeColor, bgColor, &windowId);
    HpTextIntoHealthboxObject(objVram + rightTile, windowTileData, 5);
    RemoveWindowOnHealthbox(windowId);
    // Print the rest of the chars on the left window
    txtPtr[-8] = EOS;
    // if max hp is 3 digits print on block closer to the right window, if 4 digits print further from the right window
    if (maxHp >= 1000)
    {
        x += 3, tilesCount = 1, leftTile += 64;
        windowTileData = AddTextPrinterAndCreateWindowOnHealthbox(text, x, y, font, textColor, shadeColor, bgColor, &windowId);
        HpTextIntoHealthboxObject(objVram + leftTile, windowTileData, tilesCount);
        RemoveWindowOnHealthbox(windowId);
    }
}

static void PrintHpOnHealthbar(u32 spriteId, s16 currHp, s16 maxHp, u8 x, u8 y, u8 font, u32 textColor, u32 shadeColor, u32 bgColor, u32 rightTile, u32 leftTile)
{
    u8 *windowTileData;
    u32 windowId, tilesCount, healthboxTileNum;
    u8 text[28], *txtPtr;
    void *objVram = (void *)(OBJ_VRAM0) + gSprites[spriteId].oam.tileNum * TILE_SIZE_4BPP;

    // To fit 4 digit HP values we need to modify a bit the way hp is printed on Healthbox.
    // 6 chars can fit on the right healthbox, the rest goes to the left one
    txtPtr = ConvertIntToDecimalStringN(text, currHp, STR_CONV_MODE_RIGHT_ALIGN, 4);
    *txtPtr++ = CHAR_SLASH;
    txtPtr = ConvertIntToDecimalStringN(txtPtr, maxHp, STR_CONV_MODE_LEFT_ALIGN, 4);
    // Print last 6 chars on the right window
    windowTileData = AddTextPrinterAndCreateWindowOnHealthbox(txtPtr - 4, x, y, font, textColor, shadeColor, bgColor, &windowId);
    HpTextIntoHealthboxObject(objVram + rightTile, windowTileData, 4);
    RemoveWindowOnHealthbox(windowId);
    // Print the rest of the chars on the left window
    txtPtr[-4] = EOS;
    // if max hp is 3 digits print on block closer to the right window, if 4 digits print further from the right window
    if (maxHp >= 1000)
        x += 13, tilesCount = 5, leftTile -= 64;
    else
        x += 1, tilesCount = 3;
    windowTileData = AddTextPrinterAndCreateWindowOnHealthbox(text, x, y, font, textColor, shadeColor, bgColor, &windowId);
    HpTextIntoHealthboxObject(objVram + leftTile, windowTileData, tilesCount);
    RemoveWindowOnHealthbox(windowId);
}

// Note: this is only possible to trigger via debug, it was an unused GF function.
static void UpdateOpponentHpTextDoubles(u32 healthboxSpriteId, u32 barSpriteId, s16 value, u8 maxOrCurrent)
{
    u8 text[32], *txtPtr;
    u32 i, var;
    u32 battlerId = gSprites[healthboxSpriteId].hMain_Battler;

    if (gBattleSpritesDataPtr->battlerData[battlerId].hpNumbersNoBars) // don't print text if only bars are visible
    {
        memcpy(text, sEmptyWhiteText_TransparentHighlight, sizeof(sEmptyWhiteText_TransparentHighlight));
        if (maxOrCurrent == HP_CURRENT)
            var = 0;
        else
            var = 4;

        txtPtr = ConvertIntToDecimalStringN(text + 6, value, STR_CONV_MODE_RIGHT_ALIGN, 3);
        if (!maxOrCurrent)
            StringCopy(txtPtr, gText_Slash);
        RenderTextHandleBold(gMonSpritesGfxPtr->barFontGfx, FONT_BOLD, text);

        for (i = var; i < var + 3; i++)
        {
            if (i < 3)
            {
                CpuCopy32(&gMonSpritesGfxPtr->barFontGfx[((i - var) * 64) + 32],
                      (void *)((OBJ_VRAM0) + 32 * (1 + gSprites[barSpriteId].oam.tileNum + i)),
                      0x20);
            }
            else
            {
                CpuCopy32(&gMonSpritesGfxPtr->barFontGfx[((i - var) * 64) + 32],
                      (void *)((OBJ_VRAM0 + 0x20) + 32 * (i + gSprites[barSpriteId].oam.tileNum)),
                      0x20);
            }
        }

        if (maxOrCurrent == HP_CURRENT)
        {
            CpuCopy32(&gMonSpritesGfxPtr->barFontGfx[224],
                      (void *)((OBJ_VRAM0) + ((gSprites[barSpriteId].oam.tileNum + 4) * TILE_SIZE_4BPP)),
                      0x20);
            CpuFill32(0, (void *)((OBJ_VRAM0) + (gSprites[barSpriteId].oam.tileNum * TILE_SIZE_4BPP)), 0x20);
        }
    }
}

// Same with this one.
static void UpdateOpponentHpTextSingles(u32 healthboxSpriteId, s16 value, u32 maxOrCurrent)
{
    u8 text[32];
    u32 var, i;
    u32 battler = gSprites[healthboxSpriteId].hMain_Battler;

    memcpy(text, sEmptyWhiteText_GrayHighlight, sizeof(sEmptyWhiteText_GrayHighlight));
    if (gBattleSpritesDataPtr->battlerData[battler].hpNumbersNoBars) // don't print text if only bars are visible
    {
        if (maxOrCurrent == HP_CURRENT)
            var = 21;
        else
            var = 49;

        ConvertIntToDecimalStringN(text + 6, value, STR_CONV_MODE_LEADING_ZEROS, 3);
        RenderTextHandleBold(gMonSpritesGfxPtr->barFontGfx, FONT_BOLD, text);

        for (i = 0; i < 3; i++)
        {
            CpuCopy32(&gMonSpritesGfxPtr->barFontGfx[i * 64 + 32],
                      (void *)((OBJ_VRAM0) + TILE_SIZE_4BPP * (gSprites[healthboxSpriteId].oam.tileNum + var + i)),
                      0x20);
        }
    }
}

void UpdateHpTextInHealthbox(u32 healthboxSpriteId, u32 maxOrCurrent, s16 currHp, s16 maxHp)
{
    u32 battlerId = gSprites[healthboxSpriteId].hMain_Battler;
    if (WhichBattleCoords(battlerId))
    {
        UpdateHpTextInHealthboxInDoubles(healthboxSpriteId, maxOrCurrent, currHp, maxHp);
    }
    else // Single Battle
    {
        if (GetBattlerSide(battlerId) == B_SIDE_PLAYER) // Player
        {
            PrintHpOnHealthbox(healthboxSpriteId, currHp, maxHp, 0, 5, FONT_SMALL, 2, 1, 6, 0xB00, 0x3A0);
        }
        else // Opponent
        {
            UpdateOpponentHpTextSingles(healthboxSpriteId, currHp, HP_CURRENT);
            UpdateOpponentHpTextSingles(healthboxSpriteId, maxHp, HP_MAX);
        }
    }
}

static void UpdateHpTextInHealthboxInDoubles(u32 healthboxSpriteId, u32 maxOrCurrent, s16 currHp, s16 maxHp)
{
    u32 barSpriteId = gSprites[healthboxSpriteId].data[5];

    if (GetBattlerSide(gSprites[healthboxSpriteId].hMain_Battler) == B_SIDE_PLAYER)
    {
        if (gBattleSpritesDataPtr->battlerData[gSprites[healthboxSpriteId].data[6]].hpNumbersNoBars) // don't print text if only bars are visible
        {
            PrintHpOnHealthbar(barSpriteId, currHp, maxHp, 0, 4, FONT_SMALL, 2, 5, 0, 0x80, 0x20); // Colors use HP Bar Palette
            // Clears the end of the healthbar gfx.
            // CpuCopy32(GetHealthboxElementGfxPtr(HEALTHBOX_GFX_FRAME_END),
            //               (void *)(OBJ_VRAM0 + 0x680) + (gSprites[healthboxSpriteId].oam.tileNum * TILE_SIZE_4BPP),
            //                0x20);
            // Erases HP bar leftover.
            //FillHealthboxObject((void *)(OBJ_VRAM0) + (gSprites[barSpriteId].oam.tileNum * TILE_SIZE_4BPP), 0, 2);
        }
    }
    else // Opponent
    {
        UpdateOpponentHpTextDoubles(healthboxSpriteId, barSpriteId, maxHp, HP_MAX);
        UpdateOpponentHpTextDoubles(healthboxSpriteId, barSpriteId, currHp, HP_CURRENT);
    }
}

// Prints mon's nature, catch and flee rate. Probably used to test pokeblock-related features.
static void PrintSafariMonInfo(u8 healthboxSpriteId, struct Pokemon *mon)
{
    u8 text[20];
    s32 j, spriteTileNum;
    u8 *barFontGfx;
    u8 i, var, nature, healthBarSpriteId;

    memcpy(text, sEmptyWhiteText_GrayHighlight, sizeof(sEmptyWhiteText_GrayHighlight));
    barFontGfx = &gMonSpritesGfxPtr->barFontGfx[0x520 + (GetBattlerPosition(gSprites[healthboxSpriteId].hMain_Battler) * 384)];
    var = 5;
    nature = GetNature(mon);
    StringCopy(&text[6], gNatureNamePointers[nature]);
    RenderTextHandleBold(barFontGfx, FONT_BOLD, text);

    for (j = 6, i = 0; i < var; i++, j++)
    {
        u8 elementId;

        if ((text[j] >= 55 && text[j] <= 74) || (text[j] >= 135 && text[j] <= 154))
            elementId = 44;
        else if ((text[j] >= 75 && text[j] <= 79) || (text[j] >= 155 && text[j] <= 159))
            elementId = 45;
        else
            elementId = 43;

        CpuCopy32(GetHealthboxElementGfxPtr(elementId), barFontGfx + (i * 64), 0x20);
    }

    for (j = 1; j < var + 1; j++)
    {
        spriteTileNum = (gSprites[healthboxSpriteId].oam.tileNum + (j - (j / 8 * 8)) + (j / 8 * 64)) * TILE_SIZE_4BPP;
        CpuCopy32(barFontGfx, (void *)(OBJ_VRAM0) + (spriteTileNum), 0x20);
        barFontGfx += 0x20;

        spriteTileNum = (8 + gSprites[healthboxSpriteId].oam.tileNum + (j - (j / 8 * 8)) + (j / 8 * 64)) * TILE_SIZE_4BPP;
        CpuCopy32(barFontGfx, (void *)(OBJ_VRAM0) + (spriteTileNum), 0x20);
        barFontGfx += 0x20;
    }

    healthBarSpriteId = gSprites[healthboxSpriteId].hMain_HealthBarSpriteId;
    ConvertIntToDecimalStringN(&text[6], gBattleStruct->safariCatchFactor, STR_CONV_MODE_RIGHT_ALIGN, 2);
    ConvertIntToDecimalStringN(&text[9], gBattleStruct->safariEscapeFactor, STR_CONV_MODE_RIGHT_ALIGN, 2);
    text[5] = CHAR_SPACE;
    text[8] = CHAR_SLASH;
    RenderTextHandleBold(gMonSpritesGfxPtr->barFontGfx, FONT_BOLD, text);

    j = healthBarSpriteId; // Needed to match for some reason.
    for (j = 0; j < 5; j++)
    {
        if (j <= 1)
        {
            CpuCopy32(&gMonSpritesGfxPtr->barFontGfx[0x40 * j + 0x20],
                      (void *)(OBJ_VRAM0) + (gSprites[healthBarSpriteId].oam.tileNum + 2 + j) * TILE_SIZE_4BPP,
                      32);
        }
        else
        {
            CpuCopy32(&gMonSpritesGfxPtr->barFontGfx[0x40 * j + 0x20],
                      (void *)(OBJ_VRAM0 + 0xC0) + (j + gSprites[healthBarSpriteId].oam.tileNum) * TILE_SIZE_4BPP,
                      32);
        }
    }
}

void SwapHpBarsWithHpText(void)
{
    u32 healthBarSpriteId, i;

    for (i = 0; i < gBattlersCount; i++)
    {
        if (gSprites[gHealthboxSpriteIds[i]].callback == SpriteCallbackDummy
         && GetBattlerSide(i) != B_SIDE_OPPONENT
         && (WhichBattleCoords(i) || GetBattlerSide(i) != B_SIDE_PLAYER))
        {
            s32 currHp = GetMonData(&gPlayerParty[gBattlerPartyIndexes[i]], MON_DATA_HP);
            s32 maxHp = GetMonData(&gPlayerParty[gBattlerPartyIndexes[i]], MON_DATA_MAX_HP);
            bool8 noBars;

            gBattleSpritesDataPtr->battlerData[i].hpNumbersNoBars ^= 1;
            noBars = gBattleSpritesDataPtr->battlerData[i].hpNumbersNoBars;
            if (GetBattlerSide(i) == B_SIDE_PLAYER)
            {
                if (!WhichBattleCoords(i))
                    continue;
                if (gBattleTypeFlags & BATTLE_TYPE_SAFARI)
                    continue;

                if (noBars == TRUE) // bars to text
                {
                    healthBarSpriteId = gSprites[gHealthboxSpriteIds[i]].hMain_HealthBarSpriteId;

                    // Clear entire sprite/subsprites
                    CpuFill32(0, (void *)(OBJ_VRAM0 + gSprites[healthBarSpriteId].oam.tileNum * TILE_SIZE_4BPP), 0x100);
                    UpdateHpTextInHealthboxInDoubles(gHealthboxSpriteIds[i], HP_BOTH, currHp, maxHp);
                }
                else // text to bars
                {
                    UpdateHealthboxAttribute(gHealthboxSpriteIds[i], &gPlayerParty[gBattlerPartyIndexes[i]], HEALTHBOX_HEALTH_BAR);
                }
            }
            else // Opponent
            {
                if (noBars == TRUE) // bars to text
                {
                    if (gBattleTypeFlags & BATTLE_TYPE_SAFARI)
                    {
                        // Most likely a debug function.
                        PrintSafariMonInfo(gHealthboxSpriteIds[i], &gEnemyParty[gBattlerPartyIndexes[i]]);
                    }
                    else
                    {
                        healthBarSpriteId = gSprites[gHealthboxSpriteIds[i]].hMain_HealthBarSpriteId;

                        CpuFill32(0, (void *)(OBJ_VRAM0 + gSprites[healthBarSpriteId].oam.tileNum * 32), 0x100);
                        UpdateHpTextInHealthboxInDoubles(gHealthboxSpriteIds[i], HP_BOTH, currHp, maxHp);
                    }
                }
                else // text to bars
                {
                    UpdateHealthboxAttribute(gHealthboxSpriteIds[i], &gEnemyParty[gBattlerPartyIndexes[i]], HEALTHBOX_HEALTH_BAR);
                    if (gBattleTypeFlags & BATTLE_TYPE_SAFARI)
                        UpdateHealthboxAttribute(gHealthboxSpriteIds[i], &gEnemyParty[gBattlerPartyIndexes[i]], HEALTHBOX_NICK);
                }
            }
            gSprites[gHealthboxSpriteIds[i]].hMain_Data7 ^= 1;
        }
    }
}

// Mega Evolution Trigger icon functions.
void ChangeMegaTriggerSprite(u8 spriteId, u8 animId)
{
    StartSpriteAnim(&gSprites[spriteId], animId);
}

#define SINGLES_MEGA_TRIGGER_POS_X_OPTIMAL (30)
#define SINGLES_MEGA_TRIGGER_POS_X_PRIORITY (31)
#define SINGLES_MEGA_TRIGGER_POS_X_SLIDE (15)
#define SINGLES_MEGA_TRIGGER_POS_Y_DIFF (-11)

#define DOUBLES_MEGA_TRIGGER_POS_X_OPTIMAL (30)
#define DOUBLES_MEGA_TRIGGER_POS_X_PRIORITY (31)
#define DOUBLES_MEGA_TRIGGER_POS_X_SLIDE (15)
#define DOUBLES_MEGA_TRIGGER_POS_Y_DIFF (-4)

#define tBattler    data[0]
#define tHide       data[1]

void CreateMegaTriggerSprite(u8 battlerId, u8 palId)
{
    LoadSpritePalette(&sSpritePalette_MegaTrigger);
    if (GetSpriteTileStartByTag(TAG_MEGA_TRIGGER_TILE) == 0xFFFF)
        LoadSpriteSheet(&sSpriteSheet_MegaTrigger);
    if (gBattleStruct->mega.triggerSpriteId == 0xFF)
    {
        if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
            gBattleStruct->mega.triggerSpriteId = CreateSprite(&sSpriteTemplate_MegaTrigger,
                                                             gSprites[gHealthboxSpriteIds[battlerId]].x - DOUBLES_MEGA_TRIGGER_POS_X_SLIDE,
                                                             gSprites[gHealthboxSpriteIds[battlerId]].y - DOUBLES_MEGA_TRIGGER_POS_Y_DIFF, 0);
        else
            gBattleStruct->mega.triggerSpriteId = CreateSprite(&sSpriteTemplate_MegaTrigger,
                                                             gSprites[gHealthboxSpriteIds[battlerId]].x - SINGLES_MEGA_TRIGGER_POS_X_SLIDE,
                                                             gSprites[gHealthboxSpriteIds[battlerId]].y - SINGLES_MEGA_TRIGGER_POS_Y_DIFF, 0);
    }
    gSprites[gBattleStruct->mega.triggerSpriteId].tBattler = battlerId;
    gSprites[gBattleStruct->mega.triggerSpriteId].tHide = FALSE;

    ChangeMegaTriggerSprite(gBattleStruct->mega.triggerSpriteId, palId);
}

static void SpriteCb_MegaTrigger(struct Sprite *sprite)
{
    s32 xSlide, xPriority, xOptimal;
    s32 yDiff;

    if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
    {
        xSlide = DOUBLES_MEGA_TRIGGER_POS_X_SLIDE;
        xPriority = DOUBLES_MEGA_TRIGGER_POS_X_PRIORITY;
        xOptimal = DOUBLES_MEGA_TRIGGER_POS_X_OPTIMAL;
        yDiff = DOUBLES_MEGA_TRIGGER_POS_Y_DIFF;
    }
    else
    {
        xSlide = SINGLES_MEGA_TRIGGER_POS_X_SLIDE;
        xPriority = SINGLES_MEGA_TRIGGER_POS_X_PRIORITY;
        xOptimal = SINGLES_MEGA_TRIGGER_POS_X_OPTIMAL;
        yDiff = SINGLES_MEGA_TRIGGER_POS_Y_DIFF;
    }

    if (sprite->tHide)
    {
        if (sprite->x != gSprites[gHealthboxSpriteIds[sprite->tBattler]].x - xSlide)
            sprite->x++;

        if (sprite->x >= gSprites[gHealthboxSpriteIds[sprite->tBattler]].x - xPriority)
            sprite->oam.priority = 2;
        else
            sprite->oam.priority = 1;

        sprite->y = gSprites[gHealthboxSpriteIds[sprite->tBattler]].y - yDiff;
        sprite->y2 = gSprites[gHealthboxSpriteIds[sprite->tBattler]].y2 - yDiff;
        if (sprite->x == gSprites[gHealthboxSpriteIds[sprite->tBattler]].x - xSlide)
            DestroyMegaTriggerSprite();
    }
    else
    {
        if (sprite->x != gSprites[gHealthboxSpriteIds[sprite->tBattler]].x - xOptimal)
            sprite->x--;

        if (sprite->x >= gSprites[gHealthboxSpriteIds[sprite->tBattler]].x - xPriority)
            sprite->oam.priority = 2;
        else
            sprite->oam.priority = 1;

        sprite->y = gSprites[gHealthboxSpriteIds[sprite->tBattler]].y - yDiff;
        sprite->y2 = gSprites[gHealthboxSpriteIds[sprite->tBattler]].y2 - yDiff;
    }
}

bool32 IsMegaTriggerSpriteActive(void)
{
    if (GetSpriteTileStartByTag(TAG_MEGA_TRIGGER_TILE) == 0xFFFF)
        return FALSE;
    else if (IndexOfSpritePaletteTag(TAG_MEGA_TRIGGER_PAL) != 0xFF)
        return TRUE;
    else
        return FALSE;
}

void HideMegaTriggerSprite(void)
{
    if (gBattleStruct->mega.triggerSpriteId != 0xFF)
    {
        ChangeMegaTriggerSprite(gBattleStruct->mega.triggerSpriteId, 0);
        gSprites[gBattleStruct->mega.triggerSpriteId].tHide = TRUE;
    }
}

void HideTriggerSprites(void)
{
    HideMegaTriggerSprite();
    HideZMoveTriggerSprite();
}

void DestroyMegaTriggerSprite(void)
{
    FreeSpritePaletteByTag(TAG_MEGA_TRIGGER_PAL);
    FreeSpriteTilesByTag(TAG_MEGA_TRIGGER_TILE);
    if (gBattleStruct->mega.triggerSpriteId != 0xFF)
        DestroySprite(&gSprites[gBattleStruct->mega.triggerSpriteId]);
    gBattleStruct->mega.triggerSpriteId = 0xFF;
}

#undef tBattler
#undef tHide

// for sprite data fields
#define tBattler        data[0]
#define tType           data[1] // Indicator type: mega, alpha, omega
#define tPosX           data[2]
#define tLevelXDelta    data[3] // X position depends whether level has 3, 2 or 1 digit


static const struct OamData sOamData_Indicator =
{
    .y = 0,
    .affineMode = ST_OAM_AFFINE_OFF,
    .objMode = ST_OAM_OBJ_NORMAL,
    .mosaic = FALSE,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(8x16),
    .x = 0,
    .matrixNum = 0,
    .size = SPRITE_SIZE(8x16),
    .tileNum = 0,
    .priority = 1,
    .paletteNum = 0,
    .affineParam = 0,
};

static const struct SpriteTemplate sIndicatorPlayerSpriteTemplates[2] =
{
    {
        .tileTag = TAG_INDICATOR_PLAYER1_TILE,
        .paletteTag = TAG_INDICATOR_PAL,
        .oam = &sOamData_Indicator,
        .anims = gDummySpriteAnimTable,
        .images = NULL,
        .affineAnims = gDummySpriteAffineAnimTable,
        .callback = SpriteCb_Indicator
    },
    {
        .tileTag = TAG_INDICATOR_PLAYER2_TILE,
        .paletteTag = TAG_INDICATOR_PAL,
        .oam = &sOamData_Indicator,
        .anims = gDummySpriteAnimTable,
        .images = NULL,
        .affineAnims = gDummySpriteAffineAnimTable,
        .callback = SpriteCb_Indicator
    }
};

static const struct SpriteTemplate sIndicatorOpponentSpriteTemplates[2] =
{
    {
        .tileTag = TAG_INDICATOR_OPPONENT1_TILE,
        .paletteTag = TAG_INDICATOR_PAL,
        .oam = &sOamData_Indicator,
        .anims = gDummySpriteAnimTable,
        .images = NULL,
        .affineAnims = gDummySpriteAffineAnimTable,
        .callback = SpriteCb_Indicator
    },
    {
        .tileTag = TAG_INDICATOR_OPPONENT2_TILE,
        .paletteTag = TAG_INDICATOR_PAL,
        .oam = &sOamData_Indicator,
        .anims = gDummySpriteAnimTable,
        .images = NULL,
        .affineAnims = gDummySpriteAffineAnimTable,
        .callback = SpriteCb_Indicator
    }
};

static const s8 sIndicatorPositions[][2] =
{
    [B_POSITION_PLAYER_LEFT] = {-26, -9},
    [B_POSITION_OPPONENT_LEFT] = {-10, -9},
    [B_POSITION_PLAYER_RIGHT] = {-26, -9},
    [B_POSITION_OPPONENT_RIGHT] = {-10, -9},
};

static const s8 sTypeSymbolsPositions[][2] =
{
    [B_POSITION_PLAYER_LEFT] = {-17, 6},
    [B_POSITION_OPPONENT_LEFT] = {76, 6},
    [B_POSITION_PLAYER_RIGHT] = {-17, 6},
    [B_POSITION_OPPONENT_RIGHT] = {76, 6},
};

static u8 *TypeSymbols_GetSpriteIds(u32 healthboxSpriteId)
{
    u8 healthboxRightSpriteId = gSprites[healthboxSpriteId].oam.affineParam;
    u8 *spriteIds = (u8 *)(&gSprites[healthboxRightSpriteId].data[3]);
    return spriteIds;
}

void TypeSymbols_LoadSpritesGfx(void)
{
    LoadCompressedSpriteSheet(&sSpriteSheet_MoveTypeSymbols);
    LoadCompressedPalette(gMoveTypes_Pal, OBJ_PLTT_ID(14), 2 * PLTT_SIZE_4BPP);
}

// Some attack animations overwrite the type palettes when used.
// This is called after the animation ends to restore the palettes.
void TypeSymbols_ReloadPalette(void)
{
    LoadCompressedPalette(gMoveTypes_Pal, OBJ_PLTT_ID(14), 2 * PLTT_SIZE_4BPP);
}

static void TypeSymbols_CreateSprites(u32 battlerId, u32 healthboxSpriteId)
{
    u32 position, i;
    u8 *spriteIds;
    s16 xHealthbox = 0, y = 0;
    s32 x = 0;

    position = GetBattlerPosition(battlerId);
    GetBattlerHealthboxCoords(battlerId, &xHealthbox, &y);

    x = sTypeSymbolsPositions[position][0];
    y += sTypeSymbolsPositions[position][1];

    spriteIds = TypeSymbols_GetSpriteIds(healthboxSpriteId);
    for (i = 0; i < 3; i++)
    {
        spriteIds[i] = CreateSpriteAtEnd(&sSpriteTemplate_MoveTypeSymbols, 0, y, 0);
        gSprites[spriteIds[i]].tBattler = battlerId;
        gSprites[spriteIds[i]].tPosX = x;
        gSprites[spriteIds[i]].invisible = TRUE;        
    }
}

static bool8 UseIllusionType(u8 battlerId)
{
    return  GetBattlerSide(battlerId) == B_SIDE_OPPONENT
            && gBattleStruct->illusion[battlerId].on;
}

static u8 * GetDisplayedTypes(u8 battlerId)
{
    static u8 battlerTypes[3];

    if (UseIllusionType(battlerId))
    {
        battlerTypes[0] = gSpeciesInfo[GetIllusionMonSpecies(battlerId)].types[0];
        battlerTypes[1] = gSpeciesInfo[GetIllusionMonSpecies(battlerId)].types[1];
        battlerTypes[2] = gBattleMons[battlerId].type3;
    }
    else
    {
        battlerTypes[0] = gBattleMons[battlerId].type1;
        battlerTypes[1] = gBattleMons[battlerId].type2;
        battlerTypes[2] = gBattleMons[battlerId].type3;
    }

    return battlerTypes;
}

static void SetTypeSymbolSpriteAndPal(u8 battlerId, u32 healthboxSpriteId)
{    
    u8 i;
    u8 *battlerTypes = GetDisplayedTypes(battlerId);
    u8 *spriteIds = TypeSymbols_GetSpriteIds(healthboxSpriteId);
    struct Sprite *sprite;

    for (i = 0; i < 3; i++)
    {
        sprite = &gSprites[spriteIds[i]];
        StartSpriteAnim(sprite, battlerTypes[i]);
        sprite->oam.paletteNum = gMoveTypeToOamPaletteNum[battlerTypes[i]];
    }
}

static void SetTypeSymbolPos(u32 battlerId, u32 healthboxSpriteId)
{
    s8 i, cursor = 0, lastType = TYPE_NONE;
    u16 x, y;
    u8 symbolOffset = 9; // 8px width, plus 1px spacing
    struct Sprite *sprite;
    u8 *battlerTypes = GetDisplayedTypes(battlerId);
    
    u16 position = GetBattlerPosition(battlerId);
    u8 *spriteIds = TypeSymbols_GetSpriteIds(healthboxSpriteId);

    x = gSprites[gHealthboxSpriteIds[battlerId]].x + sTypeSymbolsPositions[position][0];
    y = gSprites[gHealthboxSpriteIds[battlerId]].y + sTypeSymbolsPositions[position][1];

    if (GetBattlerSide(battlerId) == B_SIDE_OPPONENT)
    {
        for (i = 2; i >= 0; i--)
        {            
            sprite = &gSprites[spriteIds[i]];
            sprite->x = x;
            sprite->y = y;

            if ((i == 0 && cursor == 0) // Last index and haven't positioned any symbols
                || (battlerTypes[i] != TYPE_MYSTERY /*&& battlerTypes[i] != lastType*/))
            {
                // Moves cursor back for pure types, fixes an issue with the visibility
                // making type1 visible, but using type2 for positioning
                if (battlerTypes[i] == lastType)
                    cursor--;

                sprite->x = x - (symbolOffset * cursor++);
                lastType = battlerTypes[i];
            }
        }
    }
    else
    {
        for (i = 0; i < 3; i++)
        {
            sprite = &gSprites[spriteIds[i]];
            sprite->x = x;
            sprite->y = y;

            if ((i == 2 && cursor == 0) // Last index and haven't positioned any symbols
                || (battlerTypes[i] != TYPE_MYSTERY && battlerTypes[i] != lastType))
            {
                sprite->x = x + (symbolOffset * cursor++);
                lastType = battlerTypes[i];
            }
        }
    }
}

static void TypeSymbols_ShouldBeInvisible(u32 healthboxId, u8 typeSlot)
{
    u8 *spriteIds = TypeSymbols_GetSpriteIds(healthboxId); 
    u32 battlerId = gSprites[healthboxId].hMain_Battler;
    u8 *battlerTypes = GetDisplayedTypes(battlerId);

    // CAN'T DO YET, MONS MARKED AS SEEN AT START OF BATTLE
    // If Pokemon unseen, hide types
    // if (GetBattlerSide(battlerId) == B_SIDE_OPPONENT &&
    //     !GetSetPokedexFlag(SpeciesToNationalPokedexNum(gBattleMons[battlerId].species), FLAG_GET_SEEN))
    //     return TRUE;

    switch(typeSlot)
    {
        case 0:
            gSprites[spriteIds[0]].invisible = battlerTypes[0] == TYPE_MYSTERY && (battlerTypes[1] != TYPE_MYSTERY || battlerTypes[2] != TYPE_MYSTERY);
        case 1:
            gSprites[spriteIds[1]].invisible = battlerTypes[1] == TYPE_MYSTERY || battlerTypes[0] == battlerTypes[1];
        case 2:
            gSprites[spriteIds[2]].invisible = battlerTypes[2] == TYPE_MYSTERY;
    }
}

void TypeSymbols_SetVisibilities(u32 healthboxId, bool32 invisible)
{
    u32 i;
    u8 *spriteIds = TypeSymbols_GetSpriteIds(healthboxId);
    u32 battlerId = gSprites[healthboxId].hMain_Battler;

    for (i = 0; i < 3; i++)
    {
        if (invisible == TRUE)
            gSprites[spriteIds[i]].invisible = TRUE;
        else // Try visible.
            TypeSymbols_ShouldBeInvisible(healthboxId, i);
    }
}

static void TypeSymbols_UpdateOamPriorities(u32 healthboxId, u32 oamPriority)
{
    u32 i;
    u8 *spriteIds = TypeSymbols_GetSpriteIds(healthboxId);
    for (i = 0; i < 3; i++)
        gSprites[spriteIds[i]].oam.priority = oamPriority;
}

void UpdateTypeSymbols(u32 battlerId, u32 healthboxSpriteId)
{  
    u8 i;

    SetTypeSymbolPos(battlerId, healthboxSpriteId);
    SetTypeSymbolSpriteAndPal(battlerId, healthboxSpriteId);

    for (i = 0; i < 3; i++)
        TypeSymbols_ShouldBeInvisible(healthboxSpriteId, i);
}

static void SpriteCb_TypeSymbols(struct Sprite *sprite)
{
    u32 battlerId = sprite->tBattler;

    sprite->x2 = gSprites[gHealthboxSpriteIds[battlerId]].x2;
    sprite->y2 = gSprites[gHealthboxSpriteIds[battlerId]].y2;
}

static bool8 Indicator_SetIcon(u32 battlerId, u8 spriteId)
{
    u8 indicatorGfxId = INDICATOR_GFX_BLANK;
    u32 side = GetBattlerSide(battlerId);
    
    // Mega/Primals/Dynamax/Tera are prioritized
    // Caught indicator only shows in wild battle for opponents
    // Otherwise, no indicator

    if (gBattleStruct->mega.evolvedPartyIds[side] & gBitTable[gBattlerPartyIndexes[battlerId]])
        indicatorGfxId = INDICATOR_GFX_MEGA;
    else if (gBattleStruct->mega.primalRevertedPartyIds[side] & gBitTable[gBattlerPartyIndexes[battlerId]])
    {
        if (gBattleMons[battlerId].species == SPECIES_KYOGRE_PRIMAL)
            indicatorGfxId = INDICATOR_GFX_ALPHA;
        else if (gBattleMons[battlerId].species == SPECIES_GROUDON_PRIMAL)
            indicatorGfxId = INDICATOR_GFX_OMEGA;
    }
    // else if (DYNAMAX)
    //     indicatorGfxId = INDICATOR_GFX_DYNAMAX;
    // else if (TERASTAL)
    //     indicatorGfxId = INDICATOR_GFX_TERASTAL;
    else if (GetBattlerSide(battlerId) != B_SIDE_PLAYER
             && GetSetPokedexFlag(SpeciesToNationalPokedexNum(GetMonData(&gEnemyParty[gBattlerPartyIndexes[battlerId]], MON_DATA_SPECIES)), FLAG_GET_CAUGHT)
             && !(gBattleTypeFlags & BATTLE_TYPE_WALLY_TUTORIAL)
             && !(gBattleTypeFlags & BATTLE_TYPE_TRAINER))
        indicatorGfxId = INDICATOR_GFX_CAUGHT;

    CpuCopy32(GetIndicatorGfxPtr(indicatorGfxId),
                (void *)(OBJ_VRAM0 + (gSprites[spriteId].oam.tileNum) * TILE_SIZE_4BPP),
                64);
    
    return indicatorGfxId != INDICATOR_GFX_BLANK;
}

void Indicator_SetVisibility(u32 healthboxId, bool32 invisible)
{
    u32 i;
    u8 spriteId = gSprites[healthboxId].hMain_IndicatorId;
    u32 battlerId = gSprites[healthboxId].hMain_Battler;

    if (invisible == TRUE)
        gSprites[spriteId].invisible = TRUE;
    else // Try visible.
        gSprites[spriteId].invisible = !Indicator_SetIcon(battlerId, spriteId);
}

static void Indicator_UpdateOamPriorities(u32 healthboxId, u32 oamPriority)
{
    u8 spriteId = gSprites[healthboxId].hMain_IndicatorId;
    gSprites[spriteId].oam.priority = oamPriority;
}

static void Indicator_CreateSprite(u32 battlerId, u32 healthboxSpriteId)
{
    u32 position;
    u8 spriteId;
    s16 xHealthbox = 0, y = 0;
    s32 x = 0;

    position = GetBattlerPosition(battlerId);
    GetBattlerHealthboxCoords(battlerId, &xHealthbox, &y);

    x = sIndicatorPositions[position][0];
    y += sIndicatorPositions[position][1];

    if (WhichBattleCoords(battlerId) == 0) // Singles
    {
        if (GetBattlerSide(battlerId) == B_SIDE_PLAYER)
            spriteId = CreateSpriteAtEnd(&sIndicatorPlayerSpriteTemplates[0], 0, y, 0);
        else
            spriteId = CreateSpriteAtEnd(&sIndicatorOpponentSpriteTemplates[0], 0, y, 0);
    }
    else // Doubles
    {
        if (GetBattlerSide(battlerId) == B_SIDE_PLAYER)
            spriteId = CreateSpriteAtEnd(&sIndicatorPlayerSpriteTemplates[position/2], 0, y, 0);
        else
            spriteId = CreateSpriteAtEnd(&sIndicatorOpponentSpriteTemplates[position/2], 0, y, 0);
    }

    gSprites[spriteId].tBattler = battlerId;
    gSprites[spriteId].tPosX = x;
    gSprites[spriteId].invisible = TRUE;

    gSprites[healthboxSpriteId].hMain_IndicatorId = spriteId;

    CpuCopy32(GetIndicatorGfxPtr(INDICATOR_GFX_BLANK),
              (void *)(OBJ_VRAM0 + (gSprites[spriteId].oam.tileNum) * TILE_SIZE_4BPP),
              64);

}

static const u8 *GetIndicatorGfxPtr(u8 indicatorId)
{
    return gBattleInterface_IndicatorsGfxTable[indicatorId];
}

static void SpriteCb_Indicator(struct Sprite *sprite)
{
    u32 battlerId = sprite->tBattler;

    sprite->x = gSprites[gHealthboxSpriteIds[battlerId]].x + sprite->tPosX + sprite->tLevelXDelta;
    sprite->x2 = gSprites[gHealthboxSpriteIds[battlerId]].x2;
    sprite->y2 = gSprites[gHealthboxSpriteIds[battlerId]].y2;
}

static const s8 sStatusPositions[][2] =
{
    [B_POSITION_PLAYER_LEFT] = {16, 4},
    [B_POSITION_OPPONENT_LEFT] = {-8, 4},
    [B_POSITION_PLAYER_RIGHT] = {16, 4},
    [B_POSITION_OPPONENT_RIGHT] = {-8, 4},
};

static const struct OamData sOamData_Status =
{
    .y = 0,
    .affineMode = ST_OAM_AFFINE_OFF,
    .objMode = ST_OAM_OBJ_NORMAL,
    .mosaic = FALSE,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(16x8),
    .x = 0,
    .matrixNum = 0,
    .size = SPRITE_SIZE(16x8),
    .tileNum = 0,
    .priority = 1,
    .paletteNum = 0,
    .affineParam = 0,
};

static const struct SpriteTemplate sStatusPlayerSpriteTemplates[2] =
{
    {
        .tileTag = TAG_STATUS_PLAYER1_TILE,
        .paletteTag = 50000,
        .oam = &sOamData_Status,
        .anims = gDummySpriteAnimTable,
        .images = NULL,
        .affineAnims = gDummySpriteAffineAnimTable,
        .callback = SpriteCb_Status
    },
    {
        .tileTag = TAG_STATUS_PLAYER2_TILE,
        .paletteTag = 50000,
        .oam = &sOamData_Status,
        .anims = gDummySpriteAnimTable,
        .images = NULL,
        .affineAnims = gDummySpriteAffineAnimTable,
        .callback = SpriteCb_Status
    }
};

static const struct SpriteTemplate sStatusOpponentSpriteTemplates[2] =
{
    {
        .tileTag = TAG_STATUS_OPPONENT1_TILE,
        .paletteTag = 50000,
        .oam = &sOamData_Status,
        .anims = gDummySpriteAnimTable,
        .images = NULL,
        .affineAnims = gDummySpriteAffineAnimTable,
        .callback = SpriteCb_Status
    },
    {
        .tileTag = TAG_STATUS_OPPONENT2_TILE,
        .paletteTag = 50000,
        .oam = &sOamData_Status,
        .anims = gDummySpriteAnimTable,
        .images = NULL,
        .affineAnims = gDummySpriteAffineAnimTable,
        .callback = SpriteCb_Status
    }
};

static const u8 sStatusGfxToOamPaletteNum[] =
{
    [STATUS_GFX_PARALYZE] = 15,
    [STATUS_GFX_BURN] = 15,
    [STATUS_GFX_POISON] = 15,
    [STATUS_GFX_BADLY_POISON] = 15,
    [STATUS_GFX_FROZEN] = 14,
    //[STATUS_GFX_FROSTBITE] = 14,
    [STATUS_GFX_SLEEP] = 14,
    //[STATUS_GFX_DROWSY] = 14
};

static bool8 Status_SetIcon(u32 battlerId, u8 spriteId)
{
    u32 status;
    u8 statusGfxId = STATUS_GFX_BLANK;
    u32 side = GetBattlerSide(battlerId);
    
    if (GetBattlerSide(battlerId) == B_SIDE_PLAYER)
        status = GetMonData(&gPlayerParty[gBattlerPartyIndexes[battlerId]], MON_DATA_STATUS);
    else
        status = GetMonData(&gEnemyParty[gBattlerPartyIndexes[battlerId]], MON_DATA_STATUS);

    if (status & STATUS1_PARALYSIS)
        statusGfxId = STATUS_GFX_PARALYZE;
    else if (status & STATUS1_BURN)
        statusGfxId = STATUS_GFX_BURN;
    else if (status & STATUS1_POISON)
        statusGfxId = STATUS_GFX_POISON;
    else if (status & STATUS1_TOXIC_POISON)
        statusGfxId = STATUS_GFX_BADLY_POISON;
    else if (status & STATUS1_FREEZE)
        statusGfxId = STATUS_GFX_FROZEN;
    // else if (status & STATUS1_FROSTBITE)
    //     statusGfxId = STATUS_GFX_FROSTBITE;
    else if (status & STATUS1_SLEEP)
        statusGfxId = STATUS_GFX_SLEEP;
    // else if (status & STATUS1_DROWSY)
    //     statusGfxId = STATUS_GFX_DROWSY;

    CpuCopy32(GetStatusGfxPtr(statusGfxId),
                (void *)(OBJ_VRAM0 + (gSprites[spriteId].oam.tileNum) * TILE_SIZE_4BPP),
                64);
    gSprites[spriteId].oam.paletteNum = sStatusGfxToOamPaletteNum[statusGfxId];

    return statusGfxId != STATUS_GFX_BLANK;
}

void Status_SetVisibility(u32 healthboxId, bool32 invisible)
{
    u32 i;
    u8 spriteId = gSprites[healthboxId].hMain_StatusId;
    u32 battlerId = gSprites[healthboxId].hMain_Battler;

    if (invisible == TRUE)
        gSprites[spriteId].invisible = TRUE;
    else // Try visible.
        gSprites[spriteId].invisible = !Status_SetIcon(battlerId, spriteId);
}

static void Status_UpdateOamPriorities(u32 healthboxId, u32 oamPriority)
{
    u8 spriteId = gSprites[healthboxId].hMain_StatusId;
    gSprites[spriteId].oam.priority = oamPriority;
}

static void Status_CreateSprite(u32 battlerId, u32 healthboxSpriteId)
{
    u32 position;
    u8 spriteId;
    s16 xHealthbox = 0, y = 0;
    s32 x = 0;    

    position = GetBattlerPosition(battlerId);
    GetBattlerHealthboxCoords(battlerId, &xHealthbox, &y);

    x = sStatusPositions[position][0];
    y += sStatusPositions[position][1];

    if (WhichBattleCoords(battlerId) == 0) // Singles
    {
        if (GetBattlerSide(battlerId) == B_SIDE_PLAYER)
            spriteId = CreateSpriteAtEnd(&sStatusPlayerSpriteTemplates[0], 0, y, 0);
        else
            spriteId = CreateSpriteAtEnd(&sStatusOpponentSpriteTemplates[0], 0, y, 0);
    }
    else // Doubles
    {
        if (GetBattlerSide(battlerId) == B_SIDE_PLAYER)
            spriteId = CreateSpriteAtEnd(&sStatusPlayerSpriteTemplates[position/2], 0, y, 0);
        else
            spriteId = CreateSpriteAtEnd(&sStatusOpponentSpriteTemplates[position/2], 0, y, 0);
    }

    gSprites[spriteId].tBattler = battlerId;
    gSprites[spriteId].tPosX = x;
    gSprites[spriteId].invisible = TRUE;

    gSprites[healthboxSpriteId].hMain_StatusId = spriteId;

    CpuCopy32(GetStatusGfxPtr(STATUS_GFX_BLANK),
              (void *)(OBJ_VRAM0 + (gSprites[spriteId].oam.tileNum) * TILE_SIZE_4BPP),
              64);
}

static const u8 *GetStatusGfxPtr(u8 statusId)
{
    return gBattleInterface_StatusGfxTable[statusId];
}

static void SpriteCb_Status(struct Sprite *sprite)
{
    u32 battlerId = sprite->tBattler;

    sprite->x = gSprites[gHealthboxSpriteIds[battlerId]].x + sprite->tPosX;
    sprite->x2 = gSprites[gHealthboxSpriteIds[battlerId]].x2;
    sprite->y2 = gSprites[gHealthboxSpriteIds[battlerId]].y2;
}

static void UpdateStatusIconInHealthbox(u8 healthboxSpriteId)
{
    Status_SetVisibility(healthboxSpriteId, FALSE);
}

#undef tBattler
#undef tType
#undef tPosX
#undef tLevelXDelta

#define tBattler                data[0]
#define tSummaryBarSpriteId     data[1]
#define tBallIconSpriteId(n)    data[3 + n]
#define tIsBattleStart          data[10]
#define tBlend                  data[15]

u8 CreatePartyStatusSummarySprites(u8 battlerId, struct HpAndStatus *partyInfo, bool8 skipPlayer, bool8 isBattleStart)
{
    bool8 isOpponent;
    s16 bar_X, bar_Y, bar_pos2_X, bar_data0;
    s32 i, j, var;
    u8 summaryBarSpriteId;
    u8 ballIconSpritesIds[PARTY_SIZE];
    u8 taskId;

    if (!skipPlayer || GetBattlerPosition(battlerId) != B_POSITION_OPPONENT_RIGHT)
    {
        if (GetBattlerSide(battlerId) == B_SIDE_PLAYER)
        {
            isOpponent = FALSE;
            bar_X = 136, bar_Y = 96;
            bar_pos2_X = 100;
            bar_data0 = -5;
        }
        else
        {
            isOpponent = TRUE;

            if (!skipPlayer || !WhichBattleCoords(battlerId))
                bar_X = 104, bar_Y = 40;
            else
                bar_X = 104, bar_Y = 16;

            bar_pos2_X = -100;
            bar_data0 = 5;
        }
    }
    else
    {
        isOpponent = TRUE;
        bar_X = 104, bar_Y = 40;
        bar_pos2_X = -100;
        bar_data0 = 5;
    }

    LoadCompressedSpriteSheetUsingHeap(&sStatusSummaryBarSpriteSheet);
    LoadSpriteSheet(&sStatusSummaryBallsSpriteSheet);
    LoadSpritePalette(&sStatusSummaryBarSpritePal);
    LoadSpritePalette(&sStatusSummaryBallsSpritePal);

    summaryBarSpriteId = CreateSprite(&sStatusSummaryBarSpriteTemplates[isOpponent], bar_X, bar_Y, 10);
    SetSubspriteTables(&gSprites[summaryBarSpriteId], sStatusSummaryBar_SubspriteTable_Enter);
    gSprites[summaryBarSpriteId].x2 = bar_pos2_X;
    gSprites[summaryBarSpriteId].data[0] = bar_data0;

    if (isOpponent)
    {
        gSprites[summaryBarSpriteId].x -= 96;
        gSprites[summaryBarSpriteId].oam.matrixNum = ST_OAM_HFLIP;
    }
    else
    {
        gSprites[summaryBarSpriteId].x += 96;
    }

    for (i = 0; i < PARTY_SIZE; i++)
    {
        ballIconSpritesIds[i] = CreateSpriteAtEnd(&sStatusSummaryBallsSpriteTemplates[isOpponent], bar_X, bar_Y - 4, 9);

        if (!isBattleStart)
            gSprites[ballIconSpritesIds[i]].callback = SpriteCB_StatusSummaryBalls_OnSwitchout;

        if (!isOpponent)
        {
            gSprites[ballIconSpritesIds[i]].x2 = 0;
            gSprites[ballIconSpritesIds[i]].y2 = 0;
        }

        gSprites[ballIconSpritesIds[i]].data[0] = summaryBarSpriteId;

        if (!isOpponent)
        {
            gSprites[ballIconSpritesIds[i]].x += 10 * i + 24;
            gSprites[ballIconSpritesIds[i]].data[1] = i * 7 + 10;
            gSprites[ballIconSpritesIds[i]].x2 = 120;
        }
        else
        {
            gSprites[ballIconSpritesIds[i]].x -= 10 * (5 - i) + 24;
            gSprites[ballIconSpritesIds[i]].data[1] = (6 - i) * 7 + 10;
            gSprites[ballIconSpritesIds[i]].x2 = -120;
        }

        gSprites[ballIconSpritesIds[i]].data[2] = isOpponent;
    }

    if (GetBattlerSide(battlerId) == B_SIDE_PLAYER)
    {
        if (gBattleTypeFlags & BATTLE_TYPE_MULTI)
        {
            for (i = 0; i < PARTY_SIZE; i++)
            {
                if (partyInfo[i].hp == HP_EMPTY_SLOT)
                {
                    // empty slot or an egg
                    gSprites[ballIconSpritesIds[i]].oam.tileNum += 1;
                    gSprites[ballIconSpritesIds[i]].data[7] = 1;
                }
                else if (partyInfo[i].hp == 0)
                {
                    // fainted mon
                    gSprites[ballIconSpritesIds[i]].oam.tileNum += 3;
                }
                else if (partyInfo[i].status != 0)
                {
                    // mon with major status
                    gSprites[ballIconSpritesIds[i]].oam.tileNum += 2;
                }
            }
        }
        else
        {
            for (i = 0, var = PARTY_SIZE - 1, j = 0; j < PARTY_SIZE; j++)
            {
                if (partyInfo[j].hp == HP_EMPTY_SLOT)
                {
                     // empty slot or an egg
                    gSprites[ballIconSpritesIds[var]].oam.tileNum += 1;
                    gSprites[ballIconSpritesIds[var]].data[7] = 1;
                    var--;
                    continue;
                }
                else if (partyInfo[j].hp == 0)
                {
                    // fainted mon
                    gSprites[ballIconSpritesIds[i]].oam.tileNum += 3;
                }
                else if (gBattleTypeFlags & BATTLE_TYPE_ARENA && gBattleStruct->arenaLostPlayerMons & gBitTable[j])
                {
                    // fainted arena mon
                    gSprites[ballIconSpritesIds[i]].oam.tileNum += 3;
                }
                else if (partyInfo[j].status != 0)
                {
                    // mon with primary status
                    gSprites[ballIconSpritesIds[i]].oam.tileNum += 2;
                }
                i++;
            }
        }
    }
    else
    {
        if (gBattleTypeFlags & (BATTLE_TYPE_MULTI | BATTLE_TYPE_TWO_OPPONENTS))
        {
            for (var = PARTY_SIZE - 1, i = 0; i < PARTY_SIZE; i++)
            {
                if (partyInfo[i].hp == HP_EMPTY_SLOT)
                {
                    // empty slot or an egg
                    gSprites[ballIconSpritesIds[var]].oam.tileNum += 1;
                    gSprites[ballIconSpritesIds[var]].data[7] = 1;
                }
                else if (partyInfo[i].hp == 0)
                {
                    // fainted mon
                    gSprites[ballIconSpritesIds[var]].oam.tileNum += 3;
                }
                else if (partyInfo[i].status != 0)
                {
                    // mon with primary status
                    gSprites[ballIconSpritesIds[var]].oam.tileNum += 2;
                }
                var--;
            }
        }
        else
        {
            for (var = 0, i = 0, j = 0; j < PARTY_SIZE; j++)
            {
                if (partyInfo[j].hp == HP_EMPTY_SLOT)
                {
                    // empty slot or an egg
                    gSprites[ballIconSpritesIds[i]].oam.tileNum += 1;
                    gSprites[ballIconSpritesIds[i]].data[7] = 1;
                    i++;
                    continue;
                }
                else if (partyInfo[j].hp == 0)
                {
                     // fainted mon
                    gSprites[ballIconSpritesIds[PARTY_SIZE - 1 - var]].oam.tileNum += 3;
                }
                else if (gBattleTypeFlags & BATTLE_TYPE_ARENA && gBattleStruct->arenaLostOpponentMons & gBitTable[j])
                {
                     // fainted arena mon
                    gSprites[ballIconSpritesIds[PARTY_SIZE - 1 - var]].oam.tileNum += 3;
                }
                else if (partyInfo[j].status != 0)
                {
                     // mon with primary status
                    gSprites[ballIconSpritesIds[PARTY_SIZE - 1 - var]].oam.tileNum += 2;
                }
                var++;
            }
        }
    }

    taskId = CreateTask(TaskDummy, 5);
    gTasks[taskId].tBattler = battlerId;
    gTasks[taskId].tSummaryBarSpriteId = summaryBarSpriteId;

    for (i = 0; i < PARTY_SIZE; i++)
        gTasks[taskId].tBallIconSpriteId(i) = ballIconSpritesIds[i];

    gTasks[taskId].tIsBattleStart = isBattleStart;

    if (isBattleStart)
    {
        gBattleSpritesDataPtr->animationData->field_9_x1C++;
    }

    PlaySE12WithPanning(SE_BALL_TRAY_ENTER, 0);
    return taskId;
}

// Slide the party summary tray back offscreen
void Task_HidePartyStatusSummary(u8 taskId)
{
    u8 ballIconSpriteIds[PARTY_SIZE];
    bool8 isBattleStart;
    u8 summaryBarSpriteId;
    u8 battlerId;
    s32 i;

    isBattleStart = gTasks[taskId].tIsBattleStart;
    summaryBarSpriteId = gTasks[taskId].tSummaryBarSpriteId;
    battlerId = gTasks[taskId].tBattler;

    for (i = 0; i < PARTY_SIZE; i++)
        ballIconSpriteIds[i] = gTasks[taskId].tBallIconSpriteId(i);

    SetGpuReg(REG_OFFSET_BLDCNT, BLDCNT_TGT2_ALL | BLDCNT_EFFECT_BLEND);
    SetGpuReg(REG_OFFSET_BLDALPHA, BLDALPHA_BLEND(16, 0));

    gTasks[taskId].tBlend = 16;

    for (i = 0; i < PARTY_SIZE; i++)
        gSprites[ballIconSpriteIds[i]].oam.objMode = ST_OAM_OBJ_BLEND;

    gSprites[summaryBarSpriteId].oam.objMode = ST_OAM_OBJ_BLEND;

    if (isBattleStart)
    {
        for (i = 0; i < PARTY_SIZE; i++)
        {
            if (GetBattlerSide(battlerId) != B_SIDE_PLAYER)
            {
                gSprites[ballIconSpriteIds[PARTY_SIZE - 1 - i]].data[1] = 7 * i;
                gSprites[ballIconSpriteIds[PARTY_SIZE - 1 - i]].data[3] = 0;
                gSprites[ballIconSpriteIds[PARTY_SIZE - 1 - i]].data[4] = 0;
                gSprites[ballIconSpriteIds[PARTY_SIZE - 1 - i]].callback = SpriteCB_StatusSummaryBalls_Exit;
            }
            else
            {
                gSprites[ballIconSpriteIds[i]].data[1] = 7 * i;
                gSprites[ballIconSpriteIds[i]].data[3] = 0;
                gSprites[ballIconSpriteIds[i]].data[4] = 0;
                gSprites[ballIconSpriteIds[i]].callback = SpriteCB_StatusSummaryBalls_Exit;
            }
        }
        gSprites[summaryBarSpriteId].data[0] /= 2;
        gSprites[summaryBarSpriteId].data[1] = 0;
        gSprites[summaryBarSpriteId].callback = SpriteCB_StatusSummaryBar_Exit;
        SetSubspriteTables(&gSprites[summaryBarSpriteId], sStatusSummaryBar_SubspriteTable_Exit);
        gTasks[taskId].func = Task_HidePartyStatusSummary_BattleStart_1;
    }
    else
    {
        gTasks[taskId].func = Task_HidePartyStatusSummary_DuringBattle;
    }
}

static void Task_HidePartyStatusSummary_BattleStart_1(u8 taskId)
{
    if ((gTasks[taskId].data[11]++ % 2) == 0)
    {
        if (--gTasks[taskId].tBlend < 0)
            return;

        SetGpuReg(REG_OFFSET_BLDALPHA, BLDALPHA_BLEND(gTasks[taskId].tBlend, 16 - gTasks[taskId].tBlend));
    }
    if (gTasks[taskId].tBlend == 0)
        gTasks[taskId].func = Task_HidePartyStatusSummary_BattleStart_2;
}

static void Task_HidePartyStatusSummary_BattleStart_2(u8 taskId)
{
    u8 ballIconSpriteIds[PARTY_SIZE];
    s32 i;

    u8 battlerId = gTasks[taskId].tBattler;
    if (--gTasks[taskId].tBlend == -1)
    {
        u8 summaryBarSpriteId = gTasks[taskId].tSummaryBarSpriteId;

        for (i = 0; i < PARTY_SIZE; i++)
            ballIconSpriteIds[i] = gTasks[taskId].tBallIconSpriteId(i);

        gBattleSpritesDataPtr->animationData->field_9_x1C--;
        if (gBattleSpritesDataPtr->animationData->field_9_x1C == 0)
        {
            DestroySpriteAndFreeResources(&gSprites[summaryBarSpriteId]);
            DestroySpriteAndFreeResources(&gSprites[ballIconSpriteIds[0]]);
        }
        else
        {
            FreeSpriteOamMatrix(&gSprites[summaryBarSpriteId]);
            DestroySprite(&gSprites[summaryBarSpriteId]);
            FreeSpriteOamMatrix(&gSprites[ballIconSpriteIds[0]]);
            DestroySprite(&gSprites[ballIconSpriteIds[0]]);
        }

        for (i = 1; i < PARTY_SIZE; i++)
            DestroySprite(&gSprites[ballIconSpriteIds[i]]);
    }
    else if (gTasks[taskId].tBlend == -3)
    {
        gBattleSpritesDataPtr->healthBoxesData[battlerId].partyStatusSummaryShown = 0;
        SetGpuReg(REG_OFFSET_BLDCNT, 0);
        SetGpuReg(REG_OFFSET_BLDALPHA, 0);
        DestroyTask(taskId);
    }
}

static void Task_HidePartyStatusSummary_DuringBattle(u8 taskId)
{
    u8 ballIconSpriteIds[PARTY_SIZE];
    s32 i;
    u8 battlerId = gTasks[taskId].tBattler;

    if (--gTasks[taskId].tBlend >= 0)
    {
        SetGpuReg(REG_OFFSET_BLDALPHA, BLDALPHA_BLEND(gTasks[taskId].tBlend, 16 - gTasks[taskId].tBlend));
    }
    else if (gTasks[taskId].tBlend == -1)
    {
        u8 summaryBarSpriteId = gTasks[taskId].tSummaryBarSpriteId;

        for (i = 0; i < PARTY_SIZE; i++)
            ballIconSpriteIds[i] = gTasks[taskId].tBallIconSpriteId(i);

        DestroySpriteAndFreeResources(&gSprites[summaryBarSpriteId]);
        DestroySpriteAndFreeResources(&gSprites[ballIconSpriteIds[0]]);

        for (i = 1; i < PARTY_SIZE; i++)
            DestroySprite(&gSprites[ballIconSpriteIds[i]]);
    }
    else if (gTasks[taskId].tBlend == -3)
    {
        gBattleSpritesDataPtr->healthBoxesData[battlerId].partyStatusSummaryShown = 0;
        SetGpuReg(REG_OFFSET_BLDCNT, 0);
        SetGpuReg(REG_OFFSET_BLDALPHA, 0);
        DestroyTask(taskId);
    }
}

#undef tBattler
#undef tSummaryBarSpriteId
#undef tBallIconSpriteId
#undef tIsBattleStart
#undef tBlend

static void SpriteCB_StatusSummaryBar_Enter(struct Sprite *sprite)
{
    if (sprite->x2 != 0)
        sprite->x2 += sprite->data[0];
}

static void SpriteCB_StatusSummaryBar_Exit(struct Sprite *sprite)
{
    sprite->data[1] += 32;
    if (sprite->data[0] > 0)
        sprite->x2 += sprite->data[1] >> 4;
    else
        sprite->x2 -= sprite->data[1] >> 4;
    sprite->data[1] &= 0xF;
}

static void SpriteCB_StatusSummaryBalls_Enter(struct Sprite *sprite)
{
    u8 var1;
    u16 var2;
    s8 pan;

    if (sprite->data[1] > 0)
    {
        sprite->data[1]--;
        return;
    }

    var1 = sprite->data[2];
    var2 = sprite->data[3];
    var2 += 56;
    sprite->data[3] = var2 & 0xFFF0;

    if (var1 != 0)
    {
        sprite->x2 += var2 >> 4;
        if (sprite->x2 > 0)
            sprite->x2 = 0;
    }
    else
    {
        sprite->x2 -= var2 >> 4;
        if (sprite->x2 < 0)
            sprite->x2 = 0;
    }

    if (sprite->x2 == 0)
    {
        pan = SOUND_PAN_TARGET;
        if (var1 != 0)
            pan = SOUND_PAN_ATTACKER;

        if (sprite->data[7] != 0)
            PlaySE2WithPanning(SE_BALL_TRAY_EXIT, pan);
        else
            PlaySE1WithPanning(SE_BALL_TRAY_BALL, pan);

        sprite->callback = SpriteCallbackDummy;
    }
}

static void SpriteCB_StatusSummaryBalls_Exit(struct Sprite *sprite)
{
    u8 var1;
    u16 var2;

    if (sprite->data[1] > 0)
    {
        sprite->data[1]--;
        return;
    }
    var1 = sprite->data[2];
    var2 = sprite->data[3];
    var2 += 56;
    sprite->data[3] = var2 & 0xFFF0;
    if (var1 != 0)
        sprite->x2 += var2 >> 4;
    else
        sprite->x2 -= var2 >> 4;
    if (sprite->x2 + sprite->x > 248
     || sprite->x2 + sprite->x < -8)
    {
        sprite->invisible = TRUE;
        sprite->callback = SpriteCallbackDummy;
    }
}

static void SpriteCB_StatusSummaryBalls_OnSwitchout(struct Sprite *sprite)
{
    u8 barSpriteId = sprite->data[0];

    sprite->x2 = gSprites[barSpriteId].x2;
    sprite->y2 = gSprites[barSpriteId].y2;
}

static void UpdateNickInHealthbox(u8 healthboxSpriteId, struct Pokemon *mon)
{
    u8 nickname[POKEMON_NAME_LENGTH + 1];
    void *ptr;
    u32 windowId, spriteTileNum, species;
    u8 *windowTileData;
    u8 gender;
    
    struct Pokemon *illusionMon = GetIllusionMonPtr(gSprites[healthboxSpriteId].hMain_Battler);
    if (illusionMon != NULL)
        mon = illusionMon;        

    GetMonData(mon, MON_DATA_NICKNAME, nickname);
    ptr = StringAppend(gDisplayedStringBattle, nickname);

    windowTileData = AddTextPrinterAndCreateWindowOnHealthbox(nickname, 3, 2, FONT_SMALL_NARROW_OUTLINED, 2, 1, 0, &windowId);

    spriteTileNum = gSprites[healthboxSpriteId].oam.tileNum * TILE_SIZE_4BPP;

    if (GetBattlerSide(gSprites[healthboxSpriteId].data[6]) == B_SIDE_PLAYER)
    {
        TextIntoHealthboxObject((void *)(OBJ_VRAM0 + 0x40 + spriteTileNum - 32), windowTileData, 7); // Left half sprite of HealthBox
        ptr = (void *)(OBJ_VRAM0);
        if (!WhichBattleCoords(gSprites[healthboxSpriteId].data[6]))
            ptr += spriteTileNum + 0x800;
        else
            ptr += spriteTileNum + 0x400;
        TextIntoHealthboxObject(ptr, windowTileData + 0xC0 + 32, 1); // Right half sprite of HealthBox
    }
    else
    {
        TextIntoHealthboxObject((void *)(OBJ_VRAM0 + 0x20 + spriteTileNum + 64), windowTileData, 5); // Left half sprite of HealthBox
        ptr = (void *)(OBJ_VRAM0);
        if (!WhichBattleCoords(gSprites[healthboxSpriteId].data[6]))
            ptr += spriteTileNum + 0x400;
        else
            ptr += spriteTileNum + 0x400;
        TextIntoHealthboxObject(ptr, windowTileData + 0xC0 - 32, 3); // Right half sprite of HealthBox
    }

    RemoveWindowOnHealthbox(windowId);
}

static void TryAddPokeballIconToHealthbox(u8 healthboxSpriteId, bool8 noStatus)
{
    u8 battlerId, healthBarSpriteId;

    if (gBattleTypeFlags & BATTLE_TYPE_WALLY_TUTORIAL)
        return;
    if (gBattleTypeFlags & BATTLE_TYPE_TRAINER)
        return;

    battlerId = gSprites[healthboxSpriteId].hMain_Battler;
    if (GetBattlerSide(battlerId) == B_SIDE_PLAYER)
        return;
    if (!GetSetPokedexFlag(SpeciesToNationalPokedexNum(GetMonData(&gEnemyParty[gBattlerPartyIndexes[battlerId]], MON_DATA_SPECIES)), FLAG_GET_CAUGHT))
        return;

    healthBarSpriteId = gSprites[healthboxSpriteId].hMain_HealthBarSpriteId;

    if (noStatus)
        CpuCopy32(GetHealthboxElementGfxPtr(HEALTHBOX_GFX_STATUS_BALL_CAUGHT), (void *)(OBJ_VRAM0 + (gSprites[healthBarSpriteId].oam.tileNum + 8) * TILE_SIZE_4BPP), 32);
    else
        CpuFill32(0, (void *)(OBJ_VRAM0 + (gSprites[healthBarSpriteId].oam.tileNum + 8) * TILE_SIZE_4BPP), 32);
}

static void UpdateSafariBallsTextOnHealthbox(u8 healthboxSpriteId)
{
    u32 windowId, spriteTileNum;
    u8 *windowTileData;

    windowTileData = AddTextPrinterAndCreateWindowOnHealthbox(gText_SafariBalls, 0, 3, FONT_SMALL_OUTLINED, 2, 1, 0, &windowId);
    spriteTileNum = gSprites[healthboxSpriteId].oam.tileNum * TILE_SIZE_4BPP;
    TextIntoHealthboxObject((void *)(OBJ_VRAM0 + 0x40) + spriteTileNum, windowTileData, 6);
    TextIntoHealthboxObject((void *)(OBJ_VRAM0 + 0x800) + spriteTileNum, windowTileData + 0xC0, 2);
    RemoveWindowOnHealthbox(windowId);
}

static void UpdateLeftNoOfBallsTextOnHealthbox(u8 healthboxSpriteId)
{
    u8 text[16];
    u8 *txtPtr;
    u32 windowId, spriteTileNum;
    u8 *windowTileData;

    txtPtr = StringCopy(text, gText_SafariBallLeft);
    ConvertIntToDecimalStringN(txtPtr, gNumSafariBalls, STR_CONV_MODE_LEFT_ALIGN, 2);

    windowTileData = AddTextPrinterAndCreateWindowOnHealthbox(text, GetStringRightAlignXOffset(FONT_SMALL_OUTLINED, text, 0x2F), 3, FONT_SMALL_OUTLINED, 2, 1, 0, &windowId);
    spriteTileNum = gSprites[healthboxSpriteId].oam.tileNum * TILE_SIZE_4BPP;
    SafariTextIntoHealthboxObject((void *)(OBJ_VRAM0 + 0x2C0) + spriteTileNum, windowTileData, 2);
    SafariTextIntoHealthboxObject((void *)(OBJ_VRAM0 + 0xA00) + spriteTileNum, windowTileData + 0x40, 4);
    RemoveWindowOnHealthbox(windowId);
}

void UpdateHealthboxAttribute(u8 healthboxSpriteId, struct Pokemon *mon, u8 elementId)
{
    u32 battlerId = gSprites[healthboxSpriteId].hMain_Battler;
    s32 maxHp = GetMonData(mon, MON_DATA_MAX_HP);
    s32 currHp = GetMonData(mon, MON_DATA_HP);

    if (GetBattlerSide(battlerId) == B_SIDE_PLAYER)
    {
        u8 isDoubles = WhichBattleCoords(battlerId);

        if (elementId == HEALTHBOX_LEVEL || elementId == HEALTHBOX_ALL)
            UpdateLvlInHealthbox(healthboxSpriteId, mon);

        if (elementId == HEALTHBOX_ALL)
            UpdateHpTextInHealthbox(healthboxSpriteId, HP_BOTH, currHp, maxHp);
        else if (elementId == HEALTHBOX_MAX_HP)
            UpdateHpTextInHealthbox(healthboxSpriteId, HP_MAX, currHp, maxHp);
        else if (elementId == HEALTHBOX_CURRENT_HP)
            UpdateHpTextInHealthbox(healthboxSpriteId, HP_CURRENT, currHp, maxHp);

        if (elementId == HEALTHBOX_HEALTH_BAR || elementId == HEALTHBOX_ALL)
        {
            SetBattleBarStruct(battlerId, healthboxSpriteId, maxHp, currHp, 0);
            MoveBattleBar(battlerId, healthboxSpriteId, HEALTH_BAR, 0);
        }

        if (!isDoubles && (elementId == HEALTHBOX_EXP_BAR || elementId == HEALTHBOX_ALL))
        {
            u16 species;
            u32 exp, currLevelExp;
            s32 currExpBarValue, maxExpBarValue;
            u8 level;

            species = GetMonData(mon, MON_DATA_SPECIES);
            level = GetMonData(mon, MON_DATA_LEVEL);
            exp = GetMonData(mon, MON_DATA_EXP);
            currLevelExp = gExperienceTables[gSpeciesInfo[species].growthRate][level];
            currExpBarValue = exp - currLevelExp;
            maxExpBarValue = gExperienceTables[gSpeciesInfo[species].growthRate][level + 1] - currLevelExp;
            SetBattleBarStruct(battlerId, healthboxSpriteId, maxExpBarValue, currExpBarValue, isDoubles);
            MoveBattleBar(battlerId, healthboxSpriteId, EXP_BAR, 0);
        }
        if (elementId == HEALTHBOX_NICK || elementId == HEALTHBOX_ALL)
            UpdateNickInHealthbox(healthboxSpriteId, mon);
        if (elementId == HEALTHBOX_STATUS_ICON || elementId == HEALTHBOX_ALL)
            UpdateStatusIconInHealthbox(healthboxSpriteId);
        if (elementId == HEALTHBOX_SAFARI_ALL_TEXT)
            UpdateSafariBallsTextOnHealthbox(healthboxSpriteId);
        if (elementId == HEALTHBOX_SAFARI_ALL_TEXT || elementId == HEALTHBOX_SAFARI_BALLS_TEXT)
            UpdateLeftNoOfBallsTextOnHealthbox(healthboxSpriteId);
        if (elementId == HEALTHBOX_TYPE || elementId == HEALTHBOX_ALL)
            UpdateTypeSymbols(battlerId, healthboxSpriteId);
    }
    else // Opponent Side
    {
        if (elementId == HEALTHBOX_LEVEL || elementId == HEALTHBOX_ALL)
            UpdateLvlInHealthbox(healthboxSpriteId, mon);
        if (gBattleSpritesDataPtr->battlerData[battlerId].hpNumbersNoBars)
        {
            if (elementId == HEALTHBOX_ALL)
                UpdateHpTextInHealthbox(healthboxSpriteId, HP_BOTH, currHp, maxHp);
            else if (elementId == HEALTHBOX_MAX_HP)
                UpdateHpTextInHealthbox(healthboxSpriteId, HP_MAX, currHp, maxHp);
            else if (elementId == HEALTHBOX_CURRENT_HP)
                UpdateHpTextInHealthbox(healthboxSpriteId, HP_CURRENT, currHp, maxHp);
        }
        if (elementId == HEALTHBOX_HEALTH_BAR || elementId == HEALTHBOX_ALL)
        {
            SetBattleBarStruct(battlerId, healthboxSpriteId, maxHp, currHp, 0);
            MoveBattleBar(battlerId, healthboxSpriteId, HEALTH_BAR, 0);
        }
        if (elementId == HEALTHBOX_NICK || elementId == HEALTHBOX_ALL)
            UpdateNickInHealthbox(healthboxSpriteId, mon);
        if (elementId == HEALTHBOX_STATUS_ICON || elementId == HEALTHBOX_ALL)
            UpdateStatusIconInHealthbox(healthboxSpriteId);
        if (elementId == HEALTHBOX_TYPE || elementId == HEALTHBOX_ALL)
            UpdateTypeSymbols(battlerId, healthboxSpriteId);
    }
}

#define B_EXPBAR_PIXELS 64
#define B_HEALTHBAR_PIXELS 48

s32 MoveBattleBar(u8 battlerId, u8 healthboxSpriteId, u8 whichBar, u8 unused)
{
    s32 currentBarValue;

    if (whichBar == HEALTH_BAR) // health bar
    {
        currentBarValue = CalcNewBarValue(gBattleSpritesDataPtr->battleBars[battlerId].maxValue,
                    gBattleSpritesDataPtr->battleBars[battlerId].oldValue,
                    gBattleSpritesDataPtr->battleBars[battlerId].receivedValue,
                    &gBattleSpritesDataPtr->battleBars[battlerId].currValue,
                #if B_FAST_HP_DRAIN == TRUE
                    B_HEALTHBAR_PIXELS / 8, max(gBattleSpritesDataPtr->battleBars[battlerId].maxValue / B_HEALTHBAR_PIXELS, 1));
                #else
                    B_HEALTHBAR_PIXELS / 8, 1);
                #endif
    }
    else // exp bar
    {
        u16 expFraction = GetScaledExpFraction(gBattleSpritesDataPtr->battleBars[battlerId].oldValue,
                    gBattleSpritesDataPtr->battleBars[battlerId].receivedValue,
                    gBattleSpritesDataPtr->battleBars[battlerId].maxValue, 8);
        if (expFraction == 0)
            expFraction = 1;
        expFraction = abs(gBattleSpritesDataPtr->battleBars[battlerId].receivedValue / expFraction);

        currentBarValue = CalcNewBarValue(gBattleSpritesDataPtr->battleBars[battlerId].maxValue,
                    gBattleSpritesDataPtr->battleBars[battlerId].oldValue,
                    gBattleSpritesDataPtr->battleBars[battlerId].receivedValue,
                    &gBattleSpritesDataPtr->battleBars[battlerId].currValue,
                    B_EXPBAR_PIXELS / 8, expFraction);
    }

    if (whichBar == EXP_BAR || (whichBar == HEALTH_BAR && !gBattleSpritesDataPtr->battlerData[battlerId].hpNumbersNoBars))
        MoveBattleBarGraphically(battlerId, whichBar);

    if (currentBarValue == -1)
        gBattleSpritesDataPtr->battleBars[battlerId].currValue = 0;

    return currentBarValue;
}

static void MoveBattleBarGraphically(u8 battlerId, u8 whichBar)
{
    u8 array[8];
    u8 filledPixelsCount, level;
    u8 barElementId;
    u8 i;
    u8 healthbarSpriteId = gSprites[gBattleSpritesDataPtr->battleBars[battlerId].healthboxSpriteId].hMain_HealthBarSpriteId;

    switch (whichBar)
    {
    case HEALTH_BAR:
        filledPixelsCount = CalcBarFilledPixels(gBattleSpritesDataPtr->battleBars[battlerId].maxValue,
                            gBattleSpritesDataPtr->battleBars[battlerId].oldValue,
                            gBattleSpritesDataPtr->battleBars[battlerId].receivedValue,
                            &gBattleSpritesDataPtr->battleBars[battlerId].currValue,
                            array, B_HEALTHBAR_PIXELS / 8);

        if (filledPixelsCount > (B_HEALTHBAR_PIXELS * 50 / 100)) // more than 50 % hp
            barElementId = HEALTHBOX_GFX_HP_BAR_GREEN;
        else if (filledPixelsCount > (B_HEALTHBAR_PIXELS * 20 / 100)) // more than 20% hp
            barElementId = HEALTHBOX_GFX_HP_BAR_YELLOW;
        else
            barElementId = HEALTHBOX_GFX_HP_BAR_RED; // 20 % or less

        // Blit bar left edge
        CpuCopy32(GetHealthboxElementGfxPtr(HEALTHBOX_GFX_65), (void *)(OBJ_VRAM0 + (gSprites[healthbarSpriteId].oam.tileNum) * TILE_SIZE_4BPP), 32);

        for (i = 0; i < 6; i++)
        {

            // Blit 3 tiles on the left bar sprite, then the last 3 on the right subsprite
            if (i < 3)
                CpuCopy32(GetHealthboxElementGfxPtr(barElementId) + array[i] * 32,
                          (void *)(OBJ_VRAM0 + (gSprites[healthbarSpriteId].oam.tileNum + 1 + i) * TILE_SIZE_4BPP), 32);
            else
                CpuCopy32(GetHealthboxElementGfxPtr(barElementId) + array[i] * 32,
                          (void *)(OBJ_VRAM0 + 64 + (gSprites[healthbarSpriteId].oam.tileNum - 1 + i) * TILE_SIZE_4BPP), 32);
        }        

        // Blit bar right edge
        CpuCopy32(GetHealthboxElementGfxPtr(HEALTHBOX_GFX_FRAME_END_BAR), (void *)(OBJ_VRAM0 + 64 + (gSprites[healthbarSpriteId].oam.tileNum + 5) * TILE_SIZE_4BPP), 32);

        break;
    case EXP_BAR:
        CalcBarFilledPixels(gBattleSpritesDataPtr->battleBars[battlerId].maxValue,
                    gBattleSpritesDataPtr->battleBars[battlerId].oldValue,
                    gBattleSpritesDataPtr->battleBars[battlerId].receivedValue,
                    &gBattleSpritesDataPtr->battleBars[battlerId].currValue,
                    array, B_EXPBAR_PIXELS / 8);
        level = GetMonData(&gPlayerParty[gBattlerPartyIndexes[battlerId]], MON_DATA_LEVEL);
        if (level == MAX_LEVEL)
        {
            for (i = 0; i < 8; i++)
                array[i] = 0;
        }
        for (i = 0; i < 8; i++)
        {
            if (i < 3)
                CpuCopy32(GetHealthboxElementGfxPtr(HEALTHBOX_GFX_12) + array[i] * 32,
                          (void *)(OBJ_VRAM0 + 32 + (gSprites[gBattleSpritesDataPtr->battleBars[battlerId].healthboxSpriteId].oam.tileNum + 0x24 + i) * TILE_SIZE_4BPP), 32);
            else
                CpuCopy32(GetHealthboxElementGfxPtr(HEALTHBOX_GFX_12) + array[i] * 32,
                          (void *)(OBJ_VRAM0 + 0xB80 + 32 + (gSprites[gBattleSpritesDataPtr->battleBars[battlerId].healthboxSpriteId].oam.tileNum + i) * TILE_SIZE_4BPP), 32);
        }
        break;
    }
}

static s32 CalcNewBarValue(s32 maxValue, s32 oldValue, s32 receivedValue, s32 *currValue, u8 scale, u16 toAdd)
{
    s32 ret, newValue;
    scale *= 8;

    if (*currValue == -32768) // first function call
    {
        if (maxValue < scale)
            *currValue = Q_24_8(oldValue);
        else
            *currValue = oldValue;
    }

    newValue = oldValue - receivedValue;
    if (newValue < 0)
        newValue = 0;
    else if (newValue > maxValue)
        newValue = maxValue;

    if (maxValue < scale)
    {
        if (newValue == Q_24_8_TO_INT(*currValue) && (*currValue & 0xFF) == 0)
            return -1;
    }
    else
    {
        if (newValue == *currValue) // we're done, the bar's value has been updated
            return -1;
    }

    if (maxValue < scale) // handle cases of max var having less pixels than the whole bar
    {
        s32 toAdd = Q_24_8(maxValue) / scale;

        if (receivedValue < 0) // fill bar right
        {
            *currValue += toAdd;
            ret = Q_24_8_TO_INT(*currValue);
            if (ret >= newValue)
            {
                *currValue = Q_24_8(newValue);
                ret = newValue;
            }
        }
        else // move bar left
        {
            *currValue -= toAdd;
            ret = Q_24_8_TO_INT(*currValue);
            // try round up
            if ((*currValue & 0xFF) > 0)
                ret++;
            if (ret <= newValue)
            {
                *currValue = Q_24_8(newValue);
                ret = newValue;
            }
        }
    }
    else
    {
        if (receivedValue < 0) // fill bar right
        {
            *currValue += toAdd;
            if (*currValue > newValue)
                *currValue = newValue;
            ret = *currValue;
        }
        else // move bar left
        {
            *currValue -= toAdd;
            if (*currValue < newValue)
                *currValue = newValue;
            ret = *currValue;
        }
    }

    return ret;
}

static u8 CalcBarFilledPixels(s32 maxValue, s32 oldValue, s32 receivedValue, s32 *currValue, u8 *pixelsArray, u8 scale)
{
    u8 pixels, filledPixels, totalPixels;
    u8 i;

    s32 newValue = oldValue - receivedValue;
    if (newValue < 0)
        newValue = 0;
    else if (newValue > maxValue)
        newValue = maxValue;

    totalPixels = scale * 8;

    for (i = 0; i < scale; i++)
        pixelsArray[i] = 0;

    if (maxValue < totalPixels)
        pixels = (*currValue * totalPixels / maxValue) >> 8;
    else
        pixels = *currValue * totalPixels / maxValue;

    filledPixels = pixels;

    if (filledPixels == 0 && newValue > 0)
    {
        pixelsArray[0] = 1;
        filledPixels = 1;
    }
    else
    {
        for (i = 0; i < scale; i++)
        {
            if (pixels >= 8)
            {
                pixelsArray[i] = 8;
            }
            else
            {
                pixelsArray[i] = pixels;
                break;
            }
            pixels -= 8;
        }
    }

    return filledPixels;
}

static u8 GetScaledExpFraction(s32 oldValue, s32 receivedValue, s32 maxValue, u8 scale)
{
    s32 newVal, result;
    s8 oldToMax, newToMax;

    scale *= 8;
    newVal = oldValue - receivedValue;

    if (newVal < 0)
        newVal = 0;
    else if (newVal > maxValue)
        newVal = maxValue;

    oldToMax = oldValue * scale / maxValue;
    newToMax = newVal * scale / maxValue;
    result = oldToMax - newToMax;

    return abs(result);
}

u8 GetScaledHPFraction(s16 hp, s16 maxhp, u8 scale)
{
    u8 result = hp * scale / maxhp;

    if (result == 0 && hp > 0)
        return 1;

    return result;
}

u8 GetHPBarLevel(s16 hp, s16 maxhp)
{
    u8 result;

    if (hp == maxhp)
    {
        result = HP_BAR_FULL;
    }
    else
    {
        u8 fraction = GetScaledHPFraction(hp, maxhp, B_HEALTHBAR_PIXELS);
        if (fraction > (B_HEALTHBAR_PIXELS * 50 / 100)) // more than 50 % hp
            result = HP_BAR_GREEN;
        else if (fraction > (B_HEALTHBAR_PIXELS * 20 / 100)) // more than 20% hp
            result = HP_BAR_YELLOW;
        else if (fraction > 0)
            result = HP_BAR_RED;
        else
            result = HP_BAR_EMPTY;
    }

    return result;
}

static u8 *AddTextPrinterAndCreateWindowOnHealthbox(const u8 *str, u32 x, u32 y, u8 fontId, u32 textColor, u32 shadeColor, u32 bgColor, u32 *windowId)
{
    u16 winId;
    u8 color[3];
    struct WindowTemplate winTemplate = sHealthboxWindowTemplate;

    winId = AddWindow(&winTemplate);
    FillWindowPixelBuffer(winId, PIXEL_FILL(bgColor));

    color[0] = bgColor; // BG color
    color[1] = textColor; // Font Color
    color[2] = shadeColor; // Font Shadow Color

    AddTextPrinterParameterized4(winId, fontId, x, y, 0, 0, color, TEXT_SKIP_DRAW, str);

    *windowId = winId;
    return (u8 *)(GetWindowAttribute(winId, WINDOW_TILE_DATA));
}

static void RemoveWindowOnHealthbox(u32 windowId)
{
    RemoveWindow(windowId);
}

static void FillHealthboxObject(void *dest, u32 valMult, u32 numTiles)
{
    CpuFill32(0x11111111 * valMult, dest, numTiles * TILE_SIZE_4BPP);
}

static void HpTextIntoHealthboxObject(void *dest, u8 *windowTileData, u32 windowWidth)
{
    CpuCopy32(windowTileData + 256, dest, windowWidth * TILE_SIZE_4BPP);
}

static void TextIntoHealthboxObject(void *dest, u8 *windowTileData, s32 windowWidth)
{
    CpuCopy32(windowTileData + 256, dest + 256, windowWidth * TILE_SIZE_4BPP);
// + 256 as that prevents the top 4 blank rows of sHealthboxWindowTemplate from being copied
    if (windowWidth > 0)
    {
        do
        {
            CpuCopy32(windowTileData + 20, dest + 20, 12);
            dest += 32, windowTileData += 32;
            windowWidth--;
        } while (windowWidth != 0);
    }
}

static void SafariTextIntoHealthboxObject(void *dest, u8 *windowTileData, u32 windowWidth)
{
    CpuCopy32(windowTileData, dest, windowWidth * TILE_SIZE_4BPP);
    CpuCopy32(windowTileData + 256, dest + 256, windowWidth * TILE_SIZE_4BPP);
}

#define ABILITY_POP_UP_TAG          0xD720
#define ABILITY_POP_UP_OPPONENT_TAG 0xD721
#define ABILITY_POP_UP_PARTNER_TAG  0xD722

// for sprite
#define tOriginalX      data[0]
#define tHide           data[1]
#define tFrames         data[2]
#define tRightToLeft    data[3]
#define tBattlerId      data[4]
#define tIsMain         data[5]

// for task
#define tSpriteId1      data[6]
#define tSpriteId2      data[7]

static const u8 ALIGNED(4) sAbilityPopUpGfx[] = INCBIN_U8("graphics/battle_interface/ability_pop_up.4bpp");
static const u8 ALIGNED(4) sAbilityPopUpOpponentGfx[] = INCBIN_U8("graphics/battle_interface/ability_pop_up_opponent.4bpp");
static const u8 ALIGNED(4) sAbilityPopUpPartnerGfx[] = INCBIN_U8("graphics/battle_interface/ability_pop_up_partner.4bpp");
static const u16 sAbilityPopUpPalette[] = INCBIN_U16("graphics/battle_interface/ability_pop_up.gbapal");

static const struct SpriteSheet sSpriteSheet_AbilityPopUp =
{
    sAbilityPopUpGfx, sizeof(sAbilityPopUpGfx), ABILITY_POP_UP_TAG
};
static const struct SpriteSheet sSpriteSheet_AbilityPopUpOpponent =
{
    sAbilityPopUpOpponentGfx, sizeof(sAbilityPopUpOpponentGfx), ABILITY_POP_UP_OPPONENT_TAG
};
static const struct SpriteSheet sSpriteSheet_AbilityPopUpPartner =
{
    sAbilityPopUpPartnerGfx, sizeof(sAbilityPopUpPartnerGfx), ABILITY_POP_UP_PARTNER_TAG
};
static const struct SpritePalette sSpritePalette_AbilityPopUp =
{
    sAbilityPopUpPalette, ABILITY_POP_UP_TAG
};

static const struct OamData sOamData_AbilityPopUp =
{
    .affineMode = ST_OAM_AFFINE_OFF,
    .objMode = ST_OAM_OBJ_NORMAL,
    .shape = SPRITE_SHAPE(64x32),
    .size = SPRITE_SIZE(64x32),
    .priority = 0,
};

static const struct SpriteTemplate sSpriteTemplate_AbilityPopUp =
{
    .tileTag = ABILITY_POP_UP_TAG,
    .paletteTag = ABILITY_POP_UP_TAG,
    .oam = &sOamData_AbilityPopUp,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCb_AbilityPopUp
};

static const struct SpriteTemplate sSpriteTemplate_AbilityPopUpOpponent =
{
    .tileTag = ABILITY_POP_UP_OPPONENT_TAG,
    .paletteTag = ABILITY_POP_UP_TAG,
    .oam = &sOamData_AbilityPopUp,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCb_AbilityPopUp
};

static const struct SpriteTemplate sSpriteTemplate_AbilityPopUpPartner =
{
    .tileTag = ABILITY_POP_UP_PARTNER_TAG,
    .paletteTag = ABILITY_POP_UP_TAG,
    .oam = &sOamData_AbilityPopUp,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCb_AbilityPopUp
};

#define ABILITY_POP_UP_POS_X_DIFF (64 - 3) // Tuck second sprite underneath to gain proper letter spacing
#define ABILITY_POP_UP_POS_X_SLIDE 97

static const s16 sAbilityPopUpCoordsDoubles[MAX_BATTLERS_COUNT][2] =
{
    {19, 80}, // player left
    {176, 19}, // opponent left
    {19, 97}, // player right
    {176, 36}, // opponent right
};

static const s16 sAbilityPopUpCoordsSingles[MAX_BATTLERS_COUNT][2] =
{
    {19, 97}, // player
    {176, 57}, // opponent
};

#define POPUP_WINDOW_WIDTH 8
#define MAX_POPUP_STRING_WIDTH (POPUP_WINDOW_WIDTH * 8)

static u8* AddTextPrinterAndCreateWindowOnAbilityPopUp(const u8 *str, u32 x, u32 y, u8 font, u32 color1, u32 color2, u32 color3, u32 *windowId)
{
    u8 color[3] = {color1, color2, color3};
    struct WindowTemplate winTemplate = {0};
    winTemplate.width = POPUP_WINDOW_WIDTH;
    winTemplate.height = 2;

    *windowId = AddWindow(&winTemplate);
    FillWindowPixelBuffer(*windowId, PIXEL_FILL(color1));

    AddTextPrinterParameterized4(*windowId, font, x, y, 0, 0, color, TEXT_SKIP_DRAW, str);
    return (u8 *)(GetWindowAttribute(*windowId, WINDOW_TILE_DATA));
}

static void TextIntoAbilityPopUp(void *dest, u8 *windowTileData, s32 xTileAmount, bool32 arg3)
{
    CpuCopy32(windowTileData + 256, dest + 256, xTileAmount * 32);
    if (xTileAmount > 0)
    {
        do
        {
            if (arg3)
                CpuCopy32(windowTileData + 16, dest + 16, 16);
            else
                CpuCopy32(windowTileData + 20, dest + 20, 12);
            dest += 32, windowTileData += 32;
            xTileAmount--;
        } while (xTileAmount != 0);
    }
}

static void PrintOnAbilityPopUp(const u8 *str, u8 *spriteTileData1, u8 *spriteTileData2, u32 x1, u32 x2, u32 y, u8 font, u32 color1, u32 color2, u32 color3)
{
    u32 windowId;
    u8 *windowTileData;
    u16 width;

    windowTileData = AddTextPrinterAndCreateWindowOnAbilityPopUp(str, x1, y, font, color1, color2, color3, &windowId);
    TextIntoAbilityPopUp(spriteTileData1, windowTileData, 6, (y == 0));
    RemoveWindow(windowId);

    width = GetStringWidth(font, str, 0);

    if (width > 48)
    {
        windowTileData = AddTextPrinterAndCreateWindowOnAbilityPopUp(str, x2 - MAX_POPUP_STRING_WIDTH, y, font, color1, color2, color3, &windowId);
        TextIntoAbilityPopUp(spriteTileData2, windowTileData, 4, (y == 0));
        RemoveWindow(windowId);
    }
}

// Colors based of ability pop up palette
static u8 GetAbilityPopUpColor(u8 battlerId)
{
    switch (GetBattlerRelation(battlerId))
    {
    case RELATION_PLAYER:
        return 5;
    case RELATION_OPPONENT:
        return 6;
    case RELATION_PARTNER:
        return 4;
    }
}

static const u8 sText_Space16[]= _("                ");
static void ClearAbilityName(u8 battlerId, u8 spriteId1, u8 spriteId2)
{
    PrintOnAbilityPopUp(sText_Space16,
                        (void*)(OBJ_VRAM0) + (gSprites[spriteId1].oam.tileNum * 32) + 256 + 64,
                        (void*)(OBJ_VRAM0) + (gSprites[spriteId2].oam.tileNum * 32) + 256,
                        0, 18,
                        4,
                        FONT_SMALL,
                        GetAbilityPopUpColor(battlerId), 2, 1);
}

static void PrintBattlerOnAbilityPopUp(u8 battlerId, u8 spriteId1, u8 spriteId2)
{
    int i;
    u8 lastChar;
    u8* textPtr;
    u8 monName[POKEMON_NAME_LENGTH + 3] = {0};
    u8* nick = gBattleMons[battlerId].nickname; // This needs to be updated for Illusion support

    for (i = 0; i < POKEMON_NAME_LENGTH; ++i)
    {
        monName[i] = nick[i];

        if (nick[i] == EOS || i + 1 == POKEMON_NAME_LENGTH) // End of string
            break;
    }

    textPtr = monName + i + 1;

    if (*(textPtr - 1) == EOS)
        textPtr--;

    lastChar = *(textPtr - 1);

    // Make the string say "[NAME]'s" instead of "[NAME]"
    textPtr[0] = CHAR_SGL_QUOTE_RIGHT; // apostraphe
    textPtr++;
    if (lastChar != CHAR_S && lastChar != CHAR_s)
    {
        textPtr[0] = CHAR_s;
        textPtr++;
    }

    textPtr[0] = EOS;

    PrintOnAbilityPopUp((const u8 *)monName,
                        (void*)(OBJ_VRAM0) + (gSprites[spriteId1].oam.tileNum * 32) + 64,
                        (void*)(OBJ_VRAM0) + (gSprites[spriteId2].oam.tileNum * 32),
                        0, 19,
                        0,
                        FONT_TINY_OUTLINED,
                        0, 2, 1);
}

static void PrintAbilityOnAbilityPopUp(u8 battlerId, u32 ability, u8 spriteId1, u8 spriteId2)
{
    PrintOnAbilityPopUp(gAbilityNames[ability],
                        (void*)(OBJ_VRAM0) + (gSprites[spriteId1].oam.tileNum * 32) + 256 + 64,
                        (void*)(OBJ_VRAM0) + (gSprites[spriteId2].oam.tileNum * 32) + 256,
                        0, 19,
                        4,
                        FONT_SMALL,
                        GetAbilityPopUpColor(battlerId), 2, 1);
}

#define PIXEL_COORDS_TO_OFFSET(x, y)(            \
/*Add tiles by X*/                                \
((y / 8) * 32 * 8)                                \
/*Add tiles by X*/                                \
+ ((x / 8) * 32)                                \
/*Add pixels by Y*/                                \
+ ((((y) - ((y / 8) * 8))) * 4)                    \
/*Add pixels by X*/                                \
+ ((((x) - ((x / 8) * 8)) / 2)))

static const u16 sOverwrittenPixelsTable[][2] =
{
    {PIXEL_COORDS_TO_OFFSET(0, 0), 5},
    {PIXEL_COORDS_TO_OFFSET(0, 1), 5},
    {PIXEL_COORDS_TO_OFFSET(0, 2), 5},
    {PIXEL_COORDS_TO_OFFSET(0, 3), 5},
    {PIXEL_COORDS_TO_OFFSET(0, 4), 5},
    {PIXEL_COORDS_TO_OFFSET(0, 5), 5},
    {PIXEL_COORDS_TO_OFFSET(0, 6), 5},
    {PIXEL_COORDS_TO_OFFSET(0, 7), 3},
    {PIXEL_COORDS_TO_OFFSET(0, 8), 3},
    {PIXEL_COORDS_TO_OFFSET(0, 9), 3},
    {PIXEL_COORDS_TO_OFFSET(0, 10), 3},
    {PIXEL_COORDS_TO_OFFSET(0, 11), 3},
    {PIXEL_COORDS_TO_OFFSET(0, 12), 3},
    {PIXEL_COORDS_TO_OFFSET(0, 13), 8},

    {PIXEL_COORDS_TO_OFFSET(8, 13), 8},
    {PIXEL_COORDS_TO_OFFSET(16, 13), 8},
    {PIXEL_COORDS_TO_OFFSET(24, 13), 8},
    {PIXEL_COORDS_TO_OFFSET(32, 13), 8},
    {PIXEL_COORDS_TO_OFFSET(40, 13), 8},
    {PIXEL_COORDS_TO_OFFSET(48, 13), 8},
    {PIXEL_COORDS_TO_OFFSET(56, 13), 8},

    {PIXEL_COORDS_TO_OFFSET(0, 14), 8},
    {PIXEL_COORDS_TO_OFFSET(8, 14), 8},
    {PIXEL_COORDS_TO_OFFSET(16, 14), 8},
    {PIXEL_COORDS_TO_OFFSET(24, 14), 8},
    {PIXEL_COORDS_TO_OFFSET(32, 14), 8},
    {PIXEL_COORDS_TO_OFFSET(40, 14), 8},
    {PIXEL_COORDS_TO_OFFSET(48, 14), 8},
    {PIXEL_COORDS_TO_OFFSET(56, 14), 8},

    {PIXEL_COORDS_TO_OFFSET(0, 15), 3},
    {PIXEL_COORDS_TO_OFFSET(0, 16), 3},
    {PIXEL_COORDS_TO_OFFSET(0, 17), 3},
    {PIXEL_COORDS_TO_OFFSET(0, 18), 3},
    {PIXEL_COORDS_TO_OFFSET(0, 19), 3},
    {PIXEL_COORDS_TO_OFFSET(0, 20), 3},
    {PIXEL_COORDS_TO_OFFSET(0, 21), 3},
    {PIXEL_COORDS_TO_OFFSET(0, 22), 3},
    {PIXEL_COORDS_TO_OFFSET(0, 23), 3},
    {PIXEL_COORDS_TO_OFFSET(0, 24), 3},
    {PIXEL_COORDS_TO_OFFSET(0, 25), 3},
    {PIXEL_COORDS_TO_OFFSET(0, 26), 3},

//Second Row Of Image
    {PIXEL_COORDS_TO_OFFSET(0, 45), 8},
    {PIXEL_COORDS_TO_OFFSET(0, 46), 8},
    {PIXEL_COORDS_TO_OFFSET(0, 47), 8},
    {PIXEL_COORDS_TO_OFFSET(8, 45), 8},
    {PIXEL_COORDS_TO_OFFSET(8, 46), 8},
    {PIXEL_COORDS_TO_OFFSET(8, 47), 8},
    {PIXEL_COORDS_TO_OFFSET(16, 45), 8},
    {PIXEL_COORDS_TO_OFFSET(16, 46), 8},
    {PIXEL_COORDS_TO_OFFSET(16, 47), 8},
    {PIXEL_COORDS_TO_OFFSET(24, 45), 8},
    {PIXEL_COORDS_TO_OFFSET(24, 46), 8},
    {PIXEL_COORDS_TO_OFFSET(24, 47), 8},
};

static inline void CopyPixels(u8 *dest, const u8 *src, u32 pixelCount)
{
    u32 i = 0;

    if (pixelCount & 1)
    {
        while (pixelCount != 0)
        {
            dest[i] &= ~(0xF);
            dest[i] |= (src[i] & 0xF);
            if (--pixelCount != 0)
            {
                dest[i] &= ~(0xF0);
                dest[i] |= (src[i] & 0xF0);
                pixelCount--;
            }
            i++;
        }
    }
    else
    {
        for (i = 0; i < pixelCount / 2; i++)
            dest[i] = src[i];
    }
}

static void RestoreOverwrittenPixels(u8 *tiles, u8 battlerId)
{
    u32 i;
    const u8 *gfx = sAbilityPopUpGfx;

    switch (GetBattlerRelation(battlerId))
    {
    case RELATION_PLAYER:
        gfx = sAbilityPopUpGfx;
        break;
    case RELATION_OPPONENT:
        gfx = sAbilityPopUpOpponentGfx;
        break;
    case RELATION_PARTNER:
        gfx = sAbilityPopUpPartnerGfx;
        break;
    }
    
    u8 *buffer = Alloc(sizeof(sAbilityPopUpGfx) * 2);

    CpuCopy32(tiles, buffer, sizeof(sAbilityPopUpGfx));

    for (i = 0; i < ARRAY_COUNT(sOverwrittenPixelsTable); i++)
    {
        CopyPixels(buffer + sOverwrittenPixelsTable[i][0],
                   gfx + sOverwrittenPixelsTable[i][0],
                   sOverwrittenPixelsTable[i][1]);
    }

    CpuCopy32(buffer, tiles, sizeof(sAbilityPopUpGfx));
    Free(buffer);
}

void CreateAbilityPopUp(u8 battlerId, u32 ability, bool32 isDoubleBattle)
{
#if B_ABILITY_POP_UP == TRUE
    const s16 (*coords)[2];
    u8 spriteId1, spriteId2, battlerPosition, taskId;

    if (gTestRunnerEnabled)
    {
        TestRunner_Battle_RecordAbilityPopUp(battlerId, ability);
        if (gTestRunnerHeadless)
            return;
    }

    if (gBattleScripting.abilityPopupOverwrite != 0)
        ability = gBattleScripting.abilityPopupOverwrite;
    
    battlerPosition = GetBattlerPosition(battlerId);

    if (!(gBattleStruct->activeAbilityPopUps & gBitTable[battlerId]))
    {
        if (GetBattlerRelation(battlerId) == RELATION_PARTNER)
        {
            LoadSpriteSheet(&sSpriteSheet_AbilityPopUpPartner);
        }
        else if (GetBattlerRelation(battlerId) == RELATION_PLAYER)
        {
            LoadSpriteSheet(&sSpriteSheet_AbilityPopUp);
        }
        else
        {
            LoadSpriteSheet(&sSpriteSheet_AbilityPopUpOpponent);
        }
        LoadSpritePalette(&sSpritePalette_AbilityPopUp);
    }

    gBattleStruct->activeAbilityPopUps |= gBitTable[battlerId];

    if (isDoubleBattle)
        coords = sAbilityPopUpCoordsDoubles;
    else
        coords = sAbilityPopUpCoordsSingles;

    if (GetBattlerRelation(battlerId) == RELATION_PARTNER)
    {
        spriteId1 = CreateSprite(&sSpriteTemplate_AbilityPopUpPartner,
                                coords[battlerPosition][0] - ABILITY_POP_UP_POS_X_SLIDE,
                                coords[battlerPosition][1], 0);
        spriteId2 = CreateSprite(&sSpriteTemplate_AbilityPopUpPartner,
                                coords[battlerPosition][0] - ABILITY_POP_UP_POS_X_SLIDE + ABILITY_POP_UP_POS_X_DIFF,
                                coords[battlerPosition][1], 1); //Appears below

        gSprites[spriteId1].tRightToLeft = TRUE;
        gSprites[spriteId2].tRightToLeft = TRUE;

        taskId = CreateTask(Task_FreeAbilityPopUpPartnerGfx, 5);
    }
    else if (GetBattlerRelation(battlerId) == RELATION_PLAYER)
    {
        spriteId1 = CreateSprite(&sSpriteTemplate_AbilityPopUp,
                                coords[battlerPosition][0] - ABILITY_POP_UP_POS_X_SLIDE,
                                coords[battlerPosition][1], 0);
        spriteId2 = CreateSprite(&sSpriteTemplate_AbilityPopUp,
                                coords[battlerPosition][0] - ABILITY_POP_UP_POS_X_SLIDE + ABILITY_POP_UP_POS_X_DIFF,
                                coords[battlerPosition][1], 1); //Appears below

        gSprites[spriteId1].tRightToLeft = TRUE;
        gSprites[spriteId2].tRightToLeft = TRUE;
        
        taskId = CreateTask(Task_FreeAbilityPopUpGfx, 5);
    }
    else
    {
        spriteId1 = CreateSprite(&sSpriteTemplate_AbilityPopUpOpponent,
                                coords[battlerPosition][0] + ABILITY_POP_UP_POS_X_SLIDE,
                                coords[battlerPosition][1], 0);
        spriteId2 = CreateSprite(&sSpriteTemplate_AbilityPopUpOpponent,
                                coords[battlerPosition][0] + ABILITY_POP_UP_POS_X_SLIDE + ABILITY_POP_UP_POS_X_DIFF,
                                coords[battlerPosition][1], 1); //Appears below

        gSprites[spriteId1].tRightToLeft = FALSE;
        gSprites[spriteId2].tRightToLeft = FALSE;
        
        taskId = CreateTask(Task_FreeAbilityPopUpOpponentGfx, 5);
    }

    gSprites[spriteId1].tOriginalX = coords[battlerPosition][0];
    gSprites[spriteId2].tOriginalX = coords[battlerPosition][0] + ABILITY_POP_UP_POS_X_DIFF;
    gSprites[spriteId2].oam.tileNum += (8 * 4); //Second half of pop up

    gBattleStruct->abilityPopUpSpriteIds[battlerId][0] = spriteId1;
    gBattleStruct->abilityPopUpSpriteIds[battlerId][1] = spriteId2;

    gTasks[taskId].tSpriteId1 = spriteId1;
    gTasks[taskId].tSpriteId2 = spriteId2;

    gSprites[spriteId1].tIsMain = TRUE;
    gSprites[spriteId1].tBattlerId = battlerId;
    gSprites[spriteId2].tBattlerId = battlerId;

    StartSpriteAnim(&gSprites[spriteId1], 0);
    StartSpriteAnim(&gSprites[spriteId2], 0);

    PrintBattlerOnAbilityPopUp(battlerId, spriteId1, spriteId2);
    PrintAbilityOnAbilityPopUp(battlerId, ability, spriteId1, spriteId2);
    RestoreOverwrittenPixels((void*)(OBJ_VRAM0) + (gSprites[spriteId1].oam.tileNum * 32), battlerId);
#endif
}

void UpdateAbilityPopup(u8 battlerId)
{
    u8 spriteId1 = gBattleStruct->abilityPopUpSpriteIds[battlerId][0];
    u8 spriteId2 = gBattleStruct->abilityPopUpSpriteIds[battlerId][1];
    u16 ability = (gBattleScripting.abilityPopupOverwrite != 0) ? gBattleScripting.abilityPopupOverwrite : gBattleMons[battlerId].ability;

    ClearAbilityName(battlerId, spriteId1, spriteId2);
    PrintAbilityOnAbilityPopUp(battlerId, ability, spriteId1, spriteId2);
    RestoreOverwrittenPixels((void*)(OBJ_VRAM0) + (gSprites[spriteId1].oam.tileNum * 32), battlerId);
}

#define FRAMES_TO_WAIT 90
#define PIXELS_TO_MOVE_PER_FRAME 8 // Higher = faster

static void SpriteCb_AbilityPopUp(struct Sprite *sprite)
{
    if (!sprite->tHide) // Show
    {
        if (sprite->tIsMain && ++sprite->tFrames == PIXELS_TO_MOVE_PER_FRAME)
            PlaySE(SE_ICE_STAIRS); // SE_M_HARDEN is a similar alternative
        if ((!sprite->tRightToLeft && (sprite->x -= PIXELS_TO_MOVE_PER_FRAME) <= sprite->tOriginalX)
            || (sprite->tRightToLeft && (sprite->x += PIXELS_TO_MOVE_PER_FRAME) >= sprite->tOriginalX)
           )
        {
            sprite->x = sprite->tOriginalX;
            sprite->tHide = TRUE;
            sprite->tFrames = FRAMES_TO_WAIT;
        }
    }
    else // Hide
    {
        if (sprite->tFrames == 0)
        {
            if ((!sprite->tRightToLeft && (sprite->x += PIXELS_TO_MOVE_PER_FRAME) >= sprite->tOriginalX + ABILITY_POP_UP_POS_X_SLIDE)
                ||(sprite->tRightToLeft && (sprite->x -= PIXELS_TO_MOVE_PER_FRAME) <= sprite->tOriginalX - ABILITY_POP_UP_POS_X_SLIDE)
               )
            {
                gBattleStruct->activeAbilityPopUps &= ~(gBitTable[sprite->tBattlerId]);
                DestroySprite(sprite);
            }
        }
        else
        {
            if (!gBattleScripting.fixedPopup)
                sprite->tFrames--;
        }
    }
}

void DestroyAbilityPopUp(u8 battlerId)
{
    if (gBattleStruct->activeAbilityPopUps & gBitTable[battlerId])
    {
        gSprites[gBattleStruct->abilityPopUpSpriteIds[battlerId][0]].tFrames = 0;
        gSprites[gBattleStruct->abilityPopUpSpriteIds[battlerId][1]].tFrames = 0;
        gBattleScripting.fixedPopup = FALSE;
    }
}

static bool8 IsAbilityPopupActive(u8 trainerRelation)
{
    switch (trainerRelation)
    {
    case RELATION_PLAYER:
        if (gBattleTypeFlags & BATTLE_TYPE_MULTI)
            return gBattleStruct->activeAbilityPopUps & gBitTable[0];
        else
            return gBattleStruct->activeAbilityPopUps & gBitTable[0]
                   && gBattleStruct->activeAbilityPopUps & gBitTable[2];        
    case RELATION_PARTNER:
        if (gBattleTypeFlags & BATTLE_TYPE_MULTI)
            return gBattleStruct->activeAbilityPopUps & gBitTable[2];
        else
            return FALSE;
    case RELATION_OPPONENT:
        return gBattleStruct->activeAbilityPopUps & gBitTable[1]
                && gBattleStruct->activeAbilityPopUps & gBitTable[3];
    }
    return FALSE;
}

static void Task_FreeAbilityPopUpGfx(u8 taskId)
{
    if (!gSprites[gTasks[taskId].tSpriteId1].inUse
        && !gSprites[gTasks[taskId].tSpriteId2].inUse
        && !IsAbilityPopupActive(RELATION_PLAYER))
    {
        FreeSpriteTilesByTag(ABILITY_POP_UP_TAG);
        FreeSpritePaletteByTag(ABILITY_POP_UP_TAG);
        DestroyTask(taskId);
    }
}

static void Task_FreeAbilityPopUpOpponentGfx(u8 taskId)
{
    if (!gSprites[gTasks[taskId].tSpriteId1].inUse
        && !gSprites[gTasks[taskId].tSpriteId2].inUse
        && !IsAbilityPopupActive(RELATION_OPPONENT))
    {
        FreeSpriteTilesByTag(ABILITY_POP_UP_OPPONENT_TAG);
        FreeSpritePaletteByTag(ABILITY_POP_UP_TAG);
        DestroyTask(taskId);
    }
}

static void Task_FreeAbilityPopUpPartnerGfx(u8 taskId)
{
    if (!gSprites[gTasks[taskId].tSpriteId1].inUse
        && !gSprites[gTasks[taskId].tSpriteId2].inUse
        && !IsAbilityPopupActive(RELATION_PARTNER))
    {
        FreeSpriteTilesByTag(ABILITY_POP_UP_PARTNER_TAG);
        FreeSpritePaletteByTag(ABILITY_POP_UP_TAG);
        DestroyTask(taskId);
    }
}

// last used ball
#define LAST_BALL_WINDOW_TAG 0xD723

static const struct OamData sOamData_LastUsedBall =
{
    .y = 0,
    .affineMode = 0,
    .objMode = 0,
    .mosaic = 0,
    .bpp = 0,
    .shape = SPRITE_SHAPE(32x32),
    .x = 0,
    .matrixNum = 0,
    .size = SPRITE_SIZE(32x32),
    .tileNum = 0,
    .priority = 1,
    .paletteNum = 0,
    .affineParam = 0,
};

static const struct SpriteTemplate sSpriteTemplate_LastUsedBallWindow =
{
    .tileTag = LAST_BALL_WINDOW_TAG,
    .paletteTag = ABILITY_POP_UP_TAG,
    .oam = &sOamData_LastUsedBall,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCB_LastUsedBallWin
};

#if B_LAST_USED_BALL_BUTTON == R_BUTTON
    static const u8 ALIGNED(4) sLastUsedBallWindowGfx[] = INCBIN_U8("graphics/battle_interface/last_used_ball_r.4bpp");
#else
    static const u8 ALIGNED(4) sLastUsedBallWindowGfx[] = INCBIN_U8("graphics/battle_interface/last_used_ball_l.4bpp");
#endif
static const struct SpriteSheet sSpriteSheet_LastUsedBallWindow =
{
    sLastUsedBallWindowGfx, sizeof(sLastUsedBallWindowGfx), LAST_BALL_WINDOW_TAG
};

#define LAST_USED_BALL_X_F    15
#define LAST_USED_BALL_X_0    -15
#define LAST_USED_BALL_Y      ((IsDoubleBattle()) ? 78 : 68)

#define LAST_BALL_WIN_X_F       (LAST_USED_BALL_X_F - 1)
#define LAST_BALL_WIN_X_0       (LAST_USED_BALL_X_0 - 0)
#define LAST_USED_WIN_Y         (LAST_USED_BALL_Y - 8)

#define LAST_USED_BALL_DX   4
#define LAST_BALL_WIN_DX   4

#define sHide  data[0]

bool32 CanThrowLastUsedBall(void)
{
#if B_LAST_USED_BALL == FALSE
    return FALSE;
#else
    if (!CanThrowBall())
        return FALSE;
    if (gBattleTypeFlags & (BATTLE_TYPE_TRAINER | BATTLE_TYPE_FRONTIER))
        return FALSE;
    if (!CheckBagHasItem(gLastThrownBall, 1))
        return FALSE;

    return TRUE;
#endif
}

void TryAddLastUsedBallItemSprites(void)
{
#if B_LAST_USED_BALL == TRUE
    if (gLastThrownBall == 0
      || (gLastThrownBall != 0 && !CheckBagHasItem(gLastThrownBall, 1)))
    {
        // we're out of the last used ball, so just set it to the first ball in the bag
        // we have to compact the bag first bc it is typically only compacted when you open it
        CompactItemsInBagPocket(&gBagPockets[BALLS_POCKET]);
        gLastThrownBall = gBagPockets[BALLS_POCKET].itemSlots[0].itemId;
    }

    if (!CanThrowLastUsedBall())
        return;

    // ball
    if (gBattleStruct->ballSpriteIds[0] == MAX_SPRITES)
    {
        gBattleStruct->ballSpriteIds[0] = AddItemIconSprite(102, 102, gLastThrownBall);
        gSprites[gBattleStruct->ballSpriteIds[0]].x = LAST_USED_BALL_X_0;
        gSprites[gBattleStruct->ballSpriteIds[0]].y = LAST_USED_BALL_Y;
        gSprites[gBattleStruct->ballSpriteIds[0]].sHide = FALSE;   // restore
        gSprites[gBattleStruct->ballSpriteIds[0]].callback = SpriteCB_LastUsedBall;
    }

    // window
    LoadSpritePalette(&sSpritePalette_AbilityPopUp);
    if (GetSpriteTileStartByTag(LAST_BALL_WINDOW_TAG) == 0xFFFF)
        LoadSpriteSheet(&sSpriteSheet_LastUsedBallWindow);

    if (gBattleStruct->ballSpriteIds[1] == MAX_SPRITES)
    {
        gBattleStruct->ballSpriteIds[1] = CreateSprite(&sSpriteTemplate_LastUsedBallWindow,
                                                       LAST_BALL_WIN_X_0,
                                                       LAST_USED_WIN_Y, 5);
        gSprites[gBattleStruct->ballSpriteIds[0]].sHide = FALSE;   // restore
    }
#endif
}

static void DestroyLastUsedBallWinGfx(struct Sprite *sprite)
{
    FreeSpriteTilesByTag(LAST_BALL_WINDOW_TAG);
    FreeSpritePaletteByTag(ABILITY_POP_UP_TAG);
    DestroySprite(sprite);
    gBattleStruct->ballSpriteIds[1] = MAX_SPRITES;
}

static void DestroyLastUsedBallGfx(struct Sprite *sprite)
{
    FreeSpriteTilesByTag(102);
    FreeSpritePaletteByTag(102);
    DestroySprite(sprite);
    gBattleStruct->ballSpriteIds[0] = MAX_SPRITES;
}

static void SpriteCB_LastUsedBallWin(struct Sprite *sprite)
{
    if (sprite->sHide)
    {
        if (sprite->x > LAST_BALL_WIN_X_0)
            sprite->x -= LAST_BALL_WIN_DX;

        if (sprite->x <= LAST_BALL_WIN_X_0)
            DestroyLastUsedBallWinGfx(sprite);
    }
    else
    {
        if (sprite->x < LAST_BALL_WIN_X_F)
            sprite->x += LAST_BALL_WIN_DX;

        if (sprite->x > LAST_BALL_WIN_X_F)
            sprite->x = LAST_BALL_WIN_X_F;
    }
}

static void SpriteCB_LastUsedBall(struct Sprite *sprite)
{
    if (sprite->sHide)
    {
        if (sprite->x > LAST_USED_BALL_X_0)
            sprite->x -= LAST_USED_BALL_DX;

        if (sprite->x <= LAST_USED_BALL_X_0)
            DestroyLastUsedBallGfx(sprite);
    }
    else
    {
        if (sprite->x < LAST_USED_BALL_X_F)
            sprite->x += LAST_USED_BALL_DX;

        if (sprite->x > LAST_USED_BALL_X_F)
            sprite->x = LAST_USED_BALL_X_F;
    }
}

static void TryHideOrRestoreLastUsedBall(u8 caseId)
{
#if B_LAST_USED_BALL == TRUE
    if (gBattleStruct->ballSpriteIds[0] == MAX_SPRITES)
        return;

    switch (caseId)
    {
    case 0: // hide
        if (gBattleStruct->ballSpriteIds[0] != MAX_SPRITES)
            gSprites[gBattleStruct->ballSpriteIds[0]].sHide = TRUE;   // hide
        if (gBattleStruct->ballSpriteIds[1] != MAX_SPRITES)
            gSprites[gBattleStruct->ballSpriteIds[1]].sHide = TRUE;   // hide
        break;
    case 1: // restore
        if (gBattleStruct->ballSpriteIds[0] != MAX_SPRITES)
            gSprites[gBattleStruct->ballSpriteIds[0]].sHide = FALSE;   // restore
        if (gBattleStruct->ballSpriteIds[1] != MAX_SPRITES)
            gSprites[gBattleStruct->ballSpriteIds[1]].sHide = FALSE;   // restore
        break;
    }
#endif
}

void TryHideLastUsedBall(void)
{
#if B_LAST_USED_BALL == TRUE
    TryHideOrRestoreLastUsedBall(0);
#endif
}

void TryRestoreLastUsedBall(void)
{
#if B_LAST_USED_BALL == TRUE
    if (gBattleStruct->ballSpriteIds[0] != MAX_SPRITES)
        TryHideOrRestoreLastUsedBall(1);
    else
        TryAddLastUsedBallItemSprites();
#endif
}
