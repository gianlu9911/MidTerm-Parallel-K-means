import pandas as pd
import matplotlib.pyplot as plt
import os
import seaborn as sns

# Read data from CSV file
df = pd.read_csv('data/execution_times_AoS.csv')
print(df.head())  # Check the first few rows
print(df.info())  # Check for missing or inconsistent data
print(df.describe())  # Get basic statistics for numeric columns


plot_dir = 'data/plots'
os.makedirs(plot_dir, exist_ok=True)

def calculate_speedup(data):
    # Filter out the sequential data (where numThreads == 0)
    single_thread_data = data[data['numThreads'] == 0][['numPoints', 'numClusters', 'maxIterations', 'executionTime']]
    single_thread_data = single_thread_data.rename(columns={'executionTime': 'single_thread_time'})
    
    merged_data = pd.merge(data, single_thread_data, on=['numPoints', 'numClusters', 'maxIterations'], how='left')
    
    # Calculate speedup: single_thread_time / execution_time (for parallel runs)
    merged_data['speedup'] = merged_data['single_thread_time'] / merged_data['executionTime']
    
    return merged_data

# Calculate speedup
data_with_speedup = calculate_speedup(df)
print(data_with_speedup.head())
print(data_with_speedup['speedup'].isna().sum())  # Check for NaN values in speedup
print(data_with_speedup['speedup'].describe())  # Check for unusually large or small values
print(data_with_speedup.groupby(['numThreads', 'numPoints', 'numClusters', 'maxIterations'])['speedup'].mean().reset_index())


def plot_speedup_by_numPoints(data):
    # Group by numThreads, numPoints, numClusters, and maxIterations
    grouped = data.groupby(['numThreads', 'numPoints', 'numClusters', 'maxIterations'])['speedup'].mean().reset_index()
    
    unique_clusters = grouped['numClusters'].unique()
    color_palette = sns.color_palette("tab10", len(unique_clusters))  
    cluster_colors = {cluster: color_palette[i] for i, cluster in enumerate(unique_clusters)}

    unique_max_iterations = grouped['maxIterations'].unique()
    markers = ['o', 's', '^', 'D', 'p', '*', 'H', 'X', '+', 'v'] 
    max_iterations_markers = {maxIter: markers[i % len(markers)] for i, maxIter in enumerate(unique_max_iterations)}
    
    # Iterate over different numPoints and create a separate plot for each
    for numPoints in grouped['numPoints'].unique():
        plt.figure(figsize=(10, 6))

        plot_data = grouped[grouped['numPoints'] == numPoints]
        


        for (numClusters, maxIterations), group in plot_data.groupby(['numClusters', 'maxIterations']):
            color = cluster_colors[numClusters]  # Get color for this numClusters
            marker = max_iterations_markers[maxIterations]  # Get marker for this maxIterations
            plt.plot(group['numThreads'], group['speedup'], label=f'Clusters: {numClusters}, Iterations: {maxIterations}', color=color, marker=marker, markersize=8)
        
        plt.xlabel('Number of Threads', fontsize=12, fontweight='bold')
        plt.ylabel('Speedup', fontsize=12, fontweight='bold')
        plt.title(f'K-Means Speedup vs Number of Threads (Points: {numPoints})', fontsize=14, fontweight='bold')

        plt.legend(
            title='Configurations', 
            bbox_to_anchor=(1, 0), 
            loc='lower right', 
            fontsize=10, 
            title_fontsize=12,
            shadow=True, 
            fancybox=True, 
            framealpha=0.8, 
            borderpad=1,  
            borderaxespad=1  
        )
        
        plt.grid(True, linestyle='--', alpha=0.7)

        plt.tight_layout()

        plot_filename = os.path.join(plot_dir, f'kmeans_speedup_{numPoints}_points.png')
        plt.savefig(plot_filename, dpi=300)  
        plt.show()

plot_speedup_by_numPoints(data_with_speedup)
