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
 * File:   Scope.cpp
 * Author: alain
 *
 * Created on August 31, 2019, 11:24 AM
 */

#include <Surelog/Design/Scope.h>
#include <Surelog/Testbench/Variable.h>

namespace SURELOG {

void Scope::addVariable(Variable* var) {
  m_variables.insert(std::make_pair(var->getName(), var));
}

Variable* Scope::getVariable(const std::string& name) {
  VariableMap::iterator itr = m_variables.find(name);
  if (itr == m_variables.end()) {
    if (m_parentScope) {
      Variable* var = m_parentScope->getVariable(name);
      if (var) return var;
    }
    return nullptr;
  } else {
    return (*itr).second;
  }
}

DataType* Scope::getUsedDataType(const std::string& name) {
  DataTypeMap::iterator itr = m_usedDataTypes.find(name);
  if (itr == m_usedDataTypes.end()) {
    return nullptr;
  } else {
    return (*itr).second;
  }
}

}  // namespace SURELOG
