#include <string>
#include <iostream>
#include <iterator>
#include <algorithm>
using namespace std;
class line_iterator: public iterator<input_iterator_tag, string,ptrdiff_t, const string*, const string&>
{
	istream* in;
	string line;
	bool is_valid;
	void read()
	{
		if(*in)
			getline(*in, line);
		is_valid = (*in) ? true : false;
	}
public:
	line_iterator() : in(0), is_valid(false) {}
	line_iterator(istream& s): in(&s) { read(); }
	reference operator *()const { return line; }
	pointer operator->()const{ return &line; }
	line_iterator operator++()
	{
		read();
		return *this;
	}
	line_iterator operator ++(int)
	{
		line_iterator tmp = *this;
		read();
		return tmp;
	}
	bool operator ==(const line_iterator& i)const
	{
		return (in == i.in && is_valid == i.is_valid)
				|| (is_valid == false && i.is_valid == false);
	}
	bool operator !=(const line_iterator& i)const
	{
		return !(*this == i);
	}
};
