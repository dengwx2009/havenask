#include "sql/ops/tvf/builtin/SortTvfFunc.h"

#include <algorithm>
#include <cstdint>
#include <iosfwd>
#include <limits>
#include <memory>
#include <string>
#include <vector>

#include "iquan/common/catalog/TvfFunctionModel.h"
#include "matchdoc/MatchDoc.h"
#include "matchdoc/MatchDocAllocator.h"
#include "matchdoc/VectorDocStorage.h"
#include "sql/ops/test/OpTestBase.h"
#include "sql/ops/tvf/TvfFunc.h"
#include "table/Row.h"
#include "table/Table.h"
#include "table/test/MatchDocUtil.h"
#include "unittest/unittest.h"

using namespace std;
using namespace testing;
using namespace table;
using namespace matchdoc;

namespace sql {

class SortTvfFuncTest : public OpTestBase {
public:
    void setUp();
    void tearDown();
    TablePtr prepareInputTable(vector<int32_t> value);
};

void SortTvfFuncTest::setUp() {}

void SortTvfFuncTest::tearDown() {}

TablePtr SortTvfFuncTest::prepareInputTable(vector<int32_t> value) {
    MatchDocAllocatorPtr allocator(new matchdoc::MatchDocAllocator(_poolPtr));
    vector<MatchDoc> docs = allocator->batchAllocate(value);
    _matchDocUtil.extendMatchDocAllocator<int32_t>(allocator, docs, "id", value);
    TablePtr table;
    table.reset(new Table(docs, allocator));
    return table;
}

TEST_F(SortTvfFuncTest, testInit) {
    { // param expect 2
        TvfFuncInitContext context;
        SortTvfFunc tvfFunc;
        ASSERT_FALSE(tvfFunc.init(context));
    }
    { // param expect 2
        TvfFuncInitContext context;
        context.params = {"abc", "123", "ab"};
        SortTvfFunc tvfFunc;
        ASSERT_FALSE(tvfFunc.init(context));
    }
    { // sort field empty
        TvfFuncInitContext context;
        context.params = {"", "1"};
        SortTvfFunc tvfFunc;
        ASSERT_FALSE(tvfFunc.init(context));
    }
    {
        TvfFuncInitContext context;
        context.params = {"123", "-1"};
        SortTvfFunc tvfFunc;
        ASSERT_TRUE(tvfFunc.init(context));
        ASSERT_EQ(std::numeric_limits<int32_t>::max(), tvfFunc._count);
        vector<string> expectSVec = {"123"};
        vector<bool> fVec = {false};
        ASSERT_EQ(expectSVec, tvfFunc._sortFields);
        ASSERT_EQ(fVec, tvfFunc._sortFlags);
    }
    {
        TvfFuncInitContext context;
        context.params = {"123", "1"};
        SortTvfFunc tvfFunc;
        ASSERT_TRUE(tvfFunc.init(context));
        ASSERT_EQ(1, tvfFunc._count);
        vector<string> expectSVec = {"123"};
        vector<bool> fVec = {false};
        ASSERT_EQ(expectSVec, tvfFunc._sortFields);
        ASSERT_EQ(fVec, tvfFunc._sortFlags);
    }
    {
        TvfFuncInitContext context;
        context.params = {"123,456", "1"};
        SortTvfFunc tvfFunc;
        ASSERT_TRUE(tvfFunc.init(context));
        ASSERT_EQ(1, tvfFunc._count);
        vector<string> expectSVec = {"123", "456"};
        vector<bool> fVec = {false, false};
        ASSERT_EQ(expectSVec, tvfFunc._sortFields);
        ASSERT_EQ(fVec, tvfFunc._sortFlags);
    }
    {
        TvfFuncInitContext context;
        context.params = {"+123,-456,", "1"};
        SortTvfFunc tvfFunc;
        ASSERT_TRUE(tvfFunc.init(context));
        ASSERT_EQ(1, tvfFunc._count);
        vector<string> expectSVec = {"123", "456"};
        vector<bool> fVec = {false, true};
        ASSERT_EQ(expectSVec, tvfFunc._sortFields);
        ASSERT_EQ(fVec, tvfFunc._sortFlags);
    }
}

TEST_F(SortTvfFuncTest, testdoComputeCreateComparatorFail) {
    MatchDocAllocatorPtr allocator(new matchdoc::MatchDocAllocator(_poolPtr));
    vector<MatchDoc> docs = allocator->batchAllocate(3);
    _matchDocUtil.extendMatchDocAllocator<int32_t>(allocator, docs, "id", {3, 2, 1});
    _matchDocUtil.extendMatchDocAllocator<int32_t>(allocator, docs, "value", {3, 3, 2});
    TablePtr input(new Table(docs, allocator));
    TvfFuncInitContext context;
    context.params = {"value1", "2"};
    context.queryPool = _poolPtr.get();
    SortTvfFunc tvfFunc;
    ASSERT_TRUE(tvfFunc.init(context));
    ASSERT_FALSE(tvfFunc.doCompute(input));
}

TEST_F(SortTvfFuncTest, testdoComputeDefaultSort) {
    MatchDocAllocatorPtr allocator(new matchdoc::MatchDocAllocator(_poolPtr));
    vector<MatchDoc> docs = allocator->batchAllocate(5);
    _matchDocUtil.extendMatchDocAllocator<int32_t>(allocator, docs, "id", {5, 3, 2, 1, 4});
    _matchDocUtil.extendMatchDocAllocator<int32_t>(allocator, docs, "value", {6, 7, 8, 10, 9});
    TablePtr input(new Table(docs, allocator));
    TvfFuncInitContext context;
    context.params = {"value", "2"};
    context.queryPool = _poolPtr.get();
    SortTvfFunc tvfFunc;
    ASSERT_TRUE(tvfFunc.init(context));
    TablePtr output;
    ASSERT_TRUE(tvfFunc.compute(input, true, output));
    ASSERT_TRUE(output != nullptr);
    ASSERT_EQ(2, output->getRowCount());
    vector<int32_t> expectId = {5, 3};
    checkOutputColumn<int32_t>(output, "id", expectId);
    vector<int32_t> expectValue = {6, 7};
    checkOutputColumn<int32_t>(output, "value", expectValue);
}

TEST_F(SortTvfFuncTest, testComputeDefaultSortDesc) {
    MatchDocAllocatorPtr allocator(new matchdoc::MatchDocAllocator(_poolPtr));
    vector<MatchDoc> docs = allocator->batchAllocate(5);
    _matchDocUtil.extendMatchDocAllocator<int32_t>(allocator, docs, "id", {5, 3, 2, 1, 4});
    _matchDocUtil.extendMatchDocAllocator<int32_t>(allocator, docs, "value", {6, 7, 8, 10, 9});
    TablePtr input(new Table(docs, allocator));
    TvfFuncInitContext context;
    context.params = {"-value", "2"};
    context.queryPool = _poolPtr.get();
    SortTvfFunc tvfFunc;
    ASSERT_TRUE(tvfFunc.init(context));
    TablePtr output;
    ASSERT_TRUE(tvfFunc.compute(input, true, output));
    ASSERT_TRUE(output != nullptr);
    ASSERT_EQ(2, output->getRowCount());
    vector<int32_t> expectId = {1, 4};
    checkOutputColumn<int32_t>(output, "id", expectId);
    vector<int32_t> expectValue = {10, 9};
    checkOutputColumn<int32_t>(output, "value", expectValue);
}

TEST_F(SortTvfFuncTest, testComputeInputEof) {
    MatchDocAllocatorPtr allocator(new matchdoc::MatchDocAllocator(_poolPtr));
    vector<MatchDoc> docs = allocator->batchAllocate(5);
    _matchDocUtil.extendMatchDocAllocator<int32_t>(allocator, docs, "id", {5, 3, 2, 1, 4});
    _matchDocUtil.extendMatchDocAllocator<int32_t>(allocator, docs, "value", {6, 7, 8, 10, 9});
    TablePtr input(new Table(docs, allocator));
    TvfFuncInitContext context;
    context.params = {"value", "2"};
    context.queryPool = _poolPtr.get();
    SortTvfFunc tvfFunc;
    ASSERT_TRUE(tvfFunc.init(context));
    TablePtr output;
    ASSERT_TRUE(tvfFunc.compute(input, false, output));
    ASSERT_TRUE(output == nullptr);
    input.reset();
    ASSERT_TRUE(tvfFunc.compute(input, true, output));
    ASSERT_TRUE(output != nullptr);
    ASSERT_EQ(2, output->getRowCount());
    vector<int32_t> expectId = {5, 3};
    checkOutputColumn<int32_t>(output, "id", expectId);
    vector<int32_t> expectValue = {6, 7};
    checkOutputColumn<int32_t>(output, "value", expectValue);
}

TEST_F(SortTvfFuncTest, testComputeTopK_0) {
    MatchDocAllocatorPtr allocator(new matchdoc::MatchDocAllocator(_poolPtr));
    vector<MatchDoc> docs = allocator->batchAllocate(5);
    _matchDocUtil.extendMatchDocAllocator<int32_t>(allocator, docs, "id", {5, 3, 2, 1, 4});
    _matchDocUtil.extendMatchDocAllocator<int32_t>(allocator, docs, "value", {6, 7, 8, 10, 9});
    TablePtr input(new Table(docs, allocator));
    TvfFuncInitContext context;
    context.params = {"value", "0"};
    context.queryPool = _poolPtr.get();
    SortTvfFunc tvfFunc;
    ASSERT_TRUE(tvfFunc.init(context));
    TablePtr output;
    ASSERT_TRUE(tvfFunc.compute(input, true, output));
    ASSERT_TRUE(output != nullptr);
    ASSERT_EQ(0, output->getRowCount());
    vector<int32_t> expectId = {};
    checkOutputColumn<int32_t>(output, "id", expectId);
    vector<int32_t> expectValue = {};
    checkOutputColumn<int32_t>(output, "value", expectValue);
}

TEST_F(SortTvfFuncTest, testComputeLessThanTopK) {
    MatchDocAllocatorPtr allocator(new matchdoc::MatchDocAllocator(_poolPtr));
    vector<MatchDoc> docs = allocator->batchAllocate(3);
    _matchDocUtil.extendMatchDocAllocator<int32_t>(allocator, docs, "id", {3, 2, 1});
    _matchDocUtil.extendMatchDocAllocator<int32_t>(allocator, docs, "value", {3, 2, 1});
    TablePtr input(new Table(docs, allocator));
    TvfFuncInitContext context;
    context.params = {"-value", "4"};
    context.queryPool = _poolPtr.get();
    SortTvfFunc tvfFunc;
    ASSERT_TRUE(tvfFunc.init(context));
    TablePtr output;
    ASSERT_TRUE(tvfFunc.compute(input, true, output));
    ASSERT_TRUE(output != nullptr);
    ASSERT_EQ(3, output->getRowCount());
    vector<int32_t> expectId = {3, 2, 1};
    checkOutputColumn<int32_t>(output, "id", expectId);
    vector<int32_t> expectValue = {3, 2, 1};
    checkOutputColumn<int32_t>(output, "value", expectValue);
}

TEST_F(SortTvfFuncTest, testComputeLessThanTopK_max) {
    MatchDocAllocatorPtr allocator(new matchdoc::MatchDocAllocator(_poolPtr));
    vector<MatchDoc> docs = allocator->batchAllocate(3);
    _matchDocUtil.extendMatchDocAllocator<int32_t>(allocator, docs, "id", {3, 2, 1});
    _matchDocUtil.extendMatchDocAllocator<int32_t>(allocator, docs, "value", {3, 2, 1});
    TablePtr input(new Table(docs, allocator));
    TvfFuncInitContext context;
    context.params = {"-value", "-1"};
    context.queryPool = _poolPtr.get();
    SortTvfFunc tvfFunc;
    ASSERT_TRUE(tvfFunc.init(context));
    TablePtr output;
    ASSERT_TRUE(tvfFunc.compute(input, true, output));
    ASSERT_TRUE(output != nullptr);
    ASSERT_EQ(3, output->getRowCount());
    vector<int32_t> expectId = {3, 2, 1};
    checkOutputColumn<int32_t>(output, "id", expectId);
    vector<int32_t> expectValue = {3, 2, 1};
    checkOutputColumn<int32_t>(output, "value", expectValue);
}

TEST_F(SortTvfFuncTest, testComputeEmptyInput) {
    TablePtr input;
    TvfFuncInitContext context;
    context.params = {"-value", "-1"};
    context.queryPool = _poolPtr.get();
    SortTvfFunc tvfFunc;
    ASSERT_TRUE(tvfFunc.init(context));
    TablePtr output;
    ASSERT_TRUE(tvfFunc.compute(input, true, output));
    ASSERT_TRUE(output == nullptr);
}

TEST_F(SortTvfFuncTest, testRegTvfModels) {
    SortTvfFuncCreator creator;
    iquan::TvfModels tvfModels;
    ASSERT_TRUE(creator.regTvfModels(tvfModels));
    ASSERT_EQ(1, tvfModels.functions.size());
}

} // namespace sql
