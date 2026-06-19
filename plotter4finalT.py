import matplotlib.pyplot as plt
import numpy as np
import math

T_th_final = 0.09889787020657603
logT_th_final = -2.3136675754025773

i = []
T_final = []
logT_final = []

for log_id in [0, 1, 2, 3, 4, 5, 6, 7]:

    with open(f"io/log_{log_id}.txt", "r") as f:
        content = f.read()
    data = content.split('\n')
    data.pop()

    i.append(log_id-7)
    print(i)
    T_final.append(float( data[-1].split('\t')[1] ))
    logT_final.append(math.log(T_final[-1] / T_th_final))
    T_final[-1] /= T_th_final

plt.figure(figsize=(10, 6))

# plt.plot(np.array(i, int), np.array(T_final, float), '.r', label="T_final")
# plt.plot(np.array(i, int), np.array(T_final, float), 'r')
# plt.plot(np.array(i, int), np.linspace(1, 1, 8), 'g')
# plt.ylabel( "Final T / T_th" )

plt.plot(np.array(i, int), np.array(logT_final, float), '.r', label="logT_final")
plt.plot(np.array(i, int), np.array(logT_final, float), 'r')
plt.plot(np.array(i, int), np.linspace(0, 0, 8), 'g')
plt.ylabel( "ln( Final T / T_th )" )

plt.xlabel( "log2( Vel / Vmol )" )


plt.grid()
plt.legend()

# plt.savefig(f"io/logT_final.png")
plt.show()