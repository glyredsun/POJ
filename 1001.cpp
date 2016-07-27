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

	friend BigReal operator* (const BigReal &left, const BigReal &right);
	friend BigReal operator+ (const BigReal &left, const BigReal &right);

public:

	void elimateZero()
	{
		int i = -1;
		while (integerPart[++i] == '0');
		if (i)
		{
			integerPart = std::move(integerPart.substr(i));
		}

		for (int j = fractionalPart.length() - 1; j >= 0 && fractionalPart[j] == '0'; --j)
		{
			fractionalPart.pop_back();
		}
	}

private:
	bool positive{true};
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
		else if (ch == '-' && !started)
		{
			real.positive = false;
		}
		else if (started)
		{
			in.putback(ch);
			break;
		}
	}

	real.elimateZero();
	
	return in;
}

std::ostream & operator<< (std::ostream &out, BigReal &real)
{
	if (!real.positive && !real.integerPart.empty() && !real.fractionalPart.empty())
	{
		out << '-';
	}

	if (real.integerPart.empty())
	{
		out << '0';
	}
	else
	{
		out << real.integerPart;
	}
	
	if (!real.fractionalPart.empty())
	{
		out << '.' << real.fractionalPart;
	}

	return out;
}

BigReal operator* (const BigReal &left, const BigReal &right)
{
	BigReal retVal;
	BigReal temp;

	std::string buf;
	size_t bufLen = left.fractionalPart.length() + left.integerPart.length() + 1;

	char result;
	char carry = 0;
	
	for (int i = 0; i < right.fractionalPart.length(); ++i)
	{
		carry = 0;
		size_t bufPos = bufLen - 1;
		buf.resize(bufLen, '0');
		char v = right.fractionalPart[i] - '0';
		for (int j = left.fractionalPart.length() - 1; j >= 0; --j)
		{
			result = (left.fractionalPart[j] - '0') * v + carry;
			carry = result / 10;
			buf[bufPos--] = '0' + result - carry * 10;
		}
		for (int k = left.integerPart.length() - 1; k >= 0; --k)
		{
			result = (left.integerPart[k] - '0') * v + carry;
			carry = result / 10;
			buf[bufPos--] = '0' + result - carry * 10;
		}

		if (carry)
		{
			buf[bufPos--] = '0' + carry;
		}

		size_t fracPartLen = i + 1 + left.fractionalPart.length();
		size_t len = bufLen - bufPos - 1;
		temp.fractionalPart.resize(fracPartLen, '0');
		if (fracPartLen >= len)
		{
			temp.fractionalPart.replace(temp.fractionalPart.begin() + fracPartLen - len, temp.fractionalPart.end(), buf.begin() + bufPos + 1, buf.end());
		}
		else
		{
			temp.fractionalPart.replace(temp.fractionalPart.begin(), temp.fractionalPart.end(), buf.end() - len, buf.end());
			temp.integerPart.resize(len - fracPartLen);
			temp.integerPart.replace(temp.integerPart.begin(), temp.integerPart.end(), buf.begin(), buf.end() - len);
		}
		retVal = retVal + temp;
	}

	for (int i = 0; i < right.integerPart.length(); ++i)
	{
		size_t bufPos = bufLen - 1;
		buf.resize(bufLen, '0');
		char v = right.integerPart[right.integerPart.length - i - 1] - '0';
		for (int j = left.fractionalPart.length() - 1; j >= 0; --j)
		{
			result = (left.fractionalPart[j] - '0') * v + carry;
			carry = result / 10;
			buf[bufPos--] = '0' + result - carry * 10;
		}
		for (int k = left.integerPart.length() - 1; k >= 0; --k)
		{
			result = (left.integerPart[k] - '0') * v + carry;
			carry = result / 10;
			buf[bufPos--] = '0' + result - carry * 10;
		}

		if (carry)
		{
			buf[bufPos--] = '0' + carry;
		}

		size_t fracPartLen = left.fractionalPart.length() - i;
		size_t len = bufLen - bufPos - 1;
		temp.fractionalPart.resize(fracPartLen, '0');
		if (fracPartLen >= len)
		{
			temp.fractionalPart.replace(temp.fractionalPart.begin() + fracPartLen - len, temp.fractionalPart.end(), buf.begin() + bufPos + 1, buf.end());
		}
		else
		{
			temp.fractionalPart.replace(temp.fractionalPart.begin(), temp.fractionalPart.end(), buf.end() - len, buf.end());
			temp.integerPart.resize(len - fracPartLen);
			temp.integerPart.replace(temp.integerPart.begin(), temp.integerPart.end(), buf.begin(), buf.end() - len);
		}
		retVal = retVal + temp;
	}
}

BigReal operator+ (const BigReal &left, const BigReal &right)
{
	BigReal retVal;

	const std::string &longerFrac = left.fractionalPart.length() > right.fractionalPart.length() ? left.fractionalPart : right.fractionalPart;
	const std::string &shorterFrac = left.fractionalPart.length() < right.fractionalPart.length() ? left.fractionalPart : right.fractionalPart;
	const std::string &longerInt = left.integerPart.length() > right.integerPart.length() ? left.integerPart : right.integerPart;
	const std::string &shorterInt = left.integerPart.length() < right.integerPart.length() ? left.integerPart : right.integerPart;
	
	retVal.fractionalPart.resize(longerFrac.length());
	
	char carry = 0;
	char result;
	
	for (int i = shorterFrac.length() - 1; i >= 0; --i)
	{
		result = (shorterFrac[i] - '0') + (longerFrac[i] - '0') + carry;
		carry = result / 10;
		retVal.fractionalPart[i] = '0' + result - 10 * carry;
	}

	for (int j = shorterFrac.length(); j < longerFrac.length(); ++j)
	{
		retVal.fractionalPart[j] = longerFrac[j];
	}

	retVal.integerPart.resize(longerInt.length());
	
	for (int k = 0; k < longerInt.length(); ++k)
	{
		if (k < shorterInt.length())
		{
			result = (shorterInt[shorterInt.length() - k - 1] - '0') + (longerInt[longerInt.length() - k - 1] - '0') + carry;
		}
		else if (carry)
		{
			result = (longerInt[longerInt.length() - k - 1] - '0') + carry;
		}
		else
		{
			retVal.integerPart[longerInt.length() - k - 1] = longerInt[longerInt.length() - k - 1];
			continue;;
		}
		carry = result / 10;
		retVal.integerPart[longerInt.length() - k - 1] = '0' + result - 10 * carry;
	}

	if (carry)
	{
		retVal.integerPart.insert(0, 1, carry + '0');
	}
}

int main(void)
{
	BigReal real1, real2;
	std::cin >> real1 >> real2;
	std::cout << "real1 : " << real1 << ",    " << "real2 : " << real2 << std::endl;

	system("pause");
	return 0;
}