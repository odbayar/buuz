import os, sys

fp = open("mongolian_letters.txt", "rb")

to_upper = list(range(256))
to_lower = list(range(256))

while True:
    line = fp.read(8)
    if len(line) < 8:
        break
    to_upper[line[2]] = line[0]
    to_lower[line[0]] = line[2]

print("const BYTE cyrToUpper[] = {")
for y in range(16):
    print("    ", *["0x{0:02X},".format(to_upper[y * 16 + x]) for x in range(16)], sep="")
print("    };")

print()

print("const BYTE cyrToLower[] = {")
for y in range(16):
    print("    ", *["0x{0:02X},".format(to_lower[y * 16 + x]) for x in range(16)], sep="")
print("    };")
