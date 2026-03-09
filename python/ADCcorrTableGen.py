import sys

# Default filename
filename = "zeros.txt"

# Allow optional filename from command line
if len(sys.argv) > 1:
    filename = sys.argv[1]

n = 16000

with open(filename, "w") as f:
    for _ in range(n):
        f.write("0\n")

print(f"Wrote {n} zeros to {filename}")
