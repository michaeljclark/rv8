extern "C" int sete(long long a, long long b) { return a == b; }
extern "C" int setne(long long a, long long b) { return a != b; }
extern "C" int setle(long long a, long long b) { return a <= b; }
extern "C" int setl(long long a, long long b) { return a < b; }
extern "C" int setge(long long a, long long b) { return a >= b; }
extern "C" int setg(long long a, long long b) { return a > b; }
extern "C" int setbe(unsigned long long a, unsigned long long b) { return a <= b; }
extern "C" int setb(unsigned long long a, unsigned long long b) { return a < b; }
extern "C" int setae(unsigned long long a, unsigned long long b) { return a >= b; }
extern "C" int seta(unsigned long long a, unsigned long long b) { return a > b; }

int main()
{
	sete(1, 2);
	setne(1, 2);
	setle(1, 2);
	setl(1, 2);
	setge(1, 2);
	setg(1, 2);
	setbe(1, 2);
	setb(1, 2);
	setae(1, 2);
	seta(1, 2);
}
