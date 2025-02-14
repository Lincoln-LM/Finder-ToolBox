/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2021 by Admiral_Fish, bumba, and EzPzStreamz
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 3
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include "GameCubeGenerator.hpp"
#include <Core/Enum/Method.hpp>
#include <Core/Enum/ShadowType.hpp>
#include <Core/Parents/Filters/StateFilter.hpp>
#include <Core/RNG/LCRNG.hpp>

GameCubeGenerator::GameCubeGenerator(u32 initialAdvances, u32 maxAdvances, u16 tid, u16 sid, u8 genderRatio, Method method,
                                     const StateFilter &filter) :
    Generator(initialAdvances, maxAdvances, tid, sid, genderRatio, method, filter)
{
}

std::vector<GameCubeState> GameCubeGenerator::generate(u32 seed) const
{
    switch (method)
    {
    case Method::XDColo:
        return generateXDColo(seed);
    case Method::XD:
        return generateXDShadow(seed);
    case Method::Colo:
        return generateColoShadow(seed);
    case Method::Channel:
        return generateChannel(seed);
    case Method::Ageto:
        return generateAgeto(seed);
    case Method::Ageto0Difference:
        return generateAgeto0Difference(seed);
    case Method::PossibleChannel:
        return generatePossibleChannel(seed);
    default:
        return std::vector<GameCubeState>();
    }
}

void GameCubeGenerator::setShadowTeam(u8 index, u8 type)
{
    team = ShadowTeam::loadShadowTeams(method)[index];
    this->type = type;
}

std::vector<GameCubeState> GameCubeGenerator::generateXDColo(u32 seed) const
{
    std::vector<GameCubeState> states;

    XDRNG rng(seed);
    rng.advance(initialAdvances + offset);

    // Method XD/Colo [SEED] [IVS] [IVS] [BLANK] [PID] [PID]

    for (u32 cnt = 0; cnt <= maxAdvances; cnt++, rng.next())
    {
        GameCubeState state(initialAdvances + cnt);
        XDRNG go(rng.getSeed());

        u16 iv1 = go.nextUShort();
        u16 iv2 = go.nextUShort();
        u8 ability = go.nextUShort() & 1;
        u16 high = go.nextUShort();
        u16 low = go.nextUShort();

        state.setPID(high, low);
        state.setAbility(ability);
        state.setGender(low & 255, genderRatio);
        state.setNature(state.getPID() % 25);
        state.setShiny<8>(tsv, high ^ low);

        state.setIVs(iv1, iv2);
        state.calculateHiddenPower();

        if (filter.compareState(state))
        {
            states.emplace_back(state);
        }
    }

    return states;
}

std::vector<GameCubeState> GameCubeGenerator::generateAgeto(u32 seed) const
{
    std::vector<GameCubeState> states;

    XDRNG rng(seed);
    rng.advance(initialAdvances + offset);

    // Method XD/Colo [SEED] [IVS] [IVS] [BLANK] [PID] [PID]

    for (u32 cnt = 0; cnt <= maxAdvances; cnt++, rng.next())
    {
        GameCubeState state(initialAdvances + cnt);
        XDRNG go(rng.getSeed());

        u16 iv1 = go.nextUShort();
        u16 iv2 = go.nextUShort();
        u8 ability = go.nextUShort() & 1;
        u16 high = go.nextUShort();
        u16 low = go.nextUShort();
        while ((31121 ^ 00000 ^ high ^ low) < 8)
        {
            high = go.nextUShort();
            low = go.nextUShort();
        }


        state.setPID(high, low);
        state.setAbility(ability);
        state.setGender(low & 255, genderRatio);
        state.setNature(state.getPID() % 25);
        state.setShiny<8>(31121 ^ 00000, high ^ low);

        state.setIVs(iv1, iv2);
        state.calculateHiddenPower();

        if (filter.compareState(state))
        {
            states.emplace_back(state);
        }
    }

    return states;
}

std::vector<GameCubeState> GameCubeGenerator::generateAgeto0Difference(u32 seed) const
{
    std::vector<GameCubeState> states;

    XDRNG rng(seed);
    rng.advance(initialAdvances + 29278);

    // Method XD/Colo [SEED] [IVS] [IVS] [BLANK] [PID] [PID]

    for (u32 cnt = 0; cnt <= maxAdvances; cnt += 2792, rng.advance(2792))
    {
        GameCubeState state(initialAdvances + cnt + 29278-offset);
        XDRNG go(rng.getSeed());

        u16 iv1 = go.nextUShort();
        u16 iv2 = go.nextUShort();
        u8 ability = go.nextUShort() & 1;
        u16 high = go.nextUShort();
        u16 low = go.nextUShort();
        while ((31121 ^ 00000 ^ high ^ low) < 8)
        {
            high = go.nextUShort();
            low = go.nextUShort();
        }


        state.setPID(high, low);
        state.setAbility(ability);
        state.setGender(low & 255, genderRatio);
        state.setNature(state.getPID() % 25);
        state.setShiny<8>(31121 ^ 00000, high ^ low);

        state.setIVs(iv1, iv2);
        state.calculateHiddenPower();

        if (filter.compareState(state))
        {
            states.emplace_back(state);
        }
    }

    return states;
}

std::vector<GameCubeState> GameCubeGenerator::generateXDShadow(u32 seed) const
{
    std::vector<GameCubeState> states;

    XDRNG rng(seed);
    rng.advance(initialAdvances + offset);

    std::vector<LockInfo> locks = team.getLocks();

    for (u32 cnt = 0; cnt <= maxAdvances; cnt++, rng.next())
    {
        GameCubeState state(initialAdvances + cnt);

        XDRNG go(rng.getSeed());

        // Enemy TID/SID
        go.advance(2);

        for (auto lock = locks.rbegin(); lock != locks.rend(); lock++)
        {
            // Temporary PID: 2 advances
            // IVs: 2 advances
            // Ability: 1 state
            go.advance(5);

            // If we are looking at a shadow pokemon
            // We will assume it is already set and skip the PID process
            if (!lock->getFree())
            {
                u32 pid;
                do
                {
                    u16 high = go.nextUShort();
                    u16 low = go.nextUShort();
                    pid = (high << 16) | low;

                    if ((high ^ low ^ tsv) < 8) // Shiny lock is from TSV of savefile
                    {
                        continue;
                    }
                } while (!lock->compare(pid));
            }
        }

        if (team.getType() == ShadowType::SecondShadow || team.getType() == ShadowType::Salamence)
        {
            go.advance(5); // Set and Unset start the same

            if (type == 1) // Check for shiny lock with unset
            {
                u16 psv = go.nextUShort() ^ go.nextUShort();
                while ((psv ^ tsv) < 8)
                {
                    psv = go.nextUShort() ^ go.nextUShort();
                }
            }
        }

        go.advance(2); // Fake PID

        u16 iv1 = go.nextUShort();
        u16 iv2 = go.nextUShort();
        state.setIVs(iv1, iv2);
        state.calculateHiddenPower();

        state.setAbility(go.nextUShort() & 1);

        u16 high = go.nextUShort();
        u16 low = go.nextUShort();
        while ((high ^ low ^ tsv) < 8) // Shiny lock is from TSV of savefile
        {
            high = go.nextUShort();
            low = go.nextUShort();
        }

        state.setPID(high, low);
        state.setGender(low & 255, genderRatio);
        state.setNature(state.getPID() % 25);
        state.setShiny(0);

        if (filter.compareState(state))
        {
            states.emplace_back(state);
        }
    }

    return states;
}

std::vector<GameCubeState> GameCubeGenerator::generateColoShadow(u32 seed) const
{
    std::vector<GameCubeState> states;

    XDRNG rng(seed);
    rng.advance(initialAdvances + offset);

    std::vector<LockInfo> locks = team.getLocks();

    for (u32 cnt = 0; cnt <= maxAdvances; cnt++, rng.next())
    {
        GameCubeState state(initialAdvances + cnt);

        XDRNG go(rng.getSeed());

        // Trainer TID/SID
        u16 trainerTSV = go.nextUShort() ^ go.nextUShort();

        u8 ability;
        u32 pid;
        for (auto lock = locks.rbegin(); lock != locks.rend(); lock++)
        {
            // Temporary PID: 2 advances
            // IVs: 2 advances
            // Ability: 1 state
            go.advance(4);
            ability = go.nextUShort() & 1;

            do
            {
                u16 high = go.nextUShort();
                u16 low = go.nextUShort();
                pid = (high << 16) | low;

                if ((high ^ low ^ trainerTSV) < 8) // Shiny lock is from enemy TSV
                {
                    continue;
                }
            } while (!lock->compare(pid));
        }

        // E-Reader is included as part of the above loop
        // Set the PID and ability that was already computed
        // IVs are 0
        if (team.getType() == ShadowType::EReader)
        {
            state.setIVs(0);
            state.calculateHiddenPower();

            state.setPID(pid);
            state.setAbility(ability);
            state.setGender(pid & 255, genderRatio);
            state.setNature(pid % 25);
            state.setShiny<8>(tsv, (pid >> 16) ^ (pid & 0xffff));
        }
        else
        {
            go.advance(2); // Fake PID

            u16 iv1 = go.nextUShort();
            u16 iv2 = go.nextUShort();
            state.setIVs(iv1, iv2);
            state.calculateHiddenPower();

            state.setAbility(go.nextUShort() & 1);

            u16 high = go.nextUShort();
            u16 low = go.nextUShort();
            while ((high ^ low ^ trainerTSV) < 8) // Shiny lock is from enemy TSV
            {
                high = go.nextUShort();
                low = go.nextUShort();
            }

            state.setPID(high, low);
            state.setGender(low & 255, genderRatio);
            state.setNature(state.getPID() % 25);
            state.setShiny<8>(tsv, high ^ low);
        }

        if (filter.compareState(state))
        {
            states.emplace_back(state);
        }
    }

    return states;
}

std::vector<GameCubeState> GameCubeGenerator::generateChannel(u32 seed) const
{
    std::vector<GameCubeState> states;

    XDRNG rng(seed);
    rng.advance(initialAdvances + offset);

    // Method Channel [SEED] [SID] [PID] [PID] [BERRY] [GAME ORIGIN] [OT GENDER] [IV] [IV] [IV] [IV] [IV] [IV]

    for (u32 cnt = 0; cnt <= maxAdvances; cnt++, rng.next())
    {
        GameCubeState state(initialAdvances + cnt);
        XDRNG go(rng.getSeed());

        // u16 tid = 40122;
        u16 sid = go.nextUShort();
        u16 high = go.nextUShort();
        u16 low = go.nextUShort();
        go.advance(3);

        // u16 berry = go.nextUShort() >> 13; If >= 4 salac, else ganlon
        // u16 game = go.nextUShort() >> 12; If >= 8 ruby, else sapphire
        // u16 gender = go.nextUShort() >> 11; If >= 16 female, else male

        if ((low > 7 ? 0 : 1) != (high ^ 40122 ^ sid))
        {
            high ^= 0x8000;
        }

        state.setPID(high, low);
        state.setAbility(low & 1);
        state.setGender(low & 255, genderRatio);
        state.setNature(state.getPID() % 25);
        state.setShiny<8>(40122 ^ sid, high ^ low);

        u8 hp = go.nextUShort() >> 11;
        u8 atk = go.nextUShort() >> 11;
        u8 def = go.nextUShort() >> 11;
        u8 spe = go.nextUShort() >> 11;
        u8 spa = go.nextUShort() >> 11;
        u8 spd = go.nextUShort() >> 11;

        state.setIVs(hp, atk, def, spa, spd, spe);
        state.calculateHiddenPower();

        if (filter.compareState(state))
        {
            states.emplace_back(state);
        }
    }

    return states;
}

std::vector<GameCubeState> GameCubeGenerator::generatePossibleChannel(u32 seed) const
{
    std::vector<GameCubeState> states;

    XDRNG rng(seed);
    rng.advance(initialAdvances + offset);

    // Method Channel [SEED] [SID] [PID] [PID] [BERRY] [GAME ORIGIN] [OT GENDER] [IV] [IV] [IV] [IV] [IV] [IV]

    for (u32 cnt = 0; cnt <= maxAdvances; cnt++, rng.next())
    {
        if (validateJirachi(rng.getSeed()))
        {
            GameCubeState state(initialAdvances + cnt);
            XDRNG go(rng.getSeed());

            // u16 tid = 40122;
            u16 sid = go.nextUShort();
            u16 high = go.nextUShort();
            u16 low = go.nextUShort();
            go.advance(3);

            // u16 berry = go.nextUShort() >> 13; If >= 4 salac, else ganlon
            // u16 game = go.nextUShort() >> 12; If >= 8 ruby, else sapphire
            // u16 gender = go.nextUShort() >> 11; If >= 16 female, else male

            if ((low > 7 ? 0 : 1) != (high ^ 40122 ^ sid))
            {
                high ^= 0x8000;
            }

            state.setPID(high, low);
            state.setAbility(low & 1);
            state.setGender(low & 255, genderRatio);
            state.setNature(state.getPID() % 25);
            state.setShiny<8>(40122 ^ sid, high ^ low);

            u8 hp = go.nextUShort() >> 11;
            u8 atk = go.nextUShort() >> 11;
            u8 def = go.nextUShort() >> 11;
            u8 spe = go.nextUShort() >> 11;
            u8 spa = go.nextUShort() >> 11;
            u8 spd = go.nextUShort() >> 11;

            state.setIVs(hp, atk, def, spa, spd, spe);
            state.calculateHiddenPower();

            if (filter.compareState(state))
            {
                states.emplace_back(state);
            }
        }
    }

    return states;
}

// Working backwards this validates if a Jirachi seed is obtainable
// There are 3 different patterns for this (6/7/8 advances) plus menu checking
bool GameCubeGenerator::validateJirachi(u32 seed) const
{
    XDRNGR rng(seed);

    u16 num1 = rng.nextUShort();
    u16 num2 = rng.nextUShort();
    u16 num3 = rng.nextUShort();

    rng.advance(3);
    if (num1 <= 0x4000) // 6 advances
    {
        if (validateMenu(rng.getSeed()))
        {
            return true;
        }
    }

    rng.advance(1);
    if (num2 > 0x4000 && num1 <= 0x547a) // 7 advances
    {
        if (validateMenu(rng.getSeed()))
        {
            return true;
        }
    }

    rng.advance(1);
    if (num3 > 0x4000 && num2 > 0x547a) // 8 advances
    {
        if (validateMenu(rng.getSeed()))
        {
            return true;
        }
    }

    return false;
}

// Working backwards from a seed check if the menu sequence will end on said seed
// Menu will advance the prng until it collects a 1, 2, and 3
bool GameCubeGenerator::validateMenu(u32 seed) const
{
    u8 mask = 0;
    u8 target = seed >> 30;

    // Impossible to stop 0
    if (target == 0)
    {
        return false;
    }
    else
    {
        mask |= 1 << target;
    }

    auto rng = XDRNGR(seed);
    while ((mask & 14) != 14)
    {
        u8 num = rng.next() >> 30;

        // Basically this check means that while rolling for 1, 2, and 3
        // We hit our original target meaning that we can't land on the target
        if (num == target)
        {
            return false;
        }

        mask |= 1 << num;
    }

    return true;
}
