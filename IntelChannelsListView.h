//
// Created by Lukasz Klepacki on 12/11/2023.
//

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

} // EVEIntelMonitor
