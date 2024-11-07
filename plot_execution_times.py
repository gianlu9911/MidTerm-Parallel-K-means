import pandas as pd
import matplotlib.pyplot as plt
import os

# Read data from CSV file
df = pd.read_csv('data/execution_times.csv')

plot_dir = '../data/plots'
os.makedirs(plot_dir, exist_ok=True)

# Function to calculate speedup
def calculate_speedup(data):
    single_thread_data = data[data['num_threads'] == 1][['num_points', 'maxIteration', 'execution_time']]
    single_thread_data = single_thread_data.rename(columns={'execution_time': 'single_thread_time'})
    
    # Merge to get single-thread time for each combination of num_points and maxIteration
    merged_data = pd.merge(data, single_thread_data, on=['num_points', 'maxIteration'])
    merged_data['speedup'] = merged_data['single_thread_time'] / merged_data['execution_time']
    
    return merged_data


data_with_speedup = calculate_speedup(df)
