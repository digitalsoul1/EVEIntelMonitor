/*
* Copyright (c) 2024 github.com/digitalsoul1
*
* This program is free software: you can redistribute it and/or modify
        * it under the terms of the GNU General Public License as published by
* the Free Software Foundation, version 3.
*
* This program is distributed in the hope that it will be useful, but
* WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
        * General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*/
#pragma once
#include <QAbstractTableModel>
#include <qmutex.h>
#include "Intel/Intel.h"
#include "RoutePlanner/RoutePlanner.h"

#define INTELTABLEVIEW_COLUMNS 6

namespace EVEIntelMonitor {

class IntelTableView : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit IntelTableView(QObject *parent = nullptr);

    // QAbstractItemModel interface
public:
    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;

    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;


    void clear();
    void setLocation(const QWeakPointer<QString> &location);
public slots:
    void newIntel(QString channelName, QDateTime intelTime, QString solarSystemName, short jumpsOut, QString originalMessage, QString characterName);

    QHash<int, QByteArray> roleNames() const override;

private:
    QList<Intel::Intel*> m_qIntelList;
    QWeakPointer<QString> m_qsCurrentLocation;
    QMutex m_qmIntelListMutex;
    signals:
    void hostilesJumpsAway(unsigned long jumpsAway);
};
}
