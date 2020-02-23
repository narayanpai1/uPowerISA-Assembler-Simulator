rtn = {"R"+str(i): i for i in range(32)}  # converting register to number
res = {}                                  # result


X = {"and": [31, None, 0, 28, None, None],  # for X type
     "exstw": [31, None, 0, 986, None, None],
     "nand": [31, None, 0, 476, None, None],
     "or": [31, None, 0, 444, None, None],
     "xor": [31, None, 0, 316, None, None],
     "sld": [31, None, 0, 794, None, None],
     "cmp": [31, None, 0, 0, None, None]}

XO = {"add": [31, 0, 0, 266, None, None],  # for XO type
      "subf": [31, 0, 0, 40, None, None]}

D = {"addi": [14, None, None, None, None, 0],  # for D type
     "addis": [15, None, None, None, None, 0],
     "andi": [28, None, None, None, None, 0],
     "ori": [24, None, None, None, None, 0],
     "xori": [26, None, None, None, None, 0],
     "lwz": [32, None, None, None, None, 1],
     "stw": [36, None, None, None, None, 1],
     "stwu": [37, None, None, None, None, 1],
     "sth": [44, None, None, None, None, 1],
     "lbz": [34, None, None, None, None, 1],
     "stb": [38, None, None, None, None, 1]}


def avengers(lines, label, data):  # iterating over all lines to transform into 32 bit instructions

    for l in lines:

        # print(l)
        u = l
        v = lines[l]  # doubt lines[l] -> l

        instr = v[:v.index(' ')]
        argu = v[v.index(' '):]
        # print(instr, "read ")
        # print(argu)

        req = argu.split(",")
        req = [i.replace(" ", "") for i in req]

        if instr in X.keys():
            X_type(instr, req, u)

        elif instr in XO.keys():
            XO_type(instr, req, u)

        elif instr in D.keys():
            D_type(instr, req, u)

        else:

            return

    return res


def X_type(instr, req, u):
    bina = ""
    bina = bina+"{:05b}".format(rtn[req[1]])
    bina = bina+"{:05b}".format(rtn[req[0]])
    bina = bina+"{:05b}".format(rtn[req[2]])
    bina = bina+"{:010b}".format(X[instr][3])+"0"
    bina = bina.replace("0b", "")
    res[u] = bina


def XO_type(instr, req, u):
    bina = ""
    bina += "{:06b}".format(XO[instr][0])
    bina += "{:05b}".format(rtn[req[0]])
    bina += "{:05b}".format(rtn[req[1]])
    bina += "{:05b}".format(rtn[req[2]])
    bina += "{:01b}".format(XO[instr][1])
    bina += "{:09b}".format(XO[instr][3]) + "0"
    bina = bina.replace("0b", "")
    res[u] = bina


def D_type(instr, req, u):
    bina = ""
    if D[instr][-1] == 1:
        # instr RT, D(RA)
        RT = req[0].strip()
        i1 = req[1].index('(')
        i2 = req[1].index(')')

        SI = int(req[1][:i1].strip())
        RA = req[1][i1+1:i2].strip()

        bina += "{:06b}".format(D[instr][0])
        bina += "{:05b}".format(rtn[RT])
        bina += "{:05b}".format(rtn[RA])
        bina += "{:016b}".format(SI)
        bina = bina.replace("0b", "")
        res[u] = bina

    elif D[instr][-1] == 0:
        bina += "{:06b}".format(D[instr][0])
        bina += "{:05b}".format(rtn[req[0]])
        bina += "{:05b}".format(rtn[req[1]])
        bina += "{:016b}".format(rtn[req[2]])
        bina = bina.replace("0b", "")
        res[u] = bina

    else:
        print("\n\nFormat not included\n\n")

    res[u] = bina