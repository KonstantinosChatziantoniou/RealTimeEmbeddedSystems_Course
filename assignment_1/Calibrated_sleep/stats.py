import statistics
import pandas as pd
import matplotlib.pyplot as plt
import numpy 
data = pd.read_csv("./old/../timeStamps2.csv", skiprows=1, usecols=[1])
data = data.unstack()
print(data[0])
print("mean {}".format(statistics.mean(data)))
print("stdevv {}".format(statistics.stdev(data)))
median = statistics.median(data)
sum = 0
max = -1
min = data[0]
for i in data:
    sum += i
    if max < i:
        max = i
    if min > i:
        min = i

print(median)
print(max)
print(min)

#plt.scatter(range(0,len(data)),data)
#plt.show()
# bins = numpy.linspace(0, 1, 10)
# digitized = numpy.digitize(data, bins)
# bin_means = [data[digitized == i].mean() for i in range(1, len(bins))]

# print(bins)
# print(bin_means)
# bins = pd.cut(data,10,labels=False, retbins=True, right=False)
# print(bins)


num_of_bins = 10
upper_bin_limits = []
bins = [0 for i in range(num_of_bins)]
print(bins)
bin_interval = (max-min)/num_of_bins
for i in range(num_of_bins):
    upper_bin_limits.append(min + (i)*bin_interval)

for i in range(len(data)):
    pos = 0
    for j in range(num_of_bins):
       # print("{} - {} = {}",data[i],upper_bin_limits[j],data[i] > upper_bin_limits[j])
        if data[i] >= upper_bin_limits[j]:
            #print("{} - {} = {} => {}",data[i],upper_bin_limits[j],data[i] > upper_bin_limits[j],j)
            pos = j
    bins[pos] = bins[pos] + 1
print(bins)
print(upper_bin_limits)
plt.bar([1,2,3,4,5,6,7,8,9,10],bins,tick_label=upper_bin_limits, align='edge') #[1,2,3,4,5,6,7,8,9,10]
plt.title(label='Calibrated Sleep, 7200 samples, dt = 0.1sec')
plt.xlabel(xlabel= 'us')
plt.show()


plt.scatter(range(0,len(data)),data)
plt.title(label='Calibrated Sleep, 7200 samples, dt = 0.1sec')
plt.ylabel(ylabel= 'us')
plt.show()