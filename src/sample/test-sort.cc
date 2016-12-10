#include <cstdio>
#include <vector>
#include <string>
#include <algorithm>

int main()
{
	std::vector<std::string> vec = {
		"hiker",
		"intortillage",
		"noose",
		"bungee",
		"engage",
		"remeasure",
		"mimotypic",
		"banker",
		"aberuncator",
		"foolhardy"
	};

	std::sort(vec.begin(), vec.end());

	for (auto &str : vec) {
		printf("%s\n", str.c_str());
	}
}