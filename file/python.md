#

- 语法创建链表
```
f = [ i for i in range(0,10) ]
f = [ x + y for x in range(0,10) for y in range(0,20) ]
```

- 类的继承
```
#!/bin/bash

import os
import time


class clock(object):
    '''time clock'''
    def __init__(self,hour=0,minute=0,second=0):
        self.hour=hour
        self.minute=minute
        self.second=second
    
    def run(self):
        self.second += 1
        if self.second == 60:
            self.second = 0
            self.minute += 1
            if self.minute == 60:
                self.minute = 0
                self.hour += 1
                if self.hour == 24:
                    self.hour = 0

    def show(self):
        print("%02d:%02d:%02d" % (self.hour,self.minute,self.second))

class clock1(clock):
    def __init__(self,year,month,day):
        #self.hour = 10
        #self.minute = 46
        #self.second = 4
        #super().__init__(self.hour,self.minute,self.second)
        #注：上下两种方式都可以，但是这都是python3的格式
        super().__init__(10,46,4)
        self.year = year
        self.month = month
        self.day = day

    def run(self):
        super().run()


    def show(self):
    	#如果要调用子类的show则
    	super().show()
        print("%04d-%02d-%02d %02d:%02d:%02d" % (self.year,self.month,self.day,self.hour,self.minute,self.second))



def main():
    clocka = clock(23,59,50)
    clockb = clock1(2019,8,28)
    while True:
        clocka.show()
        clockb.show()
        time.sleep(1)
        clocka.run()
        clockb.run()

if __name__ == '__main__':
    main()

```

