/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2022 by Admiral_Fish, bumba, and EzPzStreamz
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

#ifndef TINYMTSEEDSEARCHER_HPP
#define TINYMTSEEDSEARCHER_HPP

#include <Core/Gen3/Searchers/SeedSearcher.hpp>
#include <Core/RNG/TinyMT.hpp>

class TinyMTSeedSearcher : public SeedSearcher
{
public:
    explicit TinyMTSeedSearcher(u32 minSeed, u32 maxSeed, u32 minAdvance, u32 maxAdvance, const std::vector<u32> &criteria);
    void startSearch(int threads);
    std::vector<u32> getAdvanceResults() const;
    int getProgress() const override;

private:
    u32 minSeed;
    u32 maxSeed;
    u32 minAdvance;
    u32 maxAdvance;
    void search(u32 start, u32 end);
    u32 searchSeed(u32 seed);
    void searchSingleSeed(u32 seed);

protected:
    std::vector<u32> advanceResults;
};

#endif // TINYMTSEEDSEARCHER_HPP
