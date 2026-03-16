/**
 * ============================================================
 * 特性名称: 类型特征 (Type Traits)
 * 所属标准: C++11
 * ============================================================
 *
 * [是什么]: <type_traits> 提供编译期类型内省和类型变换工具，
 *          可以在编译时查询和操作类型的属性。
 *
 * [为什么引入]: 泛型编程中经常需要根据类型属性做出不同决策。
 *   C++03 中需要手写复杂的模板特化来实现类型检查，type_traits
 *   提供了标准化的、经过充分测试的类型查询工具。
 *
 * [关键要点]:
 *   - 类型查询: is_integral, is_floating_point, is_pointer, is_same 等
 *   - 类型变换: remove_const, remove_reference, add_pointer, decay 等
 *   - 条件选择: conditional, enable_if
 *   - 所有操作在编译期完成，零运行时开销
 * ============================================================
 */

#include <iostream>
#include <type_traits>
#include <string>
#include <vector>

// ---- 1. 基本用法 ----
void basic_usage() {
    std::cout << "---- 1. 基本用法 ----\n";

    // 基本类型查询
    std::cout << "is_integral<int>: "
              << std::is_integral<int>::value << "\n";
    std::cout << "is_integral<double>: "
              << std::is_integral<double>::value << "\n";
    std::cout << "is_floating_point<float>: "
              << std::is_floating_point<float>::value << "\n";
    std::cout << "is_pointer<int*>: "
              << std::is_pointer<int*>::value << "\n";
    std::cout << "is_reference<int&>: "
              << std::is_reference<int&>::value << "\n";
    std::cout << "is_const<const int>: "
              << std::is_const<const int>::value << "\n";

    // is_same: 检查两个类型是否相同
    std::cout << "\nis_same<int, int>: "
              << std::is_same<int, int>::value << "\n";
    std::cout << "is_same<int, long>: "
              << std::is_same<int, long>::value << "\n";
    std::cout << "is_same<int, int32_t>: "
              << std::is_same<int, int32_t>::value << "\n";

    // 类型属性查询
    std::cout << "\nis_class<std::string>: "
              << std::is_class<std::string>::value << "\n";
    std::cout << "is_arithmetic<double>: "
              << std::is_arithmetic<double>::value << "\n";
    std::cout << "is_array<int[5]>: "
              << std::is_array<int[5]>::value << "\n";
    std::cout << "is_void<void>: "
              << std::is_void<void>::value << "\n";

    // 复合类型查询
    std::cout << "\nis_trivially_copyable<int>: "
              << std::is_trivially_copyable<int>::value << "\n";
    std::cout << "is_trivially_copyable<std::string>: "
              << std::is_trivially_copyable<std::string>::value << "\n";
}

// ---- 2. 进阶用法 ----

// 类型变换示例
void type_transformations() {
    std::cout << "\n类型变换:\n";

    // remove_const: 移除 const
    typedef std::remove_const<const int>::type NoConstInt;
    std::cout << "remove_const<const int> 是 int: "
              << std::is_same<NoConstInt, int>::value << "\n";

    // remove_reference: 移除引用
    typedef std::remove_reference<int&>::type NoRefInt;
    std::cout << "remove_reference<int&> 是 int: "
              << std::is_same<NoRefInt, int>::value << "\n";

    // add_pointer: 添加指针
    typedef std::add_pointer<int>::type IntPtr;
    std::cout << "add_pointer<int> 是 int*: "
              << std::is_same<IntPtr, int*>::value << "\n";

    // decay: 模拟按值传递的类型退化
    // 数组退化为指针，函数退化为函数指针，移除 cv 和引用
    typedef std::decay<const int&>::type DecayedA;
    typedef std::decay<int[10]>::type DecayedB;
    std::cout << "decay<const int&> 是 int: "
              << std::is_same<DecayedA, int>::value << "\n";
    std::cout << "decay<int[10]> 是 int*: "
              << std::is_same<DecayedB, int*>::value << "\n";

    // conditional: 编译期条件选择类型
    typedef std::conditional<true, int, double>::type TrueType;
    typedef std::conditional<false, int, double>::type FalseType;
    std::cout << "\nconditional<true, int, double> 是 int: "
              << std::is_same<TrueType, int>::value << "\n";
    std::cout << "conditional<false, int, double> 是 double: "
              << std::is_same<FalseType, double>::value << "\n";
}

// enable_if: SFINAE 基础 —— 根据条件启用/禁用函数重载
template <typename T>
typename std::enable_if<std::is_integral<T>::value, std::string>::type
classify(T value) {
    return "整数类型: " + std::to_string(value);
}

template <typename T>
typename std::enable_if<std::is_floating_point<T>::value, std::string>::type
classify(T value) {
    return "浮点类型: " + std::to_string(value);
}

template <typename T>
typename std::enable_if<std::is_pointer<T>::value, std::string>::type
classify(T) {
    return "指针类型";
}

// 使用 type_traits 优化：对 trivially_copyable 类型使用 memcpy
template <typename T>
void smart_copy(const T* src, T* dst, size_t count) {
    if (std::is_trivially_copyable<T>::value) {
        std::cout << "  使用 memcpy 快速复制 (trivially copyable)\n";
        std::memcpy(dst, src, count * sizeof(T));
    } else {
        std::cout << "  使用逐个拷贝构造 (非 trivially copyable)\n";
        for (size_t i = 0; i < count; ++i) {
            dst[i] = src[i];
        }
    }
}

void advanced_usage() {
    std::cout << "\n---- 2. 进阶用法 ----\n";

    // 类型变换
    type_transformations();

    // enable_if 实现类型分发
    std::cout << "\nenable_if 类型分发:\n";
    std::cout << "  " << classify(42) << "\n";
    std::cout << "  " << classify(3.14) << "\n";
    int x = 0;
    std::cout << "  " << classify(&x) << "\n";

    // 基于 type_traits 的优化选择
    std::cout << "\n基于 type_traits 的优化:\n";
    int ints_src[] = {1, 2, 3};
    int ints_dst[3];
    smart_copy(ints_src, ints_dst, 3);
    std::cout << "  结果: " << ints_dst[0] << ", " << ints_dst[1]
              << ", " << ints_dst[2] << "\n";
}

// ---- 3. 常见陷阱与最佳实践 ----
void pitfalls() {
    std::cout << "\n---- 3. 常见陷阱与最佳实践 ----\n";

    // 陷阱1: const int& 不是 const 的!
    std::cout << "陷阱1: 引用本身没有 const 属性\n";
    std::cout << "  is_const<const int>: "
              << std::is_const<const int>::value << "\n";
    std::cout << "  is_const<const int&>: "
              << std::is_const<const int&>::value
              << " (引用不是 const，被引用的值才是)\n";
    // 需要先 remove_reference 再检查 const
    std::cout << "  is_const<remove_reference<const int&>::type>: "
              << std::is_const<std::remove_reference<const int&>::type>::value
              << " (正确做法)\n";

    // 陷阱2: is_same 不考虑隐式转换
    std::cout << "\n陷阱2: is_same 是严格类型匹配\n";
    std::cout << "  is_same<int, unsigned>: "
              << std::is_same<int, unsigned>::value
              << " (int 和 unsigned 是不同类型)\n";

    // 陷阱3: 数组和指针
    std::cout << "\n陷阱3: 数组不是指针\n";
    std::cout << "  is_same<int[5], int*>: "
              << std::is_same<int[5], int*>::value
              << " (数组和指针是不同类型)\n";
    std::cout << "  is_same<decay<int[5]>::type, int*>: "
              << std::is_same<std::decay<int[5]>::type, int*>::value
              << " (decay 后相同)\n";

    // 最佳实践
    std::cout << "\n最佳实践:\n";
    std::cout << "  1. 使用 static_assert + type_traits 做编译期检查\n";
    std::cout << "  2. C++14 提供 _v 后缀简化: is_integral_v<T>\n";
    std::cout << "  3. C++14 提供 _t 后缀简化: remove_const_t<T>\n";
    std::cout << "  4. enable_if 在 C++20 中可被 concepts 替代\n";

    // 编译期检查示例
    static_assert(std::is_integral<int>::value, "int 必须是整数类型");
    static_assert(!std::is_pointer<int>::value, "int 不能是指针");
}

int main() {
    std::cout << "===== 类型特征 (type_traits) - C++11 =====\n\n";
    basic_usage();
    advanced_usage();
    pitfalls();
    return 0;
}
