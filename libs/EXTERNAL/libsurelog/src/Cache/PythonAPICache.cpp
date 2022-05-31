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
 * File:   PythonAPICache.cpp
 * Author: alain
 *
 * Created on May 28, 2017, 10:49 PM
 */

#include <Surelog/API/PythonAPI.h>
#include <Surelog/Cache/Cache.h>
#include <Surelog/Cache/PythonAPICache.h>
#include <Surelog/Cache/python_api_generated.h>
#include <Surelog/CommandLine/CommandLineParser.h>
#include <Surelog/ErrorReporting/ErrorContainer.h>
#include <Surelog/Library/Library.h>
#include <Surelog/SourceCompile/CompilationUnit.h>
#include <Surelog/SourceCompile/CompileSourceFile.h>
#include <Surelog/SourceCompile/Compiler.h>
#include <Surelog/SourceCompile/ParseFile.h>
#include <Surelog/SourceCompile/PreprocessFile.h>
#include <Surelog/SourceCompile/PythonListen.h>
#include <Surelog/SourceCompile/SymbolTable.h>
#include <Surelog/Utils/FileUtils.h>
#include <Surelog/Utils/StringUtils.h>
#include <antlr4-runtime.h>
#include <flatbuffers/util.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <cstdio>
#include <ctime>
#include <filesystem>

namespace SURELOG {
namespace fs = std::filesystem;

static std::string FlbSchemaVersion = "1.0";

PythonAPICache::PythonAPICache(PythonListen* listener) : m_listener(listener) {}

std::filesystem::path PythonAPICache::getCacheFileName_(
    const std::filesystem::path& svFileName) const {
  std::filesystem::path svFileNamePriv = svFileName;
  SymbolId cacheDirId =
      m_listener->getCompileSourceFile()->getCommandLineParser()->getCacheDir();
  fs::path cacheDirName = m_listener->getParseFile()->getSymbol(cacheDirId);
  if (svFileNamePriv.empty())
    svFileNamePriv = m_listener->getParseFile()->getFileName(LINE1);
  svFileNamePriv = FileUtils::basename(svFileNamePriv);
  Library* lib = m_listener->getCompileSourceFile()->getLibrary();
  std::string libName = lib->getName();
  fs::path cacheFileName =
      cacheDirName / libName / (svFileNamePriv.string() + ".slpy");
  return cacheFileName;
}

bool PythonAPICache::restore_(const std::filesystem::path& cacheFileName) {
  auto buffer = openFlatBuffers(cacheFileName);
  if (buffer == nullptr) return false;

  const PYTHONAPICACHE::PythonAPICache* ppcache =
      PYTHONAPICACHE::GetPythonAPICache(buffer.get());
  SymbolTable canonicalSymbols;
  restoreErrors(ppcache->m_errors(), ppcache->m_symbols(), &canonicalSymbols,
                m_listener->getCompileSourceFile()->getErrorContainer(),
                m_listener->getCompileSourceFile()->getSymbolTable());

  return true;
}

bool PythonAPICache::checkCacheIsValid_(
    const std::filesystem::path& cacheFileName) {
  auto buffer = openFlatBuffers(cacheFileName);
  if (buffer == nullptr) return false;
  if (!PYTHONAPICACHE::PythonAPICacheBufferHasIdentifier(buffer.get())) {
    return false;
  }
  const PYTHONAPICACHE::PythonAPICache* ppcache =
      PYTHONAPICACHE::GetPythonAPICache(buffer.get());
  auto header = ppcache->m_header();

  auto scriptFile = ppcache->m_python_script_file()->c_str();
  if (scriptFile) {
    time_t ct = get_mtime(cacheFileName.c_str());
    time_t ft = get_mtime(scriptFile);
    if (ft == -1) {
      return false;
    }
    if (ct == -1) {
      return false;
    }
    if (ct < ft) {
      return false;
    }
  }

  if (!checkIfCacheIsValid(header, FlbSchemaVersion, cacheFileName)) {
    return false;
  }

  return true;
}

bool PythonAPICache::isValid() {
  return checkCacheIsValid_(getCacheFileName_());
}

bool PythonAPICache::restore() {
  bool cacheAllowed = m_listener->getCompileSourceFile()
                          ->getCommandLineParser()
                          ->cacheAllowed();
  if (!cacheAllowed) return false;

  fs::path cacheFileName = getCacheFileName_();
  if (!checkCacheIsValid_(cacheFileName)) {
    return false;
  }

  return restore_(cacheFileName);
}

bool PythonAPICache::save() {
  bool cacheAllowed = m_listener->getCompileSourceFile()
                          ->getCommandLineParser()
                          ->cacheAllowed();
  if (!cacheAllowed) return false;
  fs::path svFileName = m_listener->getParseFile()->getPpFileName();
  fs::path origFileName = svFileName;

  fs::path cacheFileName = getCacheFileName_();

  flatbuffers::FlatBufferBuilder builder(1024);
  /* Create header section */
  auto header = createHeader(builder, FlbSchemaVersion, origFileName.string());

  std::string pythonScriptFile = PythonAPI::getListenerScript();
  auto scriptFile = builder.CreateString(pythonScriptFile);

  /* Cache the errors and canonical symbols */
  ErrorContainer* errorContainer =
      m_listener->getCompileSourceFile()->getErrorContainer();
  SymbolId subjectFileId = m_listener->getParseFile()->getFileId(LINE1);
  SymbolTable canonicalSymbols;
  auto errorCache = cacheErrors(
      builder, &canonicalSymbols, errorContainer,
      *m_listener->getCompileSourceFile()->getSymbolTable(), subjectFileId);

  auto symbolVec = createSymbolCache(builder, canonicalSymbols);
  ;
  /* Create Flatbuffers */
  auto ppcache = PYTHONAPICACHE::CreatePythonAPICache(
      builder, header, scriptFile, errorCache, symbolVec);
  FinishPythonAPICacheBuffer(builder, ppcache);

  /* Save Flatbuffer */
  bool status = saveFlatbuffers(builder, cacheFileName.string());

  return status;
}
}  // namespace SURELOG
