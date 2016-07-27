/*

Exponentiation

http://poj.org/problem?id=1001
*/

#include <string>
#include <iostream>

class BigReal
{
	friend std::istream & operator>> (std::istream &, BigReal &);
	friend std::ostream & operator<< (std::ostream &, BigReal &);

private:
	std::string integerPart{'0'};
	std::string fractionalPart;
};

std::istream & operator>> (std::istream &in, BigReal &real)
{
	bool inFractional = false;
	bool started = false;

	real.integerPart.clear();
	real.fractionalPart.clear();

	for (int ch = in.get(); ch != EOF; ch = in.get())
	{
		if (ch >= '0' && ch <= '9')
		{
			started = true;
			if (!inFractional)
			{
				real.integerPart.append(1, ch);
			}
			else
			{
				real.fractionalPart.append(1, ch);
			}
		}
		else if (ch == '.')
		{
			started = true;
			inFractional = true;
		}
		else if (started)
		{
			in.putback(ch);
			break;
		}
	}

	return in;
}

std::ostream & operator<< (std::ostream &out, BigReal &real)
{
	out << real.integerPart;
	if (!real.fractionalPart.empty())
	{
		out << '.' << real.fractionalPart;
	}
	return out;
}

int main(void)
{
	BigReal real1, real2;
	std::cin >> real1 >> real2;
	std::cout << "real1 : " << real1 << ",    " << "real2 : " << real2 << std::endl;

	system("pause");
	return 0;
}