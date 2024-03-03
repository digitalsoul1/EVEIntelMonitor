//
// Created by Lukasz Klepacki on 12/11/2023.
//

#include "IntelChannelsListView.h"

namespace EVEIntelMonitor {

    int IntelChannelsListView::rowCount(const QModelIndex &parent) const {
        Q_UNUSED(parent)
        return m_qIntelChannelsList->size();
    }

    QVariant IntelChannelsListView::data(const QModelIndex &index, int role) const {
        // return channel name for display role and enabled flag for check role
        if (role == Qt::DisplayRole) {
            return m_qIntelChannelsList->at(index.row()).channelName;
        }
        if (role == Qt::CheckStateRole) {
            return m_qIntelChannelsList->at(index.row()).enabled ? Qt::Checked : Qt::Unchecked;
        }
        return QVariant();
    }

    QHash<int, QByteArray> IntelChannelsListView::roleNames() const {
        // return channel name for display role and enabled flag for check role
        QHash<int, QByteArray> roles;
        roles[Qt::DisplayRole] = "channelName";
        roles[Qt::CheckStateRole] = "enabled";
        return roles;
    }

    IntelChannelsListView::IntelChannelsListView(QObject *parent) : QAbstractListModel(parent) {
        qInfo() << "IntelChannelsListView::IntelChannelsListView(QObject *parent)";
        m_qIntelChannelsList = new QList<IntelChannel>();
    }

    IntelChannelsListView::~IntelChannelsListView() {
        qInfo() << "IntelChannelsListView::~IntelChannelsListView()";
        delete m_qIntelChannelsList;
    }

    void IntelChannelsListView::addIntelChannel(const QString& channelName) {
        QMutexLocker locker(&m_qmIntelChannelsMutex);
        emit listIsNotEditable();
            beginInsertRows(QModelIndex(), m_qIntelChannelsList->size(), m_qIntelChannelsList->size());
            qInfo() << "Adding new Intel Channel: " << channelName << " to the list of " << m_qIntelChannelsList->size()
                    << " channels.";
            m_qIntelChannelsList->append({channelName, true});
            endInsertRows();
        locker.unlock();
        emit listIsEditable();

    }

    void IntelChannelsListView::initIntelChannels(QVector<Intel::IntelChannel> *intelChannels) {
        QMutexLocker locker(&m_qmIntelChannelsMutex);
        emit listIsNotEditable();
        beginInsertRows(QModelIndex(), m_qIntelChannelsList->size(), m_qIntelChannelsList->size());
        for (auto &channel: *intelChannels) {
            m_qIntelChannelsList->append({channel.getChannelName(), channel.isEnabled()});
        }
        endInsertRows();
        locker.unlock();
        qInfo() << "Intel Channel list initialized with " << m_qIntelChannelsList->size() << " channels";
        emit listIsEditable();
    }

    void IntelChannelsListView::sort(int column, Qt::SortOrder order) {
        qInfo() << "IntelChannelsListView::sort(int column, Qt::SortOrder order): not implemented";
        QAbstractItemModel::sort(column, order);
    }


} // EVEIntelMonitor