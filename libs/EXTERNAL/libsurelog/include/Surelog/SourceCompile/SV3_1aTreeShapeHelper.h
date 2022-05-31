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
 * File:   SV3_1aTreeShapeHelper.h
 * Author: alain
 *
 * Created on June 25, 2017, 2:51 PM
 */

#ifndef SURELOG_SV3_1ATREESHAPEHELPER_H
#define SURELOG_SV3_1ATREESHAPEHELPER_H
#pragma once

#include <stack>
#include <string_view>

#include <Surelog/Design/DesignElement.h>
#include <Surelog/ErrorReporting/ErrorDefinition.h>
#include <Surelog/ErrorReporting/Location.h>
#include <Surelog/SourceCompile/CommonListenerHelper.h>
#include <Surelog/SourceCompile/IncludeFileInfo.h>
#include <parser/SV3_1aParser.h>

namespace antlr4 {
class CommonTokenStream;
}

namespace SURELOG {

#define SV_MAX_IDENTIFIER_SIZE 1024
#define SV_MAX_STRING_SIZE (4 * 1024 * 1024)

class ParseFile;
class ParseLibraryDef;

class SV3_1aTreeShapeHelper : public CommonListenerHelper {
 public:
  SV3_1aTreeShapeHelper(ParseFile* pf, antlr4::CommonTokenStream* tokens,
                        unsigned int lineOffset);
  SV3_1aTreeShapeHelper(ParseLibraryDef* pf, antlr4::CommonTokenStream* tokens);

  ~SV3_1aTreeShapeHelper() override;

  void logError(ErrorDefinition::ErrorType error,
                antlr4::ParserRuleContext* ctx, std::string_view object,
                bool printColumn = false);

  void logError(ErrorDefinition::ErrorType, Location& loc,
                bool showDuplicates = false);

  void logError(ErrorDefinition::ErrorType, Location& loc, Location& extraLoc,
                bool showDuplicates = false);

  NodeId generateDesignElemId();

  NodeId generateNodeId();

  SymbolId registerSymbol(std::string_view symbol) override;

  void addNestedDesignElement(antlr4::ParserRuleContext* ctx,
                              std::string_view name,
                              DesignElement::ElemType elemtype,
                              VObjectType objtype);

  void addDesignElement(antlr4::ParserRuleContext* ctx, std::string_view name,
                        DesignElement::ElemType elemtype, VObjectType objtype);

  std::pair<double, TimeInfo::Unit> getTimeValue(
      SV3_1aParser::Time_literalContext* ctx);

  std::tuple<unsigned int, unsigned short, unsigned int, unsigned short>
  getFileLine(antlr4::ParserRuleContext* ctx, SymbolId& fileId) override;

 protected:
  ParseFile* m_pf;
  DesignElement* m_currentElement;
  std::stack<DesignElement*> m_nestedElements;
  unsigned int m_lineOffset;
  bool m_ppOutputFileLocation;
  std::stack<IncludeFileInfo> m_includeFileInfo;
};

};  // namespace SURELOG

#endif /* SURELOG_SV3_1ATREESHAPEHELPER_H */
