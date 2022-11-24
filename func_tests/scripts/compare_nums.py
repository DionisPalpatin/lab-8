#!/bin/python3
# -*- coding: UTF-8 -*-

import sys

test_data_list = [int(float(el)) for el in sys.argv[1].split()]
real_data_list = [int(float(el)) for el in sys.argv[2].split()]

exit_code = 0
if test_data_list != real_data_list:
    exit_code = 1
    # print(test_data_list)
    # print(real_data_list)

sys.exit(exit_code)
