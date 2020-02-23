import sys

from Pass1Final import marvel
from Pass2Final import avengers


print("Enter file name: ")
filename = input()
if len(filename) >= 1:
    with open(filename, encoding="utf-8", mode="r") as Fiile:
        lines = Fiile.read().splitlines()
        (instructions, label, data) = marvel(lines)

        binal = avengers(instructions, label, data)

        with open('output.o', mode="w") as Fiile:
            final = sorted([(i, binal[i])
                            for i in binal])
            final = [a[1] for a in final]
            final = "".join(final)
            Fiile.write(final)
        # print(binal)


else:
    print("sOrry invalid name ")

print("\nInstruction:\n")
for instr in instructions.keys():
    print(instr, instructions[instr], sep='   ')

print("\n\n")

print("Data: \n")
for dat in data.keys():
    print(dat, data[dat], sep='  ')

print("\n\n")

print("Label: \n")
for lab in label.keys():
    print(lab, label[lab], sep='  ')

print("\n\n")


print("Text segment: \n")
for binu in binal.keys():
    print(binu, binal[binu], sep='  ')
