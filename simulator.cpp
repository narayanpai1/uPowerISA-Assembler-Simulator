#include <bits/stdc++.h>

using namespace std;

typedef long long ll;
typedef vector<ll> vll;
#define fr(i, a, b) for (long long i = a; i < b; ++i)
#define fr2(i, b, a) for (long long i = b; i >= a; --i)

struct Register
{
	ll r[32];
	ll lr, cr, srr0;
	ll cia, nia;
};
Register r;
/*Convention
	$sp = r.r[29]
	$gp=r.r[28]
	$fp=r.r[30]
	$ra = link register=r.lr
	$cr=condition register=r.cr
	$sr=special register for comparision=r.sr
*/
#define gp r[28]
#define sp r[29]
#define fp r[30]
#define a0 r[4]
#define a1 r[5]
#define a2 r[6]
#define a3 r[7]
#define v0 r[2]
#define v1 r[3]

map<ll, vll> m_inst;
map<ll, ll> m_mem;

ll to_decimal(vll v)
{
	ll rv = 0;
	fr2(i, v.size() - 1, 0)
	{
		rv = 2 * rv + v[i];
	}
	return rv;
}

struct fields
{
	ll po, rs, ra, rb, xo, rc;
	//NOTE THE DIFFERENCE B/W sh, SH
	ll sh, SH;
	ll rt, si, mb, me, bo, bi, bd, aa, lk, oe, ds, li;
};
fields to_x(vll v)
{
	fields x;
	vll po, rs, ra, rb, xo, rc;
	fr(i, 0, 6)
	{
		po.push_back(v[i]);
	}
	fr(i, 6, 11)
	{
		rs.push_back(v[i]);
	}
	fr(i, 11, 16)
	{
		ra.push_back(v[i]);
	}
	fr(i, 16, 21)
	{
		rb.push_back(v[i]);
	}
	fr(i, 21, 31)
	{
		xo.push_back(v[i]);
	}
	fr(i, 31, 32)
	{
		rc.push_back(v[i]);
	}
	x.po = to_decimal(po);
	x.rs = to_decimal(rs);
	x.ra = to_decimal(ra);
	x.rb = to_decimal(rb);
	x.xo = to_decimal(xo);
	x.rc = to_decimal(rc);

	return x;
}

fields fields_to_i(vll v)
{
	fields x;
	vll po, li, aa, lk;
	fr(i, 0, 6)
	{
		po.push_back(v[i]);
	}
	fr(i, 6, 30)
	{
		li.push_back(v[i]);
	}
	fr(i, 30, 31)
	{
		aa.push_back(v[i]);
	}
	fr(i, 31, 32)
	{
		lk.push_back(v[i]);
	}
	x.po = to_decimal(po);
	x.li = to_decimal(li);
	x.aa = to_decimal(aa);
	x.lk = to_decimal(lk);

	return x;
}

/*---------------------ALL THE INSTRUCTIONS ---------------*/

void and2(vll v)
{
	fields x = to_x(v);
	r.r[x.ra] = r.r[x.rs] & r.r[x.rb];
}

void extsw(vll v)
{
	fields x = to_x(v);
	ll temp = r.r[x.rs] & (4294967296 - 1); //2^32 -1
	r.r[x.ra] = temp;
	if (r.r[x.rs] & (1 << 31))
	{
		temp = (2 ^ 64 - 1) - (2 ^ 32 - 1);
		r.r[x.ra] += temp;
	}
}

void nand(vll v)
{
	fields x = to_x(v);
	r.r[x.ra] = !(r.r[x.rs] & r.r[x.rb]);
}

void or2(vll v)
{
	fields x = to_x(v);
	r.r[x.ra] = r.r[x.rs] | r.r[x.rb];
}

void xor2(vll v)
{
	fields x = to_x(v);
	r.r[x.ra] = r.r[x.rs] ^ r.r[x.rb];
}

void sld(vll v)
{
	fields x = to_x(v);
	ll temp = r.r[x.rb] & (2 ^ 7 - 1);
	r.r[x.ra] = r.r[x.rs] << temp;
}

void srd(vll v)
{
	fields x = to_x(v);
	ll temp = r.r[x.rb] & (2 ^ 7 - 1);
	r.r[x.ra] = r.r[x.rs] >> temp;
}

void srad(vll v)
{
	fields x = to_x(v);
	ll temp = r.r[x.rb] & (2 ^ 7 - 1);
	r.r[x.ra] = r.r[x.rs] >> temp;
	ll zero = 0;
	ll one = 1;
	if (r.r[x.rs] & (one << 63))
	{
		r.r[x.ra] |= ((~(zero) - ((one << (64 - temp)) - 1)));
	}
}

void cmp(vll v)
{
	fields x = to_x(v);
	r.cr >>= 4;
	r.cr <<= 4;
	ll a = r.r[x.ra], b = r.r[x.rb];
	if (a < b)
	{
		r.cr |= 8;
	}
	else if (a > b)
	{
		r.cr |= 4;
	}
	else
	{
		r.cr |= 2;
	}
}

void sc(vll v)
{
	switch (r.r[3])
	{
	case 1:
	{
		cout << r.a0 << endl;
		break;
	}
	case 4:
	{
		ll temp = r.a0;
		while (m_mem[temp])
		{
			cout << (char)m_mem[temp];
		}
		cout << endl;
		break;
	}
	case 5:
	{
		ll temp;
		cin >> temp;
		r.r[3] = temp;
		break;
	}
	case 10:
	{
		cout << "Program executed successfully!!!!" << endl;
		exit(0);
	}
	case 11:
	{
		cout << char(r.a0) << endl;
		break;
	}
	case 12:
	{
		char ch;
		cin >> ch;
		r.v0 = (ll)ch;
		break;
	}
	default:
	{
		cout << "wrong argument for syscall at instruction " << r.cia << endl;
		exit(0);
	}
	}
}

void bl(vll v)
{
	fields x = to_x(v);
	r.lr = r.cia + 4;
	r.nia += x.li;
}

void bclr(vll v)
{
	fields x = to_x(v);
	r.nia = r.lr;
}
/*--------------------------------------------------------*/

void opcode31(vll v)
{
	vll v2;
	fr(i, 21, 31)
	{
		v2.push_back(v[i]);
	}
	ll XO = to_decimal(v2);

	switch (XO)
	{
	case 0:
		cmp(v);
	case 27:
		sld(v);
	case 28:
		and2(v);
	case 40:
		subf(v);
	case 266:
		add(v);
	case 316:
		xor2(v);
	case 413:
		sradi(v);
	case 444:
		or2(v);
	case 476:
		nand(v);
	case 539:
		srd(v);
	case 794:
		srad(v);
	case 986:
	default:
	{
		cout << "wrong XO at instruction " << r.cia << endl;
		exit(0);
	}
	}
}

void divide_by_opcode(vll v)
{
	vll v2;
	fr(i, 0, 6)
	{
		v2.push_back(v[i]);
	}
	ll opcode = to_decimal(v2);
	switch (opcode)
	{
	case 11:
		cmpi(v);
	case 14:
		addi(v);
	case 15:
		addis(v);
	case 17:
		sc(v);
	case 18:
		opcode18(v);
	case 19:
		opcode19(v);
	case 21:
		rlwinm(v);
	case 24:
		ori(v);
	case 26:
		xori(v);
	case 28:
		andi(v);
	case 31:
		opcode31(v);
	case 34:
		lbz(v);
	case 36:
		stw(v);
	case 37:
		stwu(v);
	case 38:
		stb(v);
	case 40:
		lhz(v);
	case 42:
		lha(v);
	case 44:
		sth(v);
	case 58:
		ld(v);
	case 62:
		std(v);
	default:
	{
		cout << "Wrong instruction\n";
		exit(0);
	}
	}
}
void store_instruction(vll v, ll index)
{
	m_inst[4194304 + (index * 4)] = v;
}

void store_memory(vll v, ll index)
{
	ll val = to_decimal(v);
	m_mem[268435456 + (index)] = val;
}

bool is_system_call(vll v)
{
	vll v2;
	fr(i, 0, 6)
	{
		v2.push_back(v[i]);
	}
	return to_decimal(v2) == 17;
}

void execute(ll index)
{
	if (m_inst[index].size() == 0)
	{
		cout << "Instruction not found\n";
		exit(0);
	}
	divide_by_opcode(m_inst[index]);

	//update the next instruction and execute it
	if (is_system_call(m_inst[index]))
	{
		r.cia = r.srr0;
	}
	else
	{
		r.cia = r.nia;
	}
	r.nia = r.cia + 4;
	execute(r.cia);
}
//initialise the registers
void init()
{
	fr(i, 0, 32)
	{
		r.r[i] = 0;
	}
	r.cia = 4194304;
	r.nia = 4194308;
	//put one element into the stack and refer that as the stack pointer
	r.sp = 274877906928;
	m_mem[r.sp] = 0;
	r.gp = 32768 + 268435456;
	r.fp = r.sp;
}
int main(int argc, char *argv[])
{
	/*----------INPUT ------------*/
	vll input;
	if (argc == 2)
	{
		FILE *fp = fopen(argv[1], "r");

		while (1)
		{
			char x = fgetc(fp);
			input.push_back(x - '0');
			if (feof(fp))
				break;
		}
	}
	else
	{
		cout << "Format not proper" << endl;
		cout << "Format is ./a.out binary_file" << endl;
		return 0;
	}
	/*----------------------------*/

	/*----------DATA SIZE AND TEXT SIZE ------------*/
	vll v;
	fr(i, 0, 32)
	{
		v.push_back(input[0]);
		input.pop_back();
	}
	ll data_size = to_decimal(v);
	v.clear();

	fr(i, 0, 32)
	{
		v.push_back(input[0]);
		input.pop_back();
	}
	ll text_size = to_decimal(v);
	/*----------------------------*/

	/*---------MAP ALL INSTRUCTIONS AND MEMORY TO ADDRESS------*/
	fr(i, 0, text_size)
	{
		v.clear();
		fr(j, 0, 32)
		{
			v.push_back(input[0]);
			input.pop_back();
		}
		store_instruction(v, i);
	}

	fr(i, 0, data_size)
	{
		v.clear();
		fr(j, 0, 8)
		{
			v.push_back(input[0]);
			input.pop_back();
		}
		store_memory(v, i);
	}
	/*----------------------------*/

	/*---------MAIN PROGRAM------*/
	init();
	execute(r.cia);
	/*----------------------------*/

	return 0;
}