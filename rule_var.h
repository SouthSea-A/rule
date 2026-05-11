#pragma once
#include <vector>
#include <optional>
#include <span>
#include "rule_base.h"

using var_single = data_type_unsigned_base;
using var_index = data_type_unsigned_base;
using var_ref = data_type_unsigned_base;

static constexpr var_single VAR_DEFAULT_CONTENT = 0x0000000000000000ULL;
static constexpr var_index VAR_INVALID_INDEX = 0xFFFFFFFFFFFFFFFFULL;
static constexpr var_ref VAR_INVALID_REF = 0xFFFFFFFFFFFFFFFFULL;

struct var
{
	var_index index;
	var_single content;
	var();
	var(var_index index_, var_single content_);
	var(const var& var_);
	var(var&& var_) noexcept;
	bool is_valid() const;
	var& operator=(const var& other);
};

struct var_count
{
	var variable;
	var_ref ref;
	var_count();
	var_count(const var& variable, var_ref ref);
	var_count(const var_count& var_count_);
	var_count(var_count&& var_count_) noexcept;
	var_count& operator=(const var_count& other);
};

class var_manager
{
private:
	std::vector<var_count> var_set;
private:
	bool test_validity_var_index(var_index index) const;
	std::optional<size_t> get_var_pos(var_index index, bool is_save_lower = true) const;
	std::optional<var_count*> get_var_count_ptr(size_t pos);
	std::optional<const var_count*> get_var_count_ptr_const(size_t pos) const;
public:
	var_manager();
	~var_manager();
public:
	bool add_var(const var& var_new);
	bool del_var(var_index index);
	std::optional<var_single> get_var(var_index index) const;
	std::optional<var_ref> get_var_ref(var_index index) const;
	bool get_var(std::vector<var_single>& save, var_index start, size_t quantity)const;
	bool set_var(const var& var_change);
	bool set_var(var_index index, var_single content);
	bool set_var(std::span<const var_single> var_all);
	bool set_var(var_index index_begin, std::span<const var_single> var_all);
	std::optional<size_t> get_quantity_var() const;
public:
	var_manager& operator=(const var_manager& other);
};
