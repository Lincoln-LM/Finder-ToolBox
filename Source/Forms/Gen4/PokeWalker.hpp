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

#ifndef POKEWALKER_HPP
#define POKEWALKER_HPP

#include <Core/Gen4/Profile4.hpp>
#include <QMenu>

class PokeWalkerGeneratorModel;
class PokeWalkerSearcherModel;

namespace Ui
{
    class PokeWalker;
}

class PokeWalker : public QWidget
{
    Q_OBJECT
signals:
    void alertProfiles(int);

public:
    explicit PokeWalker(QWidget *parent = nullptr);
    ~PokeWalker() override;
    void updateProfiles();

private:
    Ui::PokeWalker *ui;
    std::vector<Profile4> profiles;
    Profile4 currentProfile;
    PokeWalkerGeneratorModel *generatorModel = nullptr;
    PokeWalkerSearcherModel *searcherModel = nullptr;
    QMenu *generatorMenu = nullptr;
    QMenu *searcherMenu = nullptr;

    void setupModels();

private slots:
    void generate();
    void search();
    void profilesIndexChanged(int index);
    void tableViewGeneratorContextMenu(QPoint pos);
    void tableViewSearcherContextMenu(QPoint pos);
    void seedToTime();
    void profileManager();
};

#endif // POKEWALKER_HPP
