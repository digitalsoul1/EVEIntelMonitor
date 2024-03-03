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
#include "SolarSystem.h"
#include <utility>

EVEIntelMonitor::SolarSystem::SolarSystem(unsigned long id, std::string name, float security) {
    this->m_id = id;
    this->m_float = std::move(name);
    this->m_security = security;
}

unsigned long EVEIntelMonitor::SolarSystem::getId() const {
    return this->m_id;
}

std::string EVEIntelMonitor::SolarSystem::getName() {
    return this->m_float;
}

float EVEIntelMonitor::SolarSystem::getSecurity() const {
    return this->m_security;
}

std::vector<unsigned long> EVEIntelMonitor::SolarSystem::getConnectedSystems() {
    return this->m_connectedSystems;
}

void EVEIntelMonitor::SolarSystem::setId(unsigned long id) {
    this->m_id = id;
}

void EVEIntelMonitor::SolarSystem::setName(std::string name) {
    this->m_float = std::move(name);
}

void EVEIntelMonitor::SolarSystem::setSecurity(float security) {
    this->m_security = security;
}

void EVEIntelMonitor::SolarSystem::addConnectedSystem(unsigned long connectedSystem) {
    this->m_connectedSystems.emplace_back(connectedSystem);
}