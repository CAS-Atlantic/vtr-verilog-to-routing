/*
 Copyright 2019 Alain Dargelas

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

 http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
 */

/*
 * File:   ModuleDefinition.cpp
 * Author: alain
 *
 * Created on October 20, 2017, 10:29 PM
 */

#include <Surelog/Design/FileContent.h>
#include <Surelog/Design/ModPort.h>
#include <Surelog/Design/ModuleDefinition.h>

namespace SURELOG {

VObjectType ModuleDefinition::getType() const {
  return (m_fileContents.size()) ? m_fileContents[0]->Type(m_nodeIds[0])
                                 : VObjectType::slN_input_gate_instance;
}

ModuleDefinition::ModuleDefinition(const FileContent* fileContent,
                                   NodeId nodeId, const std::string_view name)
    : DesignComponent(fileContent, nullptr),
      m_name(name),
      m_gen_block_id(0),
      m_udpDefn(nullptr) {
  if (fileContent) {
    addFileContent(fileContent, nodeId);
  }
}

bool ModuleDefinition::isInstance() const {
  const VObjectType type = getType();
  return ((type == VObjectType::slN_input_gate_instance) ||
          (type == VObjectType::slModule_declaration) ||
          (type == VObjectType::slUdp_declaration));
}

ModuleDefinition::~ModuleDefinition() {}

ModuleDefinition* ModuleDefinitionFactory::newModuleDefinition(
    const FileContent* fileContent, NodeId nodeId, std::string_view name) {
  return new ModuleDefinition(fileContent, nodeId, name);
}

unsigned int ModuleDefinition::getSize() const {
  unsigned int size = 0;
  for (unsigned int i = 0; i < m_fileContents.size(); i++) {
    NodeId end = m_nodeIds[i];
    NodeId begin = m_fileContents[i]->Child(end);
    size += (end - begin);
  }
  return size;
}

void ModuleDefinition::insertModPort(const std::string& modport,
                                     const Signal& signal, NodeId nodeId) {
  ModPortSignalMap::iterator itr = m_modportSignalMap.find(modport);
  if (itr == m_modportSignalMap.end()) {
    ModPort modp(this, modport, m_fileContents[0], nodeId);
    modp.addSignal(signal);
    m_modportSignalMap.insert(std::make_pair(modport, modp));
  } else {
    (*itr).second.addSignal(signal);
  }
}

const Signal* ModuleDefinition::getModPortSignal(const std::string& modport,
                                                 NodeId port) const {
  ModPortSignalMap::const_iterator itr = m_modportSignalMap.find(modport);
  if (itr == m_modportSignalMap.end()) {
    return nullptr;
  } else {
    for (auto& sig : (*itr).second.getPorts()) {
      if (sig.getNodeId() == port) {
        return &sig;
      }
    }
  }
  return nullptr;
}

ModPort* ModuleDefinition::getModPort(const std::string& modport) {
  ModPortSignalMap::iterator itr = m_modportSignalMap.find(modport);
  if (itr == m_modportSignalMap.end()) {
    return nullptr;
  } else {
    return &(*itr).second;
  }
}

void ModuleDefinition::insertModPort(const std::string& modport,
                                     ClockingBlock& cb) {
  ModPortClockingBlockMap::iterator itr =
      m_modportClockingBlockMap.find(modport);
  if (itr == m_modportClockingBlockMap.end()) {
    std::vector<ClockingBlock> cbs;
    cbs.push_back(cb);
    m_modportClockingBlockMap.insert(std::make_pair(modport, cbs));
  } else {
    (*itr).second.push_back(cb);
  }
}

ClockingBlock* ModuleDefinition::getModPortClockingBlock(
    const std::string& modport, NodeId port) {
  ModPortClockingBlockMap::iterator itr =
      m_modportClockingBlockMap.find(modport);
  if (itr == m_modportClockingBlockMap.end()) {
    return nullptr;
  } else {
    for (auto& cb : (*itr).second) {
      if (cb.getNodeId() == port) {
        return &cb;
      }
    }
  }
  return nullptr;
}

ClassDefinition* ModuleDefinition::getClassDefinition(const std::string& name) {
  ClassNameClassDefinitionMultiMap::iterator itr =
      m_classDefinitions.find(name);
  if (itr == m_classDefinitions.end()) {
    return nullptr;
  } else {
    return (*itr).second;
  }
}

}  // namespace SURELOG
