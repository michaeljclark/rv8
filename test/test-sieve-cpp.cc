#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <chrono>

using namespace std;
using namespace std::chrono;

/* sieve */

struct sieve
{
	vector<uint32_t> primes;

	bool test(int64_t p) { return primes[p >> 5] & 1 << (p & 0x1f); }
	void set(int64_t p) { primes[p >> 5] |= 1 << (p & 0x1f); }
	bool is_prime(int64_t p) { return !test(p); }

	sieve(int64_t limit) {
		primes.resize((limit >> 5) + 1);
		int64_t p = 2, sqrt_limit = static_cast<int64_t>(sqrt(limit));
		while (p <= sqrt_limit) {
			for (int64_t n = 2 * p; n <= limit; n += p) if (!test(n)) set(n);
			while (++p <= sqrt_limit && test(p));
		}
	}
};

void find_largest_prime_factor(long long n)
{
	long long limit = sqrt(n) * 3; // hack hack hack
	sieve s(limit);
	int lf = 1;
	for (int i = 3; i < limit; i++) {
		if (s.is_prime(i) && n % i == 0) {
			lf = i;
		}
	}
	cout << lf << endl;
}

int main(int argc, const char * argv[])
{
	find_largest_prime_factor(1147072269041);
}
