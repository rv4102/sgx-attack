import numpy
from scipy import stats
import matplotlib.pyplot as plt

results_folder = "results"


sgx_ecall2_run_1_data = open(results_folder+"/sgx_ecall2_run1.csv").readlines()
sgx_ecall2_run_1_datalist = []
for datapoint in sgx_ecall2_run_1_data:
    datapoint = datapoint.strip()
    datapoint = float(datapoint)
    sgx_ecall2_run_1_datalist.append(datapoint)

sgx_ecall2_run_2_data = open(results_folder+"/sgx_ecall2_run2.csv").readlines()
sgx_ecall2_run_2_datalist = []
for datapoint in sgx_ecall2_run_2_data:
    datapoint = datapoint.strip()
    datapoint = float(datapoint)
    sgx_ecall2_run_2_datalist.append(datapoint)

sgx_ecall1_run_1_data = open(results_folder+"/sgx_ecall1_run1.csv").readlines()
sgx_ecall1_run_1_datalist = []
for datapoint in sgx_ecall1_run_1_data:
    datapoint = datapoint.strip()
    datapoint = float(datapoint)
    sgx_ecall1_run_1_datalist.append(datapoint)

sgx_ecall1_run_2_data = open(results_folder+"/sgx_ecall1_run2.csv").readlines()
sgx_ecall1_run_2_datalist = []
for datapoint in sgx_ecall1_run_2_data:
    datapoint = datapoint.strip()
    datapoint = float(datapoint)
    sgx_ecall1_run_2_datalist.append(datapoint)



# Stats for CMOV runs for bit 0
t_stat, p_value = stats.ttest_ind(sgx_ecall2_run_1_datalist, sgx_ecall2_run_2_datalist)
print("Intra-class stats for FADD. T-stat: ", t_stat, ". p-value: ", p_value)

# Stats for CMOV runs for bit 1
t_stat, p_value = stats.ttest_ind(sgx_ecall1_run_1_datalist, sgx_ecall1_run_2_datalist)
print("Intra-class stats for ADD. T-stat: ", t_stat, ". p-value: ", p_value)

# Inter class stats
t_stat, p_value = stats.ttest_ind(sgx_ecall2_run_1_datalist, sgx_ecall1_run_1_datalist)
print("Inter-class stats for ADD and FADD. T-stat: ", t_stat, ". p-value: ", p_value)

# plot the 2 readings
plt.plot(sgx_ecall2_run_1_datalist, label="ADD")
plt.plot(sgx_ecall1_run_1_datalist, label="FADD")

plt.xlabel("Reading Number")
plt.ylabel("UOPS_EXECUTED.X87")

plt.legend()
plt.savefig(fname=results_folder+"/plot.png")