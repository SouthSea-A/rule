#include "rule_var.h"

var::var() :
	content(VAR_DEFAULT_CONTENT), index(VAR_INVALID_INDEX)
{

}
var::var(var_index index_, var_single content_) :
	content(content_), index(index_)
{

}
var::var(const var& var_) :
	index(var_.index), content(var_.content)
{

}
var::var(var&& var_) noexcept :
	index(var_.index), content(var_.content)
{
	var_.index = VAR_INVALID_INDEX;
	var_.content = VAR_DEFAULT_CONTENT;
}
bool var::is_valid() const
{
	return this->index != VAR_INVALID_INDEX;
}
var& var::operator=(const var& other)
{
	if (this != &other)
	{
		this->index = other.index;
		this->content = other.content;
	}
	return *this;
}


var_count::var_count() :
	variable(), ref(VAR_INVALID_REF)
{

}
var_count::var_count(const var& variable, var_ref ref) :
	variable(variable), ref(ref)
{

}
var_count::var_count(const var_count& var_count_) :
	variable(var_count_.variable), ref(var_count_.ref)
{

}
var_count::var_count(var_count&& var_count_) noexcept :
	variable(std::move(var_count_.variable)), ref(var_count_.ref)
{
	var_count_.ref = VAR_INVALID_REF;
}
var_count& var_count::operator=(const var_count& other)
{
	if (this != &other)
	{
		this->variable = other.variable;
		this->ref = other.ref;
	}
	return *this;
}


bool var_manager::test_validity_var_index(var_index index) const
{
	return index != VAR_INVALID_INDEX;
}
std::optional<size_t> var_manager::get_var_pos(var_index index, bool is_save_lower) const
{
	if (test_validity_var_index(index) == false)
		return std::nullopt;

	std::vector<var_count>::const_iterator pos_begin = this->var_set.begin();
	std::vector<var_count>::const_iterator pos_end = this->var_set.end();
	std::vector<var_count>::const_iterator pos_seek =
		std::lower_bound(
			pos_begin,
			pos_end,
			index,
			[](const var_count& test, var_index target)->bool
			{
				return test.variable.index < target;
			}
		);

	if (pos_seek == pos_end)
		return std::nullopt;

	if (is_save_lower == false && pos_seek->variable.index != index)
		return std::nullopt;

	return std::make_optional<size_t>(pos_seek - pos_begin);
}
std::optional<var_count*> var_manager::get_var_count_ptr(size_t pos)
{
	if (pos >= this->var_set.size())
		return std::nullopt;

	return std::make_optional<var_count*>(&this->var_set[pos]);
}
std::optional<const var_count*> var_manager::get_var_count_ptr_const(size_t pos) const
{
	if (pos >= this->var_set.size())
		return std::nullopt;

	return std::make_optional<const var_count*>(&this->var_set[pos]);
}
var_manager::var_manager() :
	var_set()
{

}
var_manager::~var_manager()
{

}
bool var_manager::add_var(const var& var_new)
{
	if (var_new.is_valid() == false)
		return false;

	std::optional<size_t> pos_insert = get_var_pos(var_new.index, true);

	if (pos_insert.has_value() == true)
	{
		std::optional<var_count*> ptr_var_count = get_var_count_ptr(pos_insert.value());

		if (ptr_var_count.has_value() == false)
			return false;

		if (ptr_var_count.value()->variable.index == var_new.index)
		{
			var_count* var_count_ptr = ptr_var_count.value();
			var_count_ptr->ref++;
			var_count_ptr->variable.content = var_new.content;
		}
		else
		{
			this->var_set.insert(this->var_set.begin() + pos_insert.value(), var_count(var_new, 1));
		}
	}
	else
	{
		this->var_set.emplace_back(var_count(var_new, 1));
	}

	return true;
}
bool var_manager::del_var(var_index index)
{
	std::optional<size_t> pos_del = get_var_pos(index, false);

	if (pos_del.has_value() == false)
		return false;

	std::optional<var_count*> ptr_var_count = get_var_count_ptr(pos_del.value());

	if (ptr_var_count.has_value() == false)
		return false;

	var_count* var_count_ptr = ptr_var_count.value();

	if (--var_count_ptr->ref == 0)
		this->var_set.erase(this->var_set.begin() + pos_del.value());

	return true;
}
std::optional<var_single> var_manager::get_var(var_index index) const
{
	std::optional<size_t> pos_get = get_var_pos(index, true);

	if (pos_get.has_value() == false)
		return std::nullopt;

	std::optional<const var_count*> ptr_var_count = get_var_count_ptr_const(pos_get.value());

	if (ptr_var_count.has_value() == false)
		return std::nullopt;

	return std::make_optional<var_single>(ptr_var_count.value()->variable.content);
}
std::optional<var_ref> var_manager::get_var_ref(var_index index) const
{
	std::optional<size_t> pos_get = get_var_pos(index, true);

	if (pos_get.has_value() == false)
		return std::nullopt;

	std::optional<const var_count*> ptr_var_count = get_var_count_ptr_const(pos_get.value());

	if (ptr_var_count.has_value() == false)
		return std::nullopt;

	return std::make_optional<var_ref>(ptr_var_count.value()->ref);
}
bool var_manager::get_var(std::vector<var_single>& save, var_index start, size_t quantity)const
{
	save.clear();

	std::optional<size_t> var_pos_target_result = get_var_pos(start, true);

	if (var_pos_target_result.has_value() == false)
		return false;

	size_t pos_start = var_pos_target_result.value();

	if (this->var_set.size() - pos_start < quantity)
		return false;

	save.resize(quantity);

	std::vector<var_count>::const_iterator src_current = this->var_set.begin() + pos_start;
	size_t index = 0;

	while (quantity != 0)
	{
		save[index] = src_current->variable.content;
		src_current++;
		index++;
		quantity--;
	}

	return true;
}
bool var_manager::set_var(const var& var_change)
{
	std::optional<size_t> pos_get = get_var_pos(var_change.index, true);

	if (pos_get.has_value() == false)
		return false;

	std::optional<var_count*> ptr_var_count = get_var_count_ptr(pos_get.value());

	if (ptr_var_count.has_value() == false)
		return false;

	ptr_var_count.value()->variable.content = var_change.content;
	return true;
}
bool var_manager::set_var(var_index index, var_single content)
{
	return set_var(var(index, content));
}
bool var_manager::set_var(const std::vector<var_single>& var_all)
{
	if (var_all.size() > this->var_set.size())
		return false;

	for (size_t index = 0; index < var_all.size(); index++)
	{
		this->var_set[index].variable.content = var_all[index];
	}

	return true;
}
bool var_manager::set_var(var_index index_begin, const std::vector<var_single>& var_all)
{
	std::optional<size_t> pos_begin = get_var_pos(index_begin);

	if (pos_begin.has_value() == false)
		return false;

	if (this->var_set.size() - pos_begin.value() < var_all.size())
		return false;

	size_t index = pos_begin.value();
	size_t index_src = 0;

	while (index < var_all.size())
	{
		this->var_set[index].variable.content = var_all[index_src];
		index++;
		index_src++;
	}

	return true;
}
std::optional<size_t> var_manager::get_quantity_var() const
{
	return std::make_optional<size_t>(this->var_set.size());
}
var_manager& var_manager::operator=(const var_manager& other)
{
	if (this != &other)
		this->var_set = other.var_set;

	return *this;
}
