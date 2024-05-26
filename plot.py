import matplotlib.pyplot as plt
import pandas as pd

df = pd.read_csv("response.csv")
df.to_excel("response_excel.xlsx", index=False)

fig, ax = plt.subplots()
ax.plot(df.x, df.y)
plt.show()
