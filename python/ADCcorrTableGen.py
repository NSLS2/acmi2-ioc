import sys

# Default filename
filename = "zeros.txt"

# Allow optional filename from command line
if len(sys.argv) > 1:
    filename = sys.argv[1]

n = 16000

with open(filename, "w") as f:
    for i in range(n):
        if 1500 <= i <= 1700:
            f.write("0\n")
        else:
            f.write("0\n")

print(f"Wrote {n} values to {filename}")
