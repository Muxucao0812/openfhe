; ModuleID = '/home/CONNECT/xmeng027/work/HKS-Accelerator-hanxu/openfhe-for-HKS-ACC/src/fpga_backend/Solution_Compute_BConv_Naive/solution1/.autopilot/db/a.g.ld.5.gdce.bc'
source_filename = "llvm-link"
target datalayout = "e-m:e-i64:64-i128:128-i256:256-i512:512-i1024:1024-i2048:2048-i4096:4096-n8:16:32:64-S128-v16:16-v24:32-v32:32-v48:64-v96:128-v192:256-v256:256-v512:512-v1024:1024"
target triple = "fpga64-xilinx-none"

; Function Attrs: noinline
define void @apatb_Compute_BConv_Naive_ir([64 x [64 x i64]]* noalias nocapture nonnull "fpga.decayed.dim.hint"="8" %in_x, [5 x i64]* noalias nocapture nonnull readonly "fpga.decayed.dim.hint"="3" %in_w, i64* noalias nocapture nonnull readonly "fpga.decayed.dim.hint"="5" %out_mod, i64* noalias nocapture nonnull readonly "fpga.decayed.dim.hint"="5" %out_k_half, i64* noalias nocapture nonnull readonly "fpga.decayed.dim.hint"="5" %out_m_barrett, i32 %sizeP) local_unnamed_addr #0 {
entry:
  %malloccall = tail call i8* @malloc(i64 262144)
  %in_x_copy = bitcast i8* %malloccall to [8 x [64 x [64 x i64]]]*
  %in_w_copy = alloca [3 x [5 x i64]], align 512
  %out_mod_copy = alloca [5 x i64], align 512
  %out_k_half_copy = alloca [5 x i64], align 512
  %out_m_barrett_copy = alloca [5 x i64], align 512
  %0 = bitcast [64 x [64 x i64]]* %in_x to [8 x [64 x [64 x i64]]]*
  %1 = bitcast [5 x i64]* %in_w to [3 x [5 x i64]]*
  %2 = bitcast i64* %out_mod to [5 x i64]*
  %3 = bitcast i64* %out_k_half to [5 x i64]*
  %4 = bitcast i64* %out_m_barrett to [5 x i64]*
  call fastcc void @copy_in([8 x [64 x [64 x i64]]]* nonnull %0, [8 x [64 x [64 x i64]]]* %in_x_copy, [3 x [5 x i64]]* nonnull %1, [3 x [5 x i64]]* nonnull align 512 %in_w_copy, [5 x i64]* nonnull %2, [5 x i64]* nonnull align 512 %out_mod_copy, [5 x i64]* nonnull %3, [5 x i64]* nonnull align 512 %out_k_half_copy, [5 x i64]* nonnull %4, [5 x i64]* nonnull align 512 %out_m_barrett_copy)
  call void @apatb_Compute_BConv_Naive_hw([8 x [64 x [64 x i64]]]* %in_x_copy, [3 x [5 x i64]]* %in_w_copy, [5 x i64]* %out_mod_copy, [5 x i64]* %out_k_half_copy, [5 x i64]* %out_m_barrett_copy, i32 %sizeP)
  call void @copy_back([8 x [64 x [64 x i64]]]* %0, [8 x [64 x [64 x i64]]]* %in_x_copy, [3 x [5 x i64]]* %1, [3 x [5 x i64]]* %in_w_copy, [5 x i64]* %2, [5 x i64]* %out_mod_copy, [5 x i64]* %3, [5 x i64]* %out_k_half_copy, [5 x i64]* %4, [5 x i64]* %out_m_barrett_copy)
  tail call void @free(i8* %malloccall)
  ret void
}

declare noalias i8* @malloc(i64) local_unnamed_addr

; Function Attrs: argmemonly noinline norecurse willreturn
define internal fastcc void @copy_in([8 x [64 x [64 x i64]]]* noalias readonly, [8 x [64 x [64 x i64]]]* noalias, [3 x [5 x i64]]* noalias readonly, [3 x [5 x i64]]* noalias align 512, [5 x i64]* noalias readonly, [5 x i64]* noalias align 512, [5 x i64]* noalias readonly, [5 x i64]* noalias align 512, [5 x i64]* noalias readonly, [5 x i64]* noalias align 512) unnamed_addr #1 {
entry:
  call fastcc void @onebyonecpy_hls.p0a8a64a64i64([8 x [64 x [64 x i64]]]* %1, [8 x [64 x [64 x i64]]]* %0)
  call fastcc void @onebyonecpy_hls.p0a3a5i64([3 x [5 x i64]]* align 512 %3, [3 x [5 x i64]]* %2)
  call fastcc void @onebyonecpy_hls.p0a5i64([5 x i64]* align 512 %5, [5 x i64]* %4)
  call fastcc void @onebyonecpy_hls.p0a5i64([5 x i64]* align 512 %7, [5 x i64]* %6)
  call fastcc void @onebyonecpy_hls.p0a5i64([5 x i64]* align 512 %9, [5 x i64]* %8)
  ret void
}

; Function Attrs: argmemonly noinline norecurse willreturn
define internal fastcc void @onebyonecpy_hls.p0a8a64a64i64([8 x [64 x [64 x i64]]]* noalias %dst, [8 x [64 x [64 x i64]]]* noalias readonly %src) unnamed_addr #2 {
entry:
  %0 = icmp eq [8 x [64 x [64 x i64]]]* %dst, null
  %1 = icmp eq [8 x [64 x [64 x i64]]]* %src, null
  %2 = or i1 %0, %1
  br i1 %2, label %ret, label %copy

copy:                                             ; preds = %entry
  call void @arraycpy_hls.p0a8a64a64i64([8 x [64 x [64 x i64]]]* nonnull %dst, [8 x [64 x [64 x i64]]]* nonnull %src, i64 8)
  br label %ret

ret:                                              ; preds = %copy, %entry
  ret void
}

; Function Attrs: argmemonly noinline norecurse willreturn
define void @arraycpy_hls.p0a8a64a64i64([8 x [64 x [64 x i64]]]* %dst, [8 x [64 x [64 x i64]]]* readonly %src, i64 %num) local_unnamed_addr #3 {
entry:
  %0 = icmp eq [8 x [64 x [64 x i64]]]* %src, null
  %1 = icmp eq [8 x [64 x [64 x i64]]]* %dst, null
  %2 = or i1 %1, %0
  br i1 %2, label %ret, label %copy

copy:                                             ; preds = %entry
  %for.loop.cond1 = icmp sgt i64 %num, 0
  br i1 %for.loop.cond1, label %for.loop.lr.ph, label %copy.split

for.loop.lr.ph:                                   ; preds = %copy
  br label %for.loop

for.loop:                                         ; preds = %for.loop, %for.loop.lr.ph
  %for.loop.idx2 = phi i64 [ 0, %for.loop.lr.ph ], [ %for.loop.idx.next, %for.loop ]
  %dst.addr = getelementptr [8 x [64 x [64 x i64]]], [8 x [64 x [64 x i64]]]* %dst, i64 0, i64 %for.loop.idx2
  %src.addr = getelementptr [8 x [64 x [64 x i64]]], [8 x [64 x [64 x i64]]]* %src, i64 0, i64 %for.loop.idx2
  call void @arraycpy_hls.p0a64a64i64([64 x [64 x i64]]* %dst.addr, [64 x [64 x i64]]* %src.addr, i64 64)
  %for.loop.idx.next = add nuw nsw i64 %for.loop.idx2, 1
  %exitcond = icmp ne i64 %for.loop.idx.next, %num
  br i1 %exitcond, label %for.loop, label %copy.split

copy.split:                                       ; preds = %for.loop, %copy
  br label %ret

ret:                                              ; preds = %copy.split, %entry
  ret void
}

; Function Attrs: argmemonly noinline norecurse willreturn
define void @arraycpy_hls.p0a64a64i64([64 x [64 x i64]]* %dst, [64 x [64 x i64]]* readonly %src, i64 %num) local_unnamed_addr #3 {
entry:
  %0 = icmp eq [64 x [64 x i64]]* %src, null
  %1 = icmp eq [64 x [64 x i64]]* %dst, null
  %2 = or i1 %1, %0
  br i1 %2, label %ret, label %copy

copy:                                             ; preds = %entry
  %for.loop.cond1 = icmp sgt i64 %num, 0
  br i1 %for.loop.cond1, label %for.loop.lr.ph, label %copy.split

for.loop.lr.ph:                                   ; preds = %copy
  br label %for.loop

for.loop:                                         ; preds = %for.loop, %for.loop.lr.ph
  %for.loop.idx2 = phi i64 [ 0, %for.loop.lr.ph ], [ %for.loop.idx.next, %for.loop ]
  %dst.addr = getelementptr [64 x [64 x i64]], [64 x [64 x i64]]* %dst, i64 0, i64 %for.loop.idx2
  %src.addr = getelementptr [64 x [64 x i64]], [64 x [64 x i64]]* %src, i64 0, i64 %for.loop.idx2
  call void @arraycpy_hls.p0a64i64([64 x i64]* %dst.addr, [64 x i64]* %src.addr, i64 64)
  %for.loop.idx.next = add nuw nsw i64 %for.loop.idx2, 1
  %exitcond = icmp ne i64 %for.loop.idx.next, %num
  br i1 %exitcond, label %for.loop, label %copy.split

copy.split:                                       ; preds = %for.loop, %copy
  br label %ret

ret:                                              ; preds = %copy.split, %entry
  ret void
}

; Function Attrs: argmemonly noinline norecurse willreturn
define void @arraycpy_hls.p0a64i64([64 x i64]* %dst, [64 x i64]* readonly %src, i64 %num) local_unnamed_addr #3 {
entry:
  %0 = icmp eq [64 x i64]* %src, null
  %1 = icmp eq [64 x i64]* %dst, null
  %2 = or i1 %1, %0
  br i1 %2, label %ret, label %copy

copy:                                             ; preds = %entry
  %for.loop.cond1 = icmp sgt i64 %num, 0
  br i1 %for.loop.cond1, label %for.loop.lr.ph, label %copy.split

for.loop.lr.ph:                                   ; preds = %copy
  br label %for.loop

for.loop:                                         ; preds = %for.loop, %for.loop.lr.ph
  %for.loop.idx2 = phi i64 [ 0, %for.loop.lr.ph ], [ %for.loop.idx.next, %for.loop ]
  %dst.addr = getelementptr [64 x i64], [64 x i64]* %dst, i64 0, i64 %for.loop.idx2
  %src.addr = getelementptr [64 x i64], [64 x i64]* %src, i64 0, i64 %for.loop.idx2
  %3 = load i64, i64* %src.addr, align 8
  store i64 %3, i64* %dst.addr, align 8
  %for.loop.idx.next = add nuw nsw i64 %for.loop.idx2, 1
  %exitcond = icmp ne i64 %for.loop.idx.next, %num
  br i1 %exitcond, label %for.loop, label %copy.split

copy.split:                                       ; preds = %for.loop, %copy
  br label %ret

ret:                                              ; preds = %copy.split, %entry
  ret void
}

; Function Attrs: argmemonly noinline norecurse willreturn
define internal fastcc void @onebyonecpy_hls.p0a3a5i64([3 x [5 x i64]]* noalias align 512 %dst, [3 x [5 x i64]]* noalias readonly %src) unnamed_addr #2 {
entry:
  %0 = icmp eq [3 x [5 x i64]]* %dst, null
  %1 = icmp eq [3 x [5 x i64]]* %src, null
  %2 = or i1 %0, %1
  br i1 %2, label %ret, label %copy

copy:                                             ; preds = %entry
  call void @arraycpy_hls.p0a3a5i64([3 x [5 x i64]]* nonnull %dst, [3 x [5 x i64]]* nonnull %src, i64 3)
  br label %ret

ret:                                              ; preds = %copy, %entry
  ret void
}

; Function Attrs: argmemonly noinline norecurse willreturn
define void @arraycpy_hls.p0a3a5i64([3 x [5 x i64]]* %dst, [3 x [5 x i64]]* readonly %src, i64 %num) local_unnamed_addr #3 {
entry:
  %0 = icmp eq [3 x [5 x i64]]* %src, null
  %1 = icmp eq [3 x [5 x i64]]* %dst, null
  %2 = or i1 %1, %0
  br i1 %2, label %ret, label %copy

copy:                                             ; preds = %entry
  %for.loop.cond1 = icmp sgt i64 %num, 0
  br i1 %for.loop.cond1, label %for.loop.lr.ph, label %copy.split

for.loop.lr.ph:                                   ; preds = %copy
  br label %for.loop

for.loop:                                         ; preds = %for.loop, %for.loop.lr.ph
  %for.loop.idx2 = phi i64 [ 0, %for.loop.lr.ph ], [ %for.loop.idx.next, %for.loop ]
  %dst.addr = getelementptr [3 x [5 x i64]], [3 x [5 x i64]]* %dst, i64 0, i64 %for.loop.idx2
  %src.addr = getelementptr [3 x [5 x i64]], [3 x [5 x i64]]* %src, i64 0, i64 %for.loop.idx2
  call void @arraycpy_hls.p0a5i64([5 x i64]* %dst.addr, [5 x i64]* %src.addr, i64 5)
  %for.loop.idx.next = add nuw nsw i64 %for.loop.idx2, 1
  %exitcond = icmp ne i64 %for.loop.idx.next, %num
  br i1 %exitcond, label %for.loop, label %copy.split

copy.split:                                       ; preds = %for.loop, %copy
  br label %ret

ret:                                              ; preds = %copy.split, %entry
  ret void
}

; Function Attrs: argmemonly noinline norecurse willreturn
define void @arraycpy_hls.p0a5i64([5 x i64]* %dst, [5 x i64]* readonly %src, i64 %num) local_unnamed_addr #3 {
entry:
  %0 = icmp eq [5 x i64]* %src, null
  %1 = icmp eq [5 x i64]* %dst, null
  %2 = or i1 %1, %0
  br i1 %2, label %ret, label %copy

copy:                                             ; preds = %entry
  %for.loop.cond1 = icmp sgt i64 %num, 0
  br i1 %for.loop.cond1, label %for.loop.lr.ph, label %copy.split

for.loop.lr.ph:                                   ; preds = %copy
  br label %for.loop

for.loop:                                         ; preds = %for.loop, %for.loop.lr.ph
  %for.loop.idx2 = phi i64 [ 0, %for.loop.lr.ph ], [ %for.loop.idx.next, %for.loop ]
  %dst.addr = getelementptr [5 x i64], [5 x i64]* %dst, i64 0, i64 %for.loop.idx2
  %src.addr = getelementptr [5 x i64], [5 x i64]* %src, i64 0, i64 %for.loop.idx2
  %3 = load i64, i64* %src.addr, align 8
  store i64 %3, i64* %dst.addr, align 8
  %for.loop.idx.next = add nuw nsw i64 %for.loop.idx2, 1
  %exitcond = icmp ne i64 %for.loop.idx.next, %num
  br i1 %exitcond, label %for.loop, label %copy.split

copy.split:                                       ; preds = %for.loop, %copy
  br label %ret

ret:                                              ; preds = %copy.split, %entry
  ret void
}

; Function Attrs: argmemonly noinline norecurse willreturn
define internal fastcc void @onebyonecpy_hls.p0a5i64([5 x i64]* noalias align 512 %dst, [5 x i64]* noalias readonly %src) unnamed_addr #2 {
entry:
  %0 = icmp eq [5 x i64]* %dst, null
  %1 = icmp eq [5 x i64]* %src, null
  %2 = or i1 %0, %1
  br i1 %2, label %ret, label %copy

copy:                                             ; preds = %entry
  call void @arraycpy_hls.p0a5i64([5 x i64]* nonnull %dst, [5 x i64]* nonnull %src, i64 5)
  br label %ret

ret:                                              ; preds = %copy, %entry
  ret void
}

; Function Attrs: argmemonly noinline norecurse willreturn
define internal fastcc void @copy_out([8 x [64 x [64 x i64]]]* noalias, [8 x [64 x [64 x i64]]]* noalias readonly, [3 x [5 x i64]]* noalias, [3 x [5 x i64]]* noalias readonly align 512, [5 x i64]* noalias, [5 x i64]* noalias readonly align 512, [5 x i64]* noalias, [5 x i64]* noalias readonly align 512, [5 x i64]* noalias, [5 x i64]* noalias readonly align 512) unnamed_addr #4 {
entry:
  call fastcc void @onebyonecpy_hls.p0a8a64a64i64([8 x [64 x [64 x i64]]]* %0, [8 x [64 x [64 x i64]]]* %1)
  call fastcc void @onebyonecpy_hls.p0a3a5i64([3 x [5 x i64]]* %2, [3 x [5 x i64]]* align 512 %3)
  call fastcc void @onebyonecpy_hls.p0a5i64([5 x i64]* %4, [5 x i64]* align 512 %5)
  call fastcc void @onebyonecpy_hls.p0a5i64([5 x i64]* %6, [5 x i64]* align 512 %7)
  call fastcc void @onebyonecpy_hls.p0a5i64([5 x i64]* %8, [5 x i64]* align 512 %9)
  ret void
}

declare void @free(i8*) local_unnamed_addr

declare void @apatb_Compute_BConv_Naive_hw([8 x [64 x [64 x i64]]]*, [3 x [5 x i64]]*, [5 x i64]*, [5 x i64]*, [5 x i64]*, i32)

; Function Attrs: argmemonly noinline norecurse willreturn
define internal fastcc void @copy_back([8 x [64 x [64 x i64]]]* noalias, [8 x [64 x [64 x i64]]]* noalias readonly, [3 x [5 x i64]]* noalias, [3 x [5 x i64]]* noalias readonly align 512, [5 x i64]* noalias, [5 x i64]* noalias readonly align 512, [5 x i64]* noalias, [5 x i64]* noalias readonly align 512, [5 x i64]* noalias, [5 x i64]* noalias readonly align 512) unnamed_addr #4 {
entry:
  call fastcc void @onebyonecpy_hls.p0a8a64a64i64([8 x [64 x [64 x i64]]]* %0, [8 x [64 x [64 x i64]]]* %1)
  ret void
}

define void @Compute_BConv_Naive_hw_stub_wrapper([8 x [64 x [64 x i64]]]*, [3 x [5 x i64]]*, [5 x i64]*, [5 x i64]*, [5 x i64]*, i32) #5 {
entry:
  call void @copy_out([8 x [64 x [64 x i64]]]* null, [8 x [64 x [64 x i64]]]* %0, [3 x [5 x i64]]* null, [3 x [5 x i64]]* %1, [5 x i64]* null, [5 x i64]* %2, [5 x i64]* null, [5 x i64]* %3, [5 x i64]* null, [5 x i64]* %4)
  %6 = bitcast [8 x [64 x [64 x i64]]]* %0 to [64 x [64 x i64]]*
  %7 = bitcast [3 x [5 x i64]]* %1 to [5 x i64]*
  %8 = bitcast [5 x i64]* %2 to i64*
  %9 = bitcast [5 x i64]* %3 to i64*
  %10 = bitcast [5 x i64]* %4 to i64*
  call void @Compute_BConv_Naive_hw_stub([64 x [64 x i64]]* %6, [5 x i64]* %7, i64* %8, i64* %9, i64* %10, i32 %5)
  call void @copy_in([8 x [64 x [64 x i64]]]* null, [8 x [64 x [64 x i64]]]* %0, [3 x [5 x i64]]* null, [3 x [5 x i64]]* %1, [5 x i64]* null, [5 x i64]* %2, [5 x i64]* null, [5 x i64]* %3, [5 x i64]* null, [5 x i64]* %4)
  ret void
}

declare void @Compute_BConv_Naive_hw_stub([64 x [64 x i64]]* noalias nocapture nonnull, [5 x i64]* noalias nocapture nonnull readonly, i64* noalias nocapture nonnull readonly, i64* noalias nocapture nonnull readonly, i64* noalias nocapture nonnull readonly, i32)

attributes #0 = { noinline "fpga.wrapper.func"="wrapper" }
attributes #1 = { argmemonly noinline norecurse willreturn "fpga.wrapper.func"="copyin" }
attributes #2 = { argmemonly noinline norecurse willreturn "fpga.wrapper.func"="onebyonecpy_hls" }
attributes #3 = { argmemonly noinline norecurse willreturn "fpga.wrapper.func"="arraycpy_hls" }
attributes #4 = { argmemonly noinline norecurse willreturn "fpga.wrapper.func"="copyout" }
attributes #5 = { "fpga.wrapper.func"="stub" }

!llvm.dbg.cu = !{}
!llvm.ident = !{!0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0}
!llvm.module.flags = !{!1, !2, !3}
!blackbox_cfg = !{!4}

!0 = !{!"clang version 7.0.0 "}
!1 = !{i32 2, !"Dwarf Version", i32 4}
!2 = !{i32 2, !"Debug Info Version", i32 3}
!3 = !{i32 1, !"wchar_size", i32 4}
!4 = !{}
