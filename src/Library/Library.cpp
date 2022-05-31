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
 * File:   Library.cpp
 * Author: alain
 *
 * Created on January 27, 2018, 5:25 PM
 */

#include <Surelog/Design/ModuleDefinition.h>
#include <Surelog/Library/Library.h>
#include <Surelog/SourceCompile/SymbolTable.h>

namespace SURELOG {

void Library::addModuleDefinition(ModuleDefinition* def) {
  m_modules.insert(std::make_pair(def->getName(), def));
}

ModuleDefinition* Library::getModule(const std::string& name) const {
  std::map<std::string, ModuleDefinition*>::const_iterator itr =
      m_modules.find(name);
  if (itr == m_modules.end()) {
    return nullptr;
  } else {
    return (*itr).second;
  }
}

std::string Library::report(SymbolTable* symbols) const {
  std::string report;
  report = "LIB: " + m_name + "\n";
  for (auto id : m_fileIds) {
    report += "     " + symbols->getSymbol(id) + "\n";
  }
  return report;
}

}  // namespace SURELOG
