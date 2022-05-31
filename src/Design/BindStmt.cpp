/*
 Copyright 2021 Alain Dargelas

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
 * File:   BindStmt.cpp
 * Author: alain
 *
 * Created on May 19, 2019, 11:55 AM
 */

#include <Surelog/Design/BindStmt.h>

namespace SURELOG {

BindStmt::BindStmt(const FileContent* fC, NodeId stmtId, NodeId targetModId,
                   NodeId targetInstId, NodeId bindId, NodeId instanceId)
    : m_fC(fC),
      m_stmtId(stmtId),
      m_targetModId(targetModId),
      m_targetInstId(targetInstId),
      m_bindId(bindId),
      m_instanceId(instanceId) {}
}  // namespace SURELOG
