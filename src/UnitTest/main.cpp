#include <iostream>

#include "basic.h"

using namespace std;

int main()
{
	Time t(2015, 9, 8, 21, 02, 0);
	cout << t.GetTime() << endl;

	struct tm st;
	st.tm_year = 2015 - 1900;
	st.tm_mon = 9 - 1;
	st.tm_mday = 8;
	st.tm_hour = 21;
	st.tm_min = 2;
	st.tm_sec = 0;

	cout << mktime(&st) << endl;

	getchar();

	return 0;
}