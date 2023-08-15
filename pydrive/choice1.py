#!/usr/bin/env python3
import random
import sys

sentence = r"李允泽和乐谦都是乖宝宝"

random.seed(1)

counter = 0
print("请猜一个2以内的数字:")

while True:
    num = random.randint(1, 2)
    #print(num)
    #counter += 1
    #if counter >= 10000:
    #    break

    line = sys.stdin.readline().strip()
    if line == 'q':
        break
    elif line.isdigit():
        i = int(line)
        if num == i:
            print("*******恭喜你猜中了!******")
            print(sentence)
        else:
            print("再接再砺，下次就猜中了!")
    else:
        print("Illegal input.")

    print("中奖数字是{}".format(num))
    print("请猜一个2以内的数字:")
