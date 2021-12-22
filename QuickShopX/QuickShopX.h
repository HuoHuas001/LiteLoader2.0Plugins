#pragma once
#include "pch.h"
typedef unsigned long long chest_id;
static chest_id CID = 8832492234329ull;

struct LockChest {
	BlockPos pos;
	int dim;
	string uniqid;
	string name;
	chest_id chest_id;

	void pack(WBStream& bs)const {
		bs.apply(chest_id, uniqid, pos, dim, name);
	}
	void unpack(RBStream& bs) {
		bs.apply(chest_id, uniqid, pos, dim, name);
		string val;
	}

	static string getPosName(int a1, int a2, int a3, int a4) {
		string val;
		char x[25];
		char y[25];
		char z[25];
		char dim[25];
		auto xs = itoa1(a1, x, 10);
		auto ys = itoa1(a2, y, 10);
		auto zs = itoa1(a3, z, 10);
		auto dims = itoa1(a4, dim, 10);
		std::string const& cc = std::string(xs) + std::string(ys) + std::string(zs) + std::string(dims);
		return cc;
	}

};