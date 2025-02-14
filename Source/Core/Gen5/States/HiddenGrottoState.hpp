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

#ifndef HIDDENGROTTOSTATE_HPP
#define HIDDENGROTTOSTATE_HPP

#include <Core/Parents/States/State.hpp>
#include <Core/Util/Global.hpp>

class HiddenGrottoState
{
public:
    HiddenGrottoState() = default;

    HiddenGrottoState(u32 seed, u32 advances, u8 group, u8 slot, u8 gender) :
        seed(seed), advances(advances), group(group), slot(slot), gender(gender)
    {
    }

    u32 getSeed() const
    {
        return seed;
    }

    u32 getAdvances() const
    {
        return advances;
    }

    u8 getGroup() const
    {
        return group;
    }

    u8 getSlot() const
    {
        return slot;
    }

    u8 getGender() const
    {
        return gender;
    }

private:
    u32 seed;
    u32 advances;
    u8 group;
    u8 slot;
    u8 gender;
};

#endif // HIDDENGROTTOSTATE_HPP
