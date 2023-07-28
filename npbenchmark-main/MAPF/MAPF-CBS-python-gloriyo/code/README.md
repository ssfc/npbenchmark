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

### Q: from pathlib import Path是干啥的？

`from pathlib import Path` 是 Python 中用于操作文件和目录路径的标准库模块。Python 3.4 及以后的版本引入了 `pathlib` 模块，提供了一种更简洁和面向对象的方式来处理文件路径。

通过 `pathlib` 模块，你可以使用 `Path` 类来创建、检查、操作文件和目录的路径。`Path` 类可以代表文件路径或目录路径，并且支持许多常用的文件操作，如读取、写入、复制、移动等，以及查询文件属性和状态。

使用 `Path` 类可以更加直观和简单地处理文件路径，而不需要手动处理字符串的拼接和分隔。此外，`pathlib` 模块还提供了跨平台的路径处理，因此你可以在不同的操作系统上使用相同的代码。

下面是一个简单的例子，演示了如何使用 `Path` 类来创建和读取文件：

```python
from pathlib import Path

# 创建一个文件路径对象
file_path = Path('example.txt')

# 写入文件内容
with file_path.open('w') as file:
    file.write('Hello, world!')

# 读取文件内容
with file_path.open() as file:
    content = file.read()
    print(content)
```

在上述例子中，`Path('example.txt')` 创建了一个文件路径对象，表示名为 `example.txt` 的文件。然后使用 `open()` 方法打开文件并进行读写操作。

使用 `pathlib` 模块的 `Path` 类可以更加方便地处理文件路径和文件操作，特别适用于处理复杂的文件和目录操作场景。

### Q: 为什么不直接用open，而要用pathlib.open?

在 Python 中，使用 `open` 内置函数来打开文件是一种常见的方法，它可以直接接受文件路径字符串作为参数，并返回一个文件对象，用于读取或写入文件内容。

然而，使用 `pathlib` 模块中的 `Path` 类提供的 `open` 方法有一些优势：

1. 更加面向对象：`pathlib` 提供了一种更面向对象的方式来处理文件路径。通过创建 `Path` 对象，你可以更直观地表示文件路径，而不需要担心文件路径字符串的拼接和分隔。

2. 跨平台支持：`pathlib` 提供了跨平台的路径处理。无论在 Windows、Linux 还是 macOS 等操作系统上运行，都可以使用相同的代码，而不需要关心不同操作系统的路径分隔符。

3. 更简洁的路径操作：`pathlib` 提供了一些方法来处理文件路径，比如可以使用 `Path` 对象的 `joinpath` 方法来连接路径，或者使用 `parent` 属性获取父目录路径，这些操作更加简洁和直观。

4. 更好的错误处理：使用 `pathlib` 的 `Path` 类可以更好地处理文件路径不存在或其他错误情况。例如，使用 `Path` 对象的 `open` 方法时，如果文件路径不存在，它会自动抛出 `FileNotFoundError` 异常，而不需要显式地检查文件是否存在。

综上所述，虽然直接使用 `open` 内置函数是一种简单的方法来打开文件，但是使用 `pathlib` 模块提供的 `Path` 类可以更加灵活、面向对象，且在处理文件路径时更加优雅和方便，尤其适用于复杂的文件操作场景。如果你的代码需要频繁进行文件路径操作，那么使用 `pathlib` 可能会更加方便和推荐。
