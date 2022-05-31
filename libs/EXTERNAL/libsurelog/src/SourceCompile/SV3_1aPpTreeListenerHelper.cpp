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
 * File:   SV3_1aPpTreeListenerHelper.cpp
 * Author: alain
 *
 * Created on December 4, 2019, 8:17 PM
 */

#include <Surelog/CommandLine/CommandLineParser.h>
#include <Surelog/ErrorReporting/ErrorContainer.h>
#include <Surelog/SourceCompile/CompileSourceFile.h>
#include <Surelog/SourceCompile/MacroInfo.h>
#include <Surelog/SourceCompile/SV3_1aPpTreeListenerHelper.h>
#include <Surelog/SourceCompile/SymbolTable.h>
#include <Surelog/Utils/ParseUtils.h>

namespace SURELOG {

SymbolTable* SV3_1aPpTreeListenerHelper::getSymbolTable() const {
  return m_pp->getCompileSourceFile()->getSymbolTable();
}

SymbolId SV3_1aPpTreeListenerHelper::registerSymbol(std::string_view symbol) {
  return m_pp->getCompileSourceFile()->getSymbolTable()->registerSymbol(symbol);
}

std::tuple<unsigned int, unsigned short, unsigned int, unsigned short>
SV3_1aPpTreeListenerHelper::getFileLine(antlr4::ParserRuleContext* ctx,
                                        SymbolId& fileId) {
  std::pair<int, int> lineCol = ParseUtils::getLineColumn(m_tokens, ctx);
  std::pair<int, int> endLineCol = ParseUtils::getEndLineColumn(m_tokens, ctx);
  unsigned int line = 0;
  unsigned short column = lineCol.second;
  unsigned int endLine = 0;
  unsigned short endColumn = endLineCol.second;
  fileId = m_pp->getFileId(lineCol.first);
  line = m_pp->getLineNb(lineCol.first);
  endLine = m_pp->getLineNb(endLineCol.first);
  return std::make_tuple(line, column, endLine, endColumn);
}

void SV3_1aPpTreeListenerHelper::init() {
  static constexpr std::string_view kReservedMacros[] = {
      "define",
      "celldefine",
      "endcelldefine",
      "default_nettype",
      "undef",
      "ifdef",
      "ifndef",
      "else",
      "elsif",
      "endif",
      "include",
      "pragma",
      "begin_keywords",
      "end_keywords",
      "resetall",
      "timescale",
      "unconnected_drive",
      "nounconnected_drive",
      "line",
      "default_decay_time",
      "default_trireg_strength",
      "delay_mode_distributed",
      "delay_mode_path",
      "delay_mode_unit",
      "delay_mode_zero",
      "undefineall",
      "accelerate",
      "noaccelerate",
      "protect",
      "uselib",
      "disable_portfaults",
      "enable_portfaults",
      "nosuppress_faults",
      "suppress_faults",
      "signed",
      "unsigned",
      "endprotect",
      "protected",
      "endprotected",
      "expand_vectornets",
      "noexpand_vectornets",
      "autoexpand_vectornets",
      "remove_gatename",
      "noremove_gatenames",
      "remove_netname",
      "noremove_netnames"};

  for (const std::string_view reserved_macro : kReservedMacros) {
    m_reservedMacroNamesSet.insert(reserved_macro);
    getSymbolTable()->registerSymbol(reserved_macro);
  }
}

void SV3_1aPpTreeListenerHelper::logError(ErrorDefinition::ErrorType error,
                                          antlr4::ParserRuleContext* ctx,
                                          std::string_view object,
                                          bool printColumn) {
  if (m_instructions.m_mute) return;
  std::pair<int, int> lineCol =
      ParseUtils::getLineColumn(m_pp->getTokenStream(), ctx);
  if (m_pp->getMacroInfo()) {
    Location loc(m_pp->getMacroInfo()->m_file,
                 m_pp->getMacroInfo()->m_startLine + lineCol.first - 1,
                 lineCol.second, getSymbolTable()->registerSymbol(object));
    Location extraLoc(m_pp->getIncluderFileId(m_pp->getIncluderLine()),
                      m_pp->getIncluderLine(), 0, 0);
    Error err(error, loc, extraLoc);
    m_pp->addError(err);
  } else {
    Location loc(m_pp->getFileId(lineCol.first), m_pp->getLineNb(lineCol.first),
                 printColumn ? lineCol.second : 0,
                 getSymbolTable()->registerSymbol(object));
    Error err(error, loc);
    m_pp->addError(err);
  }
}

void SV3_1aPpTreeListenerHelper::logError(ErrorDefinition::ErrorType error,
                                          Location& loc, bool showDuplicates) {
  if (m_instructions.m_mute) return;
  Error err(error, loc);
  m_pp->getCompileSourceFile()->getErrorContainer()->addError(err,
                                                              showDuplicates);
}

void SV3_1aPpTreeListenerHelper::logError(ErrorDefinition::ErrorType error,
                                          Location& loc, Location& extraLoc,
                                          bool showDuplicates) {
  if (m_instructions.m_mute) return;
  std::vector<Location> extras;
  extras.push_back(extraLoc);
  Error err(error, loc, &extras);
  m_pp->getCompileSourceFile()->getErrorContainer()->addError(err,
                                                              showDuplicates);
}

void SV3_1aPpTreeListenerHelper::forwardToParser(
    antlr4::ParserRuleContext* ctx) {
  if (m_inActiveBranch && (!m_inMacroDefinitionParsing) &&
      (!m_pp->getCompileSourceFile()
            ->getCommandLineParser()
            ->filterSimpleDirectives()) &&
      (!(m_filterProtectedRegions && m_inProtectedRegion))) {
    // m_pp->append(ctx->getText() + "\n");
    m_pp->append(ctx->getText());
  } else {
    addLineFiller(ctx);
  }
}

void SV3_1aPpTreeListenerHelper::addLineFiller(antlr4::ParserRuleContext* ctx) {
  if (m_pp->isMacroBody()) return;
  for (char c : ctx->getText()) {
    if (c == '\n') m_pp->append("\n");
  }
}

void SV3_1aPpTreeListenerHelper::checkMultiplyDefinedMacro(
    const std::string& macroName, antlr4::ParserRuleContext* ctx) {
  std::set<PreprocessFile*> visited;
  MacroInfo* macroInf = m_pp->getMacro(macroName);
  if (macroInf) {
    std::pair<int, int> lineCol =
        ParseUtils::getLineColumn(m_pp->getTokenStream(), ctx);
    if ((macroInf->m_file == m_pp->getFileId(lineCol.first)) &&
        (m_pp->getLineNb(lineCol.first) == macroInf->m_startLine))
      return;
    Location loc(m_pp->getFileId(lineCol.first), m_pp->getLineNb(lineCol.first),
                 0, getSymbolTable()->getId(macroName));
    Location extraLoc(macroInf->m_file, macroInf->m_startLine,
                      macroInf->m_startColumn, 0);
    logError(ErrorDefinition::PP_MULTIPLY_DEFINED_MACRO, loc, extraLoc);
    visited.erase(visited.begin(), visited.end());
    // So we can store the latest declaration
    m_pp->deleteMacro(macroName, visited);
  }
}

void SV3_1aPpTreeListenerHelper::setCurrentBranchActivity(
    unsigned int currentLine) {
  PreprocessFile::IfElseStack& stack = m_pp->getStack();
  if (!stack.empty()) {
    int index = stack.size() - 1;
    bool checkPrev = true;
    while (checkPrev) {
      PreprocessFile::IfElseItem& tmpitem = stack.at(index);
      switch (tmpitem.m_type) {
        case PreprocessFile::IfElseItem::IFDEF:
        case PreprocessFile::IfElseItem::IFNDEF:
          m_inActiveBranch = tmpitem.m_previousActiveState;
          checkPrev = false;
          break;
        default:
          checkPrev = true;
          index--;
          if (index < 0) checkPrev = false;
      }
    }

    index = stack.size() - 1;
    PreprocessFile::IfElseItem& tmpitem = stack.at(index);
    switch (tmpitem.m_type) {
      case PreprocessFile::IfElseItem::IFDEF: {
        if (!tmpitem.m_defined) {
          m_inActiveBranch = false;
        }
        break;
      }
      case PreprocessFile::IfElseItem::IFNDEF: {
        if (tmpitem.m_defined) {
          m_inActiveBranch = false;
        }
        break;
      }
      case PreprocessFile::IfElseItem::ELSIF:
      case PreprocessFile::IfElseItem::ELSE: {
        if (!tmpitem.m_defined) {
          m_inActiveBranch = false;
        }
        break;
      }
    }
  } else {
    m_inActiveBranch = true;
  }
}

bool SV3_1aPpTreeListenerHelper::isPreviousBranchActive() {
  PreprocessFile::IfElseStack& stack = m_pp->getStack();
  bool previousBranchActive = false;
  bool checkPrev = true;
  if (stack.empty()) {
    return false;
  }
  int index = stack.size() - 1;
  while (checkPrev) {
    checkPrev = false;
    PreprocessFile::IfElseItem& tmpitem = stack.at(index);
    switch (tmpitem.m_type) {
      case PreprocessFile::IfElseItem::IFDEF:
        if (tmpitem.m_defined) {
          previousBranchActive = true;
        }
        checkPrev = false;
        break;
      case PreprocessFile::IfElseItem::ELSIF:
        if (tmpitem.m_defined) {
          previousBranchActive = true;
          checkPrev = false;
        } else {
          checkPrev = true;
        }
        break;
      case PreprocessFile::IfElseItem::IFNDEF:
        if (!tmpitem.m_defined) {
          previousBranchActive = true;
        }
        checkPrev = false;
        break;
      default:
        break;
    }
    if (index > 0) {
      index--;
    } else {
      if (checkPrev) previousBranchActive = false;
      checkPrev = false;
    }
  }
  return previousBranchActive;
}

}  // namespace SURELOG
