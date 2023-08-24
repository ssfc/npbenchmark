import imageio

# 读取所有的 PNG 图片
image_files = ['cbs_1.drawio.png', 'cbs_3.drawio.png', 'cbs_5.drawio.png', 'cbs_7.drawio.png', 'cbs_9.drawio.png',
               'cbs_11.drawio.png']

# 生成 GIF 动画
imageio.mimsave('animation.gif', [imageio.imread(file) for file in image_files], duration=1)
