; ModuleID = 'Max.c'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

; Function Attrs: nounwind uwtable
define i32 @max(i32 %a, i32 %b) #0 {
  %1 = alloca i32, align 4
  %2 = alloca i32, align 4
  %3 = alloca i32, align 4
  store i32 %a, i32* %2, align 4
  store i32 %b, i32* %3, align 4
  %4 = load i32, i32* %2, align 4
  %5 = load i32, i32* %3, align 4
  %6 = icmp sge i32 %4, %5
  br i1 %6, label %7, label %9

; <label>:7                                       ; preds = %0
  %8 = load i32, i32* %2, align 4
  store i32 %8, i32* %1, align 4
  br label %11

; <label>:9                                       ; preds = %0
  %10 = load i32, i32* %3, align 4
  store i32 %10, i32* %1, align 4
  br label %11

; <label>:11                                      ; preds = %9, %7
  %12 = load i32, i32* %1, align 4
  ret i32 %12
}

; Function Attrs: nounwind uwtable
define i32 @main() #0 {
  %1 = alloca i32, align 4
  %x = alloca i32, align 4
  %y = alloca i32, align 4
  store i32 0, i32* %1, align 4
  store i32 2, i32* %x, align 4
  store i32 5, i32* %y, align 4
  %2 = load i32, i32* %x, align 4
  %3 = load i32, i32* %y, align 4
  %4 = call i32 @max(i32 %2, i32 %3)
  %5 = icmp eq i32 %4, 5
  br i1 %5, label %6, label %7

; <label>:6                                       ; preds = %0
  store i32 1, i32* %1, align 4
  br label %8

; <label>:7                                       ; preds = %0
  store i32 0, i32* %1, align 4
  br label %8

; <label>:8                                       ; preds = %7, %6
  %9 = load i32, i32* %1, align 4
  ret i32 %9
}

attributes #0 = { nounwind uwtable "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.ident = !{!0}

!0 = !{!"clang version 3.8.0-2ubuntu3 (tags/RELEASE_380/final)"}
