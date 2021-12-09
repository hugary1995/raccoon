import sys

file = open(sys.argv[1], 'r')
lines = file.readlines()

for line in lines:
    # Split by whitespaces
    words = line.strip().split()
    # If the first word starts with "lines"
    if words[0].startswith("lines"):
        for word in words:
            if word.endswith("%"):
                print(word)
                exit(0)

print("Couldn't find a coverage percentage.")
exit(1)
