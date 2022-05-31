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
 * File:   ParseLibraryDef.h
 * Author: alain
 *
 * Created on January 27, 2018, 5:05 PM
 */

#ifndef SURELOG_PARSELIBRARYDEF_H
#define SURELOG_PARSELIBRARYDEF_H
#pragma once

#include <Surelog/Common/SymbolId.h>

namespace SURELOG {

class CommandLineParser;
class ConfigSet;
class ErrorContainer;
class FileContent;
class Library;
class LibrarySet;
class SymbolTable;

class ParseLibraryDef final {
 public:
  ParseLibraryDef(CommandLineParser* commandLineParser, ErrorContainer* errors,
                  SymbolTable* symbolTable, LibrarySet* librarySet,
                  ConfigSet* configSet);

  bool parseLibrariesDefinition();
  bool parseLibraryDefinition(SymbolId file, Library* lib = nullptr);
  bool parseConfigDefinition();

  SymbolId getFileId() const { return m_fileId; }
  CommandLineParser* getCommandLineParser() const {
    return m_commandLineParser;
  }

  ErrorContainer* getErrorContainer() const { return m_errors; }

  SymbolTable* getSymbolTable() const { return m_symbolTable; }

  LibrarySet* getLibrarySet() const { return m_librarySet; }

  ConfigSet* getConfigSet() const { return m_configSet; }

 private:
  ParseLibraryDef(const ParseLibraryDef& orig) = delete;

  SymbolId m_fileId;
  CommandLineParser* const m_commandLineParser;
  ErrorContainer* const m_errors;
  SymbolTable* const m_symbolTable;
  LibrarySet* const m_librarySet;
  ConfigSet* const m_configSet;
  FileContent* m_fileContent;
};

}  // namespace SURELOG

#endif /* SURELOG_PARSELIBRARYDEF_H */
