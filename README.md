# LearnModernCpp

现代 C++ 完整教学示例 — 从 C++11 到 C++23。

## 在线浏览

**[>>>>>>>>>在线浏览<<<<<<<<<](https://tskysheep.github.io/LearnModernCpp/)**

## 项目简介

107 个独立可编译的 C++ 示例文件，覆盖 C++11 至 C++23 的核心特性，每个文件包含基础用法、进阶用法和常见陷阱。

| 标准 | 示例数 | 核心内容 |
|------|:------:|----------|
| C++11 | 40 | auto、Lambda、移动语义、智能指针、并发 |
| C++14 | 11 | 泛型 Lambda、变量模板、relaxed constexpr |
| C++17 | 18 | 结构化绑定、optional/variant、if constexpr、filesystem |
| C++20 | 26 | Concepts、Ranges、Coroutines、format、jthread |
| C++23 | 12 | expected、print、推导 this、多维下标运算符 |

## 构建

```bash
cmake -B build -G "MinGW Makefiles"
cmake --build build
```

可选择性构建单个标准：

```bash
cmake -B build -DBUILD_CPP11=ON -DBUILD_CPP14=OFF -DBUILD_CPP17=OFF -DBUILD_CPP20=OFF -DBUILD_CPP23=OFF
```

## 项目结构

```
cpp11/          # C++11 示例 (40 个)
cpp14/          # C++14 示例 (11 个)
cpp17/          # C++17 示例 (18 个)
cpp20/          # C++20 示例 (26 个)
cpp23/          # C++23 示例 (12 个)
website/        # Docusaurus 文档站点
```

## License

[MIT](LICENSE)
