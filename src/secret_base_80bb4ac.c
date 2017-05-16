#include "global.h"
#include "event_data.h"
#include "rom4.h"
#include "asm.h"
#include "script.h"
#include "vars.h"
#include "text.h"
#include "field_player_avatar.h"
#include "field_camera.h"
#include "string_util.h"
#include "map_constants.h"
#include "task.h"
#include "palette.h"
#include "decoration.h"
#include "field_weather.h"

extern u8 gUnknown_020387DC;
extern u16 gSpecialVar_0x8004;
extern u16 gSpecialVar_0x8007;
extern u16 gScriptResult;
extern const struct
{
    u16 unk_083D1358_0;
    u16 unk_083D1358_1;
} gUnknown_083D1358[7];
extern const u8 gUnknown_083D1374[4 * 16];
extern void *gUnknown_0300485C;
extern u8 sub_807D770(void);


void sub_80BB4AC(struct SecretBaseRecord *record) // 080bb4ac
{
    u16 i;
    u16 j;
    record->sbr_field_0 = 0;
    for (i=0; i<7; i++)
        record->sbr_field_2[i] = 0xff;
    for (i=0; i<4; i++)
        record->trainerId[i] = 0x00;
    record->sbr_field_e = 0;
    record->sbr_field_10 = 0;
    record->sbr_field_11 = 0;
    record->sbr_field_1_0 = 0;
    record->gender = 0;
    record->sbr_field_1_5 = 0;
    record->sbr_field_1_6 = 0;
    for (i=0; i<16; i++) {
        record->decorations[i] = 0;
        record->sbr_field_22[i] = 0;
    }
    for (i=0; i<6; i++) {
        for (j=0; j<4; j++) {
            record->partyMoves[i * 4 + j] = 0;
        }
        record->partyPersonality[i] = 0;
        record->partyEVs[i] = 0;
        record->partySpecies[i] = 0;
        record->partyHeldItems[i] = 0;
        record->partyLevels[i] = 0;
    }
}

void ResetSecretBase(u8 idx) // 80bb594
{
    sub_80BB4AC(&(gSaveBlock1.secretBases[idx]));
}

void ResetSecretBases(void) // 080bb5b4
{
    u16 i;
    for (i=0; i<20; i++)
        ResetSecretBase(i);
}

void sub_80BB5D0(void) // 080bb5d0
{
    gUnknown_020387DC = gSpecialVar_0x8004;
}

void sub_80BB5E4(void) // 80bb5e4
{
    u16 idx;
    gScriptResult = 0;
    for (idx=0; idx<20; idx++) {
        if (gUnknown_020387DC != gSaveBlock1.secretBases[idx].sbr_field_0)
            continue;
        gScriptResult = 1;
        VarSet(VAR_0x4054, idx);
        break;
    }
}

void sub_80BB63C(void) // 80bb63c
{
    if (gSaveBlock1.secretBases[0].sbr_field_0)
        gScriptResult = 1;
    else
        gScriptResult = 0;
}

u8 sub_80BB66C(void) // 80bb66c
{
    s16 x, y;
    s16 v0;
    GetXYCoordsOneStepInFrontOfPlayer(&x, &y);
    v0 = MapGridGetMetatileBehaviorAt(x, y) & 0xFFF;
    if (v0 == 0x90 || v0 == 0x91)
        return 1;
    else if (v0 == 0x92 || v0 == 0x93)
        return 2;
    else if (v0 == 0x9a || v0 == 0x9b)
        return 3;
    else if (v0 == 0x94 || v0 == 0x95)
        return 4;
    else if (v0 == 0x96 || v0 == 0x97 || v0 == 0x9c || v0 == 0x9d)
        return 5;
    else if (v0 == 0x98 || v0 == 0x99)
        return 6;
    return 0;
}

void sub_80BB70C(void) // 80bb70c
{
    gSpecialVar_0x8007 = sub_80BB66C();
}

s16 unref_sub_80BB724(u16 *a0, u8 a1)
{
    u16 v2;
    for (v2=0; v2<0x200; v2++) {
        if ((a0[v2] & 0xFFF) == a1)
            return (s16)v2;
    }
    return -1;
}

void sub_80BB764(s16 *arg1, s16 *arg2, u16 arg3)
{
    s16 x, y;
    for (y=0; y<gMapHeader.mapData->height; y++) {
        for (x=0; x<gMapHeader.mapData->width; x++) {
            if ((gMapHeader.mapData->map[y * gMapHeader.mapData->width + x] & 0x3ff) == arg3) {
                *arg1 = x;
                *arg2 = y;
                return;
            }
        }
    }
}

void sub_80BB800(void)
{
    s16 x, y;
    s16 tile_id;
    u16 idx;
    GetXYCoordsOneStepInFrontOfPlayer(&x, &y);
    tile_id = MapGridGetMetatileIdAt(x, y);
    for (idx=0; idx<7; idx++) {
        if (gUnknown_083D1358[idx].unk_083D1358_0 == tile_id) {
            MapGridSetMetatileIdAt(x, y, gUnknown_083D1358[idx].unk_083D1358_1 | 0xc00);
            CurrentMapDrawMetatileAt(x, y);
            return;
        }
    }
    for (idx=0; idx<7; idx++) {
        if (gUnknown_083D1358[idx].unk_083D1358_1 == tile_id) {
            MapGridSetMetatileIdAt(x, y, gUnknown_083D1358[idx].unk_083D1358_0 | 0xc00);
            CurrentMapDrawMetatileAt(x, y);
            return;
        }
    }
}

u8 sub_80BB8A8(u8 *arg1)
{
    u8 idx;
    for (idx=0; idx<7; idx++) {
        if (arg1[idx] == EOS)
            return idx;
    }
    return 7;
}

void sub_80BB8CC(void)
{
    u8 nameLength;
    u16 idx;
    gSaveBlock1.secretBases[0].sbr_field_0 = gUnknown_020387DC;
    for (idx=0; idx<4; idx++) {
        gSaveBlock1.secretBases[0].trainerId[idx] = gSaveBlock2.playerTrainerId[idx];
    }
    VarSet(VAR_0x4054, 0);
    nameLength = sub_80BB8A8(gSaveBlock2.playerName);
    memset(gSaveBlock1.secretBases[0].sbr_field_2, 0xFF, 7);
    StringCopyN(gSaveBlock1.secretBases[0].sbr_field_2, gSaveBlock2.playerName, nameLength);
    gSaveBlock1.secretBases[0].gender = gSaveBlock2.playerGender;
    VarSet(VAR_SECRET_BASE_MAP, gMapHeader.name);
}

void sub_80BB970(struct MapEvents *events)
{
    u16 bgevidx, idx, jdx;
    s16 tile_id;
    for (bgevidx=0; bgevidx<events->bgEventCount; bgevidx++) {
        if (events->bgEvents[bgevidx].kind == 8) {
            for (jdx=0; jdx<20; jdx++) {
                if (gSaveBlock1.secretBases[jdx].sbr_field_0 == events->bgEvents[bgevidx].bgUnion.secretBaseId) {
                    tile_id = MapGridGetMetatileIdAt(events->bgEvents[bgevidx].x + 7, events->bgEvents[bgevidx].y + 7);
                    for (idx=0; idx<7; idx++) {
                        if (gUnknown_083D1358[idx].unk_083D1358_0 == tile_id) {
                            MapGridSetMetatileIdAt(events->bgEvents[bgevidx].x + 7, events->bgEvents[bgevidx].y + 7, gUnknown_083D1358[idx].unk_083D1358_1 | 0xc00);
                            break;
                        }
                    }
                    break;
                }
            }
        }
    }
}

void sub_80BBA14(void)
{
    s8 idx = 4 * (gUnknown_020387DC / 10);
    warp1_set_2(MAP_GROUP_SECRET_BASE_RED_CAVE1, gUnknown_083D1374[idx], gUnknown_083D1374[idx + 1]);
}

void sub_80BBA48(u8 taskid)
{
    u16 curbaseid;
    switch (gTasks[taskid].data[0]) {
        case 0:
            gTasks[taskid].data[0] = 1;
            break;
        case 1:
            if (!gPaletteFade.active) {
                gTasks[taskid].data[0] = 2;
            }
            break;
        case 2:
            curbaseid = VarGet(VAR_0x4054);
            if (gSaveBlock1.secretBases[curbaseid].sbr_field_10 < 0xff)
                gSaveBlock1.secretBases[curbaseid].sbr_field_10 ++;
            sub_80BBA14();
            warp_in();
            gUnknown_0300485C = sub_8080990;
            SetMainCallback2(CB2_LoadMap);
            DestroyTask(taskid);
            break;
    }
}

void sub_80BBAF0(void)
{
    CreateTask(sub_80BBA48, 0);
    fade_screen(1, 0);
    saved_warp2_set(0, gSaveBlock1.location.mapGroup, gSaveBlock1.location.mapNum, -1);
}

bool8 sub_80BBB24(void)
{
    if (gMapHeader.mapType == 9 && VarGet(VAR_0x4097) == 0)
        return FALSE;
    return TRUE;
}

void sub_80BBB50(u8 taskid)
{
    FieldObjectTurn(&(gMapObjects[gPlayerAvatar.mapObjectId]), 2);
    if (sub_807D770() == 1) {
        EnableBothScriptContexts();
        DestroyTask(taskid);
    }
}

void sub_80BBB90(void)
{
    s16 x, y;
    ScriptContext2_Enable();
    HideMapNamePopup();
    sub_80BB764(&x, &y, 0x220);
    MapGridSetMetatileIdAt(x + 7, y + 7, 0xe20);
    CurrentMapDrawMetatileAt(x + 7, y + 7);
    pal_fill_black();
    CreateTask(sub_80BBB50, 0);
}

void sub_80BBBEC(u8 taskid)
{
    s8 idx;
    if (!gPaletteFade.active) {
        idx = 4 * (gUnknown_020387DC / 10);
        warp1_set(gSaveBlock1.location.mapGroup, gSaveBlock1.location.mapNum, -1, gUnknown_083D1374[idx + 2], gUnknown_083D1374[idx + 3]);
        warp_in();
        gUnknown_0300485C = sub_80BBB90;
        SetMainCallback2(CB2_LoadMap);
        DestroyTask(taskid);
    }
}

void sub_80BBC78(void)
{
    u8 taskid = CreateTask(sub_80BBBEC, 0);
    gTasks[taskid].data[0] = 0;
    fade_screen(1, 0);
}

bool8 CurrentMapIsSecretBase(void)
{
    if (gSaveBlock1.location.mapGroup == MAP_GROUP_SECRET_BASE_SHRUB4 && (u8)(gSaveBlock1.location.mapNum) <= MAP_ID_SECRET_BASE_SHRUB4)
        return TRUE;
    return FALSE;
}

#ifdef NONMATCHING
void sub_80BBCCC(u8 flagIn)
{
    u16 curBaseId;
    u16 x, y;
    if (CurrentMapIsSecretBase()) {
        curBaseId = VarGet(VAR_0x4054);
        for (x=0; x<16; x++) {
            if ((u8)(gSaveBlock1.secretBases[curBaseId].decorations[x] - 1) <= 0x77 && gDecorations[gSaveBlock1.secretBases[curBaseId].decorations[x]].decor_field_11 != 4) {
                sub_80FF394((gSaveBlock1.secretBases[0].sbr_field_22[x] >> 4) + 7, (gSaveBlock1.secretBases[0].sbr_field_22[x] & 0xF) + 7, gSaveBlock1.secretBases[curBaseId].decorations[x]);
            }
        }
        if (curBaseId != 0) {
            sub_80BB764(&x, &y, 0x220);
            MapGridSetMetatileIdAt(x + 7, y + 7, 0xe21);
        } else if (flagIn == 1 && VarGet(VAR_0x4089) == 1) {
            sub_80BB764(&x, &y, 0x220);
            MapGridSetMetatileIdAt(x + 7, y + 7, 0xe0a);
        }
    }
}
#else
__attribute__((naked))
void sub_80BBCCC(u8 flagIn)
{
    asm(".syntax unified\n\
    push {r4-r7,lr}\n\
	mov r7, r8\n\
	push {r7}\n\
	sub sp, 0x4\n\
	lsls r0, 24\n\
	lsrs r0, 24\n\
	mov r8, r0\n\
	bl CurrentMapIsSecretBase\n\
	lsls r0, 24\n\
	cmp r0, 0\n\
	beq _080BBDBC\n\
	ldr r0, _080BBD70 @ =0x00004054\n\
	bl VarGet\n\
	lsls r0, 16\n\
	lsrs r5, r0, 16\n\
	movs r1, 0\n\
	mov r0, sp\n\
	strh r1, [r0]\n\
	ldr r6, _080BBD74 @ =gSaveBlock1\n\
	mov r4, sp\n\
	ldr r0, _080BBD78 @ =0x00001a2a\n\
	adds r7, r6, r0\n\
_080BBCFC:\n\
	lsls r0, r5, 2\n\
	adds r0, r5\n\
	lsls r0, 5\n\
	ldrh r1, [r4]\n\
	adds r2, r0, r1\n\
	ldr r1, _080BBD7C @ =0x00001a1a\n\
	adds r0, r6, r1\n\
	adds r1, r2, r0\n\
	ldrb r0, [r1]\n\
	subs r0, 0x1\n\
	lsls r0, 24\n\
	lsrs r0, 24\n\
	cmp r0, 0x77\n\
	bhi _080BBD3A\n\
	ldr r0, _080BBD80 @ =gDecorations\n\
	ldrb r3, [r1]\n\
	lsls r1, r3, 5\n\
	adds r1, r0\n\
	ldrb r0, [r1, 0x11]\n\
	cmp r0, 0x4\n\
	beq _080BBD3A\n\
	adds r0, r2, r7\n\
	ldrb r2, [r0]\n\
	lsrs r0, r2, 4\n\
	adds r0, 0x7\n\
	movs r1, 0xF\n\
	ands r1, r2\n\
	adds r1, 0x7\n\
	adds r2, r3, 0\n\
	bl sub_80FF394\n\
_080BBD3A:\n\
	ldrh r0, [r4]\n\
	adds r0, 0x1\n\
	lsls r0, 16\n\
	lsrs r0, 16\n\
	strh r0, [r4]\n\
	cmp r0, 0xF\n\
	bls _080BBCFC\n\
	cmp r5, 0\n\
	beq _080BBD88\n\
	mov r4, sp\n\
	adds r4, 0x2\n\
	movs r2, 0x88\n\
	lsls r2, 2\n\
	mov r0, sp\n\
	adds r1, r4, 0\n\
	bl sub_80BB764\n\
	mov r0, sp\n\
	ldrh r0, [r0]\n\
	adds r0, 0x7\n\
	ldrh r1, [r4]\n\
	adds r1, 0x7\n\
	ldr r2, _080BBD84 @ =0x00000e21\n\
	bl MapGridSetMetatileIdAt\n\
	b _080BBDBC\n\
	.align 2, 0\n\
_080BBD70: .4byte 0x00004054\n\
_080BBD74: .4byte gSaveBlock1\n\
_080BBD78: .4byte 0x00001a2a\n\
_080BBD7C: .4byte 0x00001a1a\n\
_080BBD80: .4byte gDecorations\n\
_080BBD84: .4byte 0x00000e21\n\
_080BBD88:\n\
	mov r0, r8\n\
	cmp r0, 0x1\n\
	bne _080BBDBC\n\
	ldr r0, _080BBDC8 @ =0x00004089\n\
	bl VarGet\n\
	lsls r0, 16\n\
	lsrs r0, 16\n\
	cmp r0, 0x1\n\
	bne _080BBDBC\n\
	mov r4, sp\n\
	adds r4, 0x2\n\
	movs r2, 0x88\n\
	lsls r2, 2\n\
	mov r0, sp\n\
	adds r1, r4, 0\n\
	bl sub_80BB764\n\
	mov r0, sp\n\
	ldrh r0, [r0]\n\
	adds r0, 0x7\n\
	ldrh r1, [r4]\n\
	adds r1, 0x7\n\
	ldr r2, _080BBDCC @ =0x00000e0a\n\
	bl MapGridSetMetatileIdAt\n\
_080BBDBC:\n\
	add sp, 0x4\n\
	pop {r3}\n\
	mov r8, r3\n\
	pop {r4-r7}\n\
	pop {r0}\n\
	bx r0\n\
	.align 2, 0\n\
_080BBDC8: .4byte 0x00004089\n\
_080BBDCC: .4byte 0x00000e0a\n\
.syntax divided\n");
}
#endif
