#!/usr/bin/env python

import numpy as np
import matplotlib.pyplot as plt
from scipy.optimize import curve_fit
from numpy import array
import math

data=array( [ 0.000007000, 0.000005000, 0.000016000, 0.000054000, 0.000230000, 0.001089000, 0.004259000, 0.017484000, 0.056744000, 0.246765000, 0.887034000, 0.465257000, 2.934070000, 19.010652000, 13.512952000 ] )
n = array( [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 ] )

plt.figure()
plt.scatter(n, data)
plt.plot(n, data, label="Branch and Bound")
plt.legend()
plt.savefig( "./graphics/branch_and_bound.png" )