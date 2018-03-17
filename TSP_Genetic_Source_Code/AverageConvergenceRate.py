from __future__ import print_function, division
import matplotlib.pyplot as plt
import numpy as np
import math

def cumsum(myarray, uptoIdx):
	total = 0
	for i in range(uptoIdx):
		total += myarray[i]
	return total


generations = []
minimumValues = []
counterArray = []
for i in range(1,11):
	filename = 'results/cr08mr08/results_cr08_mr08_' + str(i)+'.txt'
	counter = 0
	print(filename)
	with open(filename, 'r') as f:
		line = f.readline()
		while line:
			counter += 1
			[generation_number, minval, _, _] = line.split(',')
			generations.append(int(generation_number))
			minimumValues.append(int(minval))
			line = f.readline()
		counterArray.append(counter)

print(counterArray)
print(cumsum(counterArray,1))
print(cumsum(counterArray,2))

seq1 = minimumValues[:cumsum(counterArray,1)]
seq2 = minimumValues[cumsum(counterArray,1):cumsum(counterArray,2)]
seq3 = minimumValues[cumsum(counterArray,2):cumsum(counterArray,3)]
seq4 = minimumValues[cumsum(counterArray,3):cumsum(counterArray,4)]
seq5 = minimumValues[cumsum(counterArray,4):cumsum(counterArray,5)]
seq6 = minimumValues[cumsum(counterArray,5):cumsum(counterArray,6)]
seq7 = minimumValues[cumsum(counterArray,6):cumsum(counterArray,7)]
seq8 = minimumValues[cumsum(counterArray,7):cumsum(counterArray,8)]
seq9 = minimumValues[cumsum(counterArray,8):cumsum(counterArray,9)]
seq10 = minimumValues[cumsum(counterArray,9):cumsum(counterArray,10)]

plt.plot(seq1, label="1")
plt.plot(seq2, label="2")
plt.plot(seq3, label="3")
plt.plot(seq4, label="4")
plt.plot(seq5, label="5")
plt.plot(seq6, label="6")
plt.plot(seq7, label="7")
plt.plot(seq8, label="8")
plt.plot(seq9, label="9")
plt.plot(seq10, label="10")
plt.legend()

minseq = min(counterArray)
def sumupIdx(i):
	total = seq1[i]+seq2[i]+seq3[i]+seq4[i]+ seq5[i] + seq6[i]+seq7[i]+seq8[i]+seq9[i]+ seq10[i] 	
	return total / 10 

averageArray = [sumupIdx(i) for i in range(minseq)]

# Average Convergence Rate Implementation
tau = 1000
rateRange = range(tau, len(averageArray) - tau)
averageConvergenceRate = []
for t in rateRange:
    xx = (averageArray[t + tau] - averageArray[t]) / ((averageArray[t] - averageArray[t-tau]) + 1e-20)
    value = 1 -  abs(xx) ** (1/(tau))
    averageConvergenceRate.append(value)


fig = plt.figure(2)
plt.plot(averageArray,'bo-', markersize=3, label='Average of 10 runs')
plt.fill_between(range(len(averageArray)), averageArray, min(averageArray))
plt.legend()
fig.tight_layout()
plt.grid(True)

plt.figure(3)
plt.plot(rateRange, averageConvergenceRate, label='Average Convergence Rate')
plt.legend()

plt.show()

