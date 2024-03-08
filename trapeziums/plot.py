import matplotlib.pyplot as plt

def plot_trapezium(a, b, c, d):
    plt.plot([a, a], [0, b], 'k-')  # Left side
    plt.plot([c, c], [0, d], 'k-')  # Right side
    plt.plot([a, c], [b, d], 'k-')  # Top side
    plt.plot([a, c], [0, 0], 'k-')  # Bottom side

def main():
    n = int(input(""))
    for i in range(n):
        a, b, c, d = map(int, input().split())
        plot_trapezium(a, b, c, d)

    plt.gca().set_aspect('equal', adjustable='box')
    plt.xlabel('X')
    plt.ylabel('Y')
    plt.title('Trapeziums Plot')
    plt.grid(True)
    plt.show()

if __name__ == "__main__":
    main()

