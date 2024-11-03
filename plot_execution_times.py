import pandas as pd
import matplotlib.pyplot as plt

# Read data from CSV file
df = pd.read_csv('data\execution_times.csv')

# Assuming the CSV has columns: 'num_points', 'num_threads', 'execution_time'
num_points = df['num_points'].unique()

for points in num_points:
    subset = df[df['num_points'] == points]
    plt.plot(subset['num_threads'], subset['execution_time'], label=f'{points} points')

plt.xlabel('Number of Threads')
plt.ylabel('Execution Time (s)')
plt.title('Execution Time vs. Number of Threads')
plt.legend(title='Execution time')
plt.grid(True)
plt.show()
