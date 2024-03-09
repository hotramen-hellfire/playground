import sys

def filter_lines(lines, a, b):
    filtered_lines = []
    for line in lines:
        first_value = int(line.split()[0])
        if a <= first_value <= b:
            filtered_lines.append(line)
    return filtered_lines

def main():
    # Read command-line arguments for a and b
    if len(sys.argv) != 3:
        print("Usage: python program.py <a> <b>")
        return
    try:
        a = int(sys.argv[1])
        b = int(sys.argv[2])
    except ValueError:
        print("Error: Arguments a and b must be integers")
        return

    # Read the number of lines
    try:
        num_lines = int(input(""))
    except ValueError:
        print("Error: Number of lines must be an integer")
        return
    
    # Read lines from input
    lines = []
    for _ in range(num_lines):
        line = input().strip()
        lines.append(line)

    # Filter lines
    filtered_lines = filter_lines(lines, a, b)

    print(len(filtered_lines))
    # Print filtered lines
    for line in filtered_lines:
        print(line)

if __name__ == "__main__":
    main()

