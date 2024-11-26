import pandas as pd
import matplotlib.pyplot as plt

def plot_execution_times_by_threads(soa_file, aos_file):
    """
    Plots the execution times of SoA and AoS implementations,
    with the number of threads on the x-axis.

    Parameters:
    soa_file (str): Path to the SoA execution times CSV file.
    aos_file (str): Path to the AoS execution times CSV file.
    """
    # Load the data from CSV files
    soa_data = pd.read_csv(soa_file)
    aos_data = pd.read_csv(aos_file)

    # Ensure the data aligns properly by sorting based on parameters
    soa_data = soa_data.sort_values(by=['numPoints', 'numClusters', 'maxIterations', 'numThreads'])
    aos_data = aos_data.sort_values(by=['numPoints', 'numClusters', 'maxIterations', 'numThreads'])

    # Check if both files have the same experimental conditions
    if not soa_data[['numPoints', 'numClusters', 'maxIterations', 'numThreads']].equals(
        aos_data[['numPoints', 'numClusters', 'maxIterations', 'numThreads']]
    ):
        print("Error: The experimental conditions in the two files do not match.")
        return

    # Group data by the number of threads and compute mean execution times
    threads = soa_data['numThreads'].unique()
    soa_mean_times = soa_data.groupby('numThreads')['executionTime'].mean()
    aos_mean_times = aos_data.groupby('numThreads')['executionTime'].mean()

    # Plotting
    plt.figure(figsize=(10, 6))
    plt.plot(
        threads, 
        aos_mean_times, 
        marker='o', 
        linestyle='-', 
        color='red', 
        label='AoS Execution Time'
    )
    plt.plot(
        threads, 
        soa_mean_times, 
        marker='o', 
        linestyle='-', 
        color='blue', 
        label='SoA Execution Time'
    )
    plt.xlabel('Number of Threads')
    plt.ylabel('Execution Time (ms)')
    plt.title('Execution Times of AoS and SoA by Number of Threads')
    plt.legend()
    plt.grid(True)
    plt.show()

import pandas as pd
import matplotlib.pyplot as plt

def plot_execution_times_largest_config(soa_file, aos_file):
    """
    Plots the execution times of SoA and AoS implementations for the largest configuration,
    with the number of threads on the x-axis.

    Parameters:
    soa_file (str): Path to the SoA execution times CSV file.
    aos_file (str): Path to the AoS execution times CSV file.
    """
    # Load the data from CSV files
    soa_data = pd.read_csv(soa_file)
    aos_data = pd.read_csv(aos_file)

    # Find the largest configuration values
    max_points = soa_data['numPoints'].max()
    max_clusters = soa_data['numClusters'].max()
    max_iterations = soa_data['maxIterations'].max()

    # Filter the data for the largest configuration
    soa_filtered = soa_data[
        (soa_data['numPoints'] == max_points) &
        (soa_data['numClusters'] == max_clusters) &
        (soa_data['maxIterations'] == max_iterations)
    ]
    aos_filtered = aos_data[
        (aos_data['numPoints'] == max_points) &
        (aos_data['numClusters'] == max_clusters) &
        (aos_data['maxIterations'] == max_iterations)
    ]

    # Ensure the data aligns properly by sorting based on the number of threads
    soa_filtered = soa_filtered.sort_values(by=['numThreads'])
    aos_filtered = aos_filtered.sort_values(by=['numThreads'])

    # Check if both files have the same thread counts
    if not soa_filtered['numThreads'].equals(aos_filtered['numThreads']):
        print("Error: The thread counts in the two files do not match for the largest configuration.")
        return

    # Extract the number of threads and execution times
    threads = soa_filtered['numThreads']
    soa_times = soa_filtered['executionTime']
    aos_times = aos_filtered['executionTime']

    # Plotting
    plt.figure(figsize=(10, 6))
    plt.plot(
        threads, 
        aos_times, 
        marker='o', 
        linestyle='-', 
        color='red', 
        label='AoS Execution Time'
    )
    plt.plot(
        threads, 
        soa_times, 
        marker='o', 
        linestyle='-', 
        color='blue', 
        label='SoA Execution Time'
    )
    plt.xlabel('Number of Threads')
    plt.ylabel('Execution Time (ms)')
    plt.title(
        f'Execution Times of AoS and SoA by Threads\n'
        f'(Points={max_points}, Clusters={max_clusters}, Iterations={max_iterations})'
    )
    plt.legend()
    plt.grid(True)
    plt.show()

import pandas as pd
import matplotlib.pyplot as plt

def plot_execution_times_smallest_config(soa_file, aos_file):
    """
    Plots the execution times of SoA and AoS implementations for the smallest configuration,
    with the number of threads on the x-axis.

    Parameters:
    soa_file (str): Path to the SoA execution times CSV file.
    aos_file (str): Path to the AoS execution times CSV file.
    """
    # Load the data from CSV files
    soa_data = pd.read_csv(soa_file)
    aos_data = pd.read_csv(aos_file)

    # Find the smallest configuration values
    min_points = soa_data['numPoints'].min()
    min_clusters = soa_data['numClusters'].min()
    min_iterations = soa_data['maxIterations'].min()

    # Filter the data for the smallest configuration
    soa_filtered = soa_data[
        (soa_data['numPoints'] == min_points) &
        (soa_data['numClusters'] == min_clusters) &
        (soa_data['maxIterations'] == min_iterations)
    ]
    aos_filtered = aos_data[
        (aos_data['numPoints'] == min_points) &
        (aos_data['numClusters'] == min_clusters) &
        (aos_data['maxIterations'] == min_iterations)
    ]

    # Ensure the data aligns properly by sorting based on the number of threads
    soa_filtered = soa_filtered.sort_values(by=['numThreads'])
    aos_filtered = aos_filtered.sort_values(by=['numThreads'])

    # Check if both files have the same thread counts
    if not soa_filtered['numThreads'].equals(aos_filtered['numThreads']):
        print("Error: The thread counts in the two files do not match for the smallest configuration.")
        return

    # Extract the number of threads and execution times
    threads = soa_filtered['numThreads']
    soa_times = soa_filtered['executionTime']
    aos_times = aos_filtered['executionTime']

    # Plotting
    plt.figure(figsize=(10, 6))
    plt.plot(
        threads, 
        aos_times, 
        marker='o', 
        linestyle='-', 
        color='red', 
        label='AoS Execution Time'
    )
    plt.plot(
        threads, 
        soa_times, 
        marker='o', 
        linestyle='-', 
        color='blue', 
        label='SoA Execution Time'
    )
    plt.xlabel('Number of Threads')
    plt.ylabel('Execution Time (ms)')
    plt.title(
        f'Execution Times of AoS and SoA by Threads\n'
        f'(Points={min_points}, Clusters={min_clusters}, Iterations={min_iterations})'
    )
    plt.legend()
    plt.grid(True)
    plt.show()

plot_execution_times_smallest_config('data/execution_times_SoA.csv', 'data/execution_times_AoS.csv')


plot_execution_times_largest_config('data/execution_times_SoA.csv', 'data/execution_times_AoS.csv')

plot_execution_times_by_threads('data/execution_times_SoA.csv', 'data\execution_times_AoS.csv')
