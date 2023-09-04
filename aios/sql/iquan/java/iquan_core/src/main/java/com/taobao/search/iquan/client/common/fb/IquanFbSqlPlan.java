// automatically generated by the FlatBuffers compiler, do not modify

package com.taobao.search.iquan.client.common.fb;

import java.nio.*;
import java.lang.*;
import java.util.*;
import com.google.flatbuffers.*;

@SuppressWarnings("unused")
public final class IquanFbSqlPlan extends Table {
  public static IquanFbSqlPlan getRootAsIquanFbSqlPlan(ByteBuffer _bb) { return getRootAsIquanFbSqlPlan(_bb, new IquanFbSqlPlan()); }
  public static IquanFbSqlPlan getRootAsIquanFbSqlPlan(ByteBuffer _bb, IquanFbSqlPlan obj) { _bb.order(ByteOrder.LITTLE_ENDIAN); return (obj.__assign(_bb.getInt(_bb.position()) + _bb.position(), _bb)); }
  public void __init(int _i, ByteBuffer _bb) { bb_pos = _i; bb = _bb; }
  public IquanFbSqlPlan __assign(int _i, ByteBuffer _bb) { __init(_i, _bb); return this; }

  public String relPlanVersion() { int o = __offset(4); return o != 0 ? __string(o + bb_pos) : null; }
  public ByteBuffer relPlanVersionAsByteBuffer() { return __vector_as_bytebuffer(4, 1); }
  public ByteBuffer relPlanVersionInByteBuffer(ByteBuffer _bb) { return __vector_in_bytebuffer(_bb, 4, 1); }
  public IquanFbPlanOp opList(int j) { return opList(new IquanFbPlanOp(), j); }
  public IquanFbPlanOp opList(IquanFbPlanOp obj, int j) { int o = __offset(6); return o != 0 ? obj.__assign(__indirect(__vector(o) + j * 4), bb) : null; }
  public int opListLength() { int o = __offset(6); return o != 0 ? __vector_len(o) : 0; }
  public IquanFbMap execParams() { return execParams(new IquanFbMap()); }
  public IquanFbMap execParams(IquanFbMap obj) { int o = __offset(8); return o != 0 ? obj.__assign(__indirect(o + bb_pos), bb) : null; }

  public static int createIquanFbSqlPlan(FlatBufferBuilder builder,
      int rel_plan_versionOffset,
      int op_listOffset,
      int exec_paramsOffset) {
    builder.startObject(3);
    IquanFbSqlPlan.addExecParams(builder, exec_paramsOffset);
    IquanFbSqlPlan.addOpList(builder, op_listOffset);
    IquanFbSqlPlan.addRelPlanVersion(builder, rel_plan_versionOffset);
    return IquanFbSqlPlan.endIquanFbSqlPlan(builder);
  }

  public static void startIquanFbSqlPlan(FlatBufferBuilder builder) { builder.startObject(3); }
  public static void addRelPlanVersion(FlatBufferBuilder builder, int relPlanVersionOffset) { builder.addOffset(0, relPlanVersionOffset, 0); }
  public static void addOpList(FlatBufferBuilder builder, int opListOffset) { builder.addOffset(1, opListOffset, 0); }
  public static int createOpListVector(FlatBufferBuilder builder, int[] data) { builder.startVector(4, data.length, 4); for (int i = data.length - 1; i >= 0; i--) builder.addOffset(data[i]); return builder.endVector(); }
  public static void startOpListVector(FlatBufferBuilder builder, int numElems) { builder.startVector(4, numElems, 4); }
  public static void addExecParams(FlatBufferBuilder builder, int execParamsOffset) { builder.addOffset(2, execParamsOffset, 0); }
  public static int endIquanFbSqlPlan(FlatBufferBuilder builder) {
    int o = builder.endObject();
    return o;
  }
}
