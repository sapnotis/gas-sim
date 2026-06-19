import matplotlib.pyplot as plt
import numpy as np
import math

for log_id in [3]:

    with open(f"io/log_{log_id}.txt", "r") as f:
        content = f.read()
    data = content.split('\n')
    data.pop()

    P = []
    V = []
    Pt = []

    for s in data:
        row = s.split('\t')

        P.append( float(row[0]) )
        V.append( float(row[1]) )

        Pt.append( (float(V[0])/float(V[-1]))**(5/3) * float(P[0]) )

    logV = list(map(math.log, V))
    logP = list(map(math.log, P))
    logPt = list(map(math.log, Pt))

    plt.figure(figsize=(10, 6))
    plt.plot(np.array(logV, float), np.array(logP, float), '.r', label="simulated")
    plt.plot(np.array(logV, float), np.array(logPt, float), '.b', label="theory")

    plt.xlabel('ln Volume')
    plt.ylabel('ln Pressure')

    plt.grid()
    plt.legend()

    plt.title("ln Pressure (ln Volume)")

    # plt.savefig(f"io/plot_{log_id}.png")
    plt.show()