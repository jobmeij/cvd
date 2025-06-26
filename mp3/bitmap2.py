bitmap = [
  0b00000111, 0b00000000,
  0b00011000, 0b11000000,
  0b00100000, 0b00100000,
  0b01000000, 0b00010000,
  0b01000000, 0b00010000,
  0b00100000, 0b00100000,
  0b00011000, 0b11000000,
  0b00000111, 0b00000000
]

for row in range(8):
    high_byte = bitmap[row * 2]
    low_byte = bitmap[row * 2 + 1]
    combined = (high_byte << 8) | low_byte  # 16 bits

    line = ""
    for i in range(15, -1, -1):
        if combined & (1 << i):
            line += "#"
        else:
            line += " "
    print(line)
