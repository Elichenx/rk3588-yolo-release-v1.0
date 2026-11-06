# VS Code 函数跳转

用 VS Code 打开项目根目录，安装推荐的 **C/C++** 和 **CMake Tools** 扩展，等待 C/C++ 索引完成。

- `F12` 或 `Ctrl + 单击`：跳转到定义。
- `Alt + F12`：查看定义。
- `Shift + F12`：查找引用。
- `Ctrl + Shift + O`：查看当前文件符号。

配置包含所有业务模块、RKNN 和 RK3588 RGA 的头文件路径；不索引其他芯片的 RGA 目录，避免同名头文件干扰。工作区使用 C/C++ 扩展处理跳转，并关闭 clangd，避免两个语言服务同时工作。

## Windows 本地浏览

使用 `Win32` 配置，通过 PATH 中的 `g++.exe` 获取标准库路径；若使用其他工具链，请修改 `c_cpp_properties.json` 中的 `compilerPath` 和 `intelliSenseMode`。

Windows 未安装的 Linux、OpenCV、MPP、FFmpeg 头文件仍会提示缺失。项目源码可通过符号索引进行基本跳转；需要完整语义分析和引用查找时，使用依赖齐全的 Linux 环境（推荐 Remote-SSH 连接 RK3588）。

## Linux / RK3588

使用 `Linux` 配置。在依赖齐全的环境执行以下命令，生成真实编译参数：

```bash
cmake -S . -B build -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
```

也可以运行 **CMake: Configure**，工作区配置会自动开启编译数据库导出。C/C++ 扩展读取 `build/compile_commands.json`，未匹配的文件使用手动配置的路径和 C11/C++17 标准。

若跳转尚未生效，运行 **Developer: Reload Window**；仍有旧索引时，运行 **C/C++: Reset IntelliSense Database** 后等待重新索引。
