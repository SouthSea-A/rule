#include "rule_operand.h"


bool immediate::operator==(const immediate& other) const
{
	return this->value == other.value;
}
bool immediate::operator!=(const immediate& other) const
{
	return !(*this == other);
}
immediate& immediate::operator=(const immediate& other)
{
	if (this != &other)
		this->value = other.value;

	return *this;
}



bool variable_param::operator==(const variable_param& other) const
{
	return this->index == other.index;
}
bool variable_param::operator!=(const variable_param& other) const
{
	return !(*this == other);
}
variable_param& variable_param::operator=(const variable_param& other)
{
	if (this != &other)
		this->index = other.index;

	return *this;
}



bool variable_common::operator==(const variable_common& other) const
{
	return this->index == other.index;
}
bool variable_common::operator!=(const variable_common& other) const
{
	return !(*this == other);
}
variable_common& variable_common::operator=(const variable_common& other)
{
	if (this != &other)
		this->index = other.index;

	return *this;
}



bool proxy_param::operator==(const proxy_param& other) const
{
	return this->index == other.index;
}
bool proxy_param::operator!=(const proxy_param& other) const
{
	return !(*this == other);
}
proxy_param& proxy_param::operator=(const proxy_param& other)
{
	if (this != &other)
		this->index = other.index;

	return *this;
}



bool proxy_common::operator==(const proxy_common& other) const
{
	return this->index == other.index;
}
bool proxy_common::operator!=(const proxy_common& other) const
{
	return !(*this == other);
}
proxy_common& proxy_common::operator=(const proxy_common& other)
{
	if (this != &other)
		this->index = other.index;

	return *this;
}





