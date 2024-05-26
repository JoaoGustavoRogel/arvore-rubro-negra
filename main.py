from tqdm import tqdm
import pandas as pd
import subprocess
import random

with open("data/vectors", "r") as _fp:
    _fp.seek(0)
    data = _fp.readlines()

response = []

for i in tqdm(range(1, 5001)):
    line = [int(i) for i in data[i-1].split(" ")]

    aux = 0
    for _ in range(6):
        number = random.choice(line)
        result = subprocess.run([".\/a.exe", str(i), str(number)], capture_output=True)
        aux += int(result.stdout.decode().strip())
    n = aux / 6

    response.append({"x": i, "y": n})

df = pd.DataFrame(response)
df.to_csv("response.csv", index=False)
