# GDB

FQA

## 在调试中，如何查看寄存器里的值？
在调试过程中查看寄存器的值也依赖于你使用的调试工具和具体的环境。以下是在一些常见调试环境和平台中查看寄存器值的方法：

- GDB（GNU Debugger，常用于C/C++等）：
  - 在GDB命令行模式下，你可以使用 `info registers` 或者简写为 `i r`  命令来查看所有寄存器的当前值。

  - 如果只想查看特定寄存器的值，可以使用 `p/x $register_name` ，例如：`p/x $eax` 。

- IDEs，如Eclipse, Visual Studio等：
  - 在调试界面中通常有一个“寄存器”窗口或选项，展示当前的寄存器值。
  - 对于嵌入式开发，如STM32使用OpenOCD配合VSCode进行调试： 
  - 在调试视图中应该有一个"Registers"窗口，显示当前的寄存器值。

- MDK（Microcontroller Development Kit，常用于ARM Cortex-M系列）：
  - 在Debug模式下，会自动出现"Registers"窗口，显示当前的寄存器值。
  - 如果没有自动出现，可以通过菜单选项"View -> Registers Windows"来打开。

请注意，不同的调试工具和平台可能会有不同的具体操作步骤，以上提供的是一些通用的指导。
如果你使用的是其他特定的调试工具，建议查阅相关的用户手册或在线帮助以获取准确的操作方法。

## 使用gdb怎么查看寄存器里的值，请举例说明？
在GDB（GNU Debugger）中，你可以使用以下命令来查看寄存器里的值：

- 查看所有寄存器的值： `(gdb) info registers`
这个命令将会显示所有可用寄存器及其当前的值。

- 查看特定寄存器的值： `(gdb) p/x $register_name`
register_name 应替换为你想要查看的寄存器的名称。

例如，如果你想查看程序计数器（PC）寄存器的值，你可以使用以下命令：`(gdb) p/x $pc`

这将以十六进制格式显示PC寄存器的值。如果你希望以十进制格式查看，可以使用以下命令：`(gdb) p/d $pc`

# What's the step out command?

- step
- stepi 
- next
- nexti

- `finish` means the `step out`.