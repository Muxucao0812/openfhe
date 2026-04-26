# MemoryTracker: 实时存储占用监控

## Context

用户需要在 HKS KeySwitch 调度中追踪中间数据（P-tower complement buffers）的实时存储占用，用于论文中对比 DC/MP/OC 三种策略的峰值 SRAM 差异。目标是输出 CSV 时间序列日志，可直接用 Python 画折线图。

## 方案

### 1. 新增 `MemoryTracker` 类到 `hks_strategy.h`

在现有 `HKSStats` 旁边添加：

```cpp
struct MemoryEvent {
    int step;           // 操作序号（自增）
    int64_t delta;      // +alloc / -free (bytes)
    int64_t watermark;  // 操作后的当前水位
    const char* op;     // "INTT", "BConv", "NTT", "ModMul", "Assemble"
    int digit;          // 当前 digit 索引
    int tower;          // 当前 tower 索引 (-1 if N/A)
};

class MemoryTracker {
    int64_t current_ = 0;
    int64_t peak_ = 0;
    int step_ = 0;
    std::vector<MemoryEvent> log_;
public:
    void alloc(int64_t bytes, const char* op, int digit, int tower = -1);
    void free(int64_t bytes, const char* op, int digit, int tower = -1);
    int64_t peak() const;
    int64_t current() const;
    void reset();
    const std::vector<MemoryEvent>& events() const;
    void dump_csv(std::ostream& os) const;  // 输出 CSV
};
```

全局访问：`MemoryTracker& GetMemoryTracker()` — 与 `GetHKSStats()` 同模式。

### 2. 在 `keyswitch-hybrid.cpp` 的三种策略中插入探针

每个 tower 大小 = `ring_dim * sizeof(uint64_t)` bytes。

**DC 策略** (lines 413-465):
- BConv 后: `alloc(sizeP * tower_bytes, "BConv", part)` — 产生 sizeP 个 P-tower
- NTT 后 + 组装完: `free(sizeP * tower_bytes, "Assemble", part)` — 消费掉

**MP 策略** (lines 396-465):
- Phase 1 INTT: 无中间数据变化
- Phase 2 每个 BConv 后: `alloc(sizeP * tower_bytes, "BConv", part)` — 累积
- Phase 3 NTT + 组装后: `free(sizeP * tower_bytes, "Assemble", part)` — 逐步释放

**OC 策略** (lines 468-538):
- 外层 P-tower 循环每轮:
  - BConv 后: `alloc(tower_bytes, "BConv", part, p)` — 1 个 tower
  - 组装后: `free(tower_bytes, "Assemble", part, p)` — 立即释放

### 3. 在 `hks-benchmark.cpp` 中输出 CSV

在 stats 打印后调用 `GetMemoryTracker().dump_csv()`，输出到 `stdout` 或文件：

```
step,op,digit,tower,delta_bytes,watermark_bytes
0,BConv,0,-1,65536,65536
1,Assemble,0,-1,-65536,0
...
```

### 修改文件清单

| 文件 | 改动 |
|------|------|
| `src/pke/include/keyswitch/hks_strategy.h` | 新增 MemoryTracker 类、MemoryEvent 结构体、全局访问函数 |
| `src/pke/lib/keyswitch/keyswitch-hybrid.cpp` | 在 DC/MP/OC 三个分支中插入 alloc/free 调用 |
| `src/pke/examples/hks-benchmark.cpp` | 调用 dump_csv 输出日志 |

### 验证

1. 分别用 `--strategy DC/MP/OC` 运行 benchmark
2. 检查 CSV 输出：
   - MP: watermark 阶梯上升到 `numDigits * sizeP * tower_bytes`，再阶梯下降
   - DC: watermark 在 `sizeP * tower_bytes` 处反复升降
   - OC: watermark 在 `0` 和 `tower_bytes` 之间锯齿波动
3. peak 值应与 `HKSStats.peak_p_towers * tower_bytes` 一致
