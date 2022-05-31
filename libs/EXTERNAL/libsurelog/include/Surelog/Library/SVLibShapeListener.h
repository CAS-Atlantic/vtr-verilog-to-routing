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
 * File:   SVLibShapeListener.h
 * Author: alain
 *
 * Created on January 28, 2018, 10:17 PM
 */

#ifndef SURELOG_SVLIBSHAPELISTENER_H
#define SURELOG_SVLIBSHAPELISTENER_H
#pragma once

#include <Surelog/SourceCompile/SV3_1aTreeShapeHelper.h>
#include <parser/SV3_1aParserBaseListener.h>

#include <filesystem>

namespace SURELOG {

class Config;
class ParseLibraryDef;

class SVLibShapeListener : public SV3_1aParserBaseListener,
                           public SV3_1aTreeShapeHelper {
 public:
  SVLibShapeListener(ParseLibraryDef* parser, antlr4::CommonTokenStream* tokens,
                     const std::filesystem::path& relativePath);

  SymbolId registerSymbol(std::string_view symbol) final;

  antlr4::CommonTokenStream* getTokenStream() const { return m_tokens; }
  ~SVLibShapeListener() override;

  // *** LIBRARY DEFINITION PARSING ***

  // DO NOT OVERRIDE: virtual void
  // enterTop_level_library_rule(SV3_1aParser::Top_level_library_ruleContext *
  // /*ctx*/) override;
  void enterTop_level_library_rule(
      SV3_1aParser::Top_level_library_ruleContext* /*ctx*/) final {}

  void exitTop_level_library_rule(
      SV3_1aParser::Top_level_library_ruleContext* ctx) final {
    addVObject(ctx, VObjectType::slTop_level_library_rule);
  }

  void enterNull_rule(SV3_1aParser::Null_ruleContext* /*ctx*/) final {}

  void exitNull_rule(SV3_1aParser::Null_ruleContext* ctx) final {
    addVObject(ctx, VObjectType::slNull_rule);
  }

  void enterLibrary_text(SV3_1aParser::Library_textContext* /*ctx*/) final {}

  void exitLibrary_text(SV3_1aParser::Library_textContext* ctx) final {
    addVObject(ctx, VObjectType::slLibrary_text);
  }

  void enterLibrary_descriptions(
      SV3_1aParser::Library_descriptionsContext* /*ctx*/) final {}

  void exitLibrary_descriptions(
      SV3_1aParser::Library_descriptionsContext* ctx) final {
    addVObject(ctx, VObjectType::slLibrary_descriptions);
  }

  void enterLibrary_declaration(
      SV3_1aParser::Library_declarationContext* /*ctx*/) final;

  void exitLibrary_declaration(
      SV3_1aParser::Library_declarationContext* ctx) final {
    addVObject(ctx, VObjectType::slLibrary_declaration);
  }

  void enterInclude_statement(
      SV3_1aParser::Include_statementContext* /*ctx*/) final;

  void exitInclude_statement(
      SV3_1aParser::Include_statementContext* ctx) final {
    addVObject(ctx, VObjectType::slInclude_statement);
  }

  // *** CONFIG DEFINITION PARSING ***

  void enterUselib_directive(
      SV3_1aParser::Uselib_directiveContext* /*ctx*/) final;

  void enterConfig_declaration(
      SV3_1aParser::Config_declarationContext* /*ctx*/) final;

  void enterDesign_statement(
      SV3_1aParser::Design_statementContext* /*ctx*/) final;

  void enterConfig_rule_statement(
      SV3_1aParser::Config_rule_statementContext* /*ctx*/) final;

  void enterDefault_clause(SV3_1aParser::Default_clauseContext* /*ctx*/) final;

  void enterInst_clause(SV3_1aParser::Inst_clauseContext* /*ctx*/) final;

  void enterInst_name(SV3_1aParser::Inst_nameContext* /*ctx*/) final;

  void enterCell_clause(SV3_1aParser::Cell_clauseContext* /*ctx*/) final;

  void enterLiblist_clause(SV3_1aParser::Liblist_clauseContext* /*ctx*/) final;

  void enterUse_clause(SV3_1aParser::Use_clauseContext* /*ctx*/) final;

  void exitConfig_declaration(
      SV3_1aParser::Config_declarationContext* ctx) final {
    addVObject(ctx, VObjectType::slConfig_declaration);
  }

  void exitDesign_statement(SV3_1aParser::Design_statementContext* ctx) final {
    addVObject(ctx, VObjectType::slDesign_statement);
  }

  void exitConfig_rule_statement(
      SV3_1aParser::Config_rule_statementContext* ctx) final {
    addVObject(ctx, VObjectType::slConfig_rule_statement);
  }

  void exitDefault_clause(SV3_1aParser::Default_clauseContext* ctx) final {
    addVObject(ctx, VObjectType::slDefault_clause);
  }

  void exitInst_clause(SV3_1aParser::Inst_clauseContext* ctx) final {
    addVObject(ctx, VObjectType::slInst_clause);
  }

  void exitInst_name(SV3_1aParser::Inst_nameContext* ctx) final {
    addVObject(ctx, VObjectType::slInst_name);
  }

  void exitCell_clause(SV3_1aParser::Cell_clauseContext* ctx) final {
    addVObject(ctx, VObjectType::slCell_clause);
  }

  void exitLiblist_clause(SV3_1aParser::Liblist_clauseContext* ctx) final {
    addVObject(ctx, VObjectType::slLiblist_clause);
  }

  void exitUse_clause_config(
      SV3_1aParser::Use_clause_configContext* ctx) final {
    addVObject(ctx, VObjectType::slUse_clause_config);
  }

  void exitUse_clause(SV3_1aParser::Use_clauseContext* ctx) final {
    addVObject(ctx, VObjectType::slUse_clause);
  }

  void exitString_value(SV3_1aParser::String_valueContext* ctx) final;

  void exitIdentifier(SV3_1aParser::IdentifierContext* ctx) final;

  void exitHierarchical_identifier(
      SV3_1aParser::Hierarchical_identifierContext* ctx) final;

 private:
  ParseLibraryDef* m_parser;
  antlr4::CommonTokenStream* m_tokens;
  const std::filesystem::path m_relativePath;
};

};  // namespace SURELOG

#endif /* SURELOG_SVLIBSHAPELISTENER_H */
