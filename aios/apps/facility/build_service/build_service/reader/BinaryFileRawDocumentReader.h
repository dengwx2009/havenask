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
#ifndef ISEARCH_BS_BINARYFILERAWDOCUMENTREADER_H
#define ISEARCH_BS_BINARYFILERAWDOCUMENTREADER_H

#include "build_service/common_define.h"
#include "build_service/reader/FileRawDocumentReader.h"
#include "build_service/util/Log.h"

namespace build_service { namespace reader {

class BinaryFileRawDocumentReader : public FileRawDocumentReader
{
public:
    BinaryFileRawDocumentReader(bool isFixLen);
    ~BinaryFileRawDocumentReader();

private:
    BinaryFileRawDocumentReader(const BinaryFileRawDocumentReader&);
    BinaryFileRawDocumentReader& operator=(const BinaryFileRawDocumentReader&);

protected:
    MultiFileDocumentReader*
    createMultiFileDocumentReader(const CollectResults& collectResult, const KeyValueMap& kvMap,
                                  const indexlib::util::MetricProviderPtr& metricProvider) override;

private:
    bool _isFixLen;

private:
    BS_LOG_DECLARE();
};

BS_TYPEDEF_PTR(BinaryFileRawDocumentReader);

}} // namespace build_service::reader

#endif // ISEARCH_BS_BINARYFILERAWDOCUMENTREADER_H
