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
 * File:   NumUtils.cpp
 * Author: alain
 *
 * Created on December 29, 2020, 10:43 PM
 */

#include <Surelog/Utils/NumUtils.h>

#include <algorithm>
#include <bitset>
#include <iostream>
#include <locale>
#include <regex>
#include <sstream>

namespace SURELOG {

std::string NumUtils::hexToBin(const std::string &s) {
  std::string out;
  for (auto i : s) {
    uint8_t n;
    if ((i <= '9') && (i >= '0'))
      n = i - '0';
    else
      n = 10 + i - 'A';
    for (int8_t j = 3; j >= 0; --j) out.push_back((n & (1 << j)) ? '1' : '0');
  }
  out = trimLeadingZeros(out);
  return out;
}

std::string NumUtils::binToHex(const std::string &s) {
  std::string out;
  for (unsigned int i = 0; i < s.size(); i += 4) {
    int8_t n = 0;
    for (unsigned int j = i; j < i + 4; ++j) {
      n <<= 1;
      if (s[j] == '1') n |= 1;
    }

    if (n <= 9)
      out.push_back('0' + n);
    else
      out.push_back('A' + n - 10);
  }
  return out;
}

std::string NumUtils::toBinary(int size, uint64_t val) {
  int constexpr bitFieldSize = 100;
  std::string tmp = std::bitset<bitFieldSize>(val).to_string();
  if (size <= 0) {
    for (unsigned int i = 0; i < bitFieldSize; i++) {
      if (tmp[i] == '1') {
        size = bitFieldSize - i;
        break;
      }
    }
  }
  std::string result;
  for (unsigned int i = bitFieldSize - size; i < bitFieldSize; i++)
    result += tmp[i];
  return result;
}

std::string NumUtils::trimLeadingZeros(const std::string &s) {
  const uint64_t sSize = s.size();
  std::string res;
  bool nonZero = false;
  for (unsigned int i = 0; i < sSize; i++) {
    const char c = s[i];
    if (c != '0') nonZero = true;
    if (nonZero) res += c;
  }
  return res;
}

uint64_t NumUtils::getMask(uint64_t wide) {
  uint64_t mask = 0;
  uint64_t sizeInBits = sizeof(mask) * 8;
  mask = (wide >= sizeInBits)
             ? ((uint64_t)-1)
             : ((uint64_t)((uint64_t)(((uint64_t)1) << ((uint64_t)wide))) -
                (uint64_t)1);
  return mask;
}

}  // namespace SURELOG
