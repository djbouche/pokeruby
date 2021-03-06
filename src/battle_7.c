#include "global.h"
#include "battle.h"
#include "battle_anim.h"
#include "battle_interface.h"
#include "blend_palette.h"
#include "data2.h"
#include "decompress.h"
#include "main.h"
#include "m4a.h"
#include "palette.h"
#include "pokemon.h"
#include "rom_8077ABC.h"
#include "rom_8094928.h"
#include "songs.h"
#include "sound.h"
#include "species.h"
#include "sprite.h"
#include "task.h"
#include "text.h"
#include "gba/m4a_internal.h"

struct Struct2019348
{
    u8 filler0[2];
    u16 unk2;
    u8 filler4[4];
    u32 unk8;
    u32 unkC;
    u32 unk10;
};

extern u8 gBattleBufferA[][0x200];
extern u8 gActiveBank;
extern u8 gNoOfAllBanks;
extern u16 gBattlePartyID[];
extern u8 gBanksBySide[];
extern u8 gObjectBankIDs[];
extern u16 gUnknown_02024DE8;
extern u8 gDoingBattleAnim;
extern u32 gPID_perBank[];
extern struct Window gUnknown_03004210;
extern void (*gBattleBankFunc[])(void);
extern u8 gHealthboxIDs[];
extern u8 gUnknown_0300434C[];
extern struct MusicPlayerInfo gMPlay_SE1;
extern struct MusicPlayerInfo gMPlay_SE2;
extern struct MusicPlayerInfo gMPlay_BGM;
extern u32 gBitTable[];
extern u16 gBattleTypeFlags;
extern u8 gBattleMonForms[];
extern u8 gBattleAnimPlayerMonIndex;
extern u8 gBattleAnimEnemyMonIndex;
extern void (*gAnimScriptCallback)(void);
extern u8 gAnimScriptActive;
extern const u8 *const gBattleAnims_Unknown1[];
extern const u8 *const gBattleAnims_Unknown2[];
extern const struct CompressedSpriteSheet gTrainerFrontPicTable[];
extern const struct MonCoords gTrainerFrontPicCoords[];
extern const struct CompressedSpritePalette gTrainerFrontPicPaletteTable[];
extern const u8 gSubstituteDollTilemap[];
extern const u8 gSubstituteDollGfx[];
extern const u8 gSubstituteDollPal[];
extern const struct CompressedSpriteSheet gUnknown_081FAF24;
extern const struct SpriteTemplate gSpriteTemplate_81FAF34;
extern const struct CompressedSpriteSheet gUnknown_0820A47C;
extern const struct CompressedSpriteSheet gUnknown_0820A484;
extern const struct CompressedSpriteSheet gUnknown_0820A48C[];
extern const struct CompressedSpriteSheet gUnknown_0820A49C[];
extern const struct CompressedSpriteSheet gUnknown_0820A4AC;
extern const struct CompressedSpriteSheet gUnknown_0820A4B4[];
extern const struct SpritePalette gUnknown_0820A4D4[];
extern const u8 gUnknown_08D09C48[];

#define ewram19348 (*(struct Struct2019348 *)(ewram + 0x19348))

extern void c3_0802FDF4(u8);
extern void sub_80440EC();
extern void sub_804777C();
extern void sub_8141828();
extern u8 sub_8077ABC();
extern u8 sub_8078874(u8);
extern u8 sub_8077F68(u8);
extern u8 sub_8077F7C(u8);
extern void sub_8094958(void);
extern void sub_80105DC(struct Sprite *);
extern void move_anim_start_t2();

void sub_80315E8(u8);
u8 sub_803163C(u8);
void sub_80316CC(u8);
void sub_8031F0C(void);
void refresh_graphics_maybe(u8, u8, u8);
void sub_80324E0(u8 a);
void sub_80327CC(void);
void sub_8032978(struct Sprite *);
void sub_80328A4(struct Sprite *);

void sub_80312F0(struct Sprite *sprite)
{
    u8 spriteId = sprite->data1;

    if (gSprites[spriteId].affineAnimEnded && !gSprites[spriteId].invisible)
    {
        if (gSprites[spriteId].animPaused)
            gSprites[spriteId].animPaused = FALSE;
        else if (gSprites[spriteId].animEnded)
        {
            gSprites[spriteId].callback = sub_80105DC;
            StartSpriteAffineAnim(&gSprites[spriteId], 0);
            sprite->callback = SpriteCallbackDummy;
        }
    }
}

void unref_sub_8031364(struct Sprite *sprite, bool8 stupid)
{
    sprite->animPaused = TRUE;
    sprite->callback = SpriteCallbackDummy;
    if (!stupid)
        StartSpriteAffineAnim(sprite, 1);
    else
        StartSpriteAffineAnim(sprite, 1);
    AnimateSprite(sprite);
}

void sub_80313A0(struct Sprite *sprite)
{
    if (!(gUnknown_02024DE8 & 1))
    {
        sprite->pos2.x += sprite->data0;
        if (sprite->pos2.x == 0)
            sprite->callback = SpriteCallbackDummy;
    }
}

void move_anim_start_t2_for_situation(u8 a, u32 b)
{
    ewram17810[gActiveBank].unk0_4 = 1;
    if (a == 0)
    {
        if (b == 0x20)
            move_anim_start_t2(gActiveBank, 6);
        else if (b == 8 || (b & 0x80))
            move_anim_start_t2(gActiveBank, 0);
        else if (b == 0x10)
            move_anim_start_t2(gActiveBank, 2);
        else if (b & 7)
            move_anim_start_t2(gActiveBank, 4);
        else if (b == 0x40)
            move_anim_start_t2(gActiveBank, 5);
        else
            ewram17810[gActiveBank].unk0_4 = 0;
    }
    else
    {
        if (b & 0x000F0000)
            move_anim_start_t2(gActiveBank, 3);
        else if (b & 7)
            move_anim_start_t2(gActiveBank, 1);
        else if (b & 0x10000000)
            move_anim_start_t2(gActiveBank, 7);
        else if (b & 0x08000000)
            move_anim_start_t2(gActiveBank, 8);
        else if (b & 0x0000E000)
            move_anim_start_t2(gActiveBank, 9);
        else
            ewram17810[gActiveBank].unk0_4 = 0;
    }
}

bool8 move_anim_start_t3(u8 a, u8 b, u8 c, u8 d, u16 e)
{
    u8 taskId;

    if (d == 0 && (e & 0x80))
    {
        gBattleMonForms[a] = e & 0x7F;
        return TRUE;
    }
    if (ewram17800[a].substituteSprite && sub_803163C(d) == 0)
        return TRUE;
    if (ewram17800[a].substituteSprite && d == 2 && gSprites[gObjectBankIDs[a]].invisible)
    {
        refresh_graphics_maybe(a, 1, gObjectBankIDs[a]);
        sub_80324E0(a);
        return TRUE;
    }
    gBattleAnimPlayerMonIndex = b;
    gBattleAnimEnemyMonIndex = c;
    ewram17840.unk0 = e;
    DoMoveAnim(gBattleAnims_Unknown1, d, 0);
    taskId = CreateTask(sub_80315E8, 10);
    gTasks[taskId].data[0] = a;
    ewram17810[gTasks[taskId].data[0]].unk0_5 = 1;
    return FALSE;
}

void sub_80315E8(u8 taskId)
{
    gAnimScriptCallback();
    if (!gAnimScriptActive)
    {
        ewram17810[gTasks[taskId].data[0]].unk0_5 = 0;
        DestroyTask(taskId);
    }
}

u8 sub_803163C(u8 a)
{
    switch (a)
    {
    case 2:
    case 10:
    case 11:
    case 12:
    case 13:
    case 17:
        return 1;
    default:
        return 0;
    }
}

void move_anim_start_t4(u8 a, u8 b, u8 c, u8 d)
{
    u8 taskId;

    gBattleAnimPlayerMonIndex = b;
    gBattleAnimEnemyMonIndex = c;
    DoMoveAnim(gBattleAnims_Unknown2, d, 0);
    taskId = CreateTask(sub_80316CC, 10);
    gTasks[taskId].data[0] = a;
    ewram17810[gTasks[taskId].data[0]].unk0_6 = 1;
}

void sub_80316CC(u8 taskId)
{
    gAnimScriptCallback();
    if (!gAnimScriptActive)
    {
        ewram17810[gTasks[taskId].data[0]].unk0_6 = 0;
        DestroyTask(taskId);
    }
}

u8 sub_8031720(int unused1, int unused2)
{
    return 0;
}

bool8 mplay_80342A4(u8 a)
{
    u8 zero = 0;

    if (IsSEPlaying())
    {
        ewram17810[a].unk8++;
        if (ewram17810[gActiveBank].unk8 < 30)
            return TRUE;
        m4aMPlayStop(&gMPlay_SE1);
        m4aMPlayStop(&gMPlay_SE2);
    }
    if (zero == 0)
    {
        ewram17810[a].unk8 = 0;
        return FALSE;
    }
    return TRUE;
}

void BattleLoadOpponentMonSprite(struct Pokemon *pkmn, u8 b)
{
    u32 personalityValue;
    u16 species;
    u32 r7;
    u32 otId;
    u8 var;
    u16 paletteOffset;
    const u8 *lzPaletteData;

    personalityValue = GetMonData(pkmn, MON_DATA_PERSONALITY);
    if (ewram17800[b].transformedSpecies == 0)
    {
        species = GetMonData(pkmn, MON_DATA_SPECIES);
        r7 = personalityValue;
    }
    else
    {
        species = ewram17800[b].transformedSpecies;
        r7 = gPID_perBank[b];
    }
    otId = GetMonData(pkmn, MON_DATA_OT_ID);
    var = GetBankIdentity(b);
    HandleLoadSpecialPokePic(
      &gMonFrontPicTable[species],
      gMonFrontPicCoords[species].coords,
      gMonFrontPicCoords[species].y_offset,
      0x02000000,
      gUnknown_081FAF4C[var],
      species,
      r7);
    paletteOffset = 0x100 + b * 16;
    if (ewram17800[b].transformedSpecies == 0)
        lzPaletteData = pokemon_get_pal(pkmn);
    else
        lzPaletteData = species_and_otid_get_pal(species, otId, personalityValue);
    sub_800D238(lzPaletteData, ewram);
    LoadPalette(ewram, paletteOffset, 0x20);
    LoadPalette(ewram, 0x80 + b * 16, 0x20);
    if (species == SPECIES_CASTFORM)
    {
        paletteOffset = 0x100 + b * 16;
        sub_800D238(lzPaletteData, ewram + 0x16400);
        LoadPalette(ewram + 0x16400 + gBattleMonForms[b] * 32, paletteOffset, 0x20);
    }
    if (ewram17800[b].transformedSpecies != 0)
    {
        BlendPalette(paletteOffset, 16, 6, 0x7FFF);
        CpuCopy32(gPlttBufferFaded + paletteOffset, gPlttBufferUnfaded + paletteOffset, 32);
    }
}

void BattleLoadPlayerMonSprite(struct Pokemon *pkmn, u8 b)
{
    u32 personalityValue;
    u16 species;
    u32 r7;
    u32 otId;
    u8 var;
    u16 paletteOffset;
    const u8 *lzPaletteData;

    personalityValue = GetMonData(pkmn, MON_DATA_PERSONALITY);
    if (ewram17800[b].transformedSpecies == 0)
    {
        species = GetMonData(pkmn, MON_DATA_SPECIES);
        r7 = personalityValue;
    }
    else
    {
        species = ewram17800[b].transformedSpecies;
        r7 = gPID_perBank[b];
    }
    otId = GetMonData(pkmn, MON_DATA_OT_ID);
    var = GetBankIdentity(b);
    HandleLoadSpecialPokePic(
      &gMonBackPicTable[species],
      gMonBackPicCoords[species].coords,
      gMonBackPicCoords[species].y_offset,
      0x02000000,
      gUnknown_081FAF4C[var],
      species,
      r7);
    paletteOffset = 0x100 + b * 16;
    if (ewram17800[b].transformedSpecies == 0)
        lzPaletteData = pokemon_get_pal(pkmn);
    else
        lzPaletteData = species_and_otid_get_pal(species, otId, personalityValue);
    sub_800D238(lzPaletteData, ewram);
    LoadPalette(ewram, paletteOffset, 0x20);
    LoadPalette(ewram, 0x80 + b * 16, 0x20);
    if (species == SPECIES_CASTFORM)
    {
        paletteOffset = 0x100 + b * 16;
        sub_800D238(lzPaletteData, ewram + 0x16400);
        LoadPalette(ewram + 0x16400 + gBattleMonForms[b] * 32, paletteOffset, 0x20);
    }
    if (ewram17800[b].transformedSpecies != 0)
    {
        BlendPalette(paletteOffset, 16, 6, 0x7FFF);
        CpuCopy32(gPlttBufferFaded + paletteOffset, gPlttBufferUnfaded + paletteOffset, 32);
    }
}

void unref_sub_8031A64(void)
{
}

void nullsub_9(u16 unused)
{
}

void sub_8031A6C(u16 a, u8 b)
{
    u8 status;
    struct CompressedSpriteSheet spriteSheet;

    status = GetBankIdentity(b);
    DecompressPicFromTable_2(
      &gTrainerFrontPicTable[a],
      gTrainerFrontPicCoords[a].coords,
      gTrainerFrontPicCoords[a].y_offset,
      (void *)0x02000000,
      gUnknown_081FAF4C[status],
      0);
    spriteSheet.data = gUnknown_081FAF4C[status];
    spriteSheet.size = gTrainerFrontPicTable[a].size;
    spriteSheet.tag = gTrainerFrontPicTable[a].tag;
    LoadCompressedObjectPic(&spriteSheet);
    LoadCompressedObjectPalette(&gTrainerFrontPicPaletteTable[a]);
}

void LoadPlayerTrainerBankSprite(u16 a, u8 b)
{
    u8 status;

    status = GetBankIdentity(b);
    DecompressPicFromTable_2(
      &gTrainerBackPicTable[a],
      gTrainerBackPicCoords[a].coords,
      gTrainerBackPicCoords[a].y_offset,
      (void *)0x02000000,
      gUnknown_081FAF4C[status],
      0);
    LoadCompressedPalette(gTrainerBackPicPaletteTable[a].data, 0x100 + b * 16, 32);
}

void nullsub_10(int unused)
{
}

void sub_8031B74(u16 a)
{
    FreeSpritePaletteByTag(gTrainerFrontPicPaletteTable[a].tag);
    FreeSpriteTilesByTag(gTrainerFrontPicTable[a].tag);
}

void unref_sub_8031BA0(void)
{
    u8 count;
    u8 i;

    LoadSpritePalette(&gUnknown_0820A4D4[0]);
    LoadSpritePalette(&gUnknown_0820A4D4[1]);
    if (!IsDoubleBattle())
    {
        LoadCompressedObjectPic(&gUnknown_0820A47C);
        LoadCompressedObjectPic(&gUnknown_0820A484);
        count = 2;
    }
    else
    {
        LoadCompressedObjectPic(&gUnknown_0820A48C[0]);
        LoadCompressedObjectPic(&gUnknown_0820A48C[1]);
        LoadCompressedObjectPic(&gUnknown_0820A49C[0]);
        LoadCompressedObjectPic(&gUnknown_0820A49C[1]);
        count = 4;
    }
    for (i = 0; i < count; i++)
        LoadCompressedObjectPic(&gUnknown_0820A4B4[gBanksBySide[i]]);
}

bool8 sub_8031C30(u8 a)
{
    bool8 retVal = FALSE;

    if (a != 0)
    {
        if (a == 1)
        {
            LoadSpritePalette(&gUnknown_0820A4D4[0]);
            LoadSpritePalette(&gUnknown_0820A4D4[1]);
        }
        else if (!IsDoubleBattle())
        {
            if (a == 2)
            {
                if (gBattleTypeFlags & 0x80)
                    LoadCompressedObjectPic(&gUnknown_0820A4AC);
                else
                    LoadCompressedObjectPic(&gUnknown_0820A47C);
            }
            else if (a == 3)
                LoadCompressedObjectPic(&gUnknown_0820A484);
            else if (a == 4)
                LoadCompressedObjectPic(&gUnknown_0820A4B4[gBanksBySide[0]]);
            else if (a == 5)
                LoadCompressedObjectPic(&gUnknown_0820A4B4[gBanksBySide[1]]);
            else
                retVal = TRUE;
        }
        else
        {
            if (a == 2)
                LoadCompressedObjectPic(&gUnknown_0820A48C[0]);
            else if (a == 3)
                LoadCompressedObjectPic(&gUnknown_0820A48C[1]);
            else if (a == 4)
                LoadCompressedObjectPic(&gUnknown_0820A49C[0]);
            else if (a == 5)
                LoadCompressedObjectPic(&gUnknown_0820A49C[1]);
            else if (a == 6)
                LoadCompressedObjectPic(&gUnknown_0820A4B4[gBanksBySide[0]]);
            else if (a == 7)
                LoadCompressedObjectPic(&gUnknown_0820A4B4[gBanksBySide[1]]);
            else if (a == 8)
                LoadCompressedObjectPic(&gUnknown_0820A4B4[gBanksBySide[2]]);
            else if (a == 9)
                LoadCompressedObjectPic(&gUnknown_0820A4B4[gBanksBySide[3]]);
            else
                retVal = TRUE;
        }
    }
    return retVal;
}

void load_gfxc_health_bar(void)
{
    sub_800D238(gUnknown_08D09C48, (void *)0x02000000);
}

u8 battle_load_something(u8 *pState, u8 *b)
{
    bool8 retVal = FALSE;

    switch (*pState)
    {
    case 0:
        sub_8031F0C();
        (*pState)++;
        break;
    case 1:
        if (sub_8031C30(*b) == 0)
        {
            (*b)++;
        }
        else
        {
            *b = 0;
            (*pState)++;
        }
        break;
    case 2:
        (*pState)++;
        break;
    case 3:
        if ((gBattleTypeFlags & 0x80) && *b == 0)
            gHealthboxIDs[*b] = battle_make_oam_safari_battle();
        else
            gHealthboxIDs[*b] = battle_make_oam_normal_battle(*b);
        (*b)++;
        if (*b == gNoOfAllBanks)
        {
            *b = 0;
            (*pState)++;
        }
        break;
    case 4:
        sub_8043F44(*b);
        if (gBanksBySide[*b] <= 1)
            nullsub_11(gHealthboxIDs[*b], 0);
        else
            nullsub_11(gHealthboxIDs[*b], 1);
        (*b)++;
        if (*b == gNoOfAllBanks)
        {
            *b = 0;
            (*pState)++;
        }
        break;
    case 5:
        if (GetBankSide(*b) == 0)
        {
            if (!(gBattleTypeFlags & 0x80))
                sub_8045A5C(gHealthboxIDs[*b], &gPlayerParty[gBattlePartyID[*b]], 0);
        }
        else
        {
            sub_8045A5C(gHealthboxIDs[*b], &gEnemyParty[gBattlePartyID[*b]], 0);
        }
        sub_8043DB0(gHealthboxIDs[*b]);
        (*b)++;
        if (*b == gNoOfAllBanks)
        {
            *b = 0;
            (*pState)++;
        }
        break;
    case 6:
        sub_80327CC();
        sub_8094958();
        retVal = TRUE;
        break;
    }
    return retVal;
}

void sub_8031EE8(void)
{
    memset(ewram17810, 0, 0x30);
    memset(&ewram17840, 0, 0x10);
}

void sub_8031F0C(void)
{
    sub_8031EE8();
    memset(ewram17800, 0, 0x10);
}

void sub_8031F24(void)
{
    s32 i;

    for (i = 0; i < gNoOfAllBanks; i++)
        ewram17800[i].invisible = gSprites[gObjectBankIDs[i]].invisible;
}

void sub_8031F88(u8 a)
{
    ewram17800[a].invisible = gSprites[gObjectBankIDs[a]].invisible;
}

void sub_8031FC4(u8 a, u8 b, bool8 c)
{
    u16 paletteOffset;
    u16 species;
    u32 personalityValue;
    u32 otId;
    u8 r10;
    const u8 *lzPaletteData;

    if (c)
    {
        StartSpriteAnim(&gSprites[gObjectBankIDs[a]], ewram17840.unk0);
        paletteOffset = 0x100 + a * 16;
        LoadPalette(ewram + 0x16400 + ewram17840.unk0 * 32, paletteOffset, 32);
        gBattleMonForms[a] = ewram17840.unk0;
        if (ewram17800[a].transformedSpecies != 0)
        {
            BlendPalette(paletteOffset, 16, 6, 0x7FFF);
            CpuCopy32(gPlttBufferFaded + paletteOffset, gPlttBufferUnfaded + paletteOffset, 32);
        }
        gSprites[gObjectBankIDs[a]].pos1.y = sub_8077F68(a);
    }
    else
    {
        const void *src;
        void *dst;

        if (IsContest())
        {
            r10 = 0;
            species = ewram19348.unk2;
            personalityValue = ewram19348.unk8;
            otId = ewram19348.unkC;
            HandleLoadSpecialPokePic(
              &gMonBackPicTable[species],
              gMonBackPicCoords[species].coords,
              gMonBackPicCoords[species].y_offset,
              0x02000000,
              gUnknown_081FAF4C[0],
              species,
              ewram19348.unk10);
        }
        else
        {
            r10 = GetBankIdentity(a);
            if (GetBankSide(b) == 1)
                species = GetMonData(&gEnemyParty[gBattlePartyID[b]], MON_DATA_SPECIES);
            else
                species = GetMonData(&gPlayerParty[gBattlePartyID[b]], MON_DATA_SPECIES);
            if (GetBankSide(a) == 0)
            {
                personalityValue = GetMonData(&gPlayerParty[gBattlePartyID[a]], MON_DATA_PERSONALITY);
                otId = GetMonData(&gPlayerParty[gBattlePartyID[a]], MON_DATA_OT_ID);
                HandleLoadSpecialPokePic(
                  &gMonBackPicTable[species],
                  gMonBackPicCoords[species].coords,
                  gMonBackPicCoords[species].y_offset,
                  0x02000000,
                  gUnknown_081FAF4C[r10],
                  species,
                  gPID_perBank[a]);
            }
            else
            {
                personalityValue = GetMonData(&gEnemyParty[gBattlePartyID[a]], MON_DATA_PERSONALITY);
                otId = GetMonData(&gEnemyParty[gBattlePartyID[a]], MON_DATA_OT_ID);
                HandleLoadSpecialPokePic(
                  &gMonFrontPicTable[species],
                  gMonFrontPicCoords[species].coords,
                  gMonFrontPicCoords[species].y_offset,
                  0x02000000,
                  gUnknown_081FAF4C[r10],
                  species,
                  gPID_perBank[a]);
            }
        }
        src = gUnknown_081FAF4C[r10];
        dst = (void *)(VRAM + 0x10000 + gSprites[gObjectBankIDs[a]].oam.tileNum * 32);
        DmaCopy32(3, src, dst, 0x800);
        paletteOffset = 0x100 + a * 16;
        lzPaletteData = species_and_otid_get_pal(species, otId, personalityValue);
        sub_800D238(lzPaletteData, ewram);
        LoadPalette(ewram, paletteOffset, 32);
        if (species == SPECIES_CASTFORM)
        {
            u16 *paletteSrc = (u16 *)(ewram + 0x16400);

            sub_800D238(lzPaletteData, paletteSrc);
            LoadPalette(paletteSrc + gBattleMonForms[b] * 16, paletteOffset, 32);
        }
        BlendPalette(paletteOffset, 16, 6, 0x7FFF);
        CpuCopy32(gPlttBufferFaded + paletteOffset, gPlttBufferUnfaded + paletteOffset, 32);
        if (!IsContest())
        {
            ewram17800[a].transformedSpecies = species;
            gBattleMonForms[a] = gBattleMonForms[b];
        }
        gSprites[gObjectBankIDs[a]].pos1.y = sub_8077F68(a);
        StartSpriteAnim(&gSprites[gObjectBankIDs[a]], gBattleMonForms[a]);
    }
}

void BattleLoadSubstituteSprite(u8 a, u8 b)
{
    u8 r4;
    u16 foo;
    const u8 *gSubstituteDollPal_;
    void *src;
    s32 i;

    if (b == 0)
    {
        if (IsContest())
            r4 = 0;
        else
            r4 = GetBankIdentity(a);
        if (IsContest())
            LZDecompressVram(gSubstituteDollTilemap, gUnknown_081FAF4C[r4]);
        else if (GetBankSide(a) != 0)
            LZDecompressVram(gSubstituteDollGfx, gUnknown_081FAF4C[r4]);
        else
            LZDecompressVram(gSubstituteDollTilemap, gUnknown_081FAF4C[r4]);
        // There is probably a way to do this without all the temp variables, but I couldn't figure it out.
        foo = a * 16;
        gSubstituteDollPal_ = gSubstituteDollPal;
        src = gUnknown_081FAF4C[r4];
        for (i = 0; i < 3; i++)
            DmaCopy32(3, src, src + i * 0x800 + 0x800, 0x800);
        LoadCompressedPalette(gSubstituteDollPal_, 0x100 + foo, 32);
    }
    else
    {
        if (!IsContest())
        {
            if (GetBankSide(a) != 0)
                BattleLoadOpponentMonSprite(&gEnemyParty[gBattlePartyID[a]], a);
            else
                BattleLoadPlayerMonSprite(&gPlayerParty[gBattlePartyID[a]], a);
        }
    }
}

void refresh_graphics_maybe(u8 a, u8 b, u8 spriteId)
{
    BattleLoadSubstituteSprite(a, b);
    StartSpriteAnim(&gSprites[spriteId], gBattleMonForms[a]);
    if (b == 0)
        gSprites[spriteId].pos1.y = sub_8077F7C(a);
    else
        gSprites[spriteId].pos1.y = sub_8077F68(a);
}

void sub_80324BC(u8 a, u16 b)
{
    if (b == 0xA4)
        ewram17800[a].substituteSprite = 1;
}

void sub_80324E0(u8 a)
{
    ewram17800[a].substituteSprite = 0;
}

void sub_80324F8(struct Pokemon *pkmn, u8 b)
{
    u16 hp = GetMonData(pkmn, MON_DATA_HP);
    u16 maxHP = GetMonData(pkmn, MON_DATA_MAX_HP);

    if (GetHPBarLevel(hp, maxHP) == 1)
    {
        if (!ewram17800[b].unk0_1)
        {
            if (!ewram17800[b ^ 2].unk0_1)
                PlaySE(SE_HINSI);
            ewram17800[b].unk0_1 = 1;
        }
    }
    else
    {
        ewram17800[b].unk0_1 = 0;
        if (!IsDoubleBattle())
        {
            m4aSongNumStop(SE_HINSI);
            return;
        }
        if (IsDoubleBattle() && !ewram17800[b ^ 2].unk0_1)
        {
            m4aSongNumStop(SE_HINSI);
            return;
        }
    }
}

void BattleMusicStop(void)
{
    u8 r4 = GetBankByPlayerAI(0);

    ewram17800[r4].unk0_1 = 0;
    if (IsDoubleBattle())
        ewram17800[r4 ^ 2].unk0_1 = 0;
    m4aSongNumStop(SE_HINSI);
}

u8 unref_sub_8032604(struct Pokemon *pkmn)
{
    u16 hp = GetMonData(pkmn, MON_DATA_HP);
    u16 maxHP = GetMonData(pkmn, MON_DATA_MAX_HP);

    return GetHPBarLevel(hp, maxHP);
}

void sub_8032638(void)
{
    if (gMain.inBattle)
    {
        u8 r8 = GetBankByPlayerAI(0);
        u8 r9 = GetBankByPlayerAI(2);
        u8 r4 = pokemon_order_func(gBattlePartyID[r8]);
        u8 r5 = pokemon_order_func(gBattlePartyID[r9]);

        if (GetMonData(&gPlayerParty[r4], MON_DATA_HP) != 0)
            sub_80324F8(&gPlayerParty[r4], r8);
        if (IsDoubleBattle())
        {
            if (GetMonData(&gPlayerParty[r5], MON_DATA_HP) != 0)
                sub_80324F8(&gPlayerParty[r5], r9);
        }
    }
}

void sub_80326EC(u8 a)
{
    s32 i;

    for (i = 0; i < gNoOfAllBanks; i++)
    {
        if (sub_8078874(i) != 0)
        {
            gSprites[gObjectBankIDs[i]].oam.affineMode = a;
            if (a == 0)
            {
                ewram17810[i].unk6 = gSprites[gObjectBankIDs[i]].oam.matrixNum;
                gSprites[gObjectBankIDs[i]].oam.matrixNum = 0;
            }
            else
            {
                gSprites[gObjectBankIDs[i]].oam.matrixNum = ewram17810[i].unk6;
            }
        }
    }
}

void sub_80327CC(void)
{
    u8 r5;

    LoadCompressedObjectPic(&gUnknown_081FAF24);
    r5 = GetBankByPlayerAI(1);
    ewram17810[r5].unk7 = CreateSprite(&gSpriteTemplate_81FAF34, sub_8077ABC(r5, 0), sub_8077ABC(r5, 1) + 32, 0xC8);
    gSprites[ewram17810[r5].unk7].data0 = r5;
    if (IsDoubleBattle())
    {
        r5 = GetBankByPlayerAI(3);
        ewram17810[r5].unk7 = CreateSprite(&gSpriteTemplate_81FAF34, sub_8077ABC(r5, 0), sub_8077ABC(r5, 1) + 32, 0xC8);
        gSprites[ewram17810[r5].unk7].data0 = r5;
    }
}

void sub_80328A4(struct Sprite *sprite)
{
    bool8 invisible = FALSE;
    u8 r4 = sprite->data0;
    struct Sprite *r7 = &gSprites[gObjectBankIDs[r4]];

    if (!r7->inUse || sub_8078874(r4) == 0)
    {
        sprite->callback = sub_8032978;
        return;
    }
    if (gAnimScriptActive || r7->invisible)
        invisible = TRUE;
    else if (ewram17800[r4].transformedSpecies != 0 && gEnemyMonElevation[ewram17800[r4].transformedSpecies] == 0)
        invisible = TRUE;
    if (ewram17800[r4].substituteSprite)
        invisible = TRUE;
    sprite->pos1.x = r7->pos1.x;
    sprite->pos2.x = r7->pos2.x;
    sprite->invisible = invisible;
}

void sub_8032978(struct Sprite *sprite)
{
    sprite->invisible = TRUE;
}

void sub_8032984(u8 a, u16 b)
{
    if (GetBankSide(a) != 0)
    {
        if (ewram17800[a].transformedSpecies != 0)
            b = ewram17800[a].transformedSpecies;
        if (gEnemyMonElevation[b] != 0)
            gSprites[ewram17810[a].unk7].callback = sub_80328A4;
        else
            gSprites[ewram17810[a].unk7].callback = sub_8032978;
    }
}

void sub_8032A08(u8 a)
{
    gSprites[ewram17810[a].unk7].callback = sub_8032978;
}

void sub_8032A38(void)
{
    u16 *ptr = (u16 *)(VRAM + 0x240);
    s32 i;
    s32 j;

    for (i = 0; i < 9; i++)
    {
        for (j = 0; j < 16; j++)
        {
            if (!(*ptr & 0xF000))
                *ptr |= 0xF000;
            if (!(*ptr & 0x0F00))
                *ptr |= 0x0F00;
            if (!(*ptr & 0x00F0))
                *ptr |= 0x00F0;
            if (!(*ptr & 0x000F))
                *ptr |= 0x000F;
            ptr++;
        }
    }
}

void sub_8032AA8(u8 a, u8 b)
{
    ewram17800[a].transformedSpecies = 0;
    gBattleMonForms[a] = 0;
    if (b == 0)
        sub_80324E0(a);
}
