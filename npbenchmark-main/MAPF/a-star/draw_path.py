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

            canvas.create_rectangle(col*block_size, row*block_size, (col+1)*block_size, (row+1)*block_size, fill=color)

def draw_path(canvas, path_data):
    block_size = 40
    for i in range(len(path_data)):
        row, col = path_data[i]
        canvas.create_oval(col * block_size + 5, row * block_size + 5, col * block_size + block_size - 5, row * block_size + block_size - 5, fill='red')
        canvas.update()
        time.sleep(0.5)  # 路径点之间的时间间隔

def main():
    # 读取map.txt内容
    with open('map.txt', 'r') as file:
        map_data = [list(line.strip()) for line in file.readlines()]

    # 读取path.txt内容
    with open('path.txt', 'r') as file:
        num_points = int(file.readline().strip())
        path_data = [tuple(map(int, file.readline().strip().split())) for _ in range(num_points)]

    root = tk.Tk()
    root.title('Agent Path Animation')

    canvas = tk.Canvas(root, width=len(map_data[0]) * 40, height=len(map_data) * 40)
    canvas.pack()

    draw_map(canvas, map_data)
    draw_path(canvas, path_data)

    root.mainloop()

if __name__ == "__main__":
    main()
