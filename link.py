import sys

from Pass1Final import pass1

from Pass2Final import transform


def main():
    print("Enter file name: ")
    filename = "assembly2.txt"
    if len(filename) >= 1:
        with open(filename, encoding="utf-8", mode="r") as Fiile:
            lines = Fiile.read().splitlines()
            (instructions, bina2, label, data) = pass1(lines)

            binal = transform(instructions, label, data, bina2)

            with open("output2.o", mode="w") as Fiile:
                final1 = sorted([(i, binal[i]) for i in binal])
                final1 = [a[1] for a in final1]
                final1 = "".join(final1)

                final2 = sorted([(i, bina2[i]) for i in bina2])
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
    for binu in binal.keys():
        print(binu, binal[binu], sep="  ")

    print("Data segment: \n")
    for binu in bina2.keys():
        print(binu, bina2[binu], sep="  ")


if __name__ == "__main__":
    main()
