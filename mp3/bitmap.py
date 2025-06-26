bitmap = [
  0b00011000,
  0b00100100,
  0b01000010,
  0b01000010,
  0b00100100,
  0b00011000,
  0b00000000,
  0b00000000
]

for row in bitmap:
    line = ""
    for i in range(7, -1, -1):
        if row & (1 << i):
            line += "#"
        else:
            line += " "
    print(line)
