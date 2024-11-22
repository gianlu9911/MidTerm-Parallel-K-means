import matplotlib.pyplot as plt
import pandas as pd
import numpy as np

def plot_kmeans_results(dataset_file, labels_file):
    dataset = pd.read_csv(dataset_file, header=None, names=['x', 'y'])
    labels = pd.read_csv(labels_file, header=None, names=['label'])

    num_points = len(labels)
    print(num_points)

    # Use only the first 'num_points' from the dataset
    dataset = dataset.iloc[:num_points]

    labels = labels.to_numpy().flatten()
    plt.figure(figsize=(8, 6))
    unique_labels = np.unique(labels)

    for label in unique_labels:
        cluster_points = dataset[labels == label]
        plt.scatter(cluster_points['x'], cluster_points['y'], label=f'Cluster {label}', s=10)

    plt.title('K-Means Clustering Results')
    plt.xlabel('X Coordinate')
    plt.ylabel('Y Coordinate')
    
    # Show legend
    # plt.legend()

    plt.show()

plot_kmeans_results('data/dataset.csv', 'data/labels.csv')
