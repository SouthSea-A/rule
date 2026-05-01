#pragma once
#include <variant>
#include <array>
#include "rule_var.h"

struct immediate
{
	var_single value;
	bool operator==(const immediate& other) const;
	bool operator!=(const immediate& other) const;
	immediate& operator=(const immediate& other);
};

struct variable_param
{
	var_index index;
	bool operator==(const variable_param& other) const;
	bool operator!=(const variable_param& other) const;
	variable_param& operator=(const variable_param& other);
};

struct variable_common
{
	var_index index;
	bool operator==(const variable_common& other) const;
	bool operator!=(const variable_common& other) const;
	variable_common& operator=(const variable_common& other);
};

struct proxy_param
{
	var_index index;
	bool operator==(const proxy_param& other) const;
	bool operator!=(const proxy_param& other) const;
	proxy_param& operator=(const proxy_param& other);
};

struct proxy_common
{
	var_index index;
	bool operator==(const proxy_common& other) const;
	bool operator!=(const proxy_common& other) const;
	proxy_common& operator=(const proxy_common& other);
};

enum class operand_type : data_type_unsigned_base
{
	immediate,
	variable_param,
	variable_common,
	proxy_param,
	proxy_common,
	invalid,
};

using operand = std::variant<immediate, variable_param, variable_common, proxy_param, proxy_common>;
static constexpr size_t OPERAND_QUANTITY_MAX = 5;

enum class operand_quantity : data_type_unsigned_base
{
	zero,
	one,
	two,
	three,
	four,
	five,
};

enum class operand_index : data_type_unsigned_base
{
	first,
	second,
	third,
	fourth,
	fifth,
};

using oq = operand_quantity;
using oi = operand_index;

template<oq quan>
struct operand_set
{
	std::array<operand, static_cast<size_t>(quan)> set;
	operand_set() :
		set{}
	{
		static_assert(static_cast<size_t>(quan) <= OPERAND_QUANTITY_MAX, "超出最大操作数数量限制");
	}
	template<typename... ops>
	operand_set(ops&&... args) :
		set{ std::forward<ops>(args)... }
	{
		static_assert(
			sizeof...(ops) <= OPERAND_QUANTITY_MAX &&
			static_cast<size_t>(quan) <= OPERAND_QUANTITY_MAX &&
			sizeof...(ops) <= static_cast<size_t>(quan),
			"超出最大操作数数量限制"
			);

		static_assert(sizeof...(ops) == static_cast<size_t>(quan), "操作数数量不足");
	}
	operand& operator[](oi index)
	{
		return this->set[static_cast<size_t>(index)];
	}
	const operand& operator[](oi index) const
	{
		return this->set[static_cast<size_t>(index)];
	}
};
