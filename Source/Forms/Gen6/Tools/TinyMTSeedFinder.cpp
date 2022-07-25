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

#include "TinyMTSeedFinder.hpp"
#include "ui_TinyMTSeedFinder.h"
#include <Core/Enum/Method.hpp>
#include <Core/Gen6/Searchers/TinyMTSeedSearcher.hpp>
#include <Core/Util/Translator.hpp>
#include <QKeyEvent>
#include <QClipboard>
#include <QScrollBar>
#include <QCryptographicHash>
#include <QDesktopServices>
#include <QFileDialog>
#include <QMessageBox>
#include <QSettings>
#include <QThread>
#include <QTimer>
#include <QUrl>

TinyMTSeedFinder::TinyMTSeedFinder(QWidget *parent) :
    QWidget(parent), ui(new Ui::TinyMTSeedFinder)
{
    lastMS = 0;
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);
    setAttribute(Qt::WA_DeleteOnClose);

    ui->minAdvanceTextBox->setValues(InputType::Advance32Bit);
    ui->maxAdvanceTextBox->setValues(InputType::Advance32Bit);
    ui->minSeedTextBox->setValues(InputType::Seed32Bit);
    ui->maxSeedTextBox->setValues(InputType::Seed32Bit);

    connect(ui->addBlinkButton, &QPushButton::clicked, this, &TinyMTSeedFinder::addBlink);
    connect(ui->startSearchButton, &QPushButton::clicked, this, &TinyMTSeedFinder::startSearch);
    connect(ui->restartBlinksButton, &QPushButton::clicked, this, &TinyMTSeedFinder::restartBlinks);
    this->installEventFilter(this);

    QSettings setting;
    if (setting.contains("tinyMTSeedFinder/geometry"))
    {
        this->restoreGeometry(setting.value("tinyMTSeedFinder/geometry").toByteArray());
    }
}

TinyMTSeedFinder::~TinyMTSeedFinder()
{
    QSettings setting;
    setting.setValue("tinyMTSeedFinder/geometry", this->saveGeometry());

    delete ui;
}

bool TinyMTSeedFinder::eventFilter(QObject* obj, QEvent* event)
{
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent* key = static_cast<QKeyEvent*>(event);
        if ((key->key() == Qt::Key_Enter) || (key->key() == Qt::Key_Return)) {
            addBlink();
        } else {
            return QObject::eventFilter(obj, event);
        }
        return true;
    } else {
        return QObject::eventFilter(obj, event);
    }
    return false;
}

void TinyMTSeedFinder::addBlink()
{
    u64 currentMS = QDateTime::currentMSecsSinceEpoch();
    if (lastMS == 0)
    {
        lastMS = currentMS;
        ui->restartBlinksButton->setEnabled(true);
    }
    else
    {
        u16 blinkTime = double(currentMS - lastMS) / 1000 * 59.8261;
        QListWidgetItem* widgetItem = new QListWidgetItem(ui->blinkList);
        widgetItem->setFlags(widgetItem->flags() | Qt::ItemIsEditable);
        widgetItem->setText(QString::number(blinkTime));
        QScrollBar* scrollbar = ui->blinkList->verticalScrollBar();
        if (scrollbar == NULL || scrollbar->value() == scrollbar->maximum()) {
            ui->blinkList->scrollToBottom();
            ui->blinkList->addItem(widgetItem);
        }
        else
        {
            ui->blinkList->addItem(widgetItem);
        }
        
        lastMS = currentMS;
    }
    ui->addBlinkButton->setText(tr("Add Blink #%1").arg(ui->blinkList->count() + 1));
}

void TinyMTSeedFinder::restartBlinks()
{
    lastMS = 0;
    ui->addBlinkButton->setText(tr("Start Recording Blinks"));
    ui->restartBlinksButton->setEnabled(false);
    ui->blinkList->clear();
}

void TinyMTSeedFinder::updateSearch(const std::vector<u32> &seeds, const std::vector<u32> &advances)
{
    for (u32 i = 0; i < seeds.size(); i++)
    {
        QString seed = QString::number(seeds[i], 16).toUpper();
        QString advance = QString::number(advances[i]);
        QListWidgetItem* widgetItem = new QListWidgetItem(ui->resultList);
        widgetItem->setFlags(widgetItem->flags() | Qt::ItemIsEditable);
        widgetItem->setText(tr("%1 | %2").arg(seed, advance));
        QScrollBar* scrollbar = ui->resultList->verticalScrollBar();
        if (scrollbar == NULL || scrollbar->value() == scrollbar->maximum()) {
            ui->resultList->scrollToBottom();
            ui->resultList->addItem(widgetItem);
        }
        else
        {
            ui->resultList->addItem(widgetItem);
        }
    }
}

void TinyMTSeedFinder::updateProgress(int progress)
{
    ui->progressBar->setValue(progress);
}

void TinyMTSeedFinder::startSearch()
{
    ui->resultList->clear();
    std::vector<u32> blinks;
    for (int row = 0; row < ui->blinkList->count(); row++)
    {
        blinks.emplace_back(ui->blinkList->item(row)->text().toInt());
    }

    ui->startSearchButton->setEnabled(false);
    ui->cancelSearchButton->setEnabled(true);

    auto *searcher = new TinyMTSeedSearcher(ui->minSeedTextBox->getUInt(), ui->maxSeedTextBox->getUInt(), ui->minAdvanceTextBox->getUInt(), ui->maxAdvanceTextBox->getUInt(), blinks);
    QSettings setting;
    int threads = setting.value("settings/threads", QThread::idealThreadCount()).toInt();

    QThread *thread;
    ui->progressBar->setRange(0, (ui->maxSeedTextBox->getUInt() - ui->minSeedTextBox->getUInt()) >> 1);
    thread = QThread::create([=] { searcher->startSearch(threads); });
    connect(ui->cancelSearchButton, &QPushButton::clicked, [=] { searcher->cancelSearch(); });
    connect(thread, &QThread::finished, thread, &QThread::deleteLater);

    auto *timer = new QTimer();
    connect(timer, &QTimer::timeout, this, [=] { updateProgress(searcher->getProgress()); });
    connect(thread, &QThread::finished, timer, &QTimer::stop);
    connect(thread, &QThread::finished, timer, &QTimer::deleteLater);
    connect(timer, &QTimer::destroyed, this, [=] {
        ui->startSearchButton->setEnabled(true);
        ui->cancelSearchButton->setEnabled(false);
        updateProgress(searcher->getProgress());
        updateSearch(searcher->getResults(), searcher->getAdvanceResults());
        delete searcher;
    });

    thread->start();
    timer->start(1000);
}
