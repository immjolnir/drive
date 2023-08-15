#!/usr/bin/env python3
import random
import sys
from colorama import Fore, Style, Back, init


sentence = r"黄诗彤， 李允泽和乐谦都是乖宝宝"

init()
random.seed(1)

counter = 0

min_num = 1
max_num = 10

while True:
    num = random.randint(min_num, max_num)
    #print(num)
    #counter += 1
    #if counter >= 10000:
    #    break

    for i in range(2):
        print(Fore.GREEN + "请猜一个[{}, {}]的数字:".format(min_num, max_num))
        line = sys.stdin.readline().strip()
        if line == 'q':
            sys.exit(0)
        elif line.isdigit():
            i = int(line)
            if num == i:
                print(Fore.GREEN + "*******恭喜你猜中了!******")
                print(Fore.MAGENTA + sentence)
                break
            else:
                print(Fore.RED + "再接再砺，下次就猜中了!")
        else:
            print(Fore.RED + "Illegal input.")

    print(Fore.GREEN + "中奖数字是{}".format(num))

print(Fore.RESET, end="")

