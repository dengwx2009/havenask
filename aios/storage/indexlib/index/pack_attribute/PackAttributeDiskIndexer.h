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
#pragma once

#include "autil/Log.h"
#include "indexlib/index/attribute/MultiValueAttributeDiskIndexer.h"

namespace indexlibv2::index {

class PackAttributeDiskIndexer : public MultiValueAttributeDiskIndexer<char>
{
public:
    PackAttributeDiskIndexer(std::shared_ptr<AttributeMetrics> attributeMetrics,
                             const IndexerParameter& indexerParameter);
    ~PackAttributeDiskIndexer();

public:
    // Status Open(const std::shared_ptr<config::IIndexConfig>& indexConfig,
    //             const std::shared_ptr<indexlib::file_system::IDirectory>& indexDirectory) override;

private:
    std::shared_ptr<PackAttributeConfig> _packAttrConfig;

private:
    AUTIL_LOG_DECLARE();
};

} // namespace indexlibv2::index
