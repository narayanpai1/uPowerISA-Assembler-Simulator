import re


def pass1(lines):
    f = lines
    start_data = 0
    start_text = 0
    for i in range(len(f)):
        if f[i] == ".text":
            start_text = i  # start line index of text section
        if f[i] == ".data":
            start_data = i  # start line index of data section

    print("start_text " + str(start_text) + " start_data " + str(start_data))
    cur_location = 0x4000000
    text = {}
    instruction = {}
    # regular expression format for label name ( label_name: )
    var = re.compile(r".+:")
    for i in range(start_text + 1, len(f)):
        if bool(var.match(f[i])):  # doubt bool-> nothing
            x = re.split(":", f[i])  # doubt f[i]->1
            text[x[0]] = hex(cur_location)  # label name mapped with address
            cur_location = cur_location - 4
        else:
            tmp = f[i].split("#")[0]
            tmp = " ".join(tmp.split())
            if tmp.strip():
                # storing instruction in assembly language
                instruction[cur_location] = tmp
            else:
                cur_location = (
                    cur_location - 4
                )  # only line for comments without any instructions
        cur_location = cur_location + 4

    cur_location = 0  # initial curr_location variable
    data = {}
    dataseg = {}
    # processing data section
    for i in range(start_data + 1, start_text):
        if bool(var.match(f[i])):  # doubt bool-> nothing
            arr = f[i].split()
            lable = re.split(":", arr[0])[0]  # doubt arr[0]-> 1
            # mapping variable name to address
            data[lable] = hex(cur_location)
            datatype = arr[1][1:]  # neglecting . at start of datatype
            start_location = cur_location
            if datatype == "byte":
                bina = "{:08b}".format(int(arr[2])).strip()
                cur_location = cur_location + 1
            elif datatype == "word":
                bina = "{:032b}".format(int(arr[2]))
                cur_location = cur_location + 4
            elif datatype == "halfword":
                bina = "{:016b}".format(int(arr[2]))
                cur_location = cur_location + 2
            elif datatype == "space":
                bina = []
                for byte in int(arr[2]):
                    bina += "{:08b}".format(0)
                cur_location = cur_location + int(arr[2])
            elif datatype == "asciiz":
                flag = 0
                count = 0
                for chars in f[i]:
                    if chars == '"' and flag == 0:  # doubt here
                        flag = 1
                        continue
                    if flag == 1 and chars != '"':
                        bina += "{:8b}".format(int(chars))
                        count = count + 1
                    if flag == 1 and chars == '"':
                        flag = 0
                        bina += "{:8b}".format(0)
                        break
                cur_location = cur_location + count
            else:
                print("Error during data section")
            dataseg[start_location] = bina
    return (instruction, dataseg, text, data)
