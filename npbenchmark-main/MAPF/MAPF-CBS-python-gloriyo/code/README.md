This is the final project for cmpt 417

For running CBS use command python3 run_experiments.py --instance instances/test_1.txt --solver CBS

For running ICBS with prioritized conflict and bypass 
use command python3 run_experiments.py --instance instances/test_1.txt --solver ICBS_CB

For running ICBS (CBS with prioritized conflict, bypass and meta-agent) use command python3 run_experiments.py --instance instances/test_1.txt --solver ICBS

If want use disjoint splitting please add command --disjoint on the command line

外面那个readme里面有错误，你说坑不吭。(2023年7月28日)

### Q: import argparse是干啥的？

`import argparse` 是 Python 中用于处理命令行参数的标准库。它提供了一个简单而灵活的方法，让你可以在命令行中传递参数给你的 Python 脚本，从而实现在运行脚本时动态配置程序的行为。

通过 `argparse`，你可以定义命令行选项和参数，并在运行脚本时解析这些选项和参数，从而获取用户传入的值。这使得你的脚本可以更灵活地适应不同的使用场景，而不需要硬编码固定的参数。

`argparse` 模块可以帮助你实现以下功能：

1. 定义命令行选项：你可以定义需要接受哪些命令行选项，包括标志选项（例如 `--verbose`）和位置参数（例如输入文件名）。

2. 解析命令行参数：当你运行脚本时，`argparse` 可以解析命令行参数并将其转换为 Python 对象，如列表、字典等。

3. 提供帮助信息：你可以定义命令行选项的描述，以及生成自动生成的帮助文档，方便用户了解如何正确使用你的脚本。

4. 错误处理：`argparse` 可以检查用户输入的参数是否符合预期的类型和格式，并提供错误处理和提示信息。

下面是一个简单的例子，演示了如何使用 `argparse` 来接受用户传入的两个整数，并计算它们的和：

```python
import argparse

parser = argparse.ArgumentParser(description='Calculate the sum of two integers.')
parser.add_argument('num1', type=int, help='The first integer.')
parser.add_argument('num2', type=int, help='The second integer.')

args = parser.parse_args()

result = args.num1 + args.num2
print(f'The sum of {args.num1} and {args.num2} is {result}.')
```

在运行上述脚本时，你可以在命令行中输入两个整数作为参数，然后脚本会解析这两个参数并计算它们的和。例如：
```
$ python script.py 5 3
The sum of 5 and 3 is 8.
```

`argparse` 是 Python 中处理命令行参数非常方便和常用的工具，它使得编写命令行工具更加易于使用和维护。

### Q: import glob是干啥的？

`import glob` 是 Python 中用于进行文件路径匹配的标准库模块。它提供了一个简单的方法来查找匹配指定模式的文件路径。

使用 `glob` 模块可以很方便地获取文件路径列表，而无需手动编写复杂的文件搜索代码。你可以使用通配符来匹配文件名，从而找到满足特定模式的文件。

`glob` 模块中最常用的函数是 `glob.glob()`，它接受一个字符串参数，该参数包含要匹配的文件路径模式。函数返回一个包含匹配文件路径的列表。

下面是一个简单的例子，演示了如何使用 `glob` 模块来获取当前目录下所有以 `.txt` 结尾的文件路径：

```python
import glob

# 获取当前目录下所有以 .txt 结尾的文件路径
txt_files = glob.glob('*.txt')

# 打印文件路径列表
print(txt_files)
```

假设当前目录下有三个文件：`file1.txt`、`file2.txt` 和 `other.txt`。运行上述脚本后，将会输出：
```
['file1.txt', 'file2.txt']
```

可以看到，`glob.glob('*.txt')` 返回了所有以 `.txt` 结尾的文件路径的列表。

`glob` 模块对于需要在文件系统中查找符合特定模式的文件时非常有用。它可以帮助你快速地获取匹配的文件路径列表，而不需要编写繁琐的遍历和匹配代码。
