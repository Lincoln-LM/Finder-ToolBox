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

#include "TinyMTSeedSearcher.hpp"
#include <algorithm>
#include <future>
#include <iostream>

TinyMTSeedSearcher::TinyMTSeedSearcher(u32 minSeed, u32 maxSeed, u32 minAdvance, u32 maxAdvance, const std::vector<u32> &criteria) : SeedSearcher(criteria), minSeed(minSeed), maxSeed(maxSeed), minAdvance(minAdvance), maxAdvance(maxAdvance)
{
}

std::vector<u32> TinyMTSeedSearcher::getAdvanceResults() const
{
    return advanceResults;
}

void TinyMTSeedSearcher::startSearch(int threads)
{
    searching = true;
    if (minSeed != maxSeed)
    {
        std::vector<std::future<void>> threadContainer;

        u32 split = (maxSeed - minSeed) / threads;
        u32 start = minSeed;
        for (int i = 0; i < threads; i++)
        {
            if (i == threads - 1)
            {
                threadContainer.emplace_back(std::async(std::launch::async, [=] { search(start, maxSeed); }));
            }
            else
            {
                threadContainer.emplace_back(std::async(std::launch::async, [=] { search(start, start + split); }));
            }
            start += split;
        }

        for (int i = 0; i < threads; i++)
        {
            threadContainer[i].wait();
        }
        std::sort(results.begin(), results.end());
        results.erase(std::unique(results.begin(), results.end()), results.end());
    }
    else
    {
        searchSingleSeed(minSeed);
    }
}

int TinyMTSeedSearcher::getProgress() const
{
    return progress >> 1;
}

void TinyMTSeedSearcher::search(u32 start, u32 end)
{
    for (u32 seed = start; seed <= end; seed++)
    {
        if (!searching)
        {
            return;
        }
        u32 advance = searchSeed(seed);
        if (advance != 0xFFFFFFFF)
        {
            std::lock_guard<std::mutex> lock(mutex);
            results.emplace_back(seed);
            advanceResults.emplace_back(advance);
        }

        progress++;
    }
}

void TinyMTSeedSearcher::searchSingleSeed(u32 seed)
{
    TinyMT rng(seed);
    u16 leeway = 10;
    rng.advance(minAdvance);
    u16 blinkPosition = 0;
    for (u32 cnt = minAdvance; cnt < maxAdvance + criteria.size(); cnt++)
    {
        if (!searching)
        {
            return;
        }
        u16 blinkTimer = 250 + ((static_cast<u64>(rng.next()) * 240) >> 32);
        if (blinkTimer <= criteria[blinkPosition] + leeway && blinkTimer >= criteria[blinkPosition] - leeway)
        {
            blinkPosition++;
            if (blinkPosition == criteria.size())
            {
                std::lock_guard<std::mutex> lock(mutex);
                results.emplace_back(seed);
                advanceResults.emplace_back(cnt);
                blinkPosition = 0;
            }
        }
        else
        {
            if (blinkPosition == 0)
            {
                continue;
            }
            if (blinkTimer <= criteria[blinkPosition] + leeway && blinkTimer >= criteria[blinkPosition] - leeway)
            {
                blinkPosition = 1;
                if (blinkPosition == criteria.size())
                {
                    std::lock_guard<std::mutex> lock(mutex);
                    results.emplace_back(seed);
                    advanceResults.emplace_back(cnt);
                    blinkPosition = 0;
                }
            }
            else
            {
                blinkPosition = 0;
            }
        }
    }
}
u32 TinyMTSeedSearcher::searchSeed(u32 seed)
{
    TinyMT rng(seed);
    u16 leeway = 10;
    rng.advance(minAdvance);
    u16 blinkPosition = 0;
    for (u32 cnt = minAdvance; cnt < maxAdvance + criteria.size(); cnt++)
    {
        u16 blinkTimer = 250 + ((static_cast<u64>(rng.next()) * 240) >> 32);
        if (blinkTimer <= criteria[blinkPosition] + leeway && blinkTimer >= criteria[blinkPosition] - leeway)
        {
            blinkPosition++;
            if (blinkPosition == criteria.size())
            {
                return cnt;
            }
        }
        else
        {
            if (blinkPosition == 0)
            {
                continue;
            }
            if (blinkTimer <= criteria[blinkPosition] + leeway && blinkTimer >= criteria[blinkPosition] - leeway)
            {
                blinkPosition = 1;
                if (blinkPosition == criteria.size())
                {
                    return cnt;
                }
            }
            else
            {
                blinkPosition = 0;
            }
        }
    }

    return 0xFFFFFFFF;
}
