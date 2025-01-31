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
#include "navi/log/TraceCollector.h"
#include "navi/log/Layout.h"
#include "navi/proto/GraphVis.pb.h"

namespace navi {

TraceCollector::TraceCollector()
    : _formatPattern(DEFAULT_LOG_PATTERN)
{
}

TraceCollector::~TraceCollector() {
}

void TraceCollector::setFormatPattern(const std::string &pattern) {
    _formatPattern = pattern;
}

void TraceCollector::append(LoggingEvent &event) {
    _eventList.emplace_back(event);
}

void TraceCollector::append(LoggingEvent &&event) {
    _eventList.emplace_back(std::move(event));
}

void TraceCollector::merge(TraceCollector &other) {
    _eventList.splice(_eventList.end(), other._eventList);
}

void TraceCollector::format(std::vector<std::string> &traceVec) const {
    PatternLayout layout;
    layout.setLogPattern(_formatPattern);
    traceVec.reserve(_eventList.size());
    for (const auto &event : _eventList) {
        traceVec.emplace_back(layout.format(event));
    }
}

void TraceCollector::fillProto(GraphTraceDef *trace) const {
    for (const auto &event : _eventList) {
        auto eventDef = trace->add_events();
        event.fillProto(eventDef);
    }
}

void TraceCollector::serialize(autil::DataBuffer &dataBuffer) const {
    dataBuffer.write(_eventList.size());
    for (const auto &event : _eventList) {
        dataBuffer.write(event);
    }
}

void TraceCollector::deserialize(autil::DataBuffer &dataBuffer) {
    size_t traceSize;
    dataBuffer.read(traceSize);
    for (size_t i = 0; i < traceSize; ++i) {
        LoggingEvent event;
        dataBuffer.read(event);
        _eventList.emplace_back(event);
    }
}

bool TraceCollector::empty() const {
    return _eventList.empty();
}

size_t TraceCollector::eventSize() const {
    return _eventList.size();
}

}
