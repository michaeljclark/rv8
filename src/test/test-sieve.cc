#include <cstdio>
#include <cstdint>
#include <cinttypes>
#include <cmath>
#include <vector>

using namespace std;

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

int64_t find_smallest_prime_factor(int64_t n)
{
	int64_t limit = static_cast<int64_t>(sqrt(n)) * 2;
	sieve s(limit);
	for (int64_t i = 3; i < limit; i++)
		if (s.is_prime(i) && n % i == 0) return i;
	return -1;
}

int main(int argc, const char * argv[])
{
	printf("factoring... ");
	fflush(stdout);
	int64_t f = find_smallest_prime_factor(1147072269041);
	printf("%" PRId64 "\n", f);
}
