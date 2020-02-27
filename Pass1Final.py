import re


def convert_to_bin(to_be_encoded, siz):
    bina = ""
    if to_be_encoded < 0:
        # 2s compliment
        curr_index = siz - 2
        ans = -(1 << (siz - 1))
        bina += "1"
        while curr_index >= 0:
            if to_be_encoded - ans >= (1 << curr_index):
                ans += 1 << curr_index
                bina += "1"
            else:
                bina += "0"
            curr_index = curr_index - 1
    else:
        if siz == 24:
            bina += "{:024b}".format(to_be_encoded)
        else:
            bina += "{:032b}".format(to_be_encoded)
    return bina


def pass1(lines):
    f = lines
    start_data = 0
    start_text = 0
    for i in range(len(f)):
        if f[i] == ".text":
            start_text = i  # start line index of text section
        if f[i] == ".data":
            start_data = i  # start line index of data section

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
                bina = ""
                count = 0
                for ii in range(2, len(arr) - 1):
                    jj = arr[ii].index(",")
                    bina += convert_to_bin(int(arr[ii][0:jj]), 32)
                    count = count + 1
                bina += convert_to_bin(int(arr[-1]), 32)
                cur_location = cur_location + 4 * (count + 1)
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
                bina = ""
                for chars in f[i]:
                    if chars == '"' and flag == 0:  # doubt here
                        flag = 1
                        continue
                    if flag == 1 and chars != '"':
                        bina += "{:032b}".format(ord(chars))
                        count = count + 1
                    if flag == 1 and chars == '"':
                        flag = 0
                        bina += "{:032b}".format(0)
                        count = count + 1
                        break
                cur_location = cur_location + count * 4
            else:
                print("Error during data section")
            dataseg[start_location] = bina
    return (instruction, dataseg, text, data)
