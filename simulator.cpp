#include <bits/stdc++.h>

using namespace std;

typedef long long ll;
typedef vector<ll> vll;
#define fr(i, a, b) for (long long i = a; i < b; ++i)
#define fr2(i, b, a) for (long long i = b; i >= a; --i)

template <typename T>
void show_vector(vector<T> v)
{
	cout << endl
		 << "The vector is ";
	int i;
	fr(i, 0, v.size())
	{
		cout << v[i] << " ";
	}
	cout << endl;
}

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
	fr(i, 0, v.size())
	{
		rv = 2 * rv + v[i];
	}
	return rv;
}

ll to_signed_decimal(vll v)
{
	ll rv = 0;
	rv = -v[0];
	fr(i, 1, v.size())
	{
		rv = 2 * rv + v[i];
	}
	return rv;
}
struct fields
{
	ll po, rs, ra, rb, xo, rc;
	//NOTE THE DIFFERENCE B/W sh, SH
	ll sh, SH, sh1;
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

fields to_i(vll v)
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

fields to_xo(vll v)
{

	fields x;
	vll po, rt, ra, rb, oe, xo, rc;

	fr(i, 0, 6)
	{
		po.push_back(v[i]);
	}
	fr(i, 6, 11)
	{
		rt.push_back(v[i]);
	}
	fr(i, 11, 16)
	{
		ra.push_back(v[i]);
	}
	fr(i, 16, 21)
	{
		rb.push_back(v[i]);
	}
	fr(i, 21, 22)
	{
		oe.push_back(v[i]);
	}
	fr(i, 22, 31)
	{
		xo.push_back(v[i]);
	}
	fr(i, 31, 32)
	{
		rc.push_back(v[i]);
	}

	x.po = to_decimal(po);
	x.rt = to_decimal(rt);
	x.ra = to_decimal(ra);
	x.rb = to_decimal(rb);
	x.oe = to_decimal(oe);
	x.xo = to_decimal(xo);
	x.rc = to_decimal(rc);

	return x;
}

fields to_xs(vll v)
{

	fields x;
	vll po, rs, ra, sh, xo, sh1, rc;

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
		sh.push_back(v[i]);
	}
	fr(i, 21, 30)
	{
		xo.push_back(v[i]);
	}
	fr(i, 30, 31)
	{
		sh1.push_back(v[i]);
	}
	fr(i, 31, 32)
	{
		rc.push_back(v[i]);
	}

	x.po = to_decimal(po);
	x.rs = to_decimal(rs);
	x.ra = to_decimal(ra);
	x.sh = to_decimal(sh);
	x.xo = to_decimal(xo);
	x.sh1 = to_decimal(sh1);
	x.rc = to_decimal(rc);

	return x;
}

fields to_d(vll v)
{
	fields x;
	vll po, rt, ra, si;
	fr(i, 0, 6)
	{
		po.push_back(v[i]);
	}
	fr(i, 6, 11)
	{
		rt.push_back(v[i]);
	}
	fr(i, 11, 16)
	{
		ra.push_back(v[i]);
	}
	fr(i, 16, 32)
	{
		si.push_back(v[i]);
	}
	x.po = to_decimal(po);
	x.rt = to_decimal(rt);
	x.ra = to_decimal(ra);
	x.si = to_decimal(si);

	return x;
}

fields to_m(vll v)
{

	fields x;
	vll po, rs, ra, SH, mb, me, rc;

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
		SH.push_back(v[i]);
	}
	fr(i, 21, 26)
	{
		mb.push_back(v[i]);
	}
	fr(i, 26, 31)
	{
		me.push_back(v[i]);
	}
	fr(i, 31, 32)
	{
		rc.push_back(v[i]);
	}

	x.po = to_decimal(po);
	x.rs = to_decimal(rs);
	x.ra = to_decimal(ra);
	x.SH = to_decimal(SH);
	x.mb = to_decimal(mb);
	x.me = to_decimal(me);
	x.rc = to_decimal(rc);

	return x;
}

fields to_b(vll v)
{
	fields x;
	vll po, bo, bi, bd, aa, lk;
	fr(i, 0, 6)
	{
		po.push_back(v[i]);
	}
	fr(i, 6, 11)
	{
		bo.push_back(v[i]);
	}
	fr(i, 11, 16)
	{
		bi.push_back(v[i]);
	}
	fr(i, 16, 30)
	{
		bd.push_back(v[i]);
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
	x.bo = to_decimal(bo);
	x.bi = to_decimal(bi);
	x.bd = to_decimal(bd);
	x.aa = to_decimal(aa);
	x.lk = to_decimal(lk);

	return x;
}

fields to_ds(vll v)
{

	fields x;
	vll po, rt, ra, ds, xo;

	fr(i, 0, 6)
	{
		po.push_back(v[i]);
	}
	fr(i, 6, 11)
	{
		rt.push_back(v[i]);
	}
	fr(i, 11, 16)
	{
		ra.push_back(v[i]);
	}
	fr(i, 16, 30)
	{
		ds.push_back(v[i]);
	}
	fr(i, 30, 32)
	{
		xo.push_back(v[i]);
	}

	x.po = to_decimal(po);
	x.rt = to_decimal(rt);
	x.ra = to_decimal(ra);
	x.ds = to_decimal(ds);
	x.xo = to_decimal(xo);

	return x;
}

/*---------------------ALL THE INSTRUCTIONS ---------------*/

void b(vll v)
{
	vll v2;
	fr(i, 6, 30)
	{
		v2.push_back(v[i]);
	}
	ll change = to_signed_decimal(v2);
	r.nia = r.cia + change;
}

void add(vll v)
{

	fields x = to_xo(v);
	r.r[x.rt] = r.r[x.ra] + r.r[x.rb];
}

void addi(vll v)
{

	fields x = to_d(v);
	r.r[x.rt] = r.r[x.ra] + x.si;
}

void and2(vll v)
{
	fields x = to_x(v);
	r.r[x.ra] = r.r[x.rs] & r.r[x.rb];
}

void andi(vll v)
{
	fields x = to_d(v);
	r.r[x.rt] = r.r[x.ra] & x.si;
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

void lwz(vll v)
{

	fields x = to_d(v);
	r.r[x.rt] = m_mem[r.r[x.ra] + x.si];
}

void stw(vll v)
{

	fields x = to_d(v);
	m_mem[r.r[x.ra] + x.si + 4] = r.r[x.rt];
}

void or2(vll v)
{
	fields x = to_x(v);
	r.r[x.ra] = r.r[x.rs] | r.r[x.rb];
}

void ori(vll v)
{
	fields x = to_d(v);
	r.r[x.rt] = r.r[x.ra] | x.si;
}

void xor2(vll v)
{
	fields x = to_x(v);
	r.r[x.ra] = r.r[x.rs] ^ r.r[x.rb];
}

void xori(vll v)
{
	fields x = to_d(v);
	r.r[x.rt] = r.r[x.ra] ^ x.si;
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
	ll a = r.r[x.rs], b = r.r[x.rb];
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

void cmpi(vll v)
{

	fields x = to_d(v);
	r.cr >>= 4;
	r.cr <<= 4;
	ll a = r.r[x.ra], b = x.si;
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
	r.srr0 = r.nia;
	switch (r.r[2])
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
			temp += 4;
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
	fields x = to_i(v);
	r.lr = r.cia + 4;
	r.nia += x.li;
}

void bclr(vll v)
{
	fields x = to_x(v);
	r.nia = r.lr;
}

void bc(vll v)
{
	fields x = to_b(v);
	if (x.bo == 28)
	{
		if ((1 << 3) & r.cr)
			r.nia = x.bd;
	}
	else if (x.bo == 29)
	{
		if ((1 << 2) & r.cr)
			r.nia = x.bd;
	}
	else
	{
		if ((1 << 1) & r.cr)
			r.nia = r.cia + x.bd;
	}
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
		break;
	case 27:
		sld(v);
		break;
	case 28:
		and2(v);
		break;
	case 266:
		add(v);
		break;
	case 316:
		xor2(v);
		break;
	case 444:
		or2(v);
		break;
	case 476:
		nand(v);
		break;
	case 539:
		srd(v);
		break;
	case 794:
		srad(v);
		break;
	case 986:
		extsw(v);
		break;
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
		break;
	case 14:
		addi(v);
		break;
	case 17:
		sc(v);
		break;
	case 18:
		b(v);
		break;
	case 19:
		bc(v);
		break;
	case 24:
		ori(v);
		break;
	case 26:
		xori(v);
		break;
	case 28:
		andi(v);
		break;
	case 31:
		opcode31(v);
		break;
	case 32:
		lwz(v);
		break;
	case 36:
		stw(v);
		break;

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
	ll val = to_signed_decimal(v);
	m_mem[index * 4] = val;
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
		cout << "Iznstruction not found\n";
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
	char x;
	while (cin >> x)
	{
		input.push_back(x - '0');
	}
	/*----------------------------*/

	/*----------DATA SIZE AND TEXT SIZE ------------*/
	vll v;
	fr(i, 0, 32)
	{
		v.push_back(input[0]);
		input.erase(input.begin());
	}
	ll text_size = to_decimal(v);
	v.clear();

	fr(i, 0, 32)
	{
		v.push_back(input[0]);
		input.erase(input.begin());
	}
	ll data_size = to_decimal(v);
	/*----------------------------*/

	/*---------MAP ALL INSTRUCTIONS AND MEMORY TO ADDRESS------*/
	fr(i, 0, text_size)
	{
		v.clear();
		fr(j, 0, 32)
		{
			v.push_back(input[0]);
			input.erase(input.begin());
		}
		store_instruction(v, i);
	}

	fr(i, 0, data_size)
	{
		v.clear();
		fr(j, 0, 32)
		{
			v.push_back(input[0]);
			input.erase(input.begin());
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
