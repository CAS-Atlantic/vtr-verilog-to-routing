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
 * File:   PythonAPICache.h
 * Author: alain
 *
 * Created on May 28, 2017, 10:49 PM
 */

#ifndef SURELOG_PYTHONAPICACHE_H
#define SURELOG_PYTHONAPICACHE_H
#pragma once

#include <Surelog/Cache/Cache.h>

#include <filesystem>

namespace SURELOG {

class PythonListen;

class PythonAPICache final : Cache {
 public:
  PythonAPICache(PythonListen* listener);

  bool restore();
  bool save();
  bool isValid();

 private:
  PythonAPICache(const PythonAPICache& orig) = delete;

  std::filesystem::path getCacheFileName_(
      const std::filesystem::path& fileName = "") const;
  bool restore_(const std::filesystem::path& cacheFileName);
  bool checkCacheIsValid_(const std::filesystem::path& cacheFileName);

  PythonListen* m_listener;
};

};  // namespace SURELOG

#endif /* SURELOG_PYTHONAPICACHE_H */
