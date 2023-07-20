import tkinter as tk
import time


def draw_map(canvas, map_data):
    block_size = 40  # 每个方块的大小
    for row in range(len(map_data)):
        for col in range(len(map_data[0])):
            if map_data[row][col] == '.':
                color = 'white'  # 可通行方块为白色
            else:
                color = 'black'  # 障碍物为黑色

            canvas.create_rectangle(col * block_size, row * block_size, (col + 1) * block_size, (row + 1) * block_size,
                                    fill=color)


def draw_path(canvas, path_data):
    block_size = 40
    for i in range(len(path_data)):
        row, col = path_data[i]
        canvas.create_oval(col * block_size + 5, row * block_size + 5, col * block_size + block_size - 5,
                           row * block_size + block_size - 5, fill='red')
        canvas.update()
        time.sleep(0.5)  # 路径点之间的时间间隔


def main():
    # 读取map.txt内容
    with open('cmake-build-debug/map.txt', 'r') as file:
        rows, cols = map(int, file.readline().strip().split())  # Read the number of rows and columns
        map_data = [list(file.readline().strip()) for _ in range(rows)]  # Read each row and convert it to a list

    # 读取path.txt内容
    with open('cmake-build-debug/path.txt', 'r') as file:
        num_points = int(file.readline().strip())
        path_data = [tuple(map(int, file.readline().strip().split())) for _ in range(num_points)]

    root = tk.Tk()  # 创建一个Tkinter的主窗口，通常称为root窗口。这个窗口是Tkinter程序的最顶层窗口。
    root.title('Agent Path Animation')  # 设置主窗口的标题为'Agent Path Animation'，这将显示在窗口的标题栏上。

    # 创建一个Canvas组件，设置宽度和高度为地图的宽度和高度（每个方块的大小乘以地图的行数和列数）的大小。这样，Canvas的大小将适应地图的大小。
    canvas = tk.Canvas(root, width=len(map_data[0]) * 40, height=len(map_data) * 40)
    canvas.pack()  # 将Canvas组件添加到主窗口中。通过pack()方法，Canvas将会自动根据窗口大小进行适应和布局。

    draw_map(canvas, map_data)
    draw_path(canvas, path_data)

    root.mainloop()  # 启动Tkinter的事件循环，使窗口显示并等待用户交互。这个方法将会一直运行，直到用户关闭了窗口。


if __name__ == "__main__":
    main()
