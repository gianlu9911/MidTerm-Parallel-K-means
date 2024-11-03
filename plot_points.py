import pandas as pd
import matplotlib.pyplot as plt

# Define the list of colors as in your C++ vector
COLORS = [
    "black", "blue", "green","red","brown","yellow","orange", "mediumvioletred","orchid","green","cyan","blue", 
    "darkorchid","magenta", "pink", "greenyellow", "purple", "magenta", "pink", "darkgray", "darkgrey", "silver", 
    "lightgray", "lightgrey", "gainsboro", "whitesmoke", "white", "snow", "rosybrown", "lightcoral", "indianred", 
    "brown", "firebrick", "maroon", "darkred", "red", "mistyrose", "salmon", "tomato", "darksalmon", "coral", 
    "orangered", "lightsalmon", "sienna", "seashell", "chocolate", "saddlebrown", "sandybrown", "peachpuff", "peru", 
    "linen", "bisque", "darkorange", "burlywood", "antiquewhite", "tan", "navajowhite", "blanchedalmond", "papayawhip", 
    "moccasin", "orange", "wheat", "oldlace", "floralwhite", "darkgoldenrod", "goldenrod", "cornsilk", "gold", 
    "lemonchiffon", "khaki", "palegoldenrod", "darkkhaki", "ivory", "beige", "lightyellow", "lightgoldenrodyellow", 
    "olive", "yellow", "olivedrab", "yellowgreen", "darkolivegreen", "dimgrey", "chartreuse", "lawngreen", "honeydew", 
    "darkseagreen", "palegreen", "lightgreen", "forestgreen", "limegreen", "darkgreen", "green", "lime", "seagreen", 
    "mediumseagreen", "springgreen", "mintcream", "mediumspringgreen", "mediumaquamarine", "aquamarine", "turquoise", 
    "lightseagreen", "mediumturquoise", "azure", "lightcyan", "paleturquoise", "darkslategray", "darkslategrey", 
    "slategrey", "slategray", "lightslategray", "lightslategrey", "cornflowerblue", "royalblue", "ghostwhite", 
    "lavender", "midnightblue", "navy", "darkblue", "mediumblue", "slateblue", "darkslateblue", "mediumslateblue", 
    "mediumpurple", "rebeccapurple", "blueviolet", "indigo", "darkorchid", "darkviolet", "mediumorchid", "thistle", 
    "plum", "violet", "purple", "darkmagenta", "fuchsia", "orchid", "deeppink", "hotpink", "lavenderblush", 
    "palevioletred", "pink", "lightpink"
]


# Read the CSV files for data and centroids
data = pd.read_csv("data/output_parallel.csv")
centroids = pd.read_csv("data/centroids_parallel.csv")

# Get the points and cluster_id from the data CSV
x = data['x']
y = data['y']
cluster_id = data['cluster_id']
print((cluster_id))

# Get the centroids data
centroid_x = centroids['x']
centroid_y = centroids['y']
id = centroids['cluster_id']

# Map cluster_id to color for points
colors = [COLORS[int(cid) ] for cid in cluster_id]
centroids_colors = [COLORS[int(cid)] for cid in id]

# Create a scatter plot
plt.figure(figsize=(10, 6))

# Plot the points with color based on cluster_id
plt.scatter(x, y, c=colors, s=50, label='Data Points')

# Plot the centroids as cross markers ('x') with larger size and distinct colors
plt.scatter(centroid_x, centroid_y, c=centroids_colors, marker='x', s=200, label='Centroids')

# Add title and labels
plt.title("Scatter Plot of Points with Centroids")


# Move the legend below the plot, centered horizontally
plt.legend(loc='upper center', bbox_to_anchor=(0.5, -0.1), ncol=2)  # Adjust ncol to control the number of columns

# Adjust layout to ensure everything fits
plt.tight_layout(rect=[0, 0, 1, 0.9])  # Adjust to make room for the legend below

# Show the plot
plt.show()