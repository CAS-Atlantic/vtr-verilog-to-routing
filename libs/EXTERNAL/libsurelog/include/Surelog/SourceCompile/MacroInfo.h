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
 * File:   MacroInfo.h
 * Author: alain
 *
 * Created on April 5, 2017, 11:46 PM
 */

#ifndef SURELOG_MACROINFO_H
#define SURELOG_MACROINFO_H
#pragma once

#include <Surelog/Common/SymbolId.h>

#include <string>
#include <string_view>
#include <vector>

namespace SURELOG {

class MacroInfo {
 public:
  MacroInfo(std::string_view name, int type, SymbolId file,
            unsigned int startLine, unsigned short int startColumn,
            unsigned int endLine, unsigned short int endColumn,
            const std::vector<std::string>& arguments,
            const std::vector<std::string>& tokens)
      : m_name(name),
        m_type(type),
        m_file(file),
        m_startLine(startLine),
        m_startColumn(startColumn),
        m_endLine(endLine),
        m_endColumn(endColumn),
        m_arguments(arguments),
        m_tokens(tokens) {}
  enum Type {
    NO_ARGS,
    WITH_ARGS,
  };

  const std::string m_name;
  const int m_type;
  const SymbolId m_file;
  const unsigned int m_startLine;
  const unsigned short int m_startColumn;
  const unsigned int m_endLine;
  const unsigned short int m_endColumn;
  const std::vector<std::string> m_arguments;
  const std::vector<std::string> m_tokens;
};

};  // namespace SURELOG

#endif /* SURELOG_MACROINFO_H */
