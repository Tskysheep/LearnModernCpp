#!/usr/bin/env python3
"""
Batch-generate Docusaurus doc pages from C++ source files.
For each .cpp file, creates:
  - Chinese doc page under website/docs/cppXX/
  - English doc page under website/i18n/en/docusaurus-plugin-content-docs/current/cppXX/
"""

import os
import re
import glob

BASE = "G:/Test/Claude/LearnCpp"
WEBSITE = os.path.join(BASE, "website")
DOCS_DIR = os.path.join(WEBSITE, "docs")
EN_DIR = os.path.join(WEBSITE, "i18n/en/docusaurus-plugin-content-docs/current")

STANDARDS = ["cpp11", "cpp14", "cpp17", "cpp20", "cpp23"]

# Mapping of cpp file basename → (Chinese title, English title, Chinese desc, English desc, tags, cppreference_url)
# We'll extract from the file header comment automatically.

def extract_header_info(filepath):
    """Extract feature name and description from the header comment of a .cpp file."""
    with open(filepath, "r", encoding="utf-8") as f:
        content = f.read()

    # Extract Chinese feature name from header
    zh_name = ""
    m = re.search(r"特性名称:\s*(.+)", content)
    if m:
        zh_name = m.group(1).strip()

    # Extract standard
    std = ""
    m = re.search(r"所属标准:\s*(.+)", content)
    if m:
        std = m.group(1).strip()

    return zh_name, std, content


def cpp_to_md_name(cpp_filename):
    """Convert .cpp filename to .md filename: 01_auto_and_decltype.cpp → 01-auto-and-decltype.md"""
    name = os.path.splitext(cpp_filename)[0]
    name = name.replace("_", "-")
    return name + ".md"


def get_sidebar_position(filename):
    """Extract number prefix for sidebar ordering."""
    m = re.match(r"(\d+)", filename)
    if m:
        return int(m.group(1)) + 1  # +1 because index.md is position 1
    return 99


# English translations for common C++ feature names
ZH_TO_EN = {
    "auto类型推导与decltype": "auto Type Deduction and decltype",
    "基于范围的for循环": "Range-based for Loop",
    "Lambda表达式": "Lambda Expressions",
    "nullptr": "nullptr",
    "强类型枚举": "Scoped Enums (enum class)",
    "统一初始化": "Uniform Initialization",
    "default和delete": "Defaulted and Deleted Functions",
    "override和final": "override and final",
    "委托构造函数": "Delegating Constructors",
    "继承构造函数": "Inheriting Constructors",
    "移动语义": "Move Semantics",
    "完美转发": "Perfect Forwarding",
    "noexcept": "noexcept Specification",
    "static_assert": "static_assert",
    "原始字符串字面量": "Raw String Literals",
    "Unicode字面量": "Unicode Literals",
    "用户自定义字面量": "User-Defined Literals",
    "类型别名与using": "Type Aliases (using)",
    "alignof与alignas": "alignof and alignas",
    "可变参数模板": "Variadic Templates",
    "constexpr": "constexpr",
    "type_traits类型萃取": "Type Traits",
    "initializer_list": "initializer_list",
    "智能指针": "Smart Pointers",
    "tuple元组": "std::tuple",
    "std::array": "std::array",
    "无序容器": "Unordered Containers",
    "std::function与std::bind": "std::function and std::bind",
    "forward_list": "std::forward_list",
    "引用包装器": "Reference Wrappers",
    "字符串转换": "String Conversions",
    "正则表达式": "Regular Expressions (regex)",
    "线程基础": "Thread Basics",
    "互斥量与锁": "Mutex and Locks",
    "条件变量": "Condition Variables",
    "future与promise": "std::future and std::promise",
    "原子操作": "Atomic Operations",
    "时间库chrono": "Chrono Library",
    "随机数库": "Random Number Library",
    "属性标记": "Attributes",
    # cpp14
    "泛型Lambda": "Generic Lambdas",
    "返回类型推导": "Return Type Deduction",
    "变量模板": "Variable Templates",
    "二进制字面量与数字分隔符": "Binary Literals and Digit Separators",
    "放宽的constexpr": "Relaxed constexpr",
    "std::make_unique": "std::make_unique",
    "[[deprecated]]属性": "[[deprecated]] Attribute",
    "共享定时互斥量": "Shared Timed Mutex",
    "整数序列": "Integer Sequence",
    "std::exchange": "std::exchange",
    "std::quoted": "std::quoted",
    # cpp17
    "结构化绑定": "Structured Bindings",
    "if/switch初始化语句": "if/switch Init Statements",
    "类模板参数推导(CTAD)": "Class Template Argument Deduction (CTAD)",
    "折叠表达式": "Fold Expressions",
    "constexpr if": "constexpr if",
    "内联变量": "Inline Variables",
    "嵌套命名空间": "Nested Namespaces",
    "__has_include": "__has_include",
    "std::optional": "std::optional",
    "std::variant": "std::variant",
    "std::any": "std::any",
    "std::string_view": "std::string_view",
    "std::byte": "std::byte",
    "std::invoke与std::apply": "std::invoke and std::apply",
    "文件系统库": "Filesystem Library",
    "并行算法": "Parallel Algorithms",
    "clamp/gcd/lcm": "clamp / gcd / lcm",
    "C++17属性": "C++17 Attributes",
    # cpp20
    "Concepts基础": "Concepts Basics",
    "Concepts进阶": "Concepts Advanced",
    "requires表达式": "requires Expressions",
    "简写函数模板": "Abbreviated Function Templates",
    "三路比较运算符": "Three-way Comparison (Spaceship Operator)",
    "指定初始化器": "Designated Initializers",
    "consteval": "consteval",
    "constinit": "constinit",
    "模板Lambda": "Template Lambdas",
    "聚合体的括号初始化": "Parenthesized Aggregate Initialization",
    "Ranges基础": "Ranges Basics",
    "Ranges视图": "Ranges Views",
    "自定义Range": "Custom Range",
    "协程基础": "Coroutines Basics",
    "协程生成器": "Coroutine Generator",
    "std::span": "std::span",
    "std::format": "std::format",
    "日历与时区": "Calendar and Time Zones",
    "source_location": "std::source_location",
    "位操作": "Bit Operations",
    "数学常量": "Math Constants",
    "字符串操作增强": "String Operations Enhancements",
    "关联容器contains": "Associative Container contains()",
    "std::jthread": "std::jthread",
    "barrier与latch": "std::barrier and std::latch",
    "C++20属性": "C++20 Attributes",
    # cpp23
    "if consteval": "if consteval",
    "多维下标运算符": "Multidimensional Subscript Operator",
    "size_t字面量后缀": "size_t Literal Suffix",
    "Lambda改进": "Lambda Improvements",
    "std::expected": "std::expected",
    "optional单子操作": "Optional Monadic Operations",
    "move_only_function": "std::move_only_function",
    "std::to_underlying": "std::to_underlying",
    "std::unreachable": "std::unreachable",
    "栈追踪": "Stack Trace",
    "std::byteswap": "std::byteswap",
    "C++23属性": "C++23 Attributes",
}

# cppreference URL hints based on feature name keywords
CPPREFERENCE_URLS = {
    "auto": "https://en.cppreference.com/w/cpp/language/auto",
    "decltype": "https://en.cppreference.com/w/cpp/language/decltype",
    "range_based_for": "https://en.cppreference.com/w/cpp/language/range-for",
    "lambda": "https://en.cppreference.com/w/cpp/language/lambda",
    "nullptr": "https://en.cppreference.com/w/cpp/language/nullptr",
    "enum_class": "https://en.cppreference.com/w/cpp/language/enum",
    "uniform_init": "https://en.cppreference.com/w/cpp/language/list_initialization",
    "default_delete": "https://en.cppreference.com/w/cpp/language/function#Deleted_functions",
    "override": "https://en.cppreference.com/w/cpp/language/override",
    "delegating": "https://en.cppreference.com/w/cpp/language/constructor",
    "inheriting": "https://en.cppreference.com/w/cpp/language/using_declaration",
    "move_semantics": "https://en.cppreference.com/w/cpp/language/move_constructor",
    "perfect_forward": "https://en.cppreference.com/w/cpp/utility/forward",
    "noexcept": "https://en.cppreference.com/w/cpp/language/noexcept_spec",
    "static_assert": "https://en.cppreference.com/w/cpp/language/static_assert",
    "raw_string": "https://en.cppreference.com/w/cpp/language/string_literal",
    "unicode": "https://en.cppreference.com/w/cpp/language/string_literal",
    "user_defined_literal": "https://en.cppreference.com/w/cpp/language/user_literal",
    "type_alias": "https://en.cppreference.com/w/cpp/language/type_alias",
    "alignof": "https://en.cppreference.com/w/cpp/language/alignof",
    "variadic": "https://en.cppreference.com/w/cpp/language/parameter_pack",
    "constexpr": "https://en.cppreference.com/w/cpp/language/constexpr",
    "type_traits": "https://en.cppreference.com/w/cpp/header/type_traits",
    "initializer_list": "https://en.cppreference.com/w/cpp/utility/initializer_list",
    "smart_pointer": "https://en.cppreference.com/w/cpp/memory/unique_ptr",
    "tuple": "https://en.cppreference.com/w/cpp/utility/tuple",
    "array": "https://en.cppreference.com/w/cpp/container/array",
    "unordered": "https://en.cppreference.com/w/cpp/container/unordered_map",
    "function_bind": "https://en.cppreference.com/w/cpp/utility/functional/function",
    "forward_list": "https://en.cppreference.com/w/cpp/container/forward_list",
    "ref_wrapper": "https://en.cppreference.com/w/cpp/utility/functional/reference_wrapper",
    "string_conv": "https://en.cppreference.com/w/cpp/string/basic_string/stol",
    "regex": "https://en.cppreference.com/w/cpp/regex",
    "thread": "https://en.cppreference.com/w/cpp/thread/thread",
    "mutex": "https://en.cppreference.com/w/cpp/thread/mutex",
    "condition_variable": "https://en.cppreference.com/w/cpp/thread/condition_variable",
    "future": "https://en.cppreference.com/w/cpp/thread/future",
    "atomic": "https://en.cppreference.com/w/cpp/atomic/atomic",
    "chrono": "https://en.cppreference.com/w/cpp/chrono",
    "random": "https://en.cppreference.com/w/cpp/numeric/random",
    "attributes": "https://en.cppreference.com/w/cpp/language/attributes",
    # cpp14
    "generic_lambda": "https://en.cppreference.com/w/cpp/language/lambda",
    "return_type_deduction": "https://en.cppreference.com/w/cpp/language/function#Return_type_deduction",
    "variable_template": "https://en.cppreference.com/w/cpp/language/variable_template",
    "binary_literal": "https://en.cppreference.com/w/cpp/language/integer_literal",
    "make_unique": "https://en.cppreference.com/w/cpp/memory/unique_ptr/make_unique",
    "deprecated": "https://en.cppreference.com/w/cpp/language/attributes/deprecated",
    "shared_timed_mutex": "https://en.cppreference.com/w/cpp/thread/shared_timed_mutex",
    "integer_sequence": "https://en.cppreference.com/w/cpp/utility/integer_sequence",
    "exchange": "https://en.cppreference.com/w/cpp/utility/exchange",
    "quoted": "https://en.cppreference.com/w/cpp/io/manip/quoted",
    # cpp17
    "structured_binding": "https://en.cppreference.com/w/cpp/language/structured_binding",
    "if_init": "https://en.cppreference.com/w/cpp/language/if",
    "ctad": "https://en.cppreference.com/w/cpp/language/class_template_argument_deduction",
    "fold": "https://en.cppreference.com/w/cpp/language/fold",
    "constexpr_if": "https://en.cppreference.com/w/cpp/language/if#Constexpr_if",
    "inline_variable": "https://en.cppreference.com/w/cpp/language/inline",
    "nested_namespace": "https://en.cppreference.com/w/cpp/language/namespace",
    "has_include": "https://en.cppreference.com/w/cpp/preprocessor/include",
    "optional": "https://en.cppreference.com/w/cpp/utility/optional",
    "variant": "https://en.cppreference.com/w/cpp/utility/variant",
    "any": "https://en.cppreference.com/w/cpp/utility/any",
    "string_view": "https://en.cppreference.com/w/cpp/string/basic_string_view",
    "byte": "https://en.cppreference.com/w/cpp/types/byte",
    "invoke": "https://en.cppreference.com/w/cpp/utility/functional/invoke",
    "filesystem": "https://en.cppreference.com/w/cpp/filesystem",
    "parallel": "https://en.cppreference.com/w/cpp/algorithm/execution_policy_tag",
    "clamp": "https://en.cppreference.com/w/cpp/algorithm/clamp",
    # cpp20
    "concepts": "https://en.cppreference.com/w/cpp/language/constraints",
    "requires": "https://en.cppreference.com/w/cpp/language/requires",
    "abbreviated": "https://en.cppreference.com/w/cpp/language/function_template#Abbreviated_function_template",
    "spaceship": "https://en.cppreference.com/w/cpp/language/default_comparisons",
    "designated": "https://en.cppreference.com/w/cpp/language/aggregate_initialization",
    "consteval": "https://en.cppreference.com/w/cpp/language/consteval",
    "constinit": "https://en.cppreference.com/w/cpp/language/constinit",
    "template_lambda": "https://en.cppreference.com/w/cpp/language/lambda",
    "aggregate_parens": "https://en.cppreference.com/w/cpp/language/direct_initialization",
    "ranges": "https://en.cppreference.com/w/cpp/ranges",
    "coroutine": "https://en.cppreference.com/w/cpp/language/coroutines",
    "span": "https://en.cppreference.com/w/cpp/container/span",
    "format": "https://en.cppreference.com/w/cpp/utility/format/format",
    "calendar": "https://en.cppreference.com/w/cpp/chrono#Calendar",
    "source_location": "https://en.cppreference.com/w/cpp/utility/source_location",
    "bit": "https://en.cppreference.com/w/cpp/numeric/bit_cast",
    "math_const": "https://en.cppreference.com/w/cpp/numeric/constants",
    "string_op": "https://en.cppreference.com/w/cpp/string/basic_string/starts_with",
    "contains": "https://en.cppreference.com/w/cpp/container/map/contains",
    "jthread": "https://en.cppreference.com/w/cpp/thread/jthread",
    "barrier": "https://en.cppreference.com/w/cpp/thread/barrier",
    "latch": "https://en.cppreference.com/w/cpp/thread/latch",
    # cpp23
    "if_consteval": "https://en.cppreference.com/w/cpp/language/if#Consteval_if",
    "multidim_subscript": "https://en.cppreference.com/w/cpp/language/operator_member_access#Built-in_subscript_operator",
    "size_t_literal": "https://en.cppreference.com/w/cpp/language/integer_literal",
    "expected": "https://en.cppreference.com/w/cpp/utility/expected",
    "optional_monadic": "https://en.cppreference.com/w/cpp/utility/optional/and_then",
    "move_only_function": "https://en.cppreference.com/w/cpp/utility/functional/move_only_function",
    "to_underlying": "https://en.cppreference.com/w/cpp/utility/to_underlying",
    "unreachable": "https://en.cppreference.com/w/cpp/utility/unreachable",
    "stacktrace": "https://en.cppreference.com/w/cpp/utility/basic_stacktrace",
    "byteswap": "https://en.cppreference.com/w/cpp/numeric/byteswap",
}


def find_cppreference_url(filename):
    """Find the best matching cppreference URL for a .cpp filename."""
    basename = os.path.splitext(os.path.basename(filename))[0].lower()
    # Remove number prefix
    basename_no_num = re.sub(r"^\d+_", "", basename)

    for key, url in CPPREFERENCE_URLS.items():
        if key in basename_no_num or basename_no_num in key:
            return url

    # Fallback
    return "https://en.cppreference.com/w/cpp"


def get_english_title(zh_name):
    """Get English title from Chinese feature name."""
    if zh_name in ZH_TO_EN:
        return ZH_TO_EN[zh_name]
    # Fallback: return the Chinese name itself
    return zh_name


def escape_mdx(text):
    """Escape MDX-problematic characters in text outside code blocks."""
    # Replace bare <=> with `<=>`
    text = re.sub(r'(?<!`)(<=>)(?!`)', r'`\1`', text)
    # Replace bare <word> patterns (like <regex>, <random>) with `<word>`
    text = re.sub(r'(?<!`)(<[a-zA-Z_]\w*>)(?!`)', r'`\1`', text)
    return text


def escape_frontmatter(text):
    """Escape angle brackets in YAML frontmatter strings."""
    text = text.replace("<=>", "⟺")
    text = re.sub(r'<([a-zA-Z_]\w*)>', r'⟨\1⟩', text)
    return text


def generate_zh_page(zh_name, std, code, filename, sidebar_pos, cppreference_url):
    """Generate Chinese doc page content."""
    basename = os.path.basename(filename)

    tags_str = f"[{std.lower().replace('++', 'pp') if std else 'cpp'}"
    keyword = re.sub(r"^\d+_", "", os.path.splitext(basename)[0])
    tags_str += f", {keyword}]"

    # Escape for frontmatter (no angle brackets allowed in YAML strings)
    safe_zh = escape_frontmatter(zh_name)
    safe_desc = escape_frontmatter(f"{std} {zh_name} 教学示例")
    # Escape for MDX body
    mdx_zh = escape_mdx(zh_name)
    mdx_intro = escape_mdx(f"{std} 标准引入的 {zh_name} 特性完整教学示例。")
    mdx_ref = escape_mdx(zh_name)

    # slug preserves number prefix (Docusaurus 3 strips it by default)
    slug = os.path.splitext(os.path.basename(cpp_to_md_name(basename)))[0]

    page = f"""---
sidebar_position: {sidebar_pos}
slug: {slug}
title: "{safe_zh}"
description: "{safe_desc}"
tags: {tags_str}
---

# {mdx_zh}

{mdx_intro}

## 完整源码

```cpp title="{os.path.dirname(filename).split('/')[-1]}/{basename}"
{code}```

## 参考链接

- [cppreference: {mdx_ref}]({cppreference_url})
"""
    return page


def generate_en_page(zh_name, en_name, std, code, filename, sidebar_pos, cppreference_url):
    """Generate English doc page content."""
    basename = os.path.basename(filename)

    tags_str = f"[{std.lower().replace('++', 'pp') if std else 'cpp'}"
    keyword = re.sub(r"^\d+_", "", os.path.splitext(basename)[0])
    tags_str += f", {keyword}]"

    safe_en = escape_frontmatter(en_name)
    safe_desc = escape_frontmatter(f"{std} {en_name} teaching example")
    mdx_en = escape_mdx(en_name)
    mdx_intro = escape_mdx(f"Complete teaching example for the {en_name} feature introduced in {std}.")

    page = f"""---
sidebar_position: {sidebar_pos}
title: "{safe_en}"
description: "{safe_desc}"
tags: {tags_str}
---

# {mdx_en}

{mdx_intro}

## Full Source Code

```cpp title="{os.path.dirname(filename).split('/')[-1]}/{basename}"
{code}```

## Reference

- [cppreference: {escape_mdx(en_name)}]({cppreference_url})
"""
    return page


def main():
    total_created = 0

    for std_dir in STANDARDS:
        src_dir = os.path.join(BASE, std_dir)
        zh_docs_dir = os.path.join(DOCS_DIR, std_dir)

        # Ensure directories exist
        os.makedirs(zh_docs_dir, exist_ok=True)

        cpp_files = sorted(glob.glob(os.path.join(src_dir, "*.cpp")))

        for cpp_file in cpp_files:
            cpp_file = cpp_file.replace("\\", "/")
            basename = os.path.basename(cpp_file)
            md_name = cpp_to_md_name(basename)
            sidebar_pos = get_sidebar_position(basename)

            zh_name, std, code = extract_header_info(cpp_file)
            if not zh_name:
                zh_name = re.sub(r"^\d+_", "", os.path.splitext(basename)[0]).replace("_", " ")

            en_name = get_english_title(zh_name)
            cppreference_url = find_cppreference_url(cpp_file)

            # Generate Chinese page
            zh_page = generate_zh_page(zh_name, std, code, cpp_file, sidebar_pos, cppreference_url)
            zh_path = os.path.join(zh_docs_dir, md_name)
            with open(zh_path, "w", encoding="utf-8") as f:
                f.write(zh_page)

            total_created += 1
            print(f"  Created: {std_dir}/{md_name}")

        print(f"[{std_dir}] {len(cpp_files)} files → {len(cpp_files)} docs")

    print(f"\nTotal: {total_created} doc pages created.")


if __name__ == "__main__":
    main()
