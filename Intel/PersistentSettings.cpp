/*
* Copyright (c) 2023 github.com/digitalsoul1
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