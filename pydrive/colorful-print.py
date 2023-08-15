#!/usr/bin/env python3

# 三个子类
#Fore, 用于改变输出文本的颜色
#Style, 用于改变输出文本的亮度
#Back，用于改变输出文本的背景（即突显文本）

from colorama import Fore, Style, Back, init


init()

# 文本颜色
print('文本颜色')
print(Fore.RED + 'red text')
print('default color')
print(Fore.GREEN + 'green text')
print(Fore.MAGENTA + 'magenta text')
print(Fore.RESET + 'Back to normal')
print('default color')
print(Fore.RED, end='')
print('red text1')
print('red text2')
print(Fore.RESET + 'Back to normal')
print('default color')

#
print('*'*20)
print('文本背景颜色')
print(Back.RED + "some red background")
print(Back.GREEN + "some green background")
print(Back.MAGENTA + "some magenta background")
print(Back.RESET + "Back to normal")
