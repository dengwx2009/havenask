/*
 * Copyright 2014-present Alibaba Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef __INDEXLIB_SECTION_ATTRIBUTE_CONFIG_H
#define __INDEXLIB_SECTION_ATTRIBUTE_CONFIG_H

#include <memory>

#include "indexlib/common_define.h"
#include "indexlib/config/attribute_config.h"
#include "indexlib/config/config_define.h"
#include "indexlib/index/inverted_index/config/SectionAttributeConfig.h"
#include "indexlib/indexlib.h"

namespace indexlib { namespace config {

class SectionAttributeConfig : public indexlibv2::config::SectionAttributeConfig
{
public:
    SectionAttributeConfig();
    // for test only
    SectionAttributeConfig(const std::string& compressString, bool hasSectionWeight, bool hasFieldId);
    ~SectionAttributeConfig();

public:
    void AssertEqual(const SectionAttributeConfig& other) const;
    bool operator==(const SectionAttributeConfig& other) const;
    bool operator!=(const SectionAttributeConfig& other) const;

    AttributeConfigPtr CreateAttributeConfig(const std::string& indexName) const;

private:
    IE_LOG_DECLARE();
};

DEFINE_SHARED_PTR(SectionAttributeConfig);
}} // namespace indexlib::config

#endif //__INDEXLIB_SECTION_ATTRIBUTE_CONFIG_H
