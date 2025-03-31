#include "global.h"
#include "functions.h"
#include "m4a.h"
#include "main.h"
#include "constants/bg_music.h"
#include "constants/ereader.h"
#include "constants/fields.h"
#include "constants/species.h"
#include "constants/pinball_game.h"

void sub_32DF8(void);

extern const u16 gWildMonLocations[AREA_COUNT][2][WILD_MON_LOCATION_COUNT];
extern const u16 gEggLocations[MAIN_FIELD_COUNT][26];

#define BONUS_DUSKULL_TIME 7200 //2 minutes, 60FPS

static inline u32 GetTimeAdjustedRandom()
{
    return Random() + (gMain.systemFrameCount + gMain.fieldFrameCount);
}

void sub_31F6C(void)
{
    s16 threeArrows;
    s16 i;
    s16 j;
    s16 weight;
    s16 currentSpecies;
    s16 evolutionWeight;

    gCurrentPinballGame->unk12E = 0;
    if (gCurrentPinballGame->catchModeArrows == 3)
        threeArrows = 1;
    else
        threeArrows = 0;

    for (i = 0; i < WILD_MON_LOCATION_COUNT; i++)
    {
        currentSpecies = gWildMonLocations[gCurrentPinballGame->area][threeArrows][i];
        switch (currentSpecies)
        {
            // Rare pokemon
            case SPECIES_NOSEPASS:
            case SPECIES_SKARMORY:
            case SPECIES_LILEEP:
            case SPECIES_ANORITH:
            case SPECIES_FEEBAS:
            case SPECIES_CASTFORM:
            case SPECIES_KECLEON:
            case SPECIES_ABSOL:
            case SPECIES_WOBBUFFET:
                if (gMain.eReaderBonuses[EREADER_ENCOUNTER_RATE_UP_CARD])
                {
                    if (gMain_saveData.pokedexFlags[currentSpecies] < 2)
                        weight = 2;
                    else
                        weight = 4;
                }
                else
                {
                    if (gMain_saveData.pokedexFlags[currentSpecies] < 2)
                        weight = 1;
                    else
                        weight = 2;
                }

                if (gCurrentPinballGame->caughtMonCount == 0)
                    weight = 0;
                break;

            case SPECIES_CLAMPERL:
                weight = gCommonAndEggWeights[gMain_saveData.pokedexFlags[SPECIES_CLAMPERL]];
                evolutionWeight = gCommonAndEggWeights[gMain_saveData.pokedexFlags[SPECIES_HUNTAIL]];
                if (weight < evolutionWeight)
                    weight = evolutionWeight;
                evolutionWeight = gCommonAndEggWeights[gMain_saveData.pokedexFlags[SPECIES_GOREBYSS]];
                if (weight < evolutionWeight)
                    weight = evolutionWeight;
                break;
            case SPECIES_NONE:
                weight = 0;
                break;
            default:
                weight = gCommonAndEggWeights[gMain_saveData.pokedexFlags[currentSpecies]];
                for (j = 0; j < 2; j++)
                {
                    currentSpecies = gSpeciesInfo[currentSpecies].evolutionTarget;
                    if (currentSpecies < SPECIES_NONE)
                    {
                        evolutionWeight = gCommonAndEggWeights[gMain_saveData.pokedexFlags[currentSpecies]];
                        if (weight < evolutionWeight)
                        {
                            weight = evolutionWeight;
                        }
                    }
                    else
                    {
                        break;
                    }
                }
                currentSpecies = gWildMonLocations[gCurrentPinballGame->area][threeArrows][i];
                if (gCurrentPinballGame->caughtMonCount == 0 && gSpeciesInfo[currentSpecies].evolutionTarget >= SPECIES_NONE)
                {
                    weight = 0;
                }
                break;
        }

        if (gCurrentPinballGame->lastCatchSpecies == currentSpecies)
        {
            weight = 0;
        }
        gCurrentPinballGame->unk12E += weight;
        gCurrentPinballGame->unk130[i] = gCurrentPinballGame->unk12E;
    }
}

void sub_3219C(void)
{
    s16 i;
    u32 rand;
    u16 specialMons[6];

    if (gMain.eReaderBonuses[EREADER_SPECIAL_GUESTS_CARD])
    {
        gMain.eReaderBonuses[EREADER_SPECIAL_GUESTS_CARD] = 0;
        rand = GetTimeAdjustedRandom();
        rand %= NUM_BONUS_SPECIES;
        for (i = 0; i < NUM_BONUS_SPECIES; i++)
        {
            if (gMain_saveData.pokedexFlags[BONUS_SPECIES_START + ((i + rand) % NUM_BONUS_SPECIES)] < 4)
                break;
        }

        gCurrentPinballGame->currentSpecies = BONUS_SPECIES_START + ((i + rand) % NUM_BONUS_SPECIES);
    }
    else
    {
        rand = GetTimeAdjustedRandom();
        if (gMain.eReaderBonuses[EREADER_ENCOUNTER_RATE_UP_CARD])
            rand %= 50;
        else
            rand %= 100;

        if (gUnknown_02031520.unk8 < 100)
            rand = 1;

        if ((rand == 0 && gCurrentPinballGame->caughtMonCount >= 5) || gCurrentPinballGame->forceSpecialMons)
        {
            s16 numSpecialMons = 0;
            gCurrentPinballGame->currentSpecies = 0;
            if (gMain_saveData.pokedexFlags[SPECIES_AERODACTYL])
            {
                specialMons[numSpecialMons++] = SPECIES_AERODACTYL;
                if (gMain_saveData.pokedexFlags[SPECIES_AERODACTYL] < 4)
                    gCurrentPinballGame->currentSpecies = SPECIES_AERODACTYL;
            }

            if (gMain_saveData.pokedexFlags[SPECIES_CHIKORITA])
            {
                specialMons[numSpecialMons++] = SPECIES_CHIKORITA;
                if (gMain_saveData.pokedexFlags[SPECIES_CHIKORITA] < 4)
                    gCurrentPinballGame->currentSpecies = SPECIES_CHIKORITA;
            }

            if (gMain_saveData.pokedexFlags[SPECIES_TOTODILE])
            {
                specialMons[numSpecialMons++] = SPECIES_TOTODILE;
                if (gMain_saveData.pokedexFlags[SPECIES_TOTODILE] < 4)
                    gCurrentPinballGame->currentSpecies = SPECIES_TOTODILE;
            }

            if (gMain_saveData.pokedexFlags[SPECIES_CYNDAQUIL])
            {
                specialMons[numSpecialMons++] = SPECIES_CYNDAQUIL;
                if (gMain_saveData.pokedexFlags[SPECIES_CYNDAQUIL] < 4)
                    gCurrentPinballGame->currentSpecies = SPECIES_CYNDAQUIL;
            }

            if (gMain.selectedField == FIELD_RUBY)
            {
                specialMons[numSpecialMons++] = SPECIES_LATIOS;
                if (gMain_saveData.pokedexFlags[SPECIES_LATIOS] < 4)
                    gCurrentPinballGame->currentSpecies = SPECIES_LATIOS;
            }
            else
            {
                specialMons[numSpecialMons++] = SPECIES_LATIAS;
                if (gMain_saveData.pokedexFlags[SPECIES_LATIAS] < 4)
                    gCurrentPinballGame->currentSpecies = SPECIES_LATIAS;
            }

            if (gCurrentPinballGame->currentSpecies == 0)
            {
                rand = GetTimeAdjustedRandom();
                rand %= numSpecialMons;
                gCurrentPinballGame->currentSpecies = specialMons[rand];
            }
        }
        else
        {
            s16 threeArrows;
            if (gCurrentPinballGame->catchModeArrows == 3)
                threeArrows = 1;
            else
                threeArrows = 0;

            rand = GetTimeAdjustedRandom();
            rand %= gCurrentPinballGame->unk12E;
            for (i = 0; i < WILD_MON_LOCATION_COUNT && gCurrentPinballGame->unk130[i] <= rand; i++);

            gCurrentPinballGame->currentSpecies = gWildMonLocations[gCurrentPinballGame->area][threeArrows][i];
        }
    }

    gCurrentPinballGame->lastCatchSpecies = gCurrentPinballGame->currentSpecies;
}

void sub_32484(void)
{
    s16 i;
    s16 currentSpecies;
    s16 weight;
    s16 j;
    s16 evolutionWeight;

    gCurrentPinballGame->unk12E = 0;

    for (i = 0; i < 25; i++)
    {
        currentSpecies = gEggLocations[gMain.selectedField][i];

        if (currentSpecies == SPECIES_ODDISH)
        {
            if (gMain.selectedField == FIELD_RUBY)
                weight = gCommonAndEggWeights[gMain_saveData.pokedexFlags[SPECIES_VILEPLUME]];
            else
                weight = gCommonAndEggWeights[gMain_saveData.pokedexFlags[SPECIES_BELLOSSOM]];
        }
        else
        {
            weight = gCommonAndEggWeights[gMain_saveData.pokedexFlags[currentSpecies]];

            for (j = 0; j < 2; j++)
            {
                currentSpecies = gSpeciesInfo[currentSpecies].evolutionTarget;
                if (currentSpecies < SPECIES_NONE)
                {
                    evolutionWeight = gCommonAndEggWeights[gMain_saveData.pokedexFlags[currentSpecies]];
                    if (weight < evolutionWeight)
                        weight = evolutionWeight;
                }
                else
                {
                    break;
                }
            }

            currentSpecies = gEggLocations[gMain.selectedField][i];
            if (gCurrentPinballGame->caughtMonCount == 0)
            {
                if (gSpeciesInfo[currentSpecies].evolutionTarget >= SPECIES_NONE)
                {
                    weight = 0;
                }
            }
        }

        if (gCurrentPinballGame->lastEggSpecies == currentSpecies)
        {
            weight = 0;
        }

        gCurrentPinballGame->unk12E += weight;
        gCurrentPinballGame->unk130[i] = gCurrentPinballGame->unk12E;
    }
}

void sub_325E0(void)
{
    s16 i;
    u32 rand;

    rand = GetTimeAdjustedRandom();
    if (gMain.eReaderBonuses[EREADER_ENCOUNTER_RATE_UP_CARD])
        rand %= 100;
    else
        rand %= 50;

    if (gCurrentPinballGame->lastEggSpecies == SPECIES_PICHU)
        rand = 1;

    if ((rand == 0 && gCurrentPinballGame->caughtMonCount >= 5) || gCurrentPinballGame->forcePichuEgg)
    {
        gCurrentPinballGame->currentSpecies = SPECIES_PICHU;
    }
    else
    {
        rand = GetTimeAdjustedRandom();
        rand %= gCurrentPinballGame->unk12E;

        for (i = 0; i < 25 && gCurrentPinballGame->unk130[i] <= rand; i++);

        gCurrentPinballGame->currentSpecies = gEggLocations[gMain.selectedField][i];
    }

    gCurrentPinballGame->lastEggSpecies = gCurrentPinballGame->currentSpecies;
}

void sub_326F4(void)
{
    gCurrentPinballGame->catchModeArrows = 2;
    gCurrentPinballGame->unk1E5 = 0;
    gCurrentPinballGame->unk1E6 = 1;

    sub_1F2A4();
    sub_21B0C();
    sub_1F59C();
    sub_300D8();
    sub_30178();

    gCurrentPinballGame->unk1D8 = 0x100;
    gCurrentPinballGame->unk1DA = 0x100;

    sub_30480();
    sub_304C8();
    sub_31144();
    sub_313A0();
    sub_2E6AC();
    sub_31B30();
}

void sub_3276C(void)
{
    sub_19B90();
    sub_327C0();

    if (!gMain.unkF)
        sub_328C8();

    sub_32DF8();

    if (gCurrentPinballGame->unk29C)
    {
        gCurrentPinballGame->unk29C--;
        if (!gCurrentPinballGame->unk29C)
            m4aMPlayVolumeControl(&gMPlayInfo_BGM, TRACKS_ALL, 0x100);
    }
}

void sub_327C0(void)
{
    s32 temp;

    sub_4F814();
    sub_1A0F4();
    sub_2CA9C();

    if (gCurrentPinballGame->unk68 < 0x5a)
    {
        sub_304C8();
        sub_31498();
    }

    if (gCurrentPinballGame->unk13 < 3)
    {
        if (gCurrentPinballGame->unk345 == 3 && gCurrentPinballGame->unk346 == 0)
        {
            gCurrentPinballGame->unk728 = 1;
        }
        else
        {
            gCurrentPinballGame->unk728 = 0;
        }
    }
    else
    {
        gCurrentPinballGame->unk728 = 0;
    }

    if (gCurrentPinballGame->unk68 < 0x96)
    {
        sub_2E6AC();
        sub_313A0();
    }
    if (gCurrentPinballGame->unk68 < 0xC4)
    {
        sub_30178();
        sub_2F140();
    }
    if (gCurrentPinballGame->unk68 < 0xCA)
    {
        sub_2FCD0();
    }
    if (gCurrentPinballGame->unk68 < 0xDC)
    {
        sub_1F59C();
    }

    sub_308DC();

    if (0x76 < gCurrentPinballGame->unk68)
    {
        sub_2F504();
        sub_30EB4();
    }

    sub_2CE80();

    if (0xA8 < gCurrentPinballGame->unk68)
        sub_2D104();

    sub_203CC();
    sub_21B0C();
    sub_1D128();
    sub_31B30();

    if (gCurrentPinballGame->unk194)
        sub_225F0();

    sub_472E4();
}

void sub_328C8(void)
{
    sub_300D8();
    sub_32968();
    sub_1F2A4();

    if (gCurrentPinballGame->unk600)
        gCurrentPinballGame->unk600--;

    sub_2F26C();
    sub_2E67C();
    sub_31144();

    if (!(gMain.unkF & 0x20))
        sub_2F79C();
}

void sub_32914(void)
{
    if (gMain.unkF != 0)
        return;

    switch (gCurrentPinballGame->unk25)
    {
    case 1:
        sub_32B74();
        break;
    case 3:
        sub_329B0();
        break;
    case 4:
        sub_1C560();
        break;
    }

    gCurrentPinballGame->unk26 = 60;
}

void sub_32968(void)
{
    if (gCurrentPinballGame->unk26)
        gCurrentPinballGame->unk26--;

    switch (gCurrentPinballGame->unk25)
    {
    case 1:
        sub_32BE4();
        break;
    case 3:
        sub_329F4();
        break;
    case 4:
        sub_1C5AC();
        break;
    }
}

void sub_329B0(void)
{
    gCurrentPinballGame->unk28 = 0xB4;
    gCurrentPinballGame->unk1BA = 0xB4;
    gCurrentPinballGame->unk3C = 500000;
    gMain.blendControl = 0xCE;
    gMain.blendBrightness = 0;
    gCurrentPinballGame->unk5F7 = 1;
}

void sub_329F4(void)
{
    s16 i;

    if (gCurrentPinballGame->unk28 > 0x18)
    {
        gCurrentPinballGame->unk132c->unk0 = 1;
        gCurrentPinballGame->unk1F = 1;
        gCurrentPinballGame->unk28--;
        gCurrentPinballGame->unk132c->velocity.x = 0;
        gCurrentPinballGame->unk132c->velocity.y = 0;
        gCurrentPinballGame->unk132c->unk10.x = 58;
        gCurrentPinballGame->unk132c->unk10.y = 178;
        gCurrentPinballGame->unk132c->unk28.x = gCurrentPinballGame->unk132c->unk10.x * 2;
        gCurrentPinballGame->unk132c->unk28.y = gCurrentPinballGame->unk132c->unk10.y * 2;
        gCurrentPinballGame->unk132c->position.x = gCurrentPinballGame->unk132c->unk10.x << 8;
        gCurrentPinballGame->unk132c->position.y = gCurrentPinballGame->unk132c->unk10.y << 8;
        
        if (gCurrentPinballGame->unk28 <= 0x31)
        {
            gCurrentPinballGame->unk28++;
        }

        sub_1B140(gCurrentPinballGame->unk1A5);

        if (gCurrentPinballGame->unk1A5)
        {
            for (i = 0; i < 2; i++)
            {
                gCurrentPinballGame->unk36C[i] = 10;
                gCurrentPinballGame->unk36E[i] = 0;
                gCurrentPinballGame->unk36A[i] = 4;
            }
        }
    }
    else if (gCurrentPinballGame->unk28 > 0)
    {
        gCurrentPinballGame->unk28--;
    }
    else
    {
        gCurrentPinballGame->unk132c->unk0 = 0;
        gCurrentPinballGame->unk1F = 0;
        gCurrentPinballGame->unk26 = 0x3C;
        gCurrentPinballGame->unk132c->velocity.x = 0x60;
        gCurrentPinballGame->unk132c->velocity.y = 0xC0;
        gCurrentPinballGame->unk132c->unk10.x = 0x3C;
        gCurrentPinballGame->unk132c->unk10.y = 0xB4;
        gCurrentPinballGame->unk132c->unk6 = 0;
        gCurrentPinballGame->unk5F7 = 0;
        gCurrentPinballGame->unk132c->unk28.x = gCurrentPinballGame->unk132c->unk10.x * 2;
        gCurrentPinballGame->unk132c->unk28.y = gCurrentPinballGame->unk132c->unk10.y * 2;
        gCurrentPinballGame->unk25 = 0;

        m4aSongNumStart(SE_UNKNOWN_0xC3);
        
        if (gCurrentPinballGame->unk72E > 2 && gCurrentPinballGame->unk281 > 0) {
            sub_19B64(6);
        }
    }
}

void sub_32B74(void)
{
    if (gCurrentPinballGame->unk72F > 1)
        sub_19B64(4);

    gCurrentPinballGame->unk28 = 100;
    gCurrentPinballGame->unk2F4 = 2;
    gCurrentPinballGame->unk5FC = 0;
    gCurrentPinballGame->unk5FE = 30;
    gCurrentPinballGame->unk5FB = 0;
    gCurrentPinballGame->unk3C = 50000;

    m4aSongNumStart(SE_UNKNOWN_0xD5);
    sub_11B0(8);
}

void sub_32BE4(void)
{
    if (gCurrentPinballGame->unk28)
    {
        gCurrentPinballGame->unk5F7 = 1;
        gCurrentPinballGame->unk1F = 1;
        gCurrentPinballGame->unk28--;
        gCurrentPinballGame->unk132c->velocity.x = 0;
        gCurrentPinballGame->unk132c->velocity.y = 0;
        gCurrentPinballGame->unk132c->unk6 = 0;
        if (gCurrentPinballGame->unk28 > 97)
        {
            gCurrentPinballGame->unk5FA = 1;
            gCurrentPinballGame->unk2F4 = 2;
            gCurrentPinballGame->unk132c->unk10.x = 0xb5;
            gCurrentPinballGame->unk132c->unk10.y = 0xc3;
        }
        else if (gCurrentPinballGame->unk28 > 94)
        {
            gCurrentPinballGame->unk2F4 = 3;
            gCurrentPinballGame->unk132c->unk10.x = 0xb8;
            gCurrentPinballGame->unk132c->unk10.y = 0xbb;
        }
        else if (gCurrentPinballGame->unk28 > 91)
        {
            gCurrentPinballGame->unk132c->unk0 = 1;
            gCurrentPinballGame->unk2F4 = 4;
        }
        else if (gCurrentPinballGame->unk28 > 83)
        {
            gCurrentPinballGame->unk2F4 = 5;
        }
        else if (gCurrentPinballGame->unk28 > 34)
        {
            gCurrentPinballGame->unk2F4 = 6;

        }
        else if (gCurrentPinballGame->unk28 > 26)
        {
            gCurrentPinballGame->unk2F4 = 7;

        }
        else if (gCurrentPinballGame->unk28 > 18)
        {
            gCurrentPinballGame->unk2F4 = 8;

        }
        else if (gCurrentPinballGame->unk28 > 10)
        {
            gCurrentPinballGame->unk2F4 = 9;
            
        }
        else if (gCurrentPinballGame->unk28 > 4)
        {
            gCurrentPinballGame->unk132c->unk0 = 0;
            gCurrentPinballGame->unk2F4 = 10;
            gCurrentPinballGame->unk132c->unk10.x = 0xb5;
            gCurrentPinballGame->unk132c->unk10.y = 0xc3;
        }
        else
        {
            gCurrentPinballGame->unk2F4 = 11;
            gCurrentPinballGame->unk132c->unk10.x = 0xb0;
            gCurrentPinballGame->unk132c->unk10.y = 0xca;
        }

        gCurrentPinballGame->unk132c->unk28.x = gCurrentPinballGame->unk132c->unk10.x * 2;
        gCurrentPinballGame->unk132c->unk28.y = gCurrentPinballGame->unk132c->unk10.y * 2;
        gCurrentPinballGame->unk132c->position.x = gCurrentPinballGame->unk132c->unk10.x << 8;
        gCurrentPinballGame->unk132c->position.y = gCurrentPinballGame->unk132c->unk10.y << 8;
    }
    else
    {
        gCurrentPinballGame->unk2F4 = 0;
        gCurrentPinballGame->unk1F = 0;
        gCurrentPinballGame->unk26 = 60;
        gCurrentPinballGame->unk132c->unk6 = 0;
        gCurrentPinballGame->unk132c->velocity.x = -0x66;
        gCurrentPinballGame->unk132c->velocity.y = 0xC8;
        sub_11B0(7);
        gCurrentPinballGame->unk132c->unk6 = 0;
        gCurrentPinballGame->unk132c->unk10.x = 0xAB;
        gCurrentPinballGame->unk132c->unk10.y = 0xD4;
        gCurrentPinballGame->unk132c->unk28.x = gCurrentPinballGame->unk132c->unk10.x * 2;
        gCurrentPinballGame->unk132c->unk28.y = gCurrentPinballGame->unk132c->unk10.y * 2;
        gCurrentPinballGame->unk132c->unk2C = gCurrentPinballGame->unk132c->unk28;
        gCurrentPinballGame->unk25 = 0;
        gCurrentPinballGame->unk5F7 = 0;
        m4aSongNumStart(SE_UNKNOWN_0xD6);
        gCurrentPinballGame->unk5FE = 0;
        gCurrentPinballGame->unk5FB = 1;
        gCurrentPinballGame->unk5FA = 0;
    }
}

void sub_32DF8(void)
{
    int tmp;

    if (gCurrentPinballGame->newButtonActions[0])
    {
        if (gCurrentPinballGame->unk1C4 == 0 && gCurrentPinballGame->unk1E2 != 2 &&
            gCurrentPinballGame->unk1C0 == 0 && gCurrentPinballGame->unk61C == 0)
        {
            gCurrentPinballGame->unk1E2 = 0;
        }

        tmp = gCurrentPinballGame->unk710;
        gCurrentPinballGame->unk710 = gCurrentPinballGame->unk711;
        gCurrentPinballGame->unk711 = gCurrentPinballGame->unk712;
        gCurrentPinballGame->unk712 = gCurrentPinballGame->unk713;
        gCurrentPinballGame->unk713 = tmp;

        tmp = gCurrentPinballGame->unk718;
        gCurrentPinballGame->unk718 = gCurrentPinballGame->unk719;
        gCurrentPinballGame->unk719 = gCurrentPinballGame->unk71A;
        gCurrentPinballGame->unk71A = tmp;
    }

    if (gCurrentPinballGame->newButtonActions[1])
    {
        if (gCurrentPinballGame->unk1C4 == 0 && gCurrentPinballGame->unk1E2 != 2 &&
            gCurrentPinballGame->unk1C0 == 0 && gCurrentPinballGame->unk61C == 0)
        {
            gCurrentPinballGame->unk1E2 = 1;
        }

        tmp = gCurrentPinballGame->unk713;
        gCurrentPinballGame->unk713 = gCurrentPinballGame->unk712;
        gCurrentPinballGame->unk712 = gCurrentPinballGame->unk711;
        gCurrentPinballGame->unk711 = gCurrentPinballGame->unk710;
        gCurrentPinballGame->unk710 = tmp;

        tmp = gCurrentPinballGame->unk71A;
        gCurrentPinballGame->unk71A = gCurrentPinballGame->unk719;
        gCurrentPinballGame->unk719 = gCurrentPinballGame->unk718;
        gCurrentPinballGame->unk718 = tmp;
    }
}

void sub_32F3C(void)
{
    s16 i;
    gCurrentPinballGame->unk18 = 0;
    gCurrentPinballGame->unk17 = 0;
    gCurrentPinballGame->unk13 = 0;
    gCurrentPinballGame->unk294 = 1;
    gCurrentPinballGame->eventTimer = gCurrentPinballGame->timerBonus + BONUS_DUSKULL_TIME;
    gCurrentPinballGame->timerBonus = 0;
    gCurrentPinballGame->unk383 = 0;
    gCurrentPinballGame->unk388 = 3;
    gCurrentPinballGame->unk132c->unk0 = 1;
    gCurrentPinballGame->unk385 = 0;
    gCurrentPinballGame->unk386 = 0;
    gCurrentPinballGame->unk387 = 0;
    gCurrentPinballGame->unk392 = 0;
    gCurrentPinballGame->unk394 = 0;
    gCurrentPinballGame->unk396 = 0;

    for (i = 0; i < 3; i++)
    {
        gCurrentPinballGame->unk397[i] = 0;
        gCurrentPinballGame->unk39A[i] = 0;
        gCurrentPinballGame->unk39D[i] = 0;
        gCurrentPinballGame->unk3A0[i] = 0;
        gCurrentPinballGame->unk3A3[i] = 0;
        gCurrentPinballGame->unk3A6[i] = 0;
        gCurrentPinballGame->unk3A9[i] = 0;
        gCurrentPinballGame->unk3AC[i] = 0;
        gCurrentPinballGame->unk3B0[i] = 0;
        gCurrentPinballGame->unk3B6[i] = 0;
        gCurrentPinballGame->unk3BC[i] = 0;
        gCurrentPinballGame->unk3C4[i].unk0 = 0;
        gCurrentPinballGame->unk3C4[i].unk2 = 0;
        gCurrentPinballGame->unk3D0[i].unk0 = 0;
        gCurrentPinballGame->unk3D0[i].unk2 = 0;
    }

    gCurrentPinballGame->unk3DC = 0;
    gCurrentPinballGame->unk3DE = 0;
    gCurrentPinballGame->unk3DF = 0;
    gCurrentPinballGame->unk3E0 = 0;
    gCurrentPinballGame->unk3E2 = 0;
    gCurrentPinballGame->unk3E4 = 0;
    gCurrentPinballGame->unk3E6 = 0;
    gCurrentPinballGame->unk3E8 = 0;
    gCurrentPinballGame->unk3EA = 0;
    gCurrentPinballGame->unk3EC = 0;
    gCurrentPinballGame->unk3EE = 0;
    gCurrentPinballGame->unk3F0 = 0;
    gCurrentPinballGame->unk3F2 = 0;
    gCurrentPinballGame->unk1A = 0;

    sub_336E0();
    sub_340EC();

    m4aSongNumStart(MUS_BONUS_FIELD_DUSKULL);

    DmaCopy16(3, (void *)gUnknown_081B36A4, (void *)0x05000320, 32);
}

void sub_33130(void)
{
    s16 temp;
    switch (gCurrentPinballGame->unk13)
    {
        case 0:
            gCurrentPinballGame->unk5F7 = 1;
            if (gCurrentPinballGame->unk18 < 120)
            {
                temp = gCurrentPinballGame->unk18 / 24;
                DmaCopy16(3, gUnknown_082EE0E0 + temp * 40, 0x05000000, 160);
                
                gCurrentPinballGame->unkE6 = gCurrentPinballGame->unk18 / 5 + 0xFFE8;
                gCurrentPinballGame->unk18++;
            }
            else
            {
                gCurrentPinballGame->unkE6 = 0;
                gMain.spriteGroups[7].available = 1;
                gMain.spriteGroups[8].available = 1;
                gMain.spriteGroups[9].available = 1;
                gCurrentPinballGame->unk13 = 1;
                gCurrentPinballGame->unk18 = 0;
            }

            break;
        case 1:
            if (!gCurrentPinballGame->unk386)
            {
                gMain.blendControl = 0x1C10;
                gMain.blendAlpha = 0x30D;
            }
            gCurrentPinballGame->unk387 = 1;
            sub_336E0();
            sub_340EC();
            break;
        case 2:
            gCurrentPinballGame->unk13 = 3;
            gMain.spriteGroups[13].available = 1;
            gMain.spriteGroups[14].available = 1;
            gMain.spriteGroups[12].available = 1;
            gCurrentPinballGame->unk387 = 0;
            gCurrentPinballGame->unk385 = 0;
            m4aSongNumStart(MUS_BONUS_FIELD_DUSCLOPS);
            break;
        case 3:
            if (!gCurrentPinballGame->unk386)
            {
                gMain.blendControl = 0x1C10;
                gMain.blendAlpha = 0x1000;
            }
            sub_34450();
            break;
        case 4:
            if (gCurrentPinballGame->unk18 < 120)
            {
                gCurrentPinballGame->unk18++;
            }
            else
            {
                gCurrentPinballGame->unk13 = 5;
                gCurrentPinballGame->unk18 = 0;
                gMain.spriteGroups[6].available = 1;
                gMain.spriteGroups[5].available = 1;
                DmaCopy16(3, gUnknown_08138834, OBJ_VRAM1+0x1800, 8192);
                gCurrentPinballGame->unk394 = 136;
            }
            break;
        case 5:
            sub_351A8();
            if (gCurrentPinballGame->unk1C)
            {
                gCurrentPinballGame->unk18 = 181;
            }
            if (gCurrentPinballGame->unk18 == 180)
            {
                gCurrentPinballGame->unk1C = 1;
                gCurrentPinballGame->unk38 = 400000;
                gCurrentPinballGame->unk3C = 30000000;
            }
            if (gCurrentPinballGame->unk18 < 240)
            {
                if (gCurrentPinballGame->unk18 == 20)
                {
                    m4aMPlayAllStop();
                    m4aSongNumStart(MUS_SUCCESS3);
                }
                gCurrentPinballGame->unk18++;
            }
            else
            {
                gCurrentPinballGame->unk18 = 0;
                gCurrentPinballGame->unk13 = 6;
                gCurrentPinballGame->bonusFieldsComplete++;
            }

            break;
        case 6:
            sub_351A8();
            gCurrentPinballGame->unk386 = 1;
            break;
    }

    if (gCurrentPinballGame->unk294)
    {
        if (gCurrentPinballGame->eventTimer < 2)
        {
            if (!gMain.unkF)
            {
                m4aMPlayAllStop();
                m4aSongNumStart(MUS_END_OF_BALL3);
                gMain.unkF = gMain.unkF | 0x40;
            }
        }
    }

    if (gCurrentPinballGame->unk386)
        sub_350F0();

    sub_472E4();
}

void sub_3342C(s16 index1, s16 index2)
{
    u8 temp397, temp39A, temp39D, temp3A0, temp3A3, temp3A6, temp3A9, temp3AC;
    u16 temp3B0, temp3B6, temp3BC;
    struct UnkPinballGame3C4 temp3C4, temp3D0;
    
    temp397 = gCurrentPinballGame->unk397[index1];
    temp39A = gCurrentPinballGame->unk39A[index1];
    temp39D = gCurrentPinballGame->unk39D[index1];
    temp3A0 = gCurrentPinballGame->unk3A0[index1];
    temp3A3 = gCurrentPinballGame->unk3A3[index1];
    temp3A6 = gCurrentPinballGame->unk3A6[index1];
    temp3A9 = gCurrentPinballGame->unk3A9[index1];
    temp3AC = gCurrentPinballGame->unk3AC[index1];
    temp3B0 = gCurrentPinballGame->unk3B0[index1];
    temp3B6 = gCurrentPinballGame->unk3B6[index1];
    temp3BC = gCurrentPinballGame->unk3BC[index1];
    temp3C4.unk0 = gCurrentPinballGame->unk3C4[index1].unk0;
    temp3C4.unk2 = gCurrentPinballGame->unk3C4[index1].unk2;
    temp3D0.unk0 = gCurrentPinballGame->unk3D0[index1].unk0;
    temp3D0.unk2 = gCurrentPinballGame->unk3D0[index1].unk2;

    gCurrentPinballGame->unk397[index1] = gCurrentPinballGame->unk397[index2];
    gCurrentPinballGame->unk39A[index1] = gCurrentPinballGame->unk39A[index2];
    gCurrentPinballGame->unk39D[index1] = gCurrentPinballGame->unk39D[index2];
    gCurrentPinballGame->unk3A0[index1] = gCurrentPinballGame->unk3A0[index2];
    gCurrentPinballGame->unk3A3[index1] = gCurrentPinballGame->unk3A3[index2];
    gCurrentPinballGame->unk3A6[index1] = gCurrentPinballGame->unk3A6[index2];
    gCurrentPinballGame->unk3A9[index1] = gCurrentPinballGame->unk3A9[index2];
    gCurrentPinballGame->unk3AC[index1] = gCurrentPinballGame->unk3AC[index2];
    gCurrentPinballGame->unk3B0[index1] = gCurrentPinballGame->unk3B0[index2];
    gCurrentPinballGame->unk3B6[index1] = gCurrentPinballGame->unk3B6[index2];
    gCurrentPinballGame->unk3BC[index1] = gCurrentPinballGame->unk3BC[index2];
    gCurrentPinballGame->unk3C4[index1].unk0 = gCurrentPinballGame->unk3C4[index2].unk0;
    gCurrentPinballGame->unk3C4[index1].unk2 = gCurrentPinballGame->unk3C4[index2].unk2;
    gCurrentPinballGame->unk3D0[index1].unk0 = gCurrentPinballGame->unk3D0[index2].unk0;
    gCurrentPinballGame->unk3D0[index1].unk2 = gCurrentPinballGame->unk3D0[index2].unk2;

    gCurrentPinballGame->unk397[index2] = temp397;
    gCurrentPinballGame->unk39A[index2] = temp39A;
    gCurrentPinballGame->unk39D[index2] = temp39D;
    gCurrentPinballGame->unk3A0[index2] = temp3A0;
    gCurrentPinballGame->unk3A3[index2] = temp3A3;
    gCurrentPinballGame->unk3A6[index2] = temp3A6;
    gCurrentPinballGame->unk3A9[index2] = temp3A9;
    gCurrentPinballGame->unk3AC[index2] = temp3AC;
    gCurrentPinballGame->unk3B0[index2] = temp3B0;
    gCurrentPinballGame->unk3B6[index2] = temp3B6;
    gCurrentPinballGame->unk3BC[index2] = temp3BC;
    gCurrentPinballGame->unk3C4[index2].unk0 = temp3C4.unk0;
    gCurrentPinballGame->unk3C4[index2].unk2 = temp3C4.unk2;
    gCurrentPinballGame->unk3D0[index2].unk0 = temp3D0.unk0;
    gCurrentPinballGame->unk3D0[index2].unk2 = temp3D0.unk2;
}

#ifdef NONMATCHING
void sub_336E0(void)
{
    s16 i;
    s16 j;
    u8 bSpriteGroupsStillInUse = TRUE;
    s16 temp39D = 0;
    s16 temp39A = 0;
    
    if (gCurrentPinballGame->unk385 > 18)
    {
        for (i = 0; i < 3; i++)
        {
            if (gCurrentPinballGame->unk3A6[i] != 0)
            {
                bSpriteGroupsStillInUse = FALSE;
            }
        }
        if (bSpriteGroupsStillInUse != FALSE)
        {
            gCurrentPinballGame->unk13 = 2;
            gMain.spriteGroups[7].available = 0;
            gMain.spriteGroups[8].available = 0;
            gMain.spriteGroups[9].available = 0;
        }
    }
    for (i = 0; i < 3; i++)
    {
        switch (gCurrentPinballGame->unk3A0[i])
        {
            case 0:
                if (gCurrentPinballGame->unk385 <= 18 && gCurrentPinballGame->unk396 <= 1)
                {
                    gCurrentPinballGame->unk396++;
                    gCurrentPinballGame->unk3A0[i] = 1;
                    gCurrentPinballGame->unk3A3[i] = 0;
                    gCurrentPinballGame->unk3B0[i] = 0;
                    gCurrentPinballGame->unk3C4[i].unk0 = Random() % 1400 - 700;
                    gCurrentPinballGame->unk3C4[i].unk2 = (Random() % 9) * 80;
                    gCurrentPinballGame->unk3BC[i] = Random() % 1000 + 200;
                    gCurrentPinballGame->unk3A6[i] = 0;
                    gCurrentPinballGame->unk3A9[i] = 0;
                    temp39D = 4;
                    temp39A = 4;
                    gCurrentPinballGame->unk3B6[i] = 0;
                    MPlayStart(&gMPlayInfo_SE1, &gUnknown_086A1588);

                    for (j = 2; j > 0; j--)
                    {
                        if (gCurrentPinballGame->unk3C4[j].unk2 > gCurrentPinballGame->unk3C4[j - 1].unk2)
                        {
                            sub_3342C(j, j - 1);
                            j = 3;
                        }
                    }
                }
                else
                {
                    gCurrentPinballGame->unk3C4[i].unk0 = 0;
                    gCurrentPinballGame->unk3C4[i].unk2 = 120;
                    gCurrentPinballGame->unk3A6[i] = 0;
                    Random();
                }
                break;
            case 1:
                if (gCurrentPinballGame->unk3B0[i] < 20)
                {
                    gCurrentPinballGame->unk3B0[i]++;
                    gCurrentPinballGame->unk3A6[i] = gCurrentPinballGame->unk3B0[i] % 4 / 2;
                    temp39D = 4;
                    temp39A = 4;
                }
                else
                {
                    if ((gMain.systemFrameCount % 2) != 0)
                    {
                        gCurrentPinballGame->unk3A0[i] = 2;
                        gCurrentPinballGame->unk3A3[i] = 0;
                    }
                    else
                    {
                        gCurrentPinballGame->unk3A0[i] = 3;
                        gCurrentPinballGame->unk3A3[i] = 7;
                    }
                    gCurrentPinballGame->unk3B0[i] = 0;
                    gCurrentPinballGame->unk3A6[i] = 1;
                    gCurrentPinballGame->unk3A9[i] = 1;

                    temp39D = gUnknown_086AE5EC[gCurrentPinballGame->unk3A3[i]][0];
                    temp39A = gUnknown_086AE5EC[gCurrentPinballGame->unk3A3[i]][2];
                }
                break;
            case 2:
                gCurrentPinballGame->unk3B6[i]++;
                if (gCurrentPinballGame->unk3C4[i].unk0 > -800)
                {
                    gCurrentPinballGame->unk3C4[i].unk0 -= 3;

                    if (gUnknown_086AE5EC[gCurrentPinballGame->unk3A3[i]][1] <= gCurrentPinballGame->unk3B0[i])
                    {
                        gCurrentPinballGame->unk3B0[i] = 0;
                        gCurrentPinballGame->unk3A3[i]++;
                        if (gCurrentPinballGame->unk3A3[i] > 3)
                        {
                            gCurrentPinballGame->unk3A3[i] = 0;
                        }
                    }
                    else
                    {
                        gCurrentPinballGame->unk3B0[i]++;
                    }
                }
                else
                {
                    if (gUnknown_086AE5EC[gCurrentPinballGame->unk3A3[i]][1] > gCurrentPinballGame->unk3B0[i])
                    {
                        gCurrentPinballGame->unk3B0[i]++;
                    }
                    else
                    {
                        gCurrentPinballGame->unk3B0[i] = 0;
                        gCurrentPinballGame->unk3A3[i]++;
                        if (gCurrentPinballGame->unk3A3[i] > 6)
                        {
                            gCurrentPinballGame->unk3A3[i] = 7;
                            gCurrentPinballGame->unk3A0[i] = 3;
                        }
                    }
                }
                temp39D = gUnknown_086AE5EC[gCurrentPinballGame->unk3A3[i]][0];
                temp39A = gUnknown_086AE5EC[gCurrentPinballGame->unk3A3[i]][2];
                break;
            case 3:
                gCurrentPinballGame->unk3B6[i]++;
                if (gCurrentPinballGame->unk3C4[i].unk0 < 800)
                {
                    gCurrentPinballGame->unk3C4[i].unk0 += 3;

                    if (gUnknown_086AE5EC[gCurrentPinballGame->unk3A3[i]][1] <= gCurrentPinballGame->unk3B0[i])
                    {
                        gCurrentPinballGame->unk3B0[i] = 0;
                        gCurrentPinballGame->unk3A3[i]++;
                        if (gCurrentPinballGame->unk3A3[i] > 10)
                        {
                            gCurrentPinballGame->unk3A3[i] = 7;
                        }
                    }
                    else
                    {
                        gCurrentPinballGame->unk3B0[i]++;
                    }
                }
                else if (gUnknown_086AE5EC[gCurrentPinballGame->unk3A3[i]][1] > gCurrentPinballGame->unk3B0[i])
                {
                    gCurrentPinballGame->unk3B0[i]++;
                }
                else
                {
                    gCurrentPinballGame->unk3B0[i] = 0;
                    gCurrentPinballGame->unk3A3[i]++;
                    if (gCurrentPinballGame->unk3A3[i] > 13)
                    {
                        gCurrentPinballGame->unk3A3[i] = 0;
                        gCurrentPinballGame->unk3A0[i] = 2;
                    }
                }
                
                temp39D = gUnknown_086AE5EC[gCurrentPinballGame->unk3A3[i]][0];
                temp39A = gUnknown_086AE5EC[gCurrentPinballGame->unk3A3[i]][2];
                break;
            case 4:
                temp39D = gUnknown_086AE5EC[gCurrentPinballGame->unk3A3[i]][0];
                temp39A = gUnknown_086AE5EC[gCurrentPinballGame->unk3A3[i]][2];
                gCurrentPinballGame->unk3A3[i] = 14;
                gCurrentPinballGame->unk3B0[i] = 0;
                gCurrentPinballGame->unk3A0[i] = 5;
                gCurrentPinballGame->unk3AC[i] = 0;
                gCurrentPinballGame->unk3A9[i] = 0;
                if (gCurrentPinballGame->unk396 > 0)
                {
                    gCurrentPinballGame->unk396--;
                }
                gCurrentPinballGame->unk3C = 100000;
                gCurrentPinballGame->unk385++;
                m4aSongNumStart(SE_UNKNOWN_0xF2);
                sub_11B0(7);
                break;
            case 5:
                if (gUnknown_086AE5EC[gCurrentPinballGame->unk3A3[i]][1] > gCurrentPinballGame->unk3B0[i])
                {
                    gCurrentPinballGame->unk3B0[i]++;
                }
                else
                {
                    gCurrentPinballGame->unk3B0[i] = 0;
                    gCurrentPinballGame->unk3A3[i]++;
                    if (gCurrentPinballGame->unk3A3[i] > 15)
                    {
                        gCurrentPinballGame->unk3A3[i] = 15;
                        gCurrentPinballGame->unk3A0[i] = 9;
                        gCurrentPinballGame->unk3A6[i] = 0;
                    }
                }
                
                if (gCurrentPinballGame->unk3A3[i] == 15)
                {
                    gCurrentPinballGame->unk3AC[i]++;
                }
                temp39D = gUnknown_086AE5EC[gCurrentPinballGame->unk3A3[i]][0];
                temp39A = gUnknown_086AE5EC[gCurrentPinballGame->unk3A3[i]][2];
                break;
            case 6:
                if (gCurrentPinballGame->unk3B0[i] <= 5)
                {
                    gCurrentPinballGame->unk3B0[i]++;
                    temp39D = 3;
                    temp39A = 3;
                }
                else
                {
                    gCurrentPinballGame->unk3B0[i] = 0;
                    gCurrentPinballGame->unk3A3[i] = 18;
                    gCurrentPinballGame->unk3A0[i] = 8;
                    gCurrentPinballGame->unk3A9[i] = 0;
                    if (gCurrentPinballGame->unk396 > 0)
                    {
                        gCurrentPinballGame->unk396--;
                    }
                    temp39D = 3;
                    temp39A = 3;
                    m4aSongNumStart(SE_UNKNOWN_0xF3);
                }
                break;
            case 7:
                if (gCurrentPinballGame->unk3B0[i] <= 5)
                {
                    gCurrentPinballGame->unk3B0[i]++;
                    temp39D = 9;
                    temp39A = 3;
                }
                else
                {
                    gCurrentPinballGame->unk3B0[i] = 0;
                    gCurrentPinballGame->unk3A3[i] = 18;
                    gCurrentPinballGame->unk3A0[i] = 8;
                    gCurrentPinballGame->unk3A9[i] = 0;
                    if (gCurrentPinballGame->unk396 > 0)
                    {
                        gCurrentPinballGame->unk396--;
                    }
                    m4aSongNumStart(SE_UNKNOWN_0xF3);
                    temp39D = 9;
                    temp39A = 3;
                }
                break;
            case 8:
                if (gUnknown_086AE5EC[gCurrentPinballGame->unk3A3[i]][1] > gCurrentPinballGame->unk3B0[i])
                {
                    gCurrentPinballGame->unk3B0[i]++;
                }
                else
                {
                    gCurrentPinballGame->unk3B0[i] = 0;
                    gCurrentPinballGame->unk3A3[i]++;
                    if (gCurrentPinballGame->unk3A3[i] > 25) 
                    {
                        gCurrentPinballGame->unk3A3[i] = 25;
                        gCurrentPinballGame->unk3A0[i] = 9;
                    }
                }
                temp39D = gUnknown_086AE5EC[gCurrentPinballGame->unk3A3[i]][0];
                temp39A = gUnknown_086AE5EC[gCurrentPinballGame->unk3A3[i]][2];
                break;
            case 9:
                gCurrentPinballGame->unk3A6[i] = 0;
                gCurrentPinballGame->unk3A9[i] = 0;
                gCurrentPinballGame->unk3A0[i] = 0;

                temp39D = gUnknown_086AE5EC[gCurrentPinballGame->unk3A3[i]][0];
                temp39A = gUnknown_086AE5EC[gCurrentPinballGame->unk3A3[i]][2];
                break;
        }
        if (gCurrentPinballGame->unk3B6[i] > gCurrentPinballGame->unk3BC[i])
        {
            if (gCurrentPinballGame->unk385 <= 18)
            {
                gCurrentPinballGame->unk3B6[i] = 0;
                if (gCurrentPinballGame->unk3A0[i] == 2)
                {
                    gCurrentPinballGame->unk3A0[i] = 6;
                }
                else
                {
                    gCurrentPinballGame->unk3A0[i] = 7;
                }
                gCurrentPinballGame->unk3B0[i] = 0;
            }
        }
        gCurrentPinballGame->unk397[i] = gCurrentPinballGame->unk39A[i];
        gCurrentPinballGame->unk39A[i] = temp39A;
        gCurrentPinballGame->unk39D[i] = temp39D;
        gCurrentPinballGame->unk3D0[i].unk0 = gCurrentPinballGame->unk3C4[i].unk0 / 10 * 2 + 208;
        gCurrentPinballGame->unk3D0[i].unk2 = gCurrentPinballGame->unk3C4[i].unk2 / 10 * 2 + 66;
    }
}
#else
NAKED
void sub_336E0(void)
{
    asm_unified("\n\
	push {r4, r5, r6, r7, lr}\n\
	mov r7, sl\n\
	mov r6, sb\n\
	mov r5, r8\n\
	push {r5, r6, r7}\n\
	sub sp, #8\n\
	movs r4, #1\n\
	movs r0, #0\n\
	str r0, [sp]\n\
	movs r7, #0\n\
	ldr r0, _0803377C @ =gCurrentPinballGame\n\
	ldr r3, [r0]\n\
	ldr r2, _08033780 @ =0x00000385\n\
	adds r1, r3, r2\n\
	ldrb r1, [r1]\n\
	lsls r1, r1, #0x18\n\
	asrs r1, r1, #0x18\n\
	cmp r1, #0x12\n\
	ble _08033750\n\
	movs r2, #0\n\
	ldr r5, _08033784 @ =0x000003A6\n\
	adds r1, r3, r5\n\
_0803370C:\n\
	lsls r0, r2, #0x10\n\
	asrs r2, r0, #0x10\n\
	adds r0, r1, r2\n\
	ldrb r0, [r0]\n\
	lsls r0, r0, #0x18\n\
	asrs r0, r0, #0x18\n\
	cmp r0, #0\n\
	beq _0803371E\n\
	movs r4, #0\n\
_0803371E:\n\
	adds r0, r2, #1\n\
	lsls r0, r0, #0x10\n\
	lsrs r2, r0, #0x10\n\
	asrs r0, r0, #0x10\n\
	cmp r0, #2\n\
	ble _0803370C\n\
	cmp r4, #0\n\
	beq _08033750\n\
	ldr r6, _0803377C @ =gCurrentPinballGame\n\
	ldr r1, [r6]\n\
	movs r2, #0\n\
	movs r0, #2\n\
	strb r0, [r1, #0x13]\n\
	ldr r0, _08033788 @ =gMain\n\
	movs r3, #0x80\n\
	lsls r3, r3, #4\n\
	adds r1, r0, r3\n\
	strh r2, [r1]\n\
	ldr r4, _0803378C @ =0x000008B8\n\
	adds r1, r0, r4\n\
	strh r2, [r1]\n\
	movs r5, #0x97\n\
	lsls r5, r5, #4\n\
	adds r0, r0, r5\n\
	strh r2, [r0]\n\
_08033750:\n\
	movs r2, #0\n\
_08033752:\n\
	ldr r6, _0803377C @ =gCurrentPinballGame\n\
	ldr r0, [r6]\n\
	lsls r2, r2, #0x10\n\
	asrs r1, r2, #0x10\n\
	movs r3, #0xe8\n\
	lsls r3, r3, #2\n\
	adds r0, r0, r3\n\
	adds r0, r0, r1\n\
	ldrb r0, [r0]\n\
	lsls r0, r0, #0x18\n\
	asrs r0, r0, #0x18\n\
	str r2, [sp, #4]\n\
	cmp r0, #9\n\
	bls _08033772\n\
	bl _08033FD8\n\
_08033772:\n\
	lsls r0, r0, #2\n\
	ldr r1, _08033790 @ =_08033794\n\
	adds r0, r0, r1\n\
	ldr r0, [r0]\n\
	mov pc, r0\n\
	.align 2, 0\n\
_0803377C: .4byte gCurrentPinballGame\n\
_08033780: .4byte 0x00000385\n\
_08033784: .4byte 0x000003A6\n\
_08033788: .4byte gMain\n\
_0803378C: .4byte 0x000008B8\n\
_08033790: .4byte _08033794\n\
_08033794: @ jump table\n\
	.4byte _080337BC @ case 0\n\
	.4byte _0803394C @ case 1\n\
	.4byte _08033A20 @ case 2\n\
	.4byte _08033B4C @ case 3\n\
	.4byte _08033C7C @ case 4\n\
	.4byte _08033D28 @ case 5\n\
	.4byte _08033DF4 @ case 6\n\
	.4byte _08033E70 @ case 7\n\
	.4byte _08033EF0 @ case 8\n\
	.4byte _08033F90 @ case 9\n\
_080337BC:\n\
	ldr r4, _080338E0 @ =gCurrentPinballGame\n\
	ldr r1, [r4]\n\
	ldr r5, _080338E4 @ =0x00000385\n\
	adds r0, r1, r5\n\
	ldrb r0, [r0]\n\
	lsls r0, r0, #0x18\n\
	asrs r0, r0, #0x18\n\
	cmp r0, #0x12\n\
	ble _080337D0\n\
	b _08033910\n\
_080337D0:\n\
	ldr r6, _080338E8 @ =0x00000396\n\
	adds r1, r1, r6\n\
	ldrb r2, [r1]\n\
	movs r0, #0\n\
	ldrsb r0, [r1, r0]\n\
	cmp r0, #1\n\
	ble _080337E0\n\
	b _08033910\n\
_080337E0:\n\
	adds r0, r2, #1\n\
	movs r2, #0\n\
	mov sl, r2\n\
	strb r0, [r1]\n\
	ldr r1, [r4]\n\
	ldr r3, [sp, #4]\n\
	asrs r6, r3, #0x10\n\
	movs r4, #0xe8\n\
	lsls r4, r4, #2\n\
	adds r1, r1, r4\n\
	adds r1, r1, r6\n\
	movs r0, #1\n\
	strb r0, [r1]\n\
	ldr r5, _080338E0 @ =gCurrentPinballGame\n\
	ldr r0, [r5]\n\
	ldr r1, _080338EC @ =0x000003A3\n\
	adds r0, r0, r1\n\
	adds r0, r0, r6\n\
	mov r2, sl\n\
	strb r2, [r0]\n\
	ldr r0, [r5]\n\
	lsls r3, r6, #1\n\
	mov r8, r3\n\
	adds r4, #0x10\n\
	adds r0, r0, r4\n\
	add r0, r8\n\
	movs r7, #0\n\
	mov r5, sl\n\
	strh r5, [r0]\n\
	bl Random\n\
	ldr r1, _080338E0 @ =gCurrentPinballGame\n\
	ldr r4, [r1]\n\
	lsls r5, r6, #2\n\
	adds r4, r4, r5\n\
	movs r1, #0xaf\n\
	lsls r1, r1, #3\n\
	bl __umodsi3\n\
	ldr r2, _080338F0 @ =0xFFFFFD44\n\
	adds r0, r0, r2\n\
	movs r3, #0xf1\n\
	lsls r3, r3, #2\n\
	adds r4, r4, r3\n\
	strh r0, [r4]\n\
	bl Random\n\
	ldr r1, _080338E0 @ =gCurrentPinballGame\n\
	ldr r4, [r1]\n\
	adds r4, r4, r5\n\
	movs r1, #9\n\
	bl __umodsi3\n\
	lsls r1, r0, #2\n\
	adds r1, r1, r0\n\
	lsls r1, r1, #4\n\
	ldr r2, _080338F4 @ =0x000003C6\n\
	mov sb, r2\n\
	add r4, sb\n\
	strh r1, [r4]\n\
	bl Random\n\
	ldr r3, _080338E0 @ =gCurrentPinballGame\n\
	ldr r4, [r3]\n\
	movs r1, #0xef\n\
	lsls r1, r1, #2\n\
	adds r5, r4, r1\n\
	add r5, r8\n\
	adds r1, #0x2c\n\
	bl __umodsi3\n\
	adds r0, #0xc8\n\
	strh r0, [r5]\n\
	ldr r2, _080338F8 @ =0x000003A6\n\
	adds r4, r4, r2\n\
	adds r4, r4, r6\n\
	strb r7, [r4]\n\
	ldr r3, _080338E0 @ =gCurrentPinballGame\n\
	ldr r0, [r3]\n\
	ldr r4, _080338FC @ =0x000003A9\n\
	adds r0, r0, r4\n\
	adds r0, r0, r6\n\
	strb r7, [r0]\n\
	movs r5, #4\n\
	str r5, [sp]\n\
	movs r7, #4\n\
	ldr r0, [r3]\n\
	ldr r6, _08033900 @ =0x000003B6\n\
	adds r0, r0, r6\n\
	add r0, r8\n\
	mov r1, sl\n\
	strh r1, [r0]\n\
	ldr r0, _08033904 @ =gMPlayInfo_SE1\n\
	ldr r1, _08033908 @ =gUnknown_086A1588\n\
	bl MPlayStart\n\
	movs r5, #2\n\
	mov r8, sb\n\
_080338A4:\n\
	ldr r0, _080338E0 @ =gCurrentPinballGame\n\
	ldr r2, [r0]\n\
	lsls r0, r5, #0x10\n\
	asrs r4, r0, #0x10\n\
	lsls r1, r4, #2\n\
	adds r1, r2, r1\n\
	add r1, r8\n\
	subs r3, r4, #1\n\
	lsls r0, r3, #2\n\
	adds r2, r2, r0\n\
	add r2, r8\n\
	movs r6, #0\n\
	ldrsh r1, [r1, r6]\n\
	movs r6, #0\n\
	ldrsh r0, [r2, r6]\n\
	cmp r1, r0\n\
	ble _080338D2\n\
	lsls r1, r3, #0x10\n\
	asrs r1, r1, #0x10\n\
	adds r0, r4, #0\n\
	bl sub_3342C\n\
	movs r5, #3\n\
_080338D2:\n\
	lsls r0, r5, #0x10\n\
	ldr r1, _0803390C @ =0xFFFF0000\n\
	adds r0, r0, r1\n\
	lsrs r5, r0, #0x10\n\
	cmp r0, #0\n\
	bgt _080338A4\n\
	b _08033FD8\n\
	.align 2, 0\n\
_080338E0: .4byte gCurrentPinballGame\n\
_080338E4: .4byte 0x00000385\n\
_080338E8: .4byte 0x00000396\n\
_080338EC: .4byte 0x000003A3\n\
_080338F0: .4byte 0xFFFFFD44\n\
_080338F4: .4byte 0x000003C6\n\
_080338F8: .4byte 0x000003A6\n\
_080338FC: .4byte 0x000003A9\n\
_08033900: .4byte 0x000003B6\n\
_08033904: .4byte gMPlayInfo_SE1\n\
_08033908: .4byte gUnknown_086A1588\n\
_0803390C: .4byte 0xFFFF0000\n\
_08033910:\n\
	ldr r0, _08033940 @ =gCurrentPinballGame\n\
	ldr r2, [r0]\n\
	ldr r3, [sp, #4]\n\
	asrs r4, r3, #0x10\n\
	lsls r1, r4, #2\n\
	adds r1, r2, r1\n\
	movs r5, #0xf1\n\
	lsls r5, r5, #2\n\
	adds r3, r1, r5\n\
	movs r5, #0\n\
	movs r0, #0\n\
	strh r0, [r3]\n\
	ldr r6, _08033944 @ =0x000003C6\n\
	adds r1, r1, r6\n\
	movs r0, #0x78\n\
	strh r0, [r1]\n\
	ldr r0, _08033948 @ =0x000003A6\n\
	adds r2, r2, r0\n\
	adds r2, r2, r4\n\
	strb r5, [r2]\n\
	bl Random\n\
	b _08033FD8\n\
	.align 2, 0\n\
_08033940: .4byte gCurrentPinballGame\n\
_08033944: .4byte 0x000003C6\n\
_08033948: .4byte 0x000003A6\n\
_0803394C:\n\
	ldr r1, _08033980 @ =gCurrentPinballGame\n\
	ldr r2, [r1]\n\
	ldr r3, [sp, #4]\n\
	asrs r4, r3, #0x10\n\
	lsls r1, r4, #1\n\
	movs r5, #0xec\n\
	lsls r5, r5, #2\n\
	adds r0, r2, r5\n\
	adds r1, r0, r1\n\
	ldrh r0, [r1]\n\
	cmp r0, #0x13\n\
	bhi _08033988\n\
	adds r0, #1\n\
	strh r0, [r1]\n\
	ldr r6, _08033984 @ =0x000003A6\n\
	adds r2, r2, r6\n\
	adds r2, r2, r4\n\
	ldrh r0, [r1]\n\
	movs r1, #3\n\
	ands r0, r1\n\
	lsrs r0, r0, #1\n\
	strb r0, [r2]\n\
	movs r0, #4\n\
	str r0, [sp]\n\
	movs r7, #4\n\
	b _08033FD8\n\
	.align 2, 0\n\
_08033980: .4byte gCurrentPinballGame\n\
_08033984: .4byte 0x000003A6\n\
_08033988:\n\
	ldr r0, _080339B0 @ =gMain\n\
	ldr r0, [r0, #0x4c]\n\
	movs r1, #1\n\
	ands r0, r1\n\
	cmp r0, #0\n\
	beq _080339BC\n\
	movs r3, #0xe8\n\
	lsls r3, r3, #2\n\
	adds r1, r2, r3\n\
	adds r1, r1, r4\n\
	movs r2, #0\n\
	movs r0, #2\n\
	strb r0, [r1]\n\
	ldr r5, _080339B4 @ =gCurrentPinballGame\n\
	ldr r0, [r5]\n\
	ldr r6, _080339B8 @ =0x000003A3\n\
	adds r0, r0, r6\n\
	adds r0, r0, r4\n\
	strb r2, [r0]\n\
	b _080339D6\n\
	.align 2, 0\n\
_080339B0: .4byte gMain\n\
_080339B4: .4byte gCurrentPinballGame\n\
_080339B8: .4byte 0x000003A3\n\
_080339BC:\n\
	movs r1, #0xe8\n\
	lsls r1, r1, #2\n\
	adds r0, r2, r1\n\
	adds r0, r0, r4\n\
	movs r1, #3\n\
	strb r1, [r0]\n\
	ldr r2, _08033A0C @ =gCurrentPinballGame\n\
	ldr r0, [r2]\n\
	ldr r3, _08033A10 @ =0x000003A3\n\
	adds r0, r0, r3\n\
	adds r0, r0, r4\n\
	movs r1, #7\n\
	strb r1, [r0]\n\
_080339D6:\n\
	ldr r4, _08033A0C @ =gCurrentPinballGame\n\
	ldr r2, [r4]\n\
	ldr r5, [sp, #4]\n\
	asrs r3, r5, #0x10\n\
	lsls r1, r3, #1\n\
	movs r6, #0xec\n\
	lsls r6, r6, #2\n\
	adds r0, r2, r6\n\
	adds r0, r0, r1\n\
	movs r1, #0\n\
	strh r1, [r0]\n\
	ldr r0, _08033A14 @ =0x000003A6\n\
	adds r2, r2, r0\n\
	adds r2, r2, r3\n\
	movs r1, #1\n\
	strb r1, [r2]\n\
	ldr r0, [r4]\n\
	ldr r2, _08033A18 @ =0x000003A9\n\
	adds r0, r0, r2\n\
	adds r0, r0, r3\n\
	strb r1, [r0]\n\
	ldr r2, _08033A1C @ =gUnknown_086AE5EC\n\
	ldr r0, [r4]\n\
	ldr r4, _08033A10 @ =0x000003A3\n\
	adds r0, r0, r4\n\
	adds r0, r0, r3\n\
	b _08033FC2\n\
	.align 2, 0\n\
_08033A0C: .4byte gCurrentPinballGame\n\
_08033A10: .4byte 0x000003A3\n\
_08033A14: .4byte 0x000003A6\n\
_08033A18: .4byte 0x000003A9\n\
_08033A1C: .4byte gUnknown_086AE5EC\n\
_08033A20:\n\
	ldr r6, _08033AA4 @ =gCurrentPinballGame\n\
	ldr r5, [r6]\n\
	ldr r0, [sp, #4]\n\
	asrs r4, r0, #0x10\n\
	lsls r7, r4, #1\n\
	ldr r2, _08033AA8 @ =0x000003B6\n\
	adds r1, r5, r2\n\
	adds r1, r1, r7\n\
	ldrh r0, [r1]\n\
	adds r0, #1\n\
	movs r3, #0\n\
	mov sb, r3\n\
	movs r6, #0\n\
	mov r8, r6\n\
	strh r0, [r1]\n\
	lsls r0, r4, #2\n\
	adds r0, r5, r0\n\
	movs r1, #0xf1\n\
	lsls r1, r1, #2\n\
	adds r2, r0, r1\n\
	ldrh r3, [r2]\n\
	movs r6, #0\n\
	ldrsh r1, [r2, r6]\n\
	ldr r0, _08033AAC @ =0xFFFFFCE0\n\
	cmp r1, r0\n\
	ble _08033AB8\n\
	subs r0, r3, #3\n\
	strh r0, [r2]\n\
	ldr r2, _08033AB0 @ =gUnknown_086AE5EC\n\
	ldr r1, _08033AB4 @ =0x000003A3\n\
	adds r0, r5, r1\n\
	adds r6, r0, r4\n\
	movs r1, #0\n\
	ldrsb r1, [r6, r1]\n\
	lsls r0, r1, #1\n\
	adds r0, r0, r1\n\
	lsls r0, r0, #1\n\
	adds r1, r2, #2\n\
	adds r0, r0, r1\n\
	movs r3, #0\n\
	ldrsh r1, [r0, r3]\n\
	movs r3, #0xec\n\
	lsls r3, r3, #2\n\
	adds r0, r5, r3\n\
	adds r5, r0, r7\n\
	ldrh r0, [r5]\n\
	adds r3, r2, #0\n\
	cmp r1, r0\n\
	bgt _08033AE2\n\
	mov r0, r8\n\
	strh r0, [r5]\n\
	ldrb r0, [r6]\n\
	adds r0, #1\n\
	strb r0, [r6]\n\
	ldr r1, _08033AA4 @ =gCurrentPinballGame\n\
	ldr r0, [r1]\n\
	ldr r2, _08033AB4 @ =0x000003A3\n\
	adds r0, r0, r2\n\
	adds r1, r0, r4\n\
	movs r0, #0\n\
	ldrsb r0, [r1, r0]\n\
	cmp r0, #3\n\
	ble _08033B20\n\
	mov r4, sb\n\
	strb r4, [r1]\n\
	b _08033B20\n\
	.align 2, 0\n\
_08033AA4: .4byte gCurrentPinballGame\n\
_08033AA8: .4byte 0x000003B6\n\
_08033AAC: .4byte 0xFFFFFCE0\n\
_08033AB0: .4byte gUnknown_086AE5EC\n\
_08033AB4: .4byte 0x000003A3\n\
_08033AB8:\n\
	ldr r2, _08033AE8 @ =gUnknown_086AE5EC\n\
	ldr r6, _08033AEC @ =0x000003A3\n\
	adds r0, r5, r6\n\
	adds r6, r0, r4\n\
	movs r1, #0\n\
	ldrsb r1, [r6, r1]\n\
	lsls r0, r1, #1\n\
	adds r0, r0, r1\n\
	lsls r0, r0, #1\n\
	adds r1, r2, #2\n\
	adds r0, r0, r1\n\
	movs r3, #0\n\
	ldrsh r1, [r0, r3]\n\
	movs r3, #0xec\n\
	lsls r3, r3, #2\n\
	adds r0, r5, r3\n\
	adds r5, r0, r7\n\
	ldrh r0, [r5]\n\
	adds r3, r2, #0\n\
	cmp r1, r0\n\
	ble _08033AF0\n\
_08033AE2:\n\
	adds r0, #1\n\
	strh r0, [r5]\n\
	b _08033B20\n\
	.align 2, 0\n\
_08033AE8: .4byte gUnknown_086AE5EC\n\
_08033AEC: .4byte 0x000003A3\n\
_08033AF0:\n\
	mov r0, r8\n\
	strh r0, [r5]\n\
	ldrb r0, [r6]\n\
	adds r0, #1\n\
	strb r0, [r6]\n\
	ldr r1, _08033B44 @ =gCurrentPinballGame\n\
	ldr r0, [r1]\n\
	ldr r2, _08033B48 @ =0x000003A3\n\
	adds r0, r0, r2\n\
	adds r1, r0, r4\n\
	movs r0, #0\n\
	ldrsb r0, [r1, r0]\n\
	cmp r0, #6\n\
	ble _08033B20\n\
	movs r0, #7\n\
	strb r0, [r1]\n\
	ldr r5, _08033B44 @ =gCurrentPinballGame\n\
	ldr r0, [r5]\n\
	movs r6, #0xe8\n\
	lsls r6, r6, #2\n\
	adds r0, r0, r6\n\
	adds r0, r0, r4\n\
	movs r1, #3\n\
	strb r1, [r0]\n\
_08033B20:\n\
	ldr r0, _08033B44 @ =gCurrentPinballGame\n\
	ldr r0, [r0]\n\
	ldr r2, [sp, #4]\n\
	asrs r1, r2, #0x10\n\
	ldr r4, _08033B48 @ =0x000003A3\n\
	adds r0, r0, r4\n\
	adds r0, r0, r1\n\
	movs r1, #0\n\
	ldrsb r1, [r0, r1]\n\
	lsls r0, r1, #1\n\
	adds r0, r0, r1\n\
	lsls r0, r0, #1\n\
	adds r1, r0, r3\n\
	ldrh r1, [r1]\n\
	str r1, [sp]\n\
	adds r1, r3, #4\n\
	adds r0, r0, r1\n\
	b _08033FD6\n\
	.align 2, 0\n\
_08033B44: .4byte gCurrentPinballGame\n\
_08033B48: .4byte 0x000003A3\n\
_08033B4C:\n\
	ldr r5, _08033BD4 @ =gCurrentPinballGame\n\
	ldr r4, [r5]\n\
	ldr r6, [sp, #4]\n\
	asrs r7, r6, #0x10\n\
	lsls r0, r7, #1\n\
	mov r8, r0\n\
	ldr r2, _08033BD8 @ =0x000003B6\n\
	adds r1, r4, r2\n\
	add r1, r8\n\
	ldrh r0, [r1]\n\
	adds r0, #1\n\
	movs r3, #0\n\
	mov sl, r3\n\
	movs r5, #0\n\
	mov sb, r5\n\
	strh r0, [r1]\n\
	lsls r0, r7, #2\n\
	adds r5, r4, r0\n\
	movs r6, #0xf1\n\
	lsls r6, r6, #2\n\
	adds r2, r5, r6\n\
	ldrh r3, [r2]\n\
	movs r0, #0\n\
	ldrsh r1, [r2, r0]\n\
	ldr r0, _08033BDC @ =0x0000031F\n\
	cmp r1, r0\n\
	bgt _08033BE8\n\
	adds r0, r3, #3\n\
	strh r0, [r2]\n\
	ldr r2, _08033BE0 @ =gUnknown_086AE5EC\n\
	ldr r3, _08033BE4 @ =0x000003A3\n\
	adds r0, r4, r3\n\
	adds r6, r0, r7\n\
	movs r1, #0\n\
	ldrsb r1, [r6, r1]\n\
	lsls r0, r1, #1\n\
	adds r0, r0, r1\n\
	lsls r0, r0, #1\n\
	adds r1, r2, #2\n\
	adds r0, r0, r1\n\
	movs r5, #0\n\
	ldrsh r1, [r0, r5]\n\
	adds r3, #0xd\n\
	adds r0, r4, r3\n\
	mov r4, r8\n\
	adds r5, r0, r4\n\
	ldrh r0, [r5]\n\
	adds r3, r2, #0\n\
	cmp r1, r0\n\
	bgt _08033C12\n\
	mov r0, sb\n\
	strh r0, [r5]\n\
	ldrb r0, [r6]\n\
	adds r0, #1\n\
	strb r0, [r6]\n\
	ldr r1, _08033BD4 @ =gCurrentPinballGame\n\
	ldr r0, [r1]\n\
	ldr r2, _08033BE4 @ =0x000003A3\n\
	adds r0, r0, r2\n\
	adds r1, r0, r7\n\
	movs r0, #0\n\
	ldrsb r0, [r1, r0]\n\
	cmp r0, #0xa\n\
	ble _08033C50\n\
	movs r0, #7\n\
	strb r0, [r1]\n\
	b _08033C50\n\
	.align 2, 0\n\
_08033BD4: .4byte gCurrentPinballGame\n\
_08033BD8: .4byte 0x000003B6\n\
_08033BDC: .4byte 0x0000031F\n\
_08033BE0: .4byte gUnknown_086AE5EC\n\
_08033BE4: .4byte 0x000003A3\n\
_08033BE8:\n\
	ldr r2, _08033C18 @ =gUnknown_086AE5EC\n\
	ldr r3, _08033C1C @ =0x000003A3\n\
	adds r0, r4, r3\n\
	adds r6, r0, r7\n\
	movs r1, #0\n\
	ldrsb r1, [r6, r1]\n\
	lsls r0, r1, #1\n\
	adds r0, r0, r1\n\
	lsls r0, r0, #1\n\
	adds r1, r2, #2\n\
	adds r0, r0, r1\n\
	movs r5, #0\n\
	ldrsh r1, [r0, r5]\n\
	adds r3, #0xd\n\
	adds r0, r4, r3\n\
	mov r4, r8\n\
	adds r5, r0, r4\n\
	ldrh r0, [r5]\n\
	adds r3, r2, #0\n\
	cmp r1, r0\n\
	ble _08033C20\n\
_08033C12:\n\
	adds r0, #1\n\
	strh r0, [r5]\n\
	b _08033C50\n\
	.align 2, 0\n\
_08033C18: .4byte gUnknown_086AE5EC\n\
_08033C1C: .4byte 0x000003A3\n\
_08033C20:\n\
	mov r0, sb\n\
	strh r0, [r5]\n\
	ldrb r0, [r6]\n\
	adds r0, #1\n\
	strb r0, [r6]\n\
	ldr r1, _08033C74 @ =gCurrentPinballGame\n\
	ldr r0, [r1]\n\
	ldr r2, _08033C78 @ =0x000003A3\n\
	adds r0, r0, r2\n\
	adds r1, r0, r7\n\
	movs r0, #0\n\
	ldrsb r0, [r1, r0]\n\
	cmp r0, #0xd\n\
	ble _08033C50\n\
	mov r4, sl\n\
	strb r4, [r1]\n\
	ldr r5, _08033C74 @ =gCurrentPinballGame\n\
	ldr r0, [r5]\n\
	movs r6, #0xe8\n\
	lsls r6, r6, #2\n\
	adds r0, r0, r6\n\
	adds r0, r0, r7\n\
	movs r1, #2\n\
	strb r1, [r0]\n\
_08033C50:\n\
	ldr r0, _08033C74 @ =gCurrentPinballGame\n\
	ldr r0, [r0]\n\
	ldr r2, [sp, #4]\n\
	asrs r1, r2, #0x10\n\
	ldr r4, _08033C78 @ =0x000003A3\n\
	adds r0, r0, r4\n\
	adds r0, r0, r1\n\
	movs r1, #0\n\
	ldrsb r1, [r0, r1]\n\
	lsls r0, r1, #1\n\
	adds r0, r0, r1\n\
	lsls r0, r0, #1\n\
	adds r1, r0, r3\n\
	ldrh r1, [r1]\n\
	str r1, [sp]\n\
	adds r1, r3, #4\n\
	adds r0, r0, r1\n\
	b _08033FD6\n\
	.align 2, 0\n\
_08033C74: .4byte gCurrentPinballGame\n\
_08033C78: .4byte 0x000003A3\n\
_08033C7C:\n\
	ldr r3, _08033D0C @ =gUnknown_086AE5EC\n\
	ldr r6, _08033D10 @ =gCurrentPinballGame\n\
	ldr r2, [r6]\n\
	ldr r5, [sp, #4]\n\
	asrs r4, r5, #0x10\n\
	ldr r0, _08033D14 @ =0x000003A3\n\
	adds r2, r2, r0\n\
	adds r2, r2, r4\n\
	movs r1, #0\n\
	ldrsb r1, [r2, r1]\n\
	lsls r0, r1, #1\n\
	adds r0, r0, r1\n\
	lsls r0, r0, #1\n\
	adds r1, r0, r3\n\
	ldrh r1, [r1]\n\
	str r1, [sp]\n\
	adds r3, #4\n\
	adds r0, r0, r3\n\
	ldrh r7, [r0]\n\
	movs r3, #0\n\
	movs r0, #0xe\n\
	strb r0, [r2]\n\
	ldr r5, [r6]\n\
	lsls r1, r4, #1\n\
	movs r2, #0xec\n\
	lsls r2, r2, #2\n\
	adds r0, r5, r2\n\
	adds r0, r0, r1\n\
	movs r2, #0\n\
	strh r3, [r0]\n\
	movs r3, #0xe8\n\
	lsls r3, r3, #2\n\
	adds r0, r5, r3\n\
	adds r0, r0, r4\n\
	movs r1, #5\n\
	strb r1, [r0]\n\
	ldr r0, [r6]\n\
	movs r5, #0xeb\n\
	lsls r5, r5, #2\n\
	adds r0, r0, r5\n\
	adds r0, r0, r4\n\
	strb r2, [r0]\n\
	ldr r0, [r6]\n\
	ldr r1, _08033D18 @ =0x000003A9\n\
	adds r0, r0, r1\n\
	adds r0, r0, r4\n\
	strb r2, [r0]\n\
	ldr r0, [r6]\n\
	ldr r2, _08033D1C @ =0x00000396\n\
	adds r1, r0, r2\n\
	ldrb r2, [r1]\n\
	movs r0, #0\n\
	ldrsb r0, [r1, r0]\n\
	cmp r0, #0\n\
	ble _08033CEE\n\
	subs r0, r2, #1\n\
	strb r0, [r1]\n\
_08033CEE:\n\
	ldr r1, [r6]\n\
	ldr r0, _08033D20 @ =0x000186A0\n\
	str r0, [r1, #0x3c]\n\
	ldr r3, _08033D24 @ =0x00000385\n\
	adds r1, r1, r3\n\
	ldrb r0, [r1]\n\
	adds r0, #1\n\
	strb r0, [r1]\n\
	movs r0, 0xF2\n\
	bl m4aSongNumStart\n\
	movs r0, #7\n\
	bl sub_11B0\n\
	b _08033FD8\n\
	.align 2, 0\n\
_08033D0C: .4byte gUnknown_086AE5EC\n\
_08033D10: .4byte gCurrentPinballGame\n\
_08033D14: .4byte 0x000003A3\n\
_08033D18: .4byte 0x000003A9\n\
_08033D1C: .4byte 0x00000396\n\
_08033D20: .4byte 0x000186A0\n\
_08033D24: .4byte 0x00000385\n\
_08033D28:\n\
	ldr r3, _08033D60 @ =gUnknown_086AE5EC\n\
	ldr r4, _08033D64 @ =gCurrentPinballGame\n\
	ldr r2, [r4]\n\
	ldr r5, [sp, #4]\n\
	asrs r4, r5, #0x10\n\
	ldr r6, _08033D68 @ =0x000003A3\n\
	adds r0, r2, r6\n\
	adds r5, r0, r4\n\
	movs r1, #0\n\
	ldrsb r1, [r5, r1]\n\
	lsls r0, r1, #1\n\
	adds r0, r0, r1\n\
	lsls r0, r0, #1\n\
	adds r1, r3, #2\n\
	adds r0, r0, r1\n\
	movs r6, #0\n\
	ldrsh r1, [r0, r6]\n\
	lsls r0, r4, #1\n\
	movs r6, #0xec\n\
	lsls r6, r6, #2\n\
	adds r2, r2, r6\n\
	adds r2, r2, r0\n\
	ldrh r0, [r2]\n\
	cmp r1, r0\n\
	ble _08033D6C\n\
	adds r0, #1\n\
	strh r0, [r2]\n\
	b _08033DA8\n\
	.align 2, 0\n\
_08033D60: .4byte gUnknown_086AE5EC\n\
_08033D64: .4byte gCurrentPinballGame\n\
_08033D68: .4byte 0x000003A3\n\
_08033D6C:\n\
	movs r6, #0\n\
	movs r0, #0\n\
	strh r0, [r2]\n\
	ldrb r0, [r5]\n\
	adds r0, #1\n\
	strb r0, [r5]\n\
	ldr r1, _08033DEC @ =gCurrentPinballGame\n\
	ldr r0, [r1]\n\
	ldr r2, _08033DF0 @ =0x000003A3\n\
	adds r0, r0, r2\n\
	adds r1, r0, r4\n\
	movs r0, #0\n\
	ldrsb r0, [r1, r0]\n\
	cmp r0, #0xf\n\
	ble _08033DA8\n\
	movs r0, #0xf\n\
	strb r0, [r1]\n\
	ldr r5, _08033DEC @ =gCurrentPinballGame\n\
	ldr r0, [r5]\n\
	movs r1, #0xe8\n\
	lsls r1, r1, #2\n\
	adds r0, r0, r1\n\
	adds r0, r0, r4\n\
	movs r1, #9\n\
	strb r1, [r0]\n\
	ldr r0, [r5]\n\
	adds r2, #3\n\
	adds r0, r0, r2\n\
	adds r0, r0, r4\n\
	strb r6, [r0]\n\
_08033DA8:\n\
	ldr r6, _08033DEC @ =gCurrentPinballGame\n\
	ldr r1, [r6]\n\
	ldr r5, [sp, #4]\n\
	asrs r4, r5, #0x10\n\
	ldr r5, _08033DF0 @ =0x000003A3\n\
	adds r0, r1, r5\n\
	adds r0, r0, r4\n\
	ldrb r0, [r0]\n\
	lsls r0, r0, #0x18\n\
	asrs r0, r0, #0x18\n\
	cmp r0, #0xf\n\
	bne _08033DCE\n\
	movs r0, #0xeb\n\
	lsls r0, r0, #2\n\
	adds r1, r1, r0\n\
	adds r1, r1, r4\n\
	ldrb r0, [r1]\n\
	adds r0, #1\n\
	strb r0, [r1]\n\
_08033DCE:\n\
	ldr r0, [r6]\n\
	adds r0, r0, r5\n\
	adds r0, r0, r4\n\
	movs r1, #0\n\
	ldrsb r1, [r0, r1]\n\
	lsls r0, r1, #1\n\
	adds r0, r0, r1\n\
	lsls r0, r0, #1\n\
	adds r1, r0, r3\n\
	ldrh r1, [r1]\n\
	str r1, [sp]\n\
	adds r1, r3, #4\n\
	adds r0, r0, r1\n\
	b _08033FD6\n\
	.align 2, 0\n\
_08033DEC: .4byte gCurrentPinballGame\n\
_08033DF0: .4byte 0x000003A3\n\
_08033DF4:\n\
	ldr r5, _08033E14 @ =gCurrentPinballGame\n\
	ldr r4, [r5]\n\
	ldr r1, [sp, #4]\n\
	asrs r3, r1, #0x10\n\
	lsls r1, r3, #1\n\
	movs r2, #0xec\n\
	lsls r2, r2, #2\n\
	adds r0, r4, r2\n\
	adds r1, r0, r1\n\
	ldrh r0, [r1]\n\
	cmp r0, #5\n\
	bhi _08033E18\n\
	adds r0, #1\n\
	strh r0, [r1]\n\
	movs r3, #3\n\
	b _08033E8E\n\
	.align 2, 0\n\
_08033E14: .4byte gCurrentPinballGame\n\
_08033E18:\n\
	movs r2, #0\n\
	movs r0, #0\n\
	strh r0, [r1]\n\
	ldr r6, _08033E64 @ =0x000003A3\n\
	adds r0, r4, r6\n\
	adds r0, r0, r3\n\
	movs r1, #0x12\n\
	strb r1, [r0]\n\
	ldr r0, [r5]\n\
	movs r1, #0xe8\n\
	lsls r1, r1, #2\n\
	adds r0, r0, r1\n\
	adds r0, r0, r3\n\
	movs r1, #8\n\
	strb r1, [r0]\n\
	ldr r0, [r5]\n\
	ldr r4, _08033E68 @ =0x000003A9\n\
	adds r0, r0, r4\n\
	adds r0, r0, r3\n\
	strb r2, [r0]\n\
	ldr r0, [r5]\n\
	ldr r5, _08033E6C @ =0x00000396\n\
	adds r1, r0, r5\n\
	ldrb r2, [r1]\n\
	movs r0, #0\n\
	ldrsb r0, [r1, r0]\n\
	cmp r0, #0\n\
	ble _08033E54\n\
	subs r0, r2, #1\n\
	strb r0, [r1]\n\
_08033E54:\n\
	movs r6, #3\n\
	str r6, [sp]\n\
	movs r7, #3\n\
	movs r0, 0xF3\n\
	bl m4aSongNumStart\n\
	b _08033FD8\n\
	.align 2, 0\n\
_08033E64: .4byte 0x000003A3\n\
_08033E68: .4byte 0x000003A9\n\
_08033E6C: .4byte 0x00000396\n\
_08033E70:\n\
	ldr r5, _08033E94 @ =gCurrentPinballGame\n\
	ldr r4, [r5]\n\
	ldr r0, [sp, #4]\n\
	asrs r3, r0, #0x10\n\
	lsls r1, r3, #1\n\
	movs r2, #0xec\n\
	lsls r2, r2, #2\n\
	adds r0, r4, r2\n\
	adds r1, r0, r1\n\
	ldrh r0, [r1]\n\
	cmp r0, #5\n\
	bhi _08033E98\n\
	adds r0, #1\n\
	strh r0, [r1]\n\
	movs r3, #9\n\
_08033E8E:\n\
	str r3, [sp]\n\
	movs r7, #3\n\
	b _08033FD8\n\
	.align 2, 0\n\
_08033E94: .4byte gCurrentPinballGame\n\
_08033E98:\n\
	movs r2, #0\n\
	movs r0, #0\n\
	strh r0, [r1]\n\
	ldr r6, _08033EE4 @ =0x000003A3\n\
	adds r0, r4, r6\n\
	adds r0, r0, r3\n\
	movs r1, #0x12\n\
	strb r1, [r0]\n\
	ldr r0, [r5]\n\
	movs r1, #0xe8\n\
	lsls r1, r1, #2\n\
	adds r0, r0, r1\n\
	adds r0, r0, r3\n\
	movs r1, #8\n\
	strb r1, [r0]\n\
	ldr r0, [r5]\n\
	ldr r4, _08033EE8 @ =0x000003A9\n\
	adds r0, r0, r4\n\
	adds r0, r0, r3\n\
	strb r2, [r0]\n\
	ldr r0, [r5]\n\
	ldr r5, _08033EEC @ =0x00000396\n\
	adds r1, r0, r5\n\
	ldrb r2, [r1]\n\
	movs r0, #0\n\
	ldrsb r0, [r1, r0]\n\
	cmp r0, #0\n\
	ble _08033ED4\n\
	subs r0, r2, #1\n\
	strb r0, [r1]\n\
_08033ED4:\n\
	movs r0, 0xF3\n\
	bl m4aSongNumStart\n\
	movs r6, #9\n\
	str r6, [sp]\n\
	movs r7, #3\n\
	b _08033FD8\n\
	.align 2, 0\n\
_08033EE4: .4byte 0x000003A3\n\
_08033EE8: .4byte 0x000003A9\n\
_08033EEC: .4byte 0x00000396\n\
_08033EF0:\n\
	ldr r3, _08033F28 @ =gUnknown_086AE5EC\n\
	ldr r0, _08033F2C @ =gCurrentPinballGame\n\
	ldr r2, [r0]\n\
	ldr r1, [sp, #4]\n\
	asrs r4, r1, #0x10\n\
	ldr r5, _08033F30 @ =0x000003A3\n\
	adds r0, r2, r5\n\
	adds r5, r0, r4\n\
	movs r1, #0\n\
	ldrsb r1, [r5, r1]\n\
	lsls r0, r1, #1\n\
	adds r0, r0, r1\n\
	lsls r0, r0, #1\n\
	adds r1, r3, #2\n\
	adds r0, r0, r1\n\
	movs r6, #0\n\
	ldrsh r1, [r0, r6]\n\
	lsls r0, r4, #1\n\
	movs r6, #0xec\n\
	lsls r6, r6, #2\n\
	adds r2, r2, r6\n\
	adds r2, r2, r0\n\
	ldrh r0, [r2]\n\
	cmp r1, r0\n\
	ble _08033F34\n\
	adds r0, #1\n\
	strh r0, [r2]\n\
	b _08033F64\n\
	.align 2, 0\n\
_08033F28: .4byte gUnknown_086AE5EC\n\
_08033F2C: .4byte gCurrentPinballGame\n\
_08033F30: .4byte 0x000003A3\n\
_08033F34:\n\
	movs r0, #0\n\
	strh r0, [r2]\n\
	ldrb r0, [r5]\n\
	adds r0, #1\n\
	strb r0, [r5]\n\
	ldr r1, _08033F88 @ =gCurrentPinballGame\n\
	ldr r0, [r1]\n\
	ldr r2, _08033F8C @ =0x000003A3\n\
	adds r0, r0, r2\n\
	adds r1, r0, r4\n\
	movs r0, #0\n\
	ldrsb r0, [r1, r0]\n\
	cmp r0, #0x19\n\
	ble _08033F64\n\
	movs r0, #0x19\n\
	strb r0, [r1]\n\
	ldr r5, _08033F88 @ =gCurrentPinballGame\n\
	ldr r0, [r5]\n\
	movs r6, #0xe8\n\
	lsls r6, r6, #2\n\
	adds r0, r0, r6\n\
	adds r0, r0, r4\n\
	movs r1, #9\n\
	strb r1, [r0]\n\
_08033F64:\n\
	ldr r0, _08033F88 @ =gCurrentPinballGame\n\
	ldr r0, [r0]\n\
	ldr r2, [sp, #4]\n\
	asrs r1, r2, #0x10\n\
	ldr r4, _08033F8C @ =0x000003A3\n\
	adds r0, r0, r4\n\
	adds r0, r0, r1\n\
	movs r1, #0\n\
	ldrsb r1, [r0, r1]\n\
	lsls r0, r1, #1\n\
	adds r0, r0, r1\n\
	lsls r0, r0, #1\n\
	adds r1, r0, r3\n\
	ldrh r1, [r1]\n\
	str r1, [sp]\n\
	adds r1, r3, #4\n\
	adds r0, r0, r1\n\
	b _08033FD6\n\
	.align 2, 0\n\
_08033F88: .4byte gCurrentPinballGame\n\
_08033F8C: .4byte 0x000003A3\n\
_08033F90:\n\
	ldr r3, _0803401C @ =gCurrentPinballGame\n\
	ldr r0, [r3]\n\
	ldr r5, [sp, #4]\n\
	asrs r4, r5, #0x10\n\
	ldr r6, _08034020 @ =0x000003A6\n\
	adds r0, r0, r6\n\
	adds r0, r0, r4\n\
	movs r1, #0\n\
	strb r1, [r0]\n\
	ldr r0, [r3]\n\
	ldr r2, _08034024 @ =0x000003A9\n\
	adds r0, r0, r2\n\
	adds r0, r0, r4\n\
	strb r1, [r0]\n\
	ldr r0, [r3]\n\
	movs r5, #0xe8\n\
	lsls r5, r5, #2\n\
	adds r0, r0, r5\n\
	adds r0, r0, r4\n\
	strb r1, [r0]\n\
	ldr r2, _08034028 @ =gUnknown_086AE5EC\n\
	ldr r0, [r3]\n\
	subs r6, #3\n\
	adds r0, r0, r6\n\
	adds r0, r0, r4\n\
_08033FC2:\n\
	movs r1, #0\n\
	ldrsb r1, [r0, r1]\n\
	lsls r0, r1, #1\n\
	adds r0, r0, r1\n\
	lsls r0, r0, #1\n\
	adds r1, r0, r2\n\
	ldrh r1, [r1]\n\
	str r1, [sp]\n\
	adds r2, #4\n\
	adds r0, r0, r2\n\
_08033FD6:\n\
	ldrh r7, [r0]\n\
_08033FD8:\n\
	ldr r2, _0803401C @ =gCurrentPinballGame\n\
	ldr r3, [r2]\n\
	ldr r0, [sp, #4]\n\
	asrs r6, r0, #0x10\n\
	lsls r1, r6, #1\n\
	ldr r4, _0803402C @ =0x000003B6\n\
	adds r0, r3, r4\n\
	adds r5, r0, r1\n\
	adds r4, #6\n\
	adds r0, r3, r4\n\
	adds r0, r0, r1\n\
	ldrh r1, [r5]\n\
	ldrh r0, [r0]\n\
	cmp r1, r0\n\
	bls _0803404C\n\
	ldr r1, _08034030 @ =0x00000385\n\
	adds r0, r3, r1\n\
	ldrb r0, [r0]\n\
	lsls r0, r0, #0x18\n\
	asrs r0, r0, #0x18\n\
	cmp r0, #0x12\n\
	bgt _0803404C\n\
	movs r0, #0\n\
	strh r0, [r5]\n\
	movs r2, #0xe8\n\
	lsls r2, r2, #2\n\
	adds r0, r3, r2\n\
	adds r1, r0, r6\n\
	movs r0, #0\n\
	ldrsb r0, [r1, r0]\n\
	cmp r0, #2\n\
	bne _08034034\n\
	movs r0, #6\n\
	b _08034036\n\
	.align 2, 0\n\
_0803401C: .4byte gCurrentPinballGame\n\
_08034020: .4byte 0x000003A6\n\
_08034024: .4byte 0x000003A9\n\
_08034028: .4byte gUnknown_086AE5EC\n\
_0803402C: .4byte 0x000003B6\n\
_08034030: .4byte 0x00000385\n\
_08034034:\n\
	movs r0, #7\n\
_08034036:\n\
	strb r0, [r1]\n\
	ldr r3, _080340D8 @ =gCurrentPinballGame\n\
	ldr r0, [r3]\n\
	ldr r4, [sp, #4]\n\
	asrs r1, r4, #0xf\n\
	movs r5, #0xec\n\
	lsls r5, r5, #2\n\
	adds r0, r0, r5\n\
	adds r0, r0, r1\n\
	movs r1, #0\n\
	strh r1, [r0]\n\
_0803404C:\n\
	ldr r6, _080340D8 @ =gCurrentPinballGame\n\
	ldr r0, [r6]\n\
	ldr r1, [sp, #4]\n\
	asrs r5, r1, #0x10\n\
	ldr r2, _080340DC @ =0x00000397\n\
	adds r1, r0, r2\n\
	adds r1, r1, r5\n\
	adds r2, #3\n\
	adds r0, r0, r2\n\
	adds r0, r0, r5\n\
	ldrb r0, [r0]\n\
	strb r0, [r1]\n\
	ldr r0, [r6]\n\
	adds r0, r0, r2\n\
	adds r0, r0, r5\n\
	strb r7, [r0]\n\
	ldr r0, [r6]\n\
	ldr r3, _080340E0 @ =0x0000039D\n\
	adds r0, r0, r3\n\
	adds r0, r0, r5\n\
	mov r4, sp\n\
	ldrb r4, [r4]\n\
	strb r4, [r0]\n\
	ldr r4, [r6]\n\
	lsls r0, r5, #2\n\
	adds r4, r4, r0\n\
	movs r6, #0xf1\n\
	lsls r6, r6, #2\n\
	adds r0, r4, r6\n\
	movs r1, #0\n\
	ldrsh r0, [r0, r1]\n\
	movs r1, #0xa\n\
	bl __divsi3\n\
	lsls r0, r0, #0x10\n\
	asrs r0, r0, #0xf\n\
	adds r0, #0xd0\n\
	movs r2, #0xf4\n\
	lsls r2, r2, #2\n\
	adds r1, r4, r2\n\
	strh r0, [r1]\n\
	ldr r3, _080340E4 @ =0x000003C6\n\
	adds r0, r4, r3\n\
	movs r6, #0\n\
	ldrsh r0, [r0, r6]\n\
	movs r1, #0xa\n\
	bl __divsi3\n\
	lsls r0, r0, #0x10\n\
	asrs r0, r0, #0xf\n\
	adds r0, #0x42\n\
	ldr r1, _080340E8 @ =0x000003D2\n\
	adds r4, r4, r1\n\
	strh r0, [r4]\n\
	adds r5, #1\n\
	lsls r5, r5, #0x10\n\
	lsrs r2, r5, #0x10\n\
	asrs r5, r5, #0x10\n\
	cmp r5, #2\n\
	bgt _080340C8\n\
	bl _08033752\n\
_080340C8:\n\
	add sp, #8\n\
	pop {r3, r4, r5}\n\
	mov r8, r3\n\
	mov sb, r4\n\
	mov sl, r5\n\
	pop {r4, r5, r6, r7}\n\
	pop {r0}\n\
	bx r0\n\
	.align 2, 0\n\
_080340D8: .4byte gCurrentPinballGame\n\
_080340DC: .4byte 0x00000397\n\
_080340E0: .4byte 0x0000039D\n\
_080340E4: .4byte 0x000003C6\n\
_080340E8: .4byte 0x000003D2\n\
    ");
}
#endif
