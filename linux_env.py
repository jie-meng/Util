import os

os.system('docker run --rm -it -v {}:/app jmengxy/util bash'.format(os.getcwd()))
