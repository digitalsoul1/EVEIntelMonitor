#include "IntelTableView.h"

#include <utility>
#include "RoutePlanner/RoutePlanner.h"

namespace EVEIntelMonitor {
    IntelTableView::IntelTableView(QObject *parent)
            : QAbstractTableModel{parent} {

    }

    int IntelTableView::rowCount(const QModelIndex &parent) const {
        Q_UNUSED(parent)
        return static_cast<int>(m_qIntelList.size());
    }

    int IntelTableView::columnCount(const QModelIndex &parent) const {
        Q_UNUSED(parent)
        return INTELTABLEVIEW_COLUMNS;
    }

    QVariant IntelTableView::data(const QModelIndex &index, int role) const {
        if (index.isValid() && role == Qt::DisplayRole) {
            auto row = m_qIntelList[index.row()];
            QVariant column = (*row)[index.column()];

            return column;
        }

        return {};
    }

    QVariant IntelTableView::headerData(int section, Qt::Orientation orientation, int role) const  {
        if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
            switch (section) {
                case 0:
                    return QString("CHANNEL NAME");
                case 1:
                    return QString("INTEL TIME");
                case 2:
                    return QString("CHARACTER NAME");
                case 3:
                    return QString("SYSTEM");
                case 4:
                    return QString("JUMPS");
                case 5:
                    return QString("ORIGINAL MESSAGE");
                default:
                    return {};
            }
        }
        // Silence compiler warnings
        return {};
    }

    void IntelTableView::newIntel(QString channelName, QDateTime intelTime, QString solarSystemName, short jumpsOut,
                                  QString originalMessage, QString characterName) {
        jumpsOut = static_cast<short>(RoutePlanner::getInstance()->getJumpsBetween(m_qsCurrentLocation.toStrongRef()->toStdString(), solarSystemName.toStdString()).size());
        for (auto intel: m_qIntelList) {
            if (intel->getChannelName() == channelName &&
                intel->getIntelTime() == intelTime &&
                intel->getSolarSystemName() == solarSystemName &&
                intel->getJumpsOut() == jumpsOut &&
                intel->getOriginalMessage() == originalMessage &&
                intel->getCharacterName() == characterName) {
                return;
            }
        }

        m_qmIntelListMutex.lock();
        beginInsertRows(QModelIndex(), static_cast<int>(m_qIntelList.size()), static_cast<int>(m_qIntelList.size()));
        auto *newIntel = new Intel::Intel(std::move(channelName), std::move(intelTime),
                                                  std::move(characterName), std::move(solarSystemName), jumpsOut,
                                                  std::move(originalMessage), this);
        emit hostilesJumpsAway(newIntel->getJumpsOut());
        m_qIntelList.prepend(newIntel);
        endInsertRows();
        m_qmIntelListMutex.unlock();
    }

    QHash<int, QByteArray> IntelTableView::roleNames() const
    {
        return { { Qt::DisplayRole, "display" } };
    }

    void IntelTableView::clear() {
        beginResetModel();
        m_qIntelList.clear();
        endResetModel();
        qInfo() << "Intel table cleared.";
    }

    void IntelTableView::setLocation(const QWeakPointer<QString> &location) {
        m_qmIntelListMutex.lock();
        m_qsCurrentLocation = location;
        m_qmIntelListMutex.unlock();
    }
}


