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

#include <memory>
#include <stddef.h>
#include <string>

#include "table/ColumnData.h"

namespace table {
class ColumnSchema;
} // namespace table

namespace table {

class Column {
public:
    Column(ColumnSchema *columnSchema, ColumnDataBase *columnData);
    ~Column();

private:
    Column(const Column &);
    Column &operator=(const Column &);

public:
    template <typename T>
    ColumnData<T> *getColumnData() {
        return dynamic_cast<ColumnData<T> *>(_data);
    }

    ColumnSchema *getColumnSchema() const { return _schema; }

    std::string toString(size_t row) const { return _data->toString(row); }

    std::string toOriginString(size_t row) const { return _data->toOriginString(row); }

    inline size_t getRowCount() const { return _data->getRowCount(); }

private:
    ColumnSchema *_schema;
    ColumnDataBase *_data;
};

using ColumnPtr = std::shared_ptr<Column>;

} // namespace table
