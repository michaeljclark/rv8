extern "C" int sete(int a, int b) { return a == b; }
extern "C" int setne(int a, int b) { return a != b; }
extern "C" int setle(int a, int b) { return a <= b; }
extern "C" int setl(int a, int b) { return a < b; }
extern "C" int setge(int a, int b) { return a >= b; }
extern "C" int setg(int a, int b) { return a > b; }
extern "C" int setbe(unsigned a, unsigned b) { return a <= b; }
extern "C" int setb(unsigned a, unsigned b) { return a < b; }
extern "C" int setae(unsigned a, unsigned b) { return a >= b; }
extern "C" int seta(unsigned a, unsigned b) { return a > b; }

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
