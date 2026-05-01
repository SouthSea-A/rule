#include "rule_command.h"


command::command() :
		cmd(command_type::invalid), op(std::in_place_index<static_cast<size_t>(oq::one)>)
	{

	}
command::command(command_type cmd_, const command_operand& op_) :
	cmd(cmd_), op(op_)
{

}
command::command(const command& command_) :
	cmd(command_.cmd), op(command_.op)
{

}
command::command(command&& command_) noexcept :
	cmd(command_.cmd), op(std::move(command_.op))
{
	command_.cmd = command_type::invalid;
}
command& command::operator=(const command& other)
{
	if (this != &other)
	{
		this->cmd = other.cmd;
		this->op = other.op;
	}
	return *this;
}
command::~command()
{
	this->cmd = command_type::invalid;
}



command_unit::command_unit() :
	content(), last(COMMAND_INVALID_INDEX), next(COMMAND_INVALID_INDEX)
{

}
command_unit::command_unit(const command& content_, command_index last_, command_index next_) :
	content(content_), last(last_), next(next_)
{

}
command_unit::command_unit(const command_unit& command_unit_) :
	content(command_unit_.content), last(command_unit_.last), next(command_unit_.next)
{

}
command_unit::command_unit(command_unit&& command_unit_) noexcept :
	content(std::move(command_unit_.content)), last(command_unit_.last), next(command_unit_.next)
{
	command_unit_.last = COMMAND_INVALID_INDEX;
	command_unit_.next = COMMAND_INVALID_INDEX;
}
command_unit& command_unit::operator=(const command_unit& other)
{
	if (this != &other)
	{
		this->content = other.content;
		this->last = other.last;
		this->next = other.next;
	}
	return *this;
}
command_unit::~command_unit()
{
	this->last = COMMAND_INVALID_INDEX;
	this->next = COMMAND_INVALID_INDEX;
}






bool command_manager::test_validity_command(const command& command_new) const
{
	return command_new.cmd != command_type::invalid;
}
bool command_manager::test_validity_command_index(command_index index, bool is_test_del) const
{
	if (index == COMMAND_INVALID_INDEX)
		return false;

	if (static_cast<size_t>(index) >= this->command_set.size())
		return false;

	if (is_test_del == false)
		return true;

	const command_unit* target = &this->command_set[static_cast<size_t>(index)];

	return target->last != COMMAND_INVALID_INDEX || target->next != COMMAND_INVALID_INDEX || target->content.cmd != command_type::invalid;
}
std::optional<command_unit*> command_manager::get_command_unit_ptr(command_index index, bool is_save_del)
{
	if (test_validity_command_index(index, !is_save_del) == false)
		return std::nullopt;

	return std::make_optional<command_unit*>(&this->command_set[static_cast<size_t>(index)]);
}
std::optional<const command_unit*> command_manager::get_command_unit_ptr_const(command_index index) const
{
	if (test_validity_command_index(index, true) == false)
		return std::nullopt;

	return std::make_optional<const command_unit*>(&this->command_set[static_cast<size_t>(index)]);
}
std::optional<command_index> command_manager::get_free()
{
	if (this->command_set_free.empty() == false)
	{
		command_index pos_free = this->command_set_free.back();

		this->command_set_free.pop_back();
		return std::make_optional<command_index>(pos_free);
	}

	return std::nullopt;
}
bool command_manager::add_free(command_index index)
{
	std::optional<command_unit*> command_unit_ptr_result = get_command_unit_ptr(index);

	if (command_unit_ptr_result.has_value() == false)
		return false;

	command_unit* command_unit_ptr = command_unit_ptr_result.value();

	command_unit_ptr_result = get_command_unit_ptr(command_unit_ptr->last);

	if (command_unit_ptr_result.has_value() == true)
		command_unit_ptr_result.value()->next = command_unit_ptr->next;

	command_unit_ptr_result = get_command_unit_ptr(command_unit_ptr->next);

	if (command_unit_ptr_result.has_value() == true)
		command_unit_ptr_result.value()->last = command_unit_ptr->last;

	if (this->begin == index)
		this->begin = command_unit_ptr->next;

	if (this->end == index)
		this->end = command_unit_ptr->last;

	command_unit_ptr->~command_unit();
	this->command_set_free.emplace_back(index);
	return true;
}
command_manager::command_manager() :
	begin(COMMAND_INVALID_INDEX), end(COMMAND_INVALID_INDEX), command_set(), command_set_free()
{

}
command_manager::~command_manager()
{

}
std::optional<command_index> command_manager::get_begin() const
{
	return test_validity_command_index(this->begin, true) == false ? std::nullopt : std::make_optional<command_index>(this->begin);
}
std::optional<command_index> command_manager::get_end() const
{
	return test_validity_command_index(this->end, true) == false ? std::nullopt : std::make_optional<command_index>(this->end);
}
bool command_manager::set_begin(command_index index)
{
	if (test_validity_command_index(index, true) == false)
		return false;

	this->begin = index;
	return true;
}
bool command_manager::set_end(command_index index)
{
	if (test_validity_command_index(index, true) == false)
		return false;

	this->end = index;
	return true;
}
std::optional<command_index> command_manager::add_command(const command& command_new)
{
	if (test_validity_command(command_new) == false)
		return std::nullopt;

	std::optional<command_index> pos_free = get_free();
	command_index index_new = pos_free.has_value() == false ? static_cast<command_index>(this->command_set.size()) : pos_free.value();

	std::optional<command_unit*> command_unit_ptr_result = get_command_unit_ptr(this->end, true);

	if (command_unit_ptr_result.has_value() == true)
		command_unit_ptr_result.value()->next = index_new;

	if (test_validity_command_index(this->begin, true) == false)
		this->begin = index_new;

	if (pos_free.has_value() == false) this->command_set.emplace_back(command_unit(command_new, this->end, COMMAND_INVALID_INDEX));
	else this->command_set[static_cast<size_t>(index_new)] = command_unit(command_new, this->end, COMMAND_INVALID_INDEX);
	
	this->end = index_new;
	return std::make_optional<command_index>(index_new);
}
std::optional<command_index> command_manager::insert_command(command_index index_insert, const command& command_new)
{
	std::optional<command_unit*> command_unit_ptr_result = get_command_unit_ptr(index_insert, false);

	if (command_unit_ptr_result.has_value() == false)
		return std::nullopt;

	command_unit* command_unit_ptr = command_unit_ptr_result.value();

	std::optional<command_index> pos_free = get_free();
	command_index index_new = pos_free.has_value() == false ? static_cast<command_index>(this->command_set.size()) : pos_free.value();

	command_unit_ptr_result = get_command_unit_ptr(command_unit_ptr->last);

	if (command_unit_ptr_result.has_value() == true)
		command_unit_ptr_result.value()->next = index_new;

	command_index last_old = command_unit_ptr->last;
	command_unit_ptr->last = index_new;

	if (pos_free.has_value() == false) this->command_set.emplace_back(command_unit(command_new, last_old, index_insert));
	else this->command_set[static_cast<size_t>(index_new)] = command_unit(command_new, last_old, index_insert);
	
	if (this->begin == index_insert)
		this->begin = index_new;

	return std::make_optional<command_index>(index_new);
}
bool command_manager::set_command(command_index index, const command& command_new)
{
	if (test_validity_command_index(index, true) == false)
		return false;

	std::optional<command_unit*> target = get_command_unit_ptr(index);

	if (target.has_value() == false)
		return false;

	target.value()->content = command_new;
	return true;
}
std::optional<command> command_manager::get_command(command_index index) const
{
	std::optional<const command_unit*> command_unit_ptr_const_result = get_command_unit_ptr_const(index);

	if (command_unit_ptr_const_result.has_value() == false)
		return std::nullopt;

	return std::make_optional<command>(command_unit_ptr_const_result.value()->content);
};
std::optional<command> command_manager::get_command_next(command_index index) const
{
	std::optional<const command_unit*> command_unit_ptr_const_result = get_command_unit_ptr_const(index);

	if (command_unit_ptr_const_result.has_value() == false)
		return std::nullopt;

	command_unit_ptr_const_result = get_command_unit_ptr_const(command_unit_ptr_const_result.value()->next);


	if (command_unit_ptr_const_result.has_value() == false)
		return std::nullopt;

	return std::make_optional<command>(command_unit_ptr_const_result.value()->content);
}
std::optional<command> command_manager::get_command_last(command_index index) const
{
	std::optional<const command_unit*> command_unit_ptr_const_result = get_command_unit_ptr_const(index);

	if (command_unit_ptr_const_result.has_value() == false)
		return std::nullopt;

	command_unit_ptr_const_result = get_command_unit_ptr_const(command_unit_ptr_const_result.value()->last);


	if (command_unit_ptr_const_result.has_value() == false)
		return std::nullopt;

	return std::make_optional<command>(command_unit_ptr_const_result.value()->content);
}
std::optional<command*> command_manager::get_command_ptr(command_index index)
{
	std::optional<command_unit*> command_unit_ptr_const_result = get_command_unit_ptr(index);

	if (command_unit_ptr_const_result.has_value() == false)
		return std::nullopt;

	return std::make_optional<command*>(&command_unit_ptr_const_result.value()->content);
}
std::optional<command*> command_manager::get_command_ptr_next(command_index index)
{
	std::optional<command_unit*> command_unit_ptr_const_result = get_command_unit_ptr(index);

	if (command_unit_ptr_const_result.has_value() == false)
		return std::nullopt;

	command_unit_ptr_const_result = get_command_unit_ptr(command_unit_ptr_const_result.value()->next);

	if (command_unit_ptr_const_result.has_value() == false)
		return std::nullopt;

	return std::make_optional<command*>(&command_unit_ptr_const_result.value()->content);
}
std::optional<command*> command_manager::get_command_ptr_last(command_index index)
{
	std::optional<command_unit*> command_unit_ptr_const_result = get_command_unit_ptr(index);

	if (command_unit_ptr_const_result.has_value() == false)
		return std::nullopt;

	command_unit_ptr_const_result = get_command_unit_ptr(command_unit_ptr_const_result.value()->last);

	if (command_unit_ptr_const_result.has_value() == false)
		return std::nullopt;

	return std::make_optional<command*>(&command_unit_ptr_const_result.value()->content);
}
std::optional<command_index> command_manager::get_command_index_next(command_index index) const
{
	std::optional<const command_unit*> command_unit_ptr_const_result = get_command_unit_ptr_const(index);

	if (command_unit_ptr_const_result.has_value() == false)
		return std::nullopt;

	if (command_unit_ptr_const_result.value()->next == COMMAND_INVALID_INDEX)
		return std::nullopt;

	return std::make_optional<command_index>(command_unit_ptr_const_result.value()->next);
}
std::optional<command_index> command_manager::get_command_index_last(command_index index) const
{
	std::optional<const command_unit*> command_unit_ptr_const_result = get_command_unit_ptr_const(index);

	if (command_unit_ptr_const_result.has_value() == false)
		return std::nullopt;

	if (command_unit_ptr_const_result.value()->last == COMMAND_INVALID_INDEX)
		return std::nullopt;

	return std::make_optional<command_index>(command_unit_ptr_const_result.value()->last);
}
bool command_manager::del_command(command_index index)
{
	return add_free(index);
}
command_manager& command_manager::operator=(const command_manager& other)
{
	if (this != &other)
	{
		this->begin = other.begin;
		this->end = other.end;
		this->command_set = other.command_set;
		this->command_set_free = other.command_set_free;
	}

	return *this;
}
