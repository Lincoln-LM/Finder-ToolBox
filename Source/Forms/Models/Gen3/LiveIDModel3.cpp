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

#include "LiveIDModel3.hpp"

LiveIDModel3::LiveIDModel3(QObject *parent) : TableModel<LiveIDState3>(parent)
{
}

int LiveIDModel3::columnCount(const QModelIndex &parent) const
{
    (void)parent;
    return 6;
}

QVariant LiveIDModel3::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole)
    {
        const auto &state = model[index.row()];
        switch (index.column())
        {
        case 0:
            return QString::number(state.getInitial(),16).toUpper();
        case 1:
            return state.getAdvances();
        case 2:
            return state.getTID();
        case 3:
            return state.getSID();
        case 4:
            return QString::fromStdString(state.getShiny());
        case 5:
            return QString::fromStdString(state.getDate());
        }
    }

    return QVariant();
}

QVariant LiveIDModel3::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
    {
        return header[section];
    }
    return QVariant();
}
