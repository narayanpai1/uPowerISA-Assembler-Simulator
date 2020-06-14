import sys
from passes import *


def main():
    print("Enter file name: ")
    filename = input()
    if len(filename) >= 1:
        with open(filename, encoding="utf-8", mode="r") as Fiile:
            lines = Fiile.read().splitlines()
            (instructions, dataseg, label, data) = pass1(lines)

            textseg = pass2(instructions, label, data, dataseg)

            output_file = input("Enter output file name:")
            with open(output_file, mode="w") as Fiile:
                final1 = sorted([(i, textseg[i]) for i in textseg])
                final1 = [a[1] for a in final1]
                final1 = "".join(final1)

                final2 = sorted([(i, dataseg[i]) for i in dataseg])
                final2 = [a[1] for a in final2]
                final2 = "".join(final2)

                text_size = int(len(final1) / 32)
                text_size_b = "{:032b}".format(text_size)
                data_size = int(len(final2) / 32)
                data_size_b = "{:032b}".format(data_size)
                Fiile.write(text_size_b)
                Fiile.write(data_size_b)
                Fiile.write(final1)
                Fiile.write(final2)
            # print(binal)

    else:
        print("sOrry invalid name ")

    print("\nInstruction:\n")
    for instr in instructions.keys():
        print(instr, instructions[instr], sep="   ")

    print("\n\n")

    print("Data: \n")
    for dat in data.keys():
        print(dat, data[dat], sep="  ")

    print("\n\n")

    print("Label: \n")
    for lab in label.keys():
        print(lab, label[lab], sep="  ")

    print("\n\n")

    print("Text segment: \n")
    for binu in textseg.keys():
        print(binu, textseg[binu], sep="  ")

    print("Data segment: \n")
    for binu in dataseg.keys():
        print(binu, dataseg[binu], sep="  ")


if __name__ == "__main__":
    main()
