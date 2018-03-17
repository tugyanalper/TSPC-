from __future__ import print_function, division
import matplotlib.pyplot as plt
import numpy as np
import math


with open('results.txt', 'r') as f:
    generations = []
    minimumValues = []
    fImproved = []
    averageValues = []
    line = f.readline()
    while line:
        [generation_number, minval, fImp, avg, numofFitness] = line.split(',')
        generations.append(int(generation_number))
        minimumValues.append(int(minval))
        fImproved.append(int(fImp))
        averageValues.append(float(avg))
        line = f.readline()

NGEN = len(generations)
improvedGenerations = [element for i, element in enumerate(generations) if fImproved[i]]
improvements = [element for i, element in enumerate(minimumValues) if fImproved[i]]



# Convergence Degrees Implementation
convergenceDegrees = [math.atan2((improvements[idx] - improvements[idx + 1]), 
    (improvedGenerations[idx + 1] - improvedGenerations[idx])) * 180 / math.pi 
    for idx in range(len(improvedGenerations) - 1)]

averageDegrees = sum(convergenceDegrees) / len(convergenceDegrees)

greatTangentValue = math.atan2((minimumValues[0] - minimumValues[-1]),
  (generations[-1] - generations[0])) * 180 / math.pi
    
print("Average of Degrees : ", averageDegrees)
print("Great Tangent Value : ", greatTangentValue)


# Area Implementation

areaUnderCurve = np.trapz([i - minimumValues[-1] for i in minimumValues],
 generations)
totalArea = (NGEN * (minimumValues[0]- minimumValues[NGEN-1])) / 2
print("Area under curve : " , areaUnderCurve)
print("Total Area : ", totalArea)
print("Ratio of Areas : ", areaUnderCurve / float(totalArea))


# Average Convergence Rate Implementation
tau = 100
rateRange = range(tau, len(minimumValues) - tau)
averageConvergenceRate = []
for t in rateRange:
    xx = (minimumValues[t + tau] - minimumValues[t]) / ((minimumValues[t] - minimumValues[t-tau]) + 1e-20)
    value = 1 -  (abs(xx) ** (1/tau))
    averageConvergenceRate.append(value)


# Average Convergence Rate Implementation for Improved Generations
tau = 15
rRangeImp = range(tau, len(improvements) - tau)
acg = []
for t in rRangeImp:
    xx = (improvements[t + tau] - improvements[t]) / (improvements[t] - improvements[t-tau]) 
    value = 1 -  (abs(xx) ** (1/tau))
    # print(value)
    acg.append(value)


# Plots
plt.figure(1)
plt.plot(generations, minimumValues, 'bo', markersize=3, label='Minimum Fitness Values')
plt.fill_between(generations, minimumValues, minimumValues[-1])

for i in range(len(improvements)-1):
	x = [improvedGenerations[i], improvedGenerations[i+1]]
	y = [improvements[i], improvements[i+1]]
	plt.plot(x,y)

plt.plot([generations[0],generations[-1]], 
	[minimumValues[0], minimumValues[-1]], 'k-', linewidth=3)
plt.legend()
plt.grid(True)


plt.figure(2)
plt.plot(rateRange, averageConvergenceRate,'b', alpha=0.75, label='Average Convergence Rate')
plt.legend()



plt.figure(3)
plt.plot(rRangeImp, acg, 'b', label='Average Convergence Rate for Improvements')
plt.legend()
plt.show()
# a = minimumValues[20]
# b = minimumValues[10]
# c = minimumValues[0]
# print (a, b, c)
# diff1 = (a - b)
# diff2 = (b - c)
# print(diff1 / diff2)
# print(1 - (diff1 / diff2)**0.1)
