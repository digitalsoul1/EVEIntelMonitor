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

#include <QAbstractListModel>
#include <QMap>
#include <QMutex>
#include "Intel/IntelChannel.h"

namespace EVEIntelMonitor {
    class IntelChannelsListView : public QAbstractListModel {
    Q_OBJECT

    public:
        explicit IntelChannelsListView(QObject *parent = nullptr);

        [[nodiscard]] int rowCount(const QModelIndex &parent) const override;

        [[nodiscard]] QVariant data(const QModelIndex &index, int role) const override;

        [[nodiscard]] QHash<int, QByteArray> roleNames() const override;

        void initIntelChannels(QVector<Intel::IntelChannel> *intelChannels);

        void sort(int column, Qt::SortOrder order) override;

        // destructor
        ~IntelChannelsListView() override;
    public slots:
        void addIntelChannel(const QString& channelName);

    private:
        struct IntelChannel {
            QString channelName;
            bool enabled;
        };

        QList<IntelChannel> *m_qIntelChannelsList;

        QMutex m_qmIntelChannelsMutex;
    signals:
        void toggleChannel(QString channelName, bool enabled);
        void listIsEditable();
        void listIsNotEditable();
    };

}
