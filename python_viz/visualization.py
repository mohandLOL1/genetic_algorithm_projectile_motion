import matplotlib.pyplot as plt
import pandas as pd

# Scatter plot parameters
generation_number = 100  # Replace with the desired generation number
file_name = f"generation_{generation_number}.txt"

# Fixed axis limits for comparison (set according to expected ranges)
fixed_x_min = 0
fixed_x_max = 90  # Assuming angles range between 0 and 90 degrees
fixed_y_min = 0
fixed_y_max = 300  # Assuming velocities range between 0 and 300 m/s

# Fitness progress parameters
fitness_file = "fitness_progress.txt"

try:
    # Load the data for the specific generation
    data = pd.read_csv(file_name, sep='\t')

    # Create scatter plot for angles vs. velocities
    plt.figure(figsize=(8, 6))
    plt.scatter(data['Angle'], data['Velocity'], color='blue', alpha=0.7)

    # Add target point
    target_x = 100
    target_y = 20
    plt.scatter(target_x, target_y, color='red', label='Target Point', s=100, marker='*')

    # Add labels, title, and grid
    plt.xlabel('Angle (degrees)')
    plt.ylabel('Velocity (m/s)')
    plt.title(f'Scatter Plot of Angles vs Velocities (Generation {generation_number})')
    plt.legend()
    plt.grid(True)

    # Set fixed axis limits
    plt.xlim(fixed_x_min, fixed_x_max)
    plt.ylim(fixed_y_min, fixed_y_max)

    # Save and show scatter plot
    plt.savefig(f"scatter_generation_{generation_number}.png", dpi=300)
    plt.show()

    # Load fitness data
    fitness_data = pd.read_csv(fitness_file, sep='\t', header=None, names=['Generation', 'Fitness'])

    # Create fitness progress line plot
    plt.figure(figsize=(8, 6))
    plt.plot(fitness_data['Generation'], fitness_data['Fitness'], marker='o', color='green', linestyle='-')

    # Add labels, title, and grid
    plt.xlabel('Generation Number')
    plt.ylabel('Best Fitness')
    plt.title('Fitness Progress Over Generations')
    plt.grid(True)

    # Save and show fitness progress plot
    plt.savefig("fitness_progress.png", dpi=300)
    plt.show()

except FileNotFoundError as e:
    print(e)
