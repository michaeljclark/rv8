extern "C" bool sete(int a, int b) { return a == b; }
extern "C" bool setne(int a, int b) { return a != b; }
extern "C" bool setle(int a, int b) { return a <= b; }
extern "C" bool setl(int a, int b) { return a < b; }
extern "C" bool setge(int a, int b) { return a >= b; }
extern "C" bool setg(int a, int b) { return a > b; }
extern "C" bool setbe(unsigned a, unsigned b) { return a <= b; }
extern "C" bool setb(unsigned a, unsigned b) { return a < b; }
extern "C" bool setae(unsigned a, unsigned b) { return a >= b; }
extern "C" bool seta(unsigned a, unsigned b) { return a > b; }

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
