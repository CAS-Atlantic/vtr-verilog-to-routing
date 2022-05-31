/*
 Copyright 2020 Alain Dargelas

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
 * File:   NumUtils.h
 * Author: alain
 *
 * Created on Dec 29, 2020, 10:43 PM
 */

#ifndef SURELOG_NUMUTILS_H
#define SURELOG_NUMUTILS_H
#pragma once

#include <cstdint>
#include <string>

namespace SURELOG {

class NumUtils final {
 public:
  static std::string hexToBin(const std::string &s);

  static std::string binToHex(const std::string &s);

  static std::string toBinary(int size, uint64_t val);

  static std::string trimLeadingZeros(const std::string &s);

  static uint64_t getMask(uint64_t wide);

 private:
  NumUtils() = delete;
  NumUtils(const NumUtils &orig) = delete;
  ~NumUtils() = delete;

 private:
};

};  // namespace SURELOG

#endif /* SURELOG_NUMUTILS_H */
