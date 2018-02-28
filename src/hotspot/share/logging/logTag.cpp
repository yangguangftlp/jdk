/*
 * Copyright (c) 2015, 2018, Oracle and/or its affiliates. All rights reserved.
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * This code is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 only, as
 * published by the Free Software Foundation.
 *
 * This code is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * version 2 for more details (a copy is included in the LICENSE file that
 * accompanied this code).
 *
 * You should have received a copy of the GNU General Public License version
 * 2 along with this work; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 * Please contact Oracle, 500 Oracle Parkway, Redwood Shores, CA 94065 USA
 * or visit www.oracle.com if you need additional information or have any
 * questions.
 *
 */
#include "precompiled.hpp"
#include "logging/logTag.hpp"
#include "utilities/globalDefinitions.hpp"
#include "utilities/ostream.hpp"
#include "utilities/quickSort.hpp"

const char* LogTag::_name[] = {
  "", // __NO_TAG
#define LOG_TAG(name) #name,
  LOG_TAG_LIST
#undef LOG_TAG
};

LogTagType LogTag::from_string(const char* str) {
  for (uint i = 0; i < LogTag::Count; i++) {
    if (strcasecmp(str, _name[i]) == 0) {
      return static_cast<LogTagType>(i);
    }
  }
  return __NO_TAG;
}

static int cmp_logtag(LogTagType a, LogTagType b) {
  return strcmp(LogTag::name(a), LogTag::name(b));
}

static const size_t sorted_tagcount = LogTag::Count - 1; // Not counting _NO_TAG
static LogTagType sorted_tags[sorted_tagcount];

class TagSorter {
 public:
  TagSorter() {
    for (size_t i = 1; i < LogTag::Count; i++) {
      sorted_tags[i - 1] = static_cast<LogTagType>(i);
    }
    QuickSort::sort(sorted_tags, sorted_tagcount, cmp_logtag, true);
  }
};

static TagSorter tagsorter; // Sorts tags during static initialization

void LogTag::list_tags(outputStream* out) {
  for (size_t i = 0; i < sorted_tagcount; i++) {
    out->print("%s %s", (i == 0 ? "" : ","), _name[sorted_tags[i]]);
  }
  out->cr();
}
