/**
 * ============================================================
 * 特性名称: alignof 与 alignas
 * 所属标准: C++11
 * ============================================================
 *
 * [是什么]: alignof查询类型或变量的对齐要求；
 *          alignas指定变量或类型的对齐方式。
 *
 * [为什么引入]: 内存对齐对性能至关重要（CPU缓存行、SIMD指令、
 *   DMA传输等都有对齐要求）。C++03中只能使用编译器扩展
 *   (__attribute__, __declspec)，不可移植。C++11标准化了对齐操作。
 *
 * [关键要点]:
 *   - alignof(T) 返回类型T的对齐要求（字节数，总是2的幂）
 *   - alignas(N) 指定至少N字节的对齐（N必须是2的幂）
 *   - alignas可以用于变量、类/结构体
 *   - 对齐只能增大不能减小（不能小于类型的自然对齐）
 * ============================================================
 */

#include <iostream>
#include <cstdint>
#include <cstddef>   // std::max_align_t
#include <type_traits> // std::alignment_of

// ============================================================
// 基础用法
// ============================================================
void basic_usage() {
    std::cout << "===== 基础用法 =====" << std::endl;

    // 1. alignof 查询各种类型的对齐
    std::cout << "基本类型的对齐要求:" << std::endl;
    std::cout << "  alignof(char)      = " << alignof(char) << std::endl;
    std::cout << "  alignof(short)     = " << alignof(short) << std::endl;
    std::cout << "  alignof(int)       = " << alignof(int) << std::endl;
    std::cout << "  alignof(long)      = " << alignof(long) << std::endl;
    std::cout << "  alignof(long long) = " << alignof(long long) << std::endl;
    std::cout << "  alignof(float)     = " << alignof(float) << std::endl;
    std::cout << "  alignof(double)    = " << alignof(double) << std::endl;
    std::cout << "  alignof(void*)     = " << alignof(void*) << std::endl;

    // 最大默认对齐
    std::cout << "  alignof(max_align_t) = " << alignof(std::max_align_t) << std::endl;

    // 2. 结构体的对齐
    struct A {
        char c;     // 1字节
        int i;      // 4字节
        double d;   // 8字节
    };
    std::cout << "\nstruct A { char, int, double }:" << std::endl;
    std::cout << "  sizeof(A)  = " << sizeof(A) << std::endl;
    std::cout << "  alignof(A) = " << alignof(A) << std::endl;

    struct B {
        double d;   // 8字节
        int i;      // 4字节
        char c;     // 1字节
    };
    std::cout << "\nstruct B { double, int, char }:" << std::endl;
    std::cout << "  sizeof(B)  = " << sizeof(B) << std::endl;
    std::cout << "  alignof(B) = " << alignof(B) << std::endl;

    // 3. alignas 指定对齐
    alignas(16) int aligned_int = 42;
    alignas(32) char aligned_buf[64];
    alignas(double) char as_double_buf[sizeof(double)];

    std::cout << "\naligned变量的地址对齐:" << std::endl;
    std::cout << "  alignas(16) int: 地址 = "
              << reinterpret_cast<uintptr_t>(&aligned_int)
              << " (16的倍数: "
              << (reinterpret_cast<uintptr_t>(&aligned_int) % 16 == 0 ? "是" : "否")
              << ")" << std::endl;
    std::cout << "  alignas(32) char[64]: 地址 = "
              << reinterpret_cast<uintptr_t>(aligned_buf)
              << " (32的倍数: "
              << (reinterpret_cast<uintptr_t>(aligned_buf) % 32 == 0 ? "是" : "否")
              << ")" << std::endl;

    (void)as_double_buf;
}

// ============================================================
// 进阶用法
// ============================================================

// 1. 缓存行对齐 —— 避免伪共享(false sharing)
struct alignas(64) CacheLineAligned {
    int counter;
    // 填充到64字节边界
    void increment() { ++counter; }
};

// 2. SIMD对齐
struct alignas(16) SimdVector {
    float data[4]; // 128位 = 16字节，适合SSE指令
};

// 3. 在类定义中使用alignas
struct alignas(32) AlignedData {
    double values[4]; // 32字节数据，32字节对齐
};

// 4. 类型对齐的存储（手动管理对齐的缓冲区）
template <typename T>
struct AlignedStorage {
    alignas(T) unsigned char data[sizeof(T)];

    T* as_ptr() { return reinterpret_cast<T*>(data); }
    const T* as_ptr() const { return reinterpret_cast<const T*>(data); }
};

void advanced_usage() {
    std::cout << "\n===== 进阶用法 =====" << std::endl;

    // 1. 缓存行对齐
    std::cout << "缓存行对齐 (避免伪共享):" << std::endl;
    CacheLineAligned counter1, counter2;
    std::cout << "  sizeof(CacheLineAligned) = " << sizeof(CacheLineAligned) << std::endl;
    std::cout << "  alignof(CacheLineAligned) = " << alignof(CacheLineAligned) << std::endl;
    std::cout << "  counter1地址: " << reinterpret_cast<uintptr_t>(&counter1) << std::endl;
    std::cout << "  counter2地址: " << reinterpret_cast<uintptr_t>(&counter2) << std::endl;
    std::cout << "  地址差: " << (reinterpret_cast<uintptr_t>(&counter2)
                                - reinterpret_cast<uintptr_t>(&counter1))
              << " 字节" << std::endl;

    // 2. SIMD对齐
    std::cout << "\nSIMD对齐:" << std::endl;
    SimdVector v1 = {{1.0f, 2.0f, 3.0f, 4.0f}};
    std::cout << "  sizeof(SimdVector) = " << sizeof(SimdVector) << std::endl;
    std::cout << "  alignof(SimdVector) = " << alignof(SimdVector) << std::endl;
    std::cout << "  地址16字节对齐: "
              << (reinterpret_cast<uintptr_t>(&v1) % 16 == 0 ? "是" : "否")
              << std::endl;

    // 3. type_traits中的alignment_of
    std::cout << "\nstd::alignment_of:" << std::endl;
    std::cout << "  alignment_of<int>: " << std::alignment_of<int>::value << std::endl;
    std::cout << "  alignment_of<double>: " << std::alignment_of<double>::value << std::endl;
    std::cout << "  alignment_of<CacheLineAligned>: "
              << std::alignment_of<CacheLineAligned>::value << std::endl;

    // 4. 数组中每个元素的对齐
    std::cout << "\n对齐数组:" << std::endl;
    AlignedData arr[3];
    for (int i = 0; i < 3; ++i) {
        uintptr_t addr = reinterpret_cast<uintptr_t>(&arr[i]);
        std::cout << "  arr[" << i << "] 地址: " << addr
                  << " (32对齐: " << (addr % 32 == 0 ? "是" : "否") << ")"
                  << std::endl;
    }

    // 5. AlignedStorage
    std::cout << "\nAlignedStorage<double>:" << std::endl;
    AlignedStorage<double> storage;
    std::cout << "  sizeof = " << sizeof(storage) << std::endl;
    std::cout << "  alignof = " << alignof(decltype(storage)) << std::endl;
}

// ============================================================
// 易错点与陷阱
// ============================================================
void pitfalls() {
    std::cout << "\n===== 易错点与陷阱 =====" << std::endl;

    // 陷阱1: alignas不能减小对齐
    // alignas(1) double d; // 某些编译器可能忽略或报错
    // double的自然对齐是8，不能减小到1
    std::cout << "陷阱1: alignas不能减小自然对齐" << std::endl;
    std::cout << "  alignas(1) double: 可能被忽略或报错" << std::endl;
    std::cout << "  对齐只能增大（或保持不变）" << std::endl;

    // 陷阱2: alignas的值必须是2的幂
    // alignas(3) int x; // 错误! 3不是2的幂
    // alignas(0) int y; // 被忽略
    std::cout << "\n陷阱2: alignas的值必须是2的幂" << std::endl;
    std::cout << "  有效值: 1, 2, 4, 8, 16, 32, 64, ..." << std::endl;

    // 陷阱3: 动态分配(new)可能不遵守对齐
    // C++11中 new 只保证 max_align_t 级别的对齐
    // C++17引入了对齐感知的new
    std::cout << "\n陷阱3: new表达式的对齐限制" << std::endl;
    std::cout << "  C++11/14: new只保证 alignof(max_align_t)="
              << alignof(std::max_align_t) << " 字节对齐" << std::endl;
    std::cout << "  超过此值的对齐需要自定义分配器" << std::endl;
    std::cout << "  C++17: new支持任意对齐" << std::endl;

    // 陷阱4: 结构体对齐和填充
    struct Padded {
        char c;   // 1字节
        // 3字节填充
        int i;    // 4字节
    };
    struct Packed {
        int i;    // 4字节
        char c;   // 1字节
        // 3字节填充（末尾）
    };
    std::cout << "\n陷阱4: 成员顺序影响结构体大小" << std::endl;
    std::cout << "  Padded {char, int}: sizeof = " << sizeof(Padded) << std::endl;
    std::cout << "  Packed {int, char}: sizeof = " << sizeof(Packed) << std::endl;
    std::cout << "  按大小降序排列成员可减少填充" << std::endl;

    // 陷阱5: 对齐对性能的影响
    std::cout << "\n陷阱5: 过度对齐浪费内存" << std::endl;
    std::cout << "  alignas(64)的小对象会浪费大量空间" << std::endl;
    std::cout << "  只在确实需要时才使用高对齐（如避免伪共享、SIMD）" << std::endl;

    std::cout << "\n最佳实践:" << std::endl;
    std::cout << "  1. 多线程共享数据用缓存行对齐(64字节)" << std::endl;
    std::cout << "  2. SIMD数据用16/32字节对齐" << std::endl;
    std::cout << "  3. 按成员大小降序排列减少填充" << std::endl;
    std::cout << "  4. 用alignof检查而非假设对齐大小" << std::endl;
}

// ============================================================
// 主函数
// ============================================================
int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "  C++11 特性: alignof 与 alignas" << std::endl;
    std::cout << "========================================" << std::endl;

    basic_usage();
    advanced_usage();
    pitfalls();

    return 0;
}
