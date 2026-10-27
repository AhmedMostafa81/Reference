//Ref: https://g...content-available-to-author-only...b.com/kth-competitive-programming/kactl/blob/main/content/data-structures/LineContainer.h
/**
 * Author: Simon Lindholm
 * Date: 2017-04-20
 * License: CC0
 * Source: own work
 * Description: Container where you can add lines of the form kx+m, and query maximum values at points x.
 *  Useful for dynamic programming (``convex hull trick'').
 * Time: O(\log N)
 * Status: stress-tested
 */
#pragma once

struct Line {
	ll k, m;
	mutable ll p;
	bool operator<(const Line& o) const {
		return k < o.k;
	}
	bool operator<(const ll&x) const {
		return p < x;
	}
};

struct LineContainer : multiset<Line, less<>> {
	// (for doubles, use inf = 1/.0, div(a,b) = a/b)
	const ll inf = numeric_limits<ll>::max();
	ll div(ll a, ll b) { // floored division
		return a / b - ((a ^ b) < 0 && a % b);
	}
	bool isect(iterator x, iterator y) {
		if (y == end()) { x->p = inf; return false; }
		if (x->k == y->k) x->p = x->m > y->m ? inf : -inf;
		else x->p = div(y->m - x->m, x->k - y->k);
		return x->p >= y->p;
	}
	void add(ll k, ll m) { // y = kx + m
		auto z = insert({k, m, 0}), y = z++, x = y;
		while (isect(y, z)) z = erase(z);
		if (x != begin() && isect(--x, y)) isect(x, y = erase(y));
		while ((y = x) != begin() && (--x)->p >= y->p)
			isect(x, erase(y));
	}
	ll query(ll x) { // maximum y
		assert(!empty());
		auto l = *lower_bound(x);
		return l.k * x + l.m;
	}
};

// to find the minimum
// add(-k, -m)
// query(x) * -1
