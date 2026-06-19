import matplotlib.pyplot as plt
import numpy as np
import math

for log_id in [0, 1, 2, 3, 4, 5, 6, 7]:

    with open(f"io/log_{log_id}.txt", "r") as f:
        content = f.read()
    data = content.split('\n')
    data.pop()

    V = []
    T = []
    T_th = []

    for s in data[1::]:
        row = s.split('\t')

        V.append( float(row[0]) )
        T.append( float(row[1]) )

        T_th.append( (float(V[0])/float(V[-1]))**(2/3) * float(T[0]) )

    logV = list(map(math.log, V))
    logT = list(map(math.log, T))
    logTt = list(map(math.log, T_th))

    plt.figure(figsize=(10, 6))
    plt.plot(np.array(logV, float), np.array(logT, float), '.r', label="simulated")
    plt.plot(np.array(logV, float), np.array(logTt, float), 'b', label="theory")

    plt.xlabel('ln Volume')
    plt.ylabel('ln Temperature')

    plt.grid()
    plt.legend()

    plt.title("ln Temperature (ln Volume)")

    plt.savefig(f"io/plot_{log_id}.png")
    plt.show()