import random
import csv

def generate_random_2d_points(output_file, num_points, min_val=-100, max_val=100):
    """
    Generate a random dataset of 2D points with random float coordinates and save it to a CSV file.

    Args:
        output_file (str): Path to save the CSV file.
        num_points (int): Number of random 2D points to generate.
        min_val (float): Minimum value for random coordinates.
        max_val (float): Maximum value for random coordinates.

    Returns:
        None
    """
    with open(output_file, mode="w", newline="") as file:
        writer = csv.writer(file)
        #writer.writerow(["x", "y"])  # Write header

        for _ in range(num_points):
            x = random.uniform(min_val, max_val)
            y = random.uniform(min_val, max_val)
            writer.writerow([x, y]) 

    print(f"Random dataset of {num_points} 2D points saved to {output_file}")


if __name__ == "__main__":
    output_file = "data/dataset.csv"
    num_points = 10**8  
    generate_random_2d_points(output_file, num_points)
