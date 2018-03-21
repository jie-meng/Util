import os

os.system('docker run --rm -it -v {}:/app jmengxy/util sh'.format(os.getcwd()))
