#ifndef GUARD_EVOLUTION_GRAPHICS_H
#define GUARD_EVOLUTION_GRAPHICS_H

void LoadEvoSparkleSpriteAndPal(void);

u8 LaunchTask_PreEvoSparklesSet1(u16 arg0);
u8 LaunchTask_PreEvoSparklesSet2(void);
u8 LaunchTask_PostEvoSparklesSet1(void);
u8 LaunchTask_PostEvoSparklesSet2AndFlash(u16 arg0);
u8 LaunchTask_PostEvoSparklesSet2AndFlash_Trade(u16 arg0);
u8 sub_8149E7C(u8 preEvoSpriteID, u8 postEvoSpriteID);

#endif // GUARD_EVOLUTION_GRAPHICS_H
