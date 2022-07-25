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

#ifndef TINYMTSEEDFINDER_HPP
#define TINYMTSEEDFINDER_HPP

#include <Core/Util/Global.hpp>
#include <QWidget>

namespace Ui
{
    class TinyMTSeedFinder;
}

class TinyMTSeedFinder : public QWidget
{
    Q_OBJECT
public:
    explicit TinyMTSeedFinder(QWidget *parent = nullptr);
    ~TinyMTSeedFinder() override;

private:
    Ui::TinyMTSeedFinder *ui;
    u64 lastMS;

    void addBlink();
    void restartBlinks();
    void updateSearch(const std::vector<u32> &seeds, const std::vector<u32> &advances);
    void updateProgress(int progress);

private slots:
    void startSearch();

protected:
    bool eventFilter(QObject* obj, QEvent* event);
};

#endif // TINYMTSEEDFINDER_HPP
