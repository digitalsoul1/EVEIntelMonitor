#include "PersistentSettings.h"
#include <cassert>


namespace EVEIntelMonitor::Intel {
    PersistentSettings::PersistentSettings(QObject *parent) : QObject(parent) {
        this->m_ulCharacterId = 0;
        this->m_pConfigBackend = EVEIntelMonitor::ConfigBackend::getInstance();
    }

    QVector<IntelChannel> *PersistentSettings::loadSettings() {
        assert(this->m_ulCharacterId != 0);
        auto *intelChannels = new QVector<IntelChannel>();
        QStringList channels = this->m_pConfigBackend->getGroups(
                "IntelChannels/" + QString::number(this->m_ulCharacterId));
        for (auto &channel: channels) {
            IntelChannel intelChannel(channel);
            intelChannel.setEnabled(this->m_pConfigBackend->getValue(
                    "IntelChannels/" + QString::number(this->m_ulCharacterId) + "/" + channel + "/Enabled").toBool());
            intelChannel.setEnabledSince(QDateTime::currentDateTime());
            intelChannels->append(intelChannel);
        }
        return intelChannels;
    }

    void PersistentSettings::clearSettings() {
        assert(this->m_ulCharacterId != 0);
        this->m_pConfigBackend->removeGroup("IntelChannels/" + QString::number(this->m_ulCharacterId));
    }

    void PersistentSettings::saveSettings(IntelChannel &intelChannel) {
        // lock mutex
        QMutexLocker locker(&this->m_qmMutexSettings);
        this->m_pConfigBackend->setValue(
                "IntelChannels/" + QString::number(this->m_ulCharacterId) + "/" + intelChannel.getChannelName() +
                "/Enabled", intelChannel.isEnabled());
    }

    bool PersistentSettings::isEnabled(const IntelChannel &intelChannel) {
        // lock mutex
        QMutexLocker locker(&this->m_qmMutexSettings);
        return this->m_pConfigBackend->getValue(
                "IntelChannels/" + QString::number(this->m_ulCharacterId) + "/" + intelChannel.getChannelName() +
                "/Enabled").toBool();
    }

} // Intel
// EVEIntelMonitor