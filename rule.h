#pragma once
#include <vector>

using data_type_base_unsigned = uint64_t;
using data_type_base_signed = int64_t;

using command_index = data_type_base_unsigned;

constexpr command_index COMMAND_INVALID_INDEX = 0xFFFFFFFFFFFFFFFF;

enum class command_type : data_type_base_unsigned
{
	invalid,

	write,
	copy,
	compare,
	jump,
	jump_if_equal,
	jump_if_greater,
	jump_if_lower,
	jump_if_greater_or_equal,
	jump_if_lower_or_equal,
	add,
	subtract,
	multiply,
	divide,
	call,
	end,

	write_command_write,
	write_command_copy,
	write_command_compare,
	write_command_jump,
	write_command_jump_if_equal,
	write_command_jump_if_greater,
	write_command_jump_if_lower,
	write_command_jump_if_greater_or_equal,
	write_command_jump_if_lower_or_equal,
	write_command_add,
	write_command_subtract,
	write_command_multiply,
	write_command_divide,
	write_command_call,
	write_command_end,
};

enum class var_type :data_type_base_unsigned
{
	invalid,

	param,
	common,
	param_proxy,
	common_proxy,
};

using var_index = data_type_base_unsigned;
using var_single = data_type_base_unsigned;
using var_ref = data_type_base_unsigned;

constexpr var_index VAR_INVALID_INDEX = 0xFFFFFFFFFFFFFFFF;
constexpr var_single VAR_CONTENT_DEFAULT = 0x0000000000000000;

struct var
{
	var_type type;
	var_index index;
	var() :
		type(), index()
	{

	}
	var(var_type type, var_index index) :
		type(type), index(index)
	{

	}
	bool operator==(const var& var_) const
	{
		return this->type == var_.type && this->index == var_.index;
	}
	bool operator!=(const var& var_) const
	{
		return !(*this == var_);
	}
};

using rule_index = data_type_base_unsigned;

constexpr rule_index RULE_INVALID_INDEX = 0xFFFFFFFFFFFFFFFF;

struct rule_var_info;

struct command_param_write
{
	var target;
	var_single content;
	command_param_write() :
		target(), content()
	{

	}
	command_param_write(const var& target_, var_single content_) :
		target(target_), content(content_)
	{

	}
};

struct command_param_copy
{
	var target;
	var src;
	command_param_copy() :
		target(), src()
	{

	}
	command_param_copy(const var& target_, const var& src_) :
		target(target_), src(src_)
	{

	}
};

struct command_param_compare
{
	var one;
	var sec;
	var save;
	command_param_compare() :
		one(), sec(), save()
	{

	}
	command_param_compare(const var& one_, const var& sec_, const var& save_) :
		one(one_), sec(sec_), save(save_)
	{

	}
};

struct command_param_jump
{
	command_index target;
	command_param_jump() :
		target()
	{

	}
	command_param_jump(command_index target_) :
		target(target_)
	{

	}
};

struct command_param_jump_if
{
	var result;
	command_index target;
	command_param_jump_if() :
		result(), target()
	{

	}
	command_param_jump_if(const var& result_, command_index target_) :
		result(result_), target(target_)
	{

	}
};

struct command_param_calculate
{
	var left;
	var right;
	var save;
	command_param_calculate():
		left(), right(), save()
	{

	}
	command_param_calculate(const var& left_, const var& rigt_, const var& save_) :
		left(left_), right(rigt_), save(save_)
	{

	}
};

struct command_param_call
{
	rule_index target;
	std::vector<rule_var_info> param;
	var save;
	command_param_call() :
		target(), param(), save()
	{

	}
	command_param_call(const command_param_call& command_param_call_) :
		target(command_param_call_.target), param(command_param_call_.param), save(command_param_call_.save)
	{

	}
	command_param_call(rule_index target_,const std::vector<rule_var_info>& param_, const var& save_) :
		target(target_), param(param_), save(save_)
	{

	}
};

struct command_param_end
{
	var out;
	command_param_end() :
		out()
	{

	}
	command_param_end(const var& out_) :
		out(out_)
	{

	}
};

struct command_param_write_command
{
	rule_index index;
	command_index target;
	command_param_write_command() :
		index(), target()
	{

	}
	command_param_write_command(rule_index index_, command_index target_) :
		index(index_), target(target_)
	{

	}
};

struct command_param_write_command_write
{
	command_param_write_command target_write;
	command_param_write target_command;
	command_param_write_command_write() :
		target_write(), target_command()
	{

	}
	command_param_write_command_write(const command_param_write_command& target_write_,const command_param_write& target_command_) :
		target_write(target_write_), target_command(target_command_)
	{

	}
};

struct command_param_write_command_copy
{
	command_param_write_command target_write;
	command_param_copy target_command;
	command_param_write_command_copy() :
		target_write(), target_command()
	{

	}
	command_param_write_command_copy(const command_param_write_command& target_write_, const command_param_copy& target_command_) :
		target_write(target_write_), target_command(target_command_)
	{

	}
};

struct command_param_write_command_compare
{
	command_param_write_command target_write;
	command_param_compare target_command;
	command_param_write_command_compare() :
		target_write(), target_command()
	{

	}
	command_param_write_command_compare(const command_param_write_command& target_write_, const command_param_compare& target_command_) :
		target_write(target_write_), target_command(target_command_)
	{

	}
};

struct command_param_write_command_jump
{
	command_param_write_command target_write;
	command_param_jump target_command;
	command_param_write_command_jump() :
		target_write(), target_command()
	{

	}
	command_param_write_command_jump(const command_param_write_command& target_write_, const command_param_jump& target_command_) :
		target_write(target_write_), target_command(target_command_)
	{

	}
};

struct command_param_write_command_jump_if
{
	command_param_write_command target_write;
	command_param_jump_if target_command;
	command_param_write_command_jump_if() :
		target_write(), target_command()
	{

	}
	command_param_write_command_jump_if(const command_param_write_command& target_write_, const command_param_jump_if& target_command_) :
		target_write(target_write_), target_command(target_command_)
	{

	}
};

struct command_param_write_command_calculate
{
	command_param_write_command target_write;
	command_param_calculate target_command;
	command_param_write_command_calculate() :
		target_write(), target_command()
	{

	}
	command_param_write_command_calculate(const command_param_write_command& target_write_, const command_param_calculate& target_command_) :
		target_write(target_write_), target_command(target_command_)
	{

	}
};

struct command_param_write_command_call
{
	command_param_write_command target_write;
	command_param_call target_command;
	command_param_write_command_call() :
		target_write(), target_command()
	{

	}
	command_param_write_command_call(const command_param_write_command_call& command_param_write_command_call_) :
		target_write(command_param_write_command_call_.target_write), target_command(command_param_write_command_call_.target_command)
	{

	}
	command_param_write_command_call(const command_param_write_command& target_write_, const command_param_call& target_command_) :
		target_write(target_write_), target_command(target_command_)
	{

	}
};

struct command_param_write_command_end
{
	command_param_write_command target_write;
	command_param_end target_command;
	command_param_write_command_end() :
		target_write(), target_command()
	{

	}
	command_param_write_command_end(const command_param_write_command& target_write_,const command_param_end& target_command_) :
		target_write(target_write_), target_command(target_command_)
	{

	}
};

struct command_content
{
	command_type type;
	union command_param
	{
		command_param_write write;
		command_param_copy copy;
		command_param_compare compare;
		command_param_jump jump;
		command_param_jump_if jump_if;
		command_param_calculate calculate;
		command_param_call call;
		command_param_end end;
		command_param_write_command_write write_command_write;
		command_param_write_command_copy write_command_copy;
		command_param_write_command_compare write_command_compare;
		command_param_write_command_jump write_command_jump;
		command_param_write_command_jump_if write_command_jump_if;
		command_param_write_command_calculate write_command_calculate;
		command_param_write_command_call write_command_call;
		command_param_write_command_end write_command_end;
		command_param() :
			write()
		{

		}
		~command_param() noexcept
		{

		}
	} param;

	command_content() :
		type(command_type::write)
	{
		param.write = command_param_write();
	}
	command_content(const command_content& command_content_) :
		type(command_content_.type)
	{
		switch (command_content_.type)
		{
		case command_type::write: { param.write = command_content_.param.write; break; }
		case command_type::copy: { param.copy = command_content_.param.copy; break; }
		case command_type::compare: { param.compare = command_content_.param.compare; break; }
		case command_type::jump: { param.jump = command_content_.param.jump; break; }
		case command_type::jump_if_equal: { param.jump_if = command_content_.param.jump_if; break; }
		case command_type::jump_if_greater: { param.jump_if = command_content_.param.jump_if; break; }
		case command_type::jump_if_lower: { param.jump_if = command_content_.param.jump_if; break; }
		case command_type::jump_if_greater_or_equal: { param.jump_if = command_content_.param.jump_if; break; }
		case command_type::jump_if_lower_or_equal: { param.jump_if = command_content_.param.jump_if; break; }
		case command_type::add: { param.calculate = command_content_.param.calculate; break; }
		case command_type::subtract: { param.calculate = command_content_.param.calculate; break; }
		case command_type::multiply: { param.calculate = command_content_.param.calculate; break; }
		case command_type::divide: { param.calculate = command_content_.param.calculate; break; }
		case command_type::call: { new (&param.call) command_param_call(command_content_.param.call); break; }
		case command_type::end: { param.end = command_content_.param.end; break; }
		case command_type::write_command_write: { param.write_command_write = command_content_.param.write_command_write; break; }
		case command_type::write_command_copy: { param.write_command_copy = command_content_.param.write_command_copy; break; }
		case command_type::write_command_compare: { param.write_command_compare = command_content_.param.write_command_compare; break; }
		case command_type::write_command_jump: { param.write_command_jump = command_content_.param.write_command_jump; break; }
		case command_type::write_command_jump_if_equal: { param.write_command_jump_if = command_content_.param.write_command_jump_if; break; }
		case command_type::write_command_jump_if_greater: { param.write_command_jump_if = command_content_.param.write_command_jump_if; break; }
		case command_type::write_command_jump_if_lower: { param.write_command_jump_if = command_content_.param.write_command_jump_if; break; }
		case command_type::write_command_jump_if_greater_or_equal: { param.write_command_jump_if = command_content_.param.write_command_jump_if; break; }
		case command_type::write_command_jump_if_lower_or_equal: { param.write_command_jump_if = command_content_.param.write_command_jump_if; break; }
		case command_type::write_command_add: { param.write_command_calculate = command_content_.param.write_command_calculate; break; }
		case command_type::write_command_subtract: { param.write_command_calculate = command_content_.param.write_command_calculate; break; }
		case command_type::write_command_multiply: { param.write_command_calculate = command_content_.param.write_command_calculate; break; }
		case command_type::write_command_divide: { param.write_command_calculate = command_content_.param.write_command_calculate; break; }
		case command_type::write_command_call: { new (&param.write_command_call) command_param_write_command_call(command_content_.param.write_command_call); break; }
		case command_type::write_command_end: { param.write_command_end = command_content_.param.write_command_end; break; }
		default: break;
		}

	}
	command_content(const command_param_write& write_) :
		type(command_type::write)
	{
		param.write = write_;
	}
	command_content(const command_param_copy& copy_) :
		type(command_type::copy)
	{
		param.copy = copy_;
	}
	command_content(const command_param_compare& compare_) :
		type(command_type::compare)
	{
		param.compare = compare_;
	}
	command_content(const command_param_jump& jump_) :
		type(command_type::jump)
	{
		param.jump = jump_;
	}
	command_content(command_type type_, const command_param_jump_if& jump_if_) :
		type(type_)
	{
		param.jump_if = jump_if_;
	}
	command_content(command_type type_, const command_param_calculate& calculate_) :
		type(type_)
	{
		param.calculate = calculate_;
	}
	command_content(const command_param_call& call_) :
		type(command_type::call)
	{
		new (&param.call) command_param_call(call_);
	}
	command_content(const command_param_end& end_) :
		type(command_type::end)
	{
		param.end = end_;
	}
	command_content(const command_param_write_command_write& write_command_write_) :
		type(command_type::write_command_write)
	{
		param.write_command_write = write_command_write_;
	}
	command_content(const command_param_write_command_copy& write_command_copy_) :
		type(command_type::write_command_copy)
	{
		param.write_command_copy = write_command_copy_;
	}
	command_content(const command_param_write_command_compare& write_command_compare_) :
		type(command_type::write_command_compare)
	{
		param.write_command_compare = write_command_compare_;
	}
	command_content(const command_param_write_command_jump& write_command_jump_) :
		type(command_type::write_command_jump)
	{
		param.write_command_jump = write_command_jump_;
	}
	command_content(command_type type_, const command_param_write_command_jump_if& write_command_jump_if_) :
		type(type_)
	{
		param.write_command_jump_if = write_command_jump_if_;
	}
	command_content(command_type type_, const command_param_write_command_calculate& write_command_calculate_) :
		type(type_)
	{
		param.write_command_calculate = write_command_calculate_;
	}
	command_content(const command_param_write_command_call& write_command_call_) :
		type(command_type::call)
	{
		new (&param.write_command_call) command_param_write_command_call(write_command_call_);
	}
	command_content(const command_param_write_command_end& write_command_end_) :
		type(command_type::end)
	{
		param.write_command_end = write_command_end_;
	}
	~command_content() noexcept
	{
		switch (type)
		{
		case command_type::call: param.call.param.~vector(); break;
		case command_type::write_command_call: param.write_command_call.target_command.param.~vector(); break;
		default: break;
		}

	}
};

struct command
{
	command_content cmd;
	command_index next;
	command() :
		cmd(), next()
	{

	}
	command(const command_content& cmd_, command_index next_) :
		cmd(cmd_), next(next_)
	{

	}
	command(const command& command_) :
		cmd(command_.cmd), next(command_.next)
	{

	}
	~command() noexcept
	{

	}
};

enum class compare_result : data_type_base_unsigned
{
	equal,
	greater,
	lower
};

struct rule_var
{
	var_index index;
	var_single content;
	var_ref ref;
	rule_var() :
		index(), content(), ref()
	{

	}
	rule_var(var_index index_, var_single content_, var_ref ref_) :
		index(index_), content(content_), ref(ref_)
	{

	}
	rule_var(const rule_var& rule_var_) :
		index(rule_var_.index), content(rule_var_.content), ref(rule_var_.ref)
	{

	}
};

struct rule_var_info
{
	var_index index;
	bool is_var;

	union rule_var_info_param
	{
		var_single content;
		var varible;
		rule_var_info_param() :
			content(VAR_CONTENT_DEFAULT)
		{

		}
		rule_var_info_param(var_single content_) :
			content(content_)
		{

		}
		rule_var_info_param(const var& varible_) :
			varible(varible_)
		{

		}
		~rule_var_info_param()
		{

		}
	} pram;

	rule_var_info() :
		index(VAR_INVALID_INDEX), is_var(false), pram()
	{

	}
	rule_var_info(var_index index_, var_single var_content) :
		index(index_), is_var(false), pram(var_content)
	{

	}
	rule_var_info(var_index index_,var varible) :
		index(index_), is_var(true), pram(varible)
	{

	}
	~rule_var_info()
	{

	}
};


using rule_var_pos = data_type_base_unsigned;
constexpr rule_var_pos RULE_VAR_INVALID_POS = 0xFFFFFFFFFFFFFFFF;

enum class interrupt_type : data_type_base_unsigned
{
	invalid,
	end,
	call,
};

struct rule_run_result
{
	interrupt_type type;
	command_index next;

	union interrupt_param
	{
		const command_param_call* call;
		var_single end;
		interrupt_param() :
			end(VAR_CONTENT_DEFAULT)
		{

		}
		~interrupt_param() noexcept
		{

		}
	} param;

	rule_run_result() :
		type(interrupt_type::invalid), next(COMMAND_INVALID_INDEX), param()
	{

	}
	rule_run_result(command_index next_, const command_param_call* call_) :
		type(interrupt_type::call), next(next_)
	{
		param.call = call_;
	}
	rule_run_result(var_single end_) :
		type(interrupt_type::end), next(COMMAND_INVALID_INDEX)
	{
		param.end = end_;
	}
	rule_run_result(const rule_run_result& rule_run_result_) :
		type(rule_run_result_.type), next(rule_run_result_.next), param(rule_run_result_.param)
	{

	}
	~rule_run_result()
	{

	}
};

class rule_manager;

class rule
{
private:
	command_index begin;
	command_index end;
	std::vector<rule_var> param_set;
	std::vector<command> command_set;
	std::vector<command_index> command_index_free;
	std::vector<rule_var> var_set;
private:
	bool test_validity_command(const command_content& command) const
	{
		return command.type != command_type::invalid;
	}
	bool test_validity_command_index(command_index index, bool is_test_del) const
	{
		size_t pos = static_cast<size_t>(index);

		if (index == COMMAND_INVALID_INDEX || pos >= this->command_set.size())
			return false;

		return this->command_set[pos].cmd.type == command_type::invalid && is_test_del == true ? false : true;
	}
	bool test_validity_var_index(const var& var_test, bool is_test_exist) const noexcept
	{
		if (var_test.type == var_type::invalid)
			return false;

		if (var_test.index == VAR_INVALID_INDEX)
			return false;

		if (is_test_exist == true)
		{
			bool is_param = var_test.type == var_type::param || var_test.type == var_type::param_proxy;

			std::vector<rule_var>::const_iterator pos_begin = is_param ? this->param_set.begin() : this->var_set.begin();
			std::vector<rule_var>::const_iterator pos_end = is_param ? this->param_set.end() : this->var_set.end();
			std::vector<rule_var>::const_iterator pos_target = std::lower_bound(
				pos_begin,
				pos_end,
				var_test.index,
				[](const rule_var& var_temp, var_index inde_target)->bool
				{
					return var_temp.index < inde_target;
				}
			);

			return pos_target->index == var_test.index ? true : false;
		}

		return true;
	}
	bool test_validity_rule_var_pos(var_type type, rule_var_pos pos) const
	{
		if (type == var_type::invalid)
			return false;

		return (pos != RULE_VAR_INVALID_POS) && (static_cast<size_t>(pos) < (type == var_type::param || type == var_type::param_proxy ? this->param_set.size() : this->var_set.size()));
	}
	bool test_validity_rule_index(rule_index index) const noexcept
	{
		return index != RULE_INVALID_INDEX;
	}
	command* get_command_by_command_index_ptr(command_index index, bool is_save_del)
	{
		return test_validity_command_index(index, !is_save_del) == false ? nullptr : &this->command_set[static_cast<size_t>(index)];
	}
	const command* get_const_command_by_command_index_ptr(command_index index, bool is_save_del) const
	{
		return test_validity_command_index(index, !is_save_del) == false ? nullptr : &this->command_set[static_cast<size_t>(index)];
	}
	bool get_rule_var_pos_by_var(const var& var_target, rule_var_pos& save) const
	{
		if (test_validity_var_index(var_target, false) == false)
			return RULE_INVALID_INDEX;

		bool is_param = var_target.type == var_type::param || var_target.type == var_type::param_proxy;

		std::vector<rule_var>::const_iterator pos_begin = is_param ? this->param_set.begin() : this->var_set.begin();
		std::vector<rule_var>::const_iterator pos_end = is_param ? this->param_set.end() : this->var_set.end();
		std::vector<rule_var>::const_iterator pos_target = std::lower_bound(
			pos_begin,
			pos_end,
			var_target.index,
			[](const rule_var& var_temp, var_index inde_target)->bool
			{
				return var_temp.index < inde_target;
			}
		);

		save = static_cast<rule_var_pos>(pos_target - pos_begin);

		if (pos_target == pos_end)
			return false;

		return pos_target->index == var_target.index ? true : false;
	}
	rule_var* get_rule_var_by_rule_var_pos_ptr(var_type type, rule_var_pos pos)
	{
		if (type == var_type::invalid)
			return nullptr;

		if (test_validity_rule_var_pos(type, pos) == false)
			return nullptr;

		size_t pos_seek = static_cast<size_t>(pos);

		return type == var_type::param || type == var_type::param_proxy ? &this->param_set[pos_seek] : &this->var_set[pos_seek];
	}
	const rule_var* get_const_rule_var_by_rule_var_pos_ptr(var_type type, rule_var_pos pos) const
	{
		if (type == var_type::invalid)
			return nullptr;

		if (test_validity_rule_var_pos(type, pos) == false)
			return nullptr;

		size_t pos_seek = static_cast<size_t>(pos);

		return type == var_type::param ? &this->param_set[pos_seek] : &this->var_set[pos_seek];
	}
	rule_var* get_rule_var_by_var_ptr(const var& var_target)
	{
		rule_var_pos pos_target = RULE_VAR_INVALID_POS;

		if (get_rule_var_pos_by_var(var_target, pos_target) == false)
			return nullptr;

		rule_var* ptr_target = get_rule_var_by_rule_var_pos_ptr(var_target.type, pos_target);

		if (var_target.type == var_type::common_proxy || var_target.type == var_type::param_proxy)
		{
			if (get_rule_var_pos_by_var(var(var_target.type, ptr_target->content), pos_target) == false)
				return nullptr;

			ptr_target = get_rule_var_by_rule_var_pos_ptr(var_target.type, pos_target);
		}

		return ptr_target;
	}
	const rule_var* get_const_rule_var_by_var_ptr(const var& var_target) const
	{
		rule_var_pos pos_target = RULE_VAR_INVALID_POS;

		if (get_rule_var_pos_by_var(var_target, pos_target) == false)
			return nullptr;

		const rule_var* ptr_target = get_const_rule_var_by_rule_var_pos_ptr(var_target.type, pos_target);

		if (var_target.type == var_type::common_proxy || var_target.type == var_type::param_proxy)
		{
			if (get_rule_var_pos_by_var(var(var_target.type, ptr_target->content), pos_target) == false)
				return nullptr;

			ptr_target = get_const_rule_var_by_rule_var_pos_ptr(var_target.type, pos_target);
		}

		return ptr_target;
	}
	command_index get_command_last(command_index index) const
	{
		if (test_validity_command_index(index, true) == false)
			return COMMAND_INVALID_INDEX;

		for (command_index index_current = 0; static_cast<size_t>(index_current) < this->command_set.size(); index_current++)
		{
			if (this->command_set[static_cast<size_t>(index_current)].next == index)
				return index_current;
		}

		return COMMAND_INVALID_INDEX;
	}
	command_index get_command_index_free()
	{
		if (this->command_index_free.empty() == false)
		{
			command_index free = this->command_index_free.back();
			this->command_index_free.pop_back();
			return free;
		}

		return COMMAND_INVALID_INDEX;
	}
	command_index create_command_new(const command& new_command)
	{
		if (test_validity_command(new_command.cmd) == false)
			return COMMAND_INVALID_INDEX;

		this->command_set.emplace_back(new_command);
		return static_cast<command_index>(this->command_set.size() - 1);
	}
	command_index create_command(const command& new_command)
	{
		if (test_validity_command(new_command.cmd) == false)
			return COMMAND_INVALID_INDEX;

		command_index free = get_command_index_free();
		command_index index_new = COMMAND_INVALID_INDEX;

		if (test_validity_command_index(free, false) == false)
		{
			index_new = create_command_new(new_command);

			if (test_validity_command_index(index_new, false) == false)
				return false;
		}
		else
		{
			new (get_command_by_command_index_ptr(free, true)) command(new_command);
			index_new = free;
		}

		return index_new;
	}
	bool del_command_by_command_index(command_index index)
	{
		command* target = get_command_by_command_index_ptr(index, false);

		if (target == nullptr)
			return false;

		target->~command();
		target->cmd.type = command_type::invalid;
		target->next = COMMAND_INVALID_INDEX;
		this->command_index_free.emplace_back(index);

		return true;
	}
	bool add_var(const var& variable, bool is_write, var_single content = VAR_CONTENT_DEFAULT)
	{
		if (test_validity_var_index(variable, false) == false)
			return false;

		rule_var_pos pos_target = RULE_VAR_INVALID_POS;
		
		if (get_rule_var_pos_by_var(variable, pos_target) == false)
		{
			size_t pos_seek = static_cast<size_t>(pos_target);
			var_single content_new = is_write == true ? content : VAR_CONTENT_DEFAULT;

			if (variable.type == var_type::param || variable.type == var_type::param_proxy) this->param_set.insert(this->param_set.begin() + pos_seek, rule_var(variable.index, content_new, 1));
			else this->var_set.insert(this->var_set.begin() + pos_seek, rule_var(variable.index, content_new, 1));
		
			return true;
		}
			
		rule_var* target = get_rule_var_by_rule_var_pos_ptr(variable.type, pos_target);

		if (is_write == true)
			target->content = content;
		
		target->ref++;
		return true;
	}
	bool del_var(const var& variable)
	{
		if (variable.type == var_type::invalid)
			return false;

		if (test_validity_var_index(variable, false) == false)
			return false;

		rule_var_pos pos_target = RULE_VAR_INVALID_POS;

		if (get_rule_var_pos_by_var(variable, pos_target) == false)
			return false;

		rule_var* var_target = get_rule_var_by_rule_var_pos_ptr(variable.type, pos_target);

		if (--var_target->ref == 0)
		{
			size_t pos_del = static_cast<size_t>(pos_target);

			if (variable.type == var_type::param || variable.type == var_type::param_proxy) this->param_set.erase(this->param_set.begin() + pos_del);
			else this->var_set.erase(this->var_set.begin() + pos_del);
		}
			
		return true;
	}
	void add_command_var_manager(const command_content& new_command_content)
	{
		switch (new_command_content.type)
		{
		case command_type::invalid:
		{
			return;
			break;
		}
		case command_type::write:
		{
			const command_param_write& cmd_write = new_command_content.param.write;

			add_var(cmd_write.target, false);

			break;
		}
		case command_type::copy:
		{
			const command_param_copy& cmd_copy = new_command_content.param.copy;

			add_var(cmd_copy.src, false);

			if (cmd_copy.target != cmd_copy.src)
				add_var(cmd_copy.target, false);

			break;
		}
		case command_type::compare:
		{
			const command_param_compare& cmd_compare = new_command_content.param.compare;

			add_var(cmd_compare.one, false);

			if (cmd_compare.sec != cmd_compare.one)
				add_var(cmd_compare.sec, false);

			if (cmd_compare.save != cmd_compare.one && cmd_compare.save != cmd_compare.sec)
				add_var(cmd_compare.save, false);

			break;
		}
		case command_type::jump:
		{
			const command_param_jump& cmd_jump = new_command_content.param.jump;

			break;
		}
		case command_type::jump_if_equal:
		{
			const command_param_jump_if& cmd_jump_if_equal = new_command_content.param.jump_if;

			add_var(cmd_jump_if_equal.result, false);

			break;
		}
		case command_type::jump_if_greater:
		{
			const command_param_jump_if& cmd_jump_if_greater = new_command_content.param.jump_if;

			add_var(cmd_jump_if_greater.result, false);

			break;
		}
		case command_type::jump_if_lower:
		{
			const command_param_jump_if& cmd_jump_if_lower = new_command_content.param.jump_if;

			add_var(cmd_jump_if_lower.result, false);

			break;
		}
		case command_type::jump_if_greater_or_equal:
		{
			const command_param_jump_if& cmd_jump_if_greater_or_equal = new_command_content.param.jump_if;

			add_var(cmd_jump_if_greater_or_equal.result, false);

			break;
		}
		case command_type::jump_if_lower_or_equal:
		{
			const command_param_jump_if& cmd_jump_if_lower_or_equal = new_command_content.param.jump_if;

			add_var(cmd_jump_if_lower_or_equal.result, false);

			break;
		}
		case command_type::add:
		{
			const command_param_calculate& cmd_add = new_command_content.param.calculate;

			add_var(cmd_add.left, false);

			if (cmd_add.right != cmd_add.left)
				add_var(cmd_add.right, false);

			if (cmd_add.save != cmd_add.left && cmd_add.save != cmd_add.right)
				add_var(cmd_add.save, false);

			break;
		}
		case command_type::subtract:
		{
			const command_param_calculate& cmd_subtract = new_command_content.param.calculate;

			add_var(cmd_subtract.left, false);

			if (cmd_subtract.right != cmd_subtract.left)
				add_var(cmd_subtract.right, false);

			if (cmd_subtract.save != cmd_subtract.left && cmd_subtract.save != cmd_subtract.right)
				add_var(cmd_subtract.save, false);

			break;
		}
		case command_type::multiply:
		{
			const command_param_calculate& cmd_multiply = new_command_content.param.calculate;

			add_var(cmd_multiply.left, false);

			if (cmd_multiply.right != cmd_multiply.left)
				add_var(cmd_multiply.right, false);

			if (cmd_multiply.save != cmd_multiply.left && cmd_multiply.save != cmd_multiply.right)
				add_var(cmd_multiply.save, false);

			break;
		}
		case command_type::divide:
		{
			const command_param_calculate& cmd_divide = new_command_content.param.calculate;

			add_var(cmd_divide.left, false);

			if (cmd_divide.right != cmd_divide.left)
				add_var(cmd_divide.right, false);

			if (cmd_divide.save != cmd_divide.left && cmd_divide.save != cmd_divide.right)
				add_var(cmd_divide.save, false);

			break;
		}
		case command_type::call:
		{
			const command_param_call& cmd_call = new_command_content.param.call;

			add_var(cmd_call.save, false);

			break;
		}
		case command_type::end:
		{
			const command_param_end& cmd_end = new_command_content.param.end;

			add_var(cmd_end.out, false);

			break;
		}
		case command_type::write_command_write:
		{
			const command_param_write_command_write& cmd_write_command_write = new_command_content.param.write_command_write;

			break;
		}
		case command_type::write_command_copy:
		{
			const command_param_write_command_copy& cmd_write_command_copy = new_command_content.param.write_command_copy;

			break;
		}
		case command_type::write_command_compare:
		{
			const command_param_write_command_compare& cmd_write_command_compare = new_command_content.param.write_command_compare;

			break;
		}
		case command_type::write_command_jump:
		{
			const command_param_write_command_jump& cmd_write_command_jump = new_command_content.param.write_command_jump;

			break;
		}
		case command_type::write_command_jump_if_equal:
		{
			const command_param_write_command_jump_if& cmd_write_command_jump_if_equal = new_command_content.param.write_command_jump_if;

			break;
		}
		case command_type::write_command_jump_if_greater:
		{
			const command_param_write_command_jump_if& cmd_write_command_jump_if_greater = new_command_content.param.write_command_jump_if;

			break;
		}
		case command_type::write_command_jump_if_lower:
		{
			const command_param_write_command_jump_if& cmd_write_command_jump_if_lower = new_command_content.param.write_command_jump_if;

			break;
		}
		case command_type::write_command_jump_if_greater_or_equal:
		{
			const command_param_write_command_jump_if& cmd_write_command_jump_if_greater_or_equal = new_command_content.param.write_command_jump_if;

			break;
		}
		case command_type::write_command_jump_if_lower_or_equal:
		{
			const command_param_write_command_jump_if& cmd_write_command_jump_if_lower_or_equal = new_command_content.param.write_command_jump_if;

			break;
		}
		case command_type::write_command_add:
		{
			const command_param_write_command_calculate& cmd_write_command_add = new_command_content.param.write_command_calculate;

			break;
		}
		case command_type::write_command_subtract:
		{
			const command_param_write_command_calculate& cmd_write_command_subtract = new_command_content.param.write_command_calculate;

			break;
		}
		case command_type::write_command_multiply:
		{
			const command_param_write_command_calculate& cmd_write_command_multiply = new_command_content.param.write_command_calculate;

			break;
		}
		case command_type::write_command_divide:
		{
			const command_param_write_command_calculate& cmd_write_command_divide = new_command_content.param.write_command_calculate;

			break;
		}
		case command_type::write_command_call:
		{
			const command_param_write_command_call& cmd_write_command_call = new_command_content.param.write_command_call;

			break;
		}
		case command_type::write_command_end:
		{
			const command_param_write_command_end& cmd_write_command_end = new_command_content.param.write_command_end;

			break;
		}
		default: break;
		}

		return;
	}
	void del_command_var_manager(const command_content& del_command_content)
	{
		switch (del_command_content.type)
		{
		case command_type::invalid:
		{
			return;
			break;
		}
		case command_type::write:
		{
			const command_param_write& cmd_write = del_command_content.param.write;

			del_var(cmd_write.target);

			break;
		}
		case command_type::copy:
		{
			const command_param_copy& cmd_copy = del_command_content.param.copy;

			del_var(cmd_copy.src);

			if (cmd_copy.target != cmd_copy.src)
				del_var(cmd_copy.target);

			break;
		}
		case command_type::compare:
		{
			const command_param_compare& cmd_compare = del_command_content.param.compare;

			del_var(cmd_compare.one);

			if (cmd_compare.sec != cmd_compare.one)
				del_var(cmd_compare.sec);

			if (cmd_compare.save != cmd_compare.one && cmd_compare.save != cmd_compare.sec)
				del_var(cmd_compare.save);

			break;
		}
		case command_type::jump:
		{
			const command_param_jump& cmd_jump = del_command_content.param.jump;

			break;
		}
		case command_type::jump_if_equal:
		{
			const command_param_jump_if& cmd_jump_if_equal = del_command_content.param.jump_if;

			del_var(cmd_jump_if_equal.result);

			break;
		}
		case command_type::jump_if_greater:
		{
			const command_param_jump_if& cmd_jump_if_greater = del_command_content.param.jump_if;

			del_var(cmd_jump_if_greater.result);

			break;
		}
		case command_type::jump_if_lower:
		{
			const command_param_jump_if& cmd_jump_if_lower = del_command_content.param.jump_if;

			del_var(cmd_jump_if_lower.result);

			break;
		}
		case command_type::jump_if_greater_or_equal:
		{
			const command_param_jump_if& cmd_jump_if_greater_or_equal = del_command_content.param.jump_if;

			del_var(cmd_jump_if_greater_or_equal.result);

			break;
		}
		case command_type::jump_if_lower_or_equal:
		{
			const command_param_jump_if& cmd_jump_if_lower_or_equal = del_command_content.param.jump_if;

			del_var(cmd_jump_if_lower_or_equal.result);

			break;
		}
		case command_type::add:
		{
			const command_param_calculate& cmd_add = del_command_content.param.calculate;

			del_var(cmd_add.left);

			if (cmd_add.right != cmd_add.left)
				del_var(cmd_add.right);

			if (cmd_add.save != cmd_add.left && cmd_add.save != cmd_add.right)
				del_var(cmd_add.save);

			break;
		}
		case command_type::subtract:
		{
			const command_param_calculate& cmd_subtract = del_command_content.param.calculate;

			del_var(cmd_subtract.left);

			if (cmd_subtract.right != cmd_subtract.left)
				del_var(cmd_subtract.right);

			if (cmd_subtract.save != cmd_subtract.left && cmd_subtract.save != cmd_subtract.right)
				del_var(cmd_subtract.save);

			break;
		}
		case command_type::multiply:
		{
			const command_param_calculate& cmd_multiply = del_command_content.param.calculate;

			del_var(cmd_multiply.left);

			if (cmd_multiply.right != cmd_multiply.left)
				del_var(cmd_multiply.right);

			if (cmd_multiply.save != cmd_multiply.left && cmd_multiply.save != cmd_multiply.right)
				del_var(cmd_multiply.save);

			break;
		}
		case command_type::divide:
		{
			const command_param_calculate& cmd_divide = del_command_content.param.calculate;

			del_var(cmd_divide.left);

			if (cmd_divide.right != cmd_divide.left)
				del_var(cmd_divide.right);

			if (cmd_divide.save != cmd_divide.left && cmd_divide.save != cmd_divide.right)
				del_var(cmd_divide.save);

			break;
		}
		case command_type::call:
		{
			const command_param_call& cmd_call = del_command_content.param.call;

			del_var(cmd_call.save);

			break;
		}
		case command_type::end:
		{
			const command_param_end& cmd_end = del_command_content.param.end;

			del_var(cmd_end.out);

			break;
		}
		case command_type::write_command_write:
		{
			const command_param_write_command_write& cmd_write_command_write = del_command_content.param.write_command_write;

			break;
		}
		case command_type::write_command_copy:
		{
			const command_param_write_command_copy& cmd_write_command_copy = del_command_content.param.write_command_copy;

			break;
		}
		case command_type::write_command_compare:
		{
			const command_param_write_command_compare& cmd_write_command_compare = del_command_content.param.write_command_compare;

			break;
		}
		case command_type::write_command_jump:
		{
			const command_param_write_command_jump& cmd_write_command_jump = del_command_content.param.write_command_jump;

			break;
		}
		case command_type::write_command_jump_if_equal:
		{
			const command_param_write_command_jump_if& cmd_write_command_jump_if_equal = del_command_content.param.write_command_jump_if;

			break;
		}
		case command_type::write_command_jump_if_greater:
		{
			const command_param_write_command_jump_if& cmd_write_command_jump_if_greater = del_command_content.param.write_command_jump_if;

			break;
		}
		case command_type::write_command_jump_if_lower:
		{
			const command_param_write_command_jump_if& cmd_write_command_jump_if_lower = del_command_content.param.write_command_jump_if;

			break;
		}
		case command_type::write_command_jump_if_greater_or_equal:
		{
			const command_param_write_command_jump_if& cmd_write_command_jump_if_greater_or_equal = del_command_content.param.write_command_jump_if;

			break;
		}
		case command_type::write_command_jump_if_lower_or_equal:
		{
			const command_param_write_command_jump_if& cmd_write_command_jump_if_lower_or_equal = del_command_content.param.write_command_jump_if;

			break;
		}
		case command_type::write_command_add:
		{
			const command_param_write_command_calculate& cmd_write_command_add = del_command_content.param.write_command_calculate;

			break;
		}
		case command_type::write_command_subtract:
		{
			const command_param_write_command_calculate& cmd_write_command_subtract = del_command_content.param.write_command_calculate;

			break;
		}
		case command_type::write_command_multiply:
		{
			const command_param_write_command_calculate& cmd_write_command_multiply = del_command_content.param.write_command_calculate;

			break;
		}
		case command_type::write_command_divide:
		{
			const command_param_write_command_calculate& cmd_write_command_divide = del_command_content.param.write_command_calculate;

			break;
		}
		case command_type::write_command_call:
		{
			const command_param_write_command_call& cmd_write_command_call = del_command_content.param.write_command_call;

			break;
		}
		case command_type::write_command_end:
		{
			const command_param_write_command_end& cmd_write_command_end = del_command_content.param.write_command_end;

			break;
		}
		default: break;
		}

		return;
	}
public:
	rule() :
		begin(COMMAND_INVALID_INDEX), end(COMMAND_INVALID_INDEX), param_set(), command_set(), command_index_free(), var_set()
	{

	}
	rule(const rule& rule_) :
		begin(rule_.begin), end(rule_.end), param_set(rule_.param_set), command_set(rule_.command_set), command_index_free(rule_.command_index_free), var_set(rule_.var_set)
	{

	}
	rule(rule&& rule_) noexcept :
		begin(rule_.begin), end(rule_.end), param_set(std::move(rule_.param_set)), command_set(std::move(rule_.command_set)), command_index_free(std::move(rule_.command_index_free)), var_set(std::move(rule_.var_set))
	{
		rule_.begin = COMMAND_INVALID_INDEX;
		rule_.end = COMMAND_INVALID_INDEX;
	}
	~rule() noexcept
	{

	}
public:
	size_t get_param_quantity() const
	{
		return this->param_set.size();
	}
	bool set_param(const std::vector<rule_var_info>& param)
	{
		size_t param_quan = get_param_quantity();

		if (param.size() != param_quan)
			return false;

		for (size_t pos = 0; pos < param_quan; pos++)
		{
			rule_var& param_target = this->param_set[pos];

			if (param[pos].is_var == true)
				return false;

			param_target.content = param[pos].pram.content;
		}

		return true;
	}
	bool set_begin(command_index index)
	{
		if (test_validity_command_index(index, true) == false)
			return false;

		this->begin = index;
		return true;
	}
	bool get_var(const var& target, var_single& save, bool is_get_proxy = false) const
	{
		rule_var_pos pos_target = RULE_VAR_INVALID_POS;

		if (get_rule_var_pos_by_var(target, pos_target) == false)
			return false;

		const rule_var* var_target = get_const_rule_var_by_rule_var_pos_ptr(target.type, pos_target);

		if ((target.type == var_type::common_proxy || target.type == var_type::param_proxy) && is_get_proxy == true)
		{
			if (get_rule_var_pos_by_var(var(target.type, var_target->content), pos_target) == false)
				return false;

			var_target = get_const_rule_var_by_rule_var_pos_ptr(target.type, pos_target);
		}

		save = var_target->content;
		return true;
	}
	bool set_var(const var& target, var_single content, bool is_write_proxy = false)
	{
		rule_var_pos pos_target = RULE_VAR_INVALID_POS;

		if (get_rule_var_pos_by_var(target, pos_target) == false)
			return false;

		rule_var* var_target = get_rule_var_by_rule_var_pos_ptr(target.type, pos_target);

		if ((target.type == var_type::common_proxy || target.type == var_type::param_proxy) && is_write_proxy == true)
		{
			if (get_rule_var_pos_by_var(var(target.type, var_target->content), pos_target) == false)
				return false;

			var_target = get_rule_var_by_rule_var_pos_ptr(target.type, pos_target);
		}
		
		var_target->content = content;
		return true;
	}
	command_index get_begin() const
	{
		return this->begin;
	}
	command_index add_command(const command_content& new_command_content)
	{
		command_index index_new = create_command(command(new_command_content, COMMAND_INVALID_INDEX));

		if (test_validity_command_index(index_new, true) == false)
			return COMMAND_INVALID_INDEX;

		if (this->command_set.size() - this->command_index_free.size() - 1 > 0)
			get_command_by_command_index_ptr(this->end, false)->next = index_new;

		if (test_validity_command_index(this->begin, true) == false)
			this->begin = index_new;

		this->end = index_new;

		add_command_var_manager(new_command_content);

		return index_new;
	}
	command_index add_command_write(const var& target, var_single content)
	{
		return add_command(command_content(command_param_write(target, content)));
	}
	command_index add_command_copy(const var& target, const var& src)
	{
		return add_command(command_content(command_param_copy(target, src)));
	}
	command_index add_command_compare(const var& one, const var& sec, const var& save)
	{
		return add_command(command_content(command_param_compare(one, sec, save)));
	}
	command_index add_command_jump(command_index target)
	{
		return add_command(command_content(command_param_jump(target)));
	}
	command_index add_command_jump_if_equal(const var& result, command_index target)
	{
		return add_command(command_content(command_type::jump_if_equal, command_param_jump_if(command_param_jump_if(result, target))));
	}
	command_index add_command_jump_if_greater(const var& result, command_index target)
	{
		return add_command(command_content(command_type::jump_if_greater, command_param_jump_if(command_param_jump_if(result, target))));
	}
	command_index add_command_jump_if_lower(const var& result, command_index target)
	{
		return add_command(command_content(command_type::jump_if_lower, command_param_jump_if(command_param_jump_if(result, target))));
	}
	command_index add_command_jump_if_greater_or_equal(const var& result, command_index target)
	{
		return add_command(command_content(command_type::jump_if_greater_or_equal, command_param_jump_if(command_param_jump_if(result, target))));
	}
	command_index add_command_jump_if_lower_or_equal(const var& result, command_index target)
	{
		return add_command(command_content(command_type::jump_if_lower_or_equal, command_param_jump_if(command_param_jump_if(result, target))));
	}
	command_index add_command_add(const var& left, const var& right, const var& save)
	{
		return add_command(command_content(command_type::add, command_param_calculate(left, right, save)));
	}
	command_index add_command_subtract(const var& left, const var& right, const var& save)
	{
		return add_command(command_content(command_type::subtract, command_param_calculate(left, right, save)));
	}
	command_index add_command_multiply(const var& left, const var& right, const var& save)
	{
		return add_command(command_content(command_type::multiply, command_param_calculate(left, right, save)));
	}
	command_index add_command_divide(const var& left, const var& right, const var& save)
	{
		return add_command(command_content(command_type::divide, command_param_calculate(left, right, save)));
	}
	command_index add_command_call(rule_index target, const std::vector<rule_var_info>& param, const var& save)
	{
		return add_command(command_content(command_param_call(target, param, save)));
	}
	command_index add_command_end(const var& out)
	{
		return add_command(command_content(command_param_end(out)));
	}
	command_index add_command_write_command_write(rule_index index, command_index target, const var& var_target, var_single content)
	{
		return add_command(command_content(command_param_write_command_write(command_param_write_command(index, target), command_param_write(var_target, content))));
	}
	command_index add_command_write_command_copy(rule_index index, command_index target, const var& var_target, const var& src)
	{
		return add_command(command_content(command_param_write_command_copy(command_param_write_command(index, target), command_param_copy(var_target, src))));
	}
	command_index add_command_write_command_compare(rule_index index, command_index target, const var& one, const var& sec, const var& save)
	{
		return add_command(command_content(command_param_write_command_compare(command_param_write_command(index, target), command_param_compare(one, sec, save))));
	}
	command_index add_command_write_command_jump(rule_index index, command_index target, command_index jump_target)
	{
		return add_command(command_content(command_param_write_command_jump(command_param_write_command(index, target), command_param_jump(jump_target))));
	}
	command_index add_command_write_command_jump_if_equal(rule_index index, command_index target, const var& result, command_index target_compare)
	{
		return add_command(command_content(command_type::write_command_jump_if_equal, command_param_write_command_jump_if(command_param_write_command(index, target), command_param_jump_if(result, target_compare))));
	}
	command_index add_command_write_command_jump_if_greater(rule_index index, command_index target, const var& result, command_index target_compare)
	{
		return add_command(command_content(command_type::write_command_jump_if_greater, command_param_write_command_jump_if(command_param_write_command(index, target), command_param_jump_if(result, target_compare))));
	}
	command_index add_command_write_command_jump_if_lower(rule_index index, command_index target, const var& result, command_index target_compare)
	{
		return add_command(command_content(command_type::write_command_jump_if_lower, command_param_write_command_jump_if(command_param_write_command(index, target), command_param_jump_if(result, target_compare))));
	}
	command_index add_command_write_command_jump_if_greater_or_equal(rule_index index, command_index target, const var& result, command_index target_compare)
	{
		return add_command(command_content(command_type::write_command_jump_if_greater_or_equal, command_param_write_command_jump_if(command_param_write_command(index, target), command_param_jump_if(result, target_compare))));
	}
	command_index add_command_write_command_jump_if_lower_or_equal(rule_index index, command_index target, const var& result, command_index target_compare)
	{
		return add_command(command_content(command_type::write_command_jump_if_lower_or_equal, command_param_write_command_jump_if(command_param_write_command(index, target), command_param_jump_if(result, target_compare))));
	}
	command_index add_command_write_command_add(rule_index index, command_index target, const var& left, const var& right, const var& save)
	{
		return add_command(command_content(command_type::write_command_add, command_param_write_command_calculate(command_param_write_command(index, target), command_param_calculate(left, right, save))));
	}
	command_index add_command_write_command_subtract(rule_index index, command_index target, const var& left, const var& right, const var& save)
	{
		return add_command(command_content(command_type::write_command_subtract, command_param_write_command_calculate(command_param_write_command(index, target), command_param_calculate(left, right, save))));
	}
	command_index add_command_write_command_multiply(rule_index index, command_index target, const var& left, const var& right, const var& save)
	{
		return add_command(command_content(command_type::write_command_multiply, command_param_write_command_calculate(command_param_write_command(index, target), command_param_calculate(left, right, save))));
	}
	command_index add_command_write_command_divide(rule_index index, command_index target, const var& left, const var& right, const var& save)
	{
		return add_command(command_content(command_type::write_command_divide, command_param_write_command_calculate(command_param_write_command(index, target), command_param_calculate(left, right, save))));
	}
	command_index add_command_write_command_call(rule_index index, command_index target, rule_index target_call, const std::vector<rule_var_info>& param, const var& save)
	{
		return add_command(command_content(command_param_write_command_call(command_param_write_command(index, target), command_param_call(target_call, param, save))));
	}
	command_index add_command_write_command_end(rule_index index, command_index target, const var& out)
	{
		return add_command(command_content(command_param_write_command_end(command_param_write_command(index, target), command_param_end(out))));
	}
	bool del_command(command_index index)
	{
		if (test_validity_command_index(index, true) == false)
			return false;

		command_index target_last = get_command_last(index);
		bool is_valid = test_validity_command_index(target_last, true) == true;

		if (is_valid == false)
		{
			command* del_target = get_command_by_command_index_ptr(index, false);

			this->begin = del_target->next;

			del_command_var_manager(del_target->cmd);

			if (del_command_by_command_index(index) == false)
				return false;
		}
		else
		{
			command* del_target = get_command_by_command_index_ptr(index, false);
			command_index target_next = del_target->next;

			del_command_var_manager(del_target->cmd);

			if (del_command_by_command_index(index) == false)
				return false;

			get_command_by_command_index_ptr(target_last, false)->next = target_next;
		}

		if (this->end == index)
			this->end = is_valid == true ? target_last : COMMAND_INVALID_INDEX;

		return true;
	}
	bool set_command(command_index index, const command_content& new_command_content)
	{
		if (test_validity_command(new_command_content) == false)
			return false;

		command* target = get_command_by_command_index_ptr(index, false);

		if (target == nullptr)
			return false;

		command_index next_old = target->next;

		del_command_var_manager(target->cmd);
		target->~command();
		new (target) command(new_command_content, next_old);
		add_command_var_manager(new_command_content);

		return true;
	}
	command_index insert_command(command_index index_insert, const command_content& new_command_content)
	{
		if (test_validity_command_index(index_insert, true) == false)
			return COMMAND_INVALID_INDEX;

		command_index index_new = create_command(command(new_command_content, COMMAND_INVALID_INDEX));

		if (test_validity_command_index(index_new, true) == false)
			return COMMAND_INVALID_INDEX;

		command_index target_last = get_command_last(index_insert);

		get_command_by_command_index_ptr(index_new, true)->next = index_insert;

		if (test_validity_command_index(target_last, true) == false) this->begin = index_new;
		else get_command_by_command_index_ptr(target_last, false)->next = index_new;

		add_command_var_manager(new_command_content);

		return index_new;
	}
	bool get_command(command_index index, command_content& save) const
	{
		const command* target = get_const_command_by_command_index_ptr(index, false);

		if (target == nullptr)
			return false;

		new (&save) command_content(target->cmd);
		return true;
	}
	bool run(rule_manager& ownership, rule_run_result& save, command_index begin = COMMAND_INVALID_INDEX);
};

struct rule_manager_rule
{
	bool is_del;
	rule rule_single;
	rule_manager_rule() :
		is_del(false), rule_single()
	{

	}
	rule_manager_rule(bool is_del_, const rule& rule_single_) :
		is_del(is_del_), rule_single(rule_single_)
	{

	}
	rule_manager_rule(const rule_manager_rule& rule_manager_rule_) :
		is_del(rule_manager_rule_.is_del), rule_single(rule_manager_rule_.rule_single)
	{

	}
	rule_manager_rule(rule_manager_rule&& rule_manager_rule_) noexcept:
		is_del(rule_manager_rule_.is_del), rule_single(std::move(rule_manager_rule_.rule_single))
	{

	}
};

struct rule_manager_stack
{
	rule_index index;
	command_index next;
	var save;
	rule_manager_stack() :
		index(), next(), save()
	{

	}
	rule_manager_stack(rule_index index_, command_index next_, var save_) :
		index(index_), next(next_), save(save_)
	{

	}
	rule_manager_stack(const rule_manager_stack& rule_manager_stack_) :
		index(rule_manager_stack_.index), next(rule_manager_stack_.next), save(rule_manager_stack_.save)
	{

	}
};

class rule_manager
{
private:
	rule_index begin;
	std::vector<rule_manager_rule> rule_set;
	std::vector<rule_index> rule_index_free;
	std::vector<rule_manager_stack> stack;
private:
	bool test_validity_rule_index(rule_index index, bool is_test_del) const
	{
		size_t pos = static_cast<size_t>(index);

		if (index == RULE_INVALID_INDEX || pos >= this->rule_set.size())
			return false;

		return is_test_del == true ? this->rule_set[pos].is_del == false : true;
	}
	rule_index get_rule_index_free()
	{
		if (this->rule_index_free.empty() == false)
		{
			rule_index free = this->rule_index_free.back();
			this->rule_index_free.pop_back();

			return free;
		}

		return RULE_INVALID_INDEX;
	}
	rule_index create_rule()
	{
		rule_index free = get_rule_index_free();
		rule_index index_new = RULE_INVALID_INDEX;

		if (test_validity_rule_index(free, false) == false)
		{
			this->rule_set.emplace_back();
			index_new = static_cast<rule_index>(this->rule_set.size() - 1);
		}
		else
		{
			rule_manager_rule* rule_new = &this->rule_set[static_cast<size_t>(free)];

			new (rule_new) rule_manager_rule();
			index_new = free;
		}

		return index_new;
	}
	rule_manager_rule* get_rule_manager_rule_ptr_by_rule_index(rule_index index)
	{
		if (test_validity_rule_index(index, true) == false)
			return nullptr;

		return &this->rule_set[static_cast<size_t>(index)];
	}
	const rule_manager_rule* get_const_rule_manager_rule_ptr_by_rule_index(rule_index index) const
	{
		if (test_validity_rule_index(index, true) == false)
			return nullptr;

		return &this->rule_set[static_cast<size_t>(index)];
	}
	bool del_rule_by_rule_index(rule_index index)
	{
		rule_manager_rule* target = get_rule_manager_rule_ptr_by_rule_index(index);

		if (target == nullptr)
			return false;

		target->rule_single.~rule();
		target->is_del = true;

		this->rule_index_free.emplace_back(index);

		return true;
	}
public:
	rule_manager() :
		begin(RULE_INVALID_INDEX), rule_set(), rule_index_free(), stack()
	{

	}
	rule_manager(const rule_manager& rule_manager_) :
		begin(rule_manager_.begin), rule_set(rule_manager_.rule_set), rule_index_free(rule_manager_.rule_index_free), stack(rule_manager_.stack)
	{

	}
	rule_manager(rule_manager&& rule_manager_) noexcept:
		begin(rule_manager_.begin), rule_set(std::move(rule_manager_.rule_set)), rule_index_free(std::move(rule_manager_.rule_index_free)),stack(std::move(rule_manager_.stack))
	{
		rule_manager_.begin = RULE_INVALID_INDEX;
	}
	~rule_manager() noexcept
	{

	}
public:
	bool set_begin(rule_index index)
	{
		if (test_validity_rule_index(index, true) == false)
			return false;

		this->begin = index;
		return true;
	}
	rule_index add_rule()
	{
		rule_index rule_new = create_rule();

		if (test_validity_rule_index(rule_new, true) == true)
		{
			if (test_validity_rule_index(this->begin, true) == false)
				this->begin = rule_new;
		}

		return rule_new;
	}
	bool del_rule(rule_index index)
	{
		if (index == this->begin)
			this->begin = RULE_INVALID_INDEX;

		return del_rule_by_rule_index(index);
	}
	rule* get_rule(rule_index index)
	{
		rule_manager_rule* target = get_rule_manager_rule_ptr_by_rule_index(index);

		if (target == nullptr)
			return nullptr;

		return &target->rule_single;
	}
	const rule* get_const_rule(rule_index index) const
	{
		const rule_manager_rule* target = get_const_rule_manager_rule_ptr_by_rule_index(index);

		if (target == nullptr)
			return nullptr;

		return &target->rule_single;
	}
	bool run(rule_index begin_index, const std::vector<rule_var_info>& param_begin, var_single& save)
	{
		rule_index rule_index_current = begin_index;
		rule_index rule_index_next = RULE_INVALID_INDEX;
		rule_manager_rule* rule_current = get_rule_manager_rule_ptr_by_rule_index(rule_index_current);

		if (rule_current == nullptr)
		{
			save = VAR_CONTENT_DEFAULT;
			return false;
		}

		std::vector<rule_var_info> param_current;

		param_current.clear();

		for (std::vector<rule_var_info>::const_iterator pos_param = param_begin.begin(); pos_param != param_begin.end(); ++pos_param)
		{
			if (pos_param->is_var == true)
				return false;

			param_current.emplace_back(rule_var_info(pos_param->index, pos_param->pram.content));
		}

		rule_run_result result_save;
		var_single result_last_call = VAR_CONTENT_DEFAULT;
		command_index begin_current = rule_current->rule_single.get_begin();
		bool is_need_pop = false;
		bool is_need_set_param = true;
		bool is_set_param_success = true;

		this->stack.clear();

		do
		{
			if (is_need_pop == true)
			{
				this->stack.pop_back();
				is_need_pop = false;
			}

			if (is_need_set_param == true)
			{
				is_set_param_success = rule_current->rule_single.set_param(param_current) == true;
				is_need_set_param = false;
			}

			if (is_set_param_success == true)
			{
				if (rule_current->rule_single.run(*this, result_save, begin_current) == true)
				{
					switch (result_save.type)
					{
					case interrupt_type::invalid:
					{

						break;
					}
					case interrupt_type::end:
					{
						result_last_call = result_save.param.end;
						break;
					}
					case interrupt_type::call:
					{
						const command_param_call* call_next = result_save.param.call;

						if (test_validity_rule_index(call_next->target, true) == true)
						{
							rule_manager_rule* call_target = get_rule_manager_rule_ptr_by_rule_index(call_next->target);

							param_current.clear();

							for (std::vector<rule_var_info>::const_iterator pos_param = call_next->param.begin(); pos_param != call_next->param.end(); ++pos_param)
							{
								if (pos_param->is_var == false)
								{
									param_current.emplace_back(rule_var_info(pos_param->index, pos_param->pram.content));
								}
								else
								{
									var_single var_param = VAR_CONTENT_DEFAULT;

									if (get_rule(rule_index_current)->get_var(pos_param->pram.varible, var_param, true) == false)
										break;

									param_current.emplace_back(rule_var_info(pos_param->index, var_param));
								}
							}

							rule_index_next = call_next->target;
							begin_current = call_target->rule_single.get_begin();

							this->stack.emplace_back(rule_manager_stack(rule_index_current, result_save.next, call_next->save));
							is_need_set_param = true;
						}

						break;
					}
					default:
					{
						break;
					}
					}
				}
			}
			
			if (test_validity_rule_index(rule_index_next, true) == true)
			{
				rule_index_current = rule_index_next;
				rule_index_next = RULE_INVALID_INDEX;
			}
			else if(this->stack.empty() == false)
			{
				rule_manager_stack last = this->stack.back();

				rule_index_current = last.index;
				begin_current = last.next;

				get_rule(rule_index_current)->set_var(last.save, result_last_call, true);

				is_need_pop = true;
			}

			rule_current = get_rule_manager_rule_ptr_by_rule_index(rule_index_current);

		} while (this->stack.empty() == false);

		save = result_last_call;
		return true;
	}

};

bool rule::run(rule_manager& ownership, rule_run_result& save, command_index begin)
{
	new (&save) rule_run_result();
	command* command_current = get_command_by_command_index_ptr(begin, false);

	if (command_current == nullptr)
	{
		command_current = get_command_by_command_index_ptr(this->begin, false);

		if (command_current == nullptr)
			return false;
	}

	bool is_end = false;
	command_index command_next = COMMAND_INVALID_INDEX;

	while (is_end == false)
	{
		switch (command_current->cmd.type)
		{
		case command_type::invalid:
		{
			new (&save) rule_run_result();

			is_end = true;
			break;
		}
		case command_type::write:
		{
			command_param_write& cmd_write = command_current->cmd.param.write;

			rule_var* target = get_rule_var_by_var_ptr(cmd_write.target);

			if (target == nullptr)
				break;

			target->content = cmd_write.content;			
			break;
		}
		case command_type::copy:
		{
			command_param_copy& cmd_copy = command_current->cmd.param.copy;

			if (cmd_copy.src != cmd_copy.target)
			{
				rule_var* var_src = get_rule_var_by_var_ptr(cmd_copy.src);

				if (var_src == nullptr)
					break;

				rule_var* var_target = get_rule_var_by_var_ptr(cmd_copy.target);

				if (var_target == nullptr)
					break;

				var_target->content = var_src->content;
			}

			break;
		}
		case command_type::compare:
		{
			command_param_compare& cmd_compare = command_current->cmd.param.compare;
			rule_var* var_save = get_rule_var_by_var_ptr(cmd_compare.save);

			if (var_save == nullptr)
				break;

			if (cmd_compare.one != cmd_compare.sec)
			{
				rule_var* var_one = get_rule_var_by_var_ptr(cmd_compare.one);

				if (var_one == nullptr)
					break;

				rule_var* var_sec = get_rule_var_by_var_ptr(cmd_compare.sec);

				if (var_sec == nullptr)
					break;

				if (var_one->content == var_sec->content)
					var_save->content = static_cast<var_single>(compare_result::equal);

				if (var_one->content > var_sec->content)
					var_save->content = static_cast<var_single>(compare_result::greater);

				if (var_one->content < var_sec->content)
					var_save->content = static_cast<var_single>(compare_result::lower);
			}
			else
			{
				var_save->content = static_cast<var_single>(compare_result::equal);
			}

			break;
		}
		case command_type::jump:
		{
			command_param_jump& cmd_jump = command_current->cmd.param.jump;

			command_next = cmd_jump.target;
			break;
		}
		case command_type::jump_if_equal:
		{
			command_param_jump_if& cmd_jump_if_equal = command_current->cmd.param.jump_if;

			rule_var_pos pos_result = RULE_VAR_INVALID_POS;

			if (get_rule_var_pos_by_var(cmd_jump_if_equal.result, pos_result) == true)
			{
				if (get_rule_var_by_rule_var_pos_ptr(cmd_jump_if_equal.result.type, pos_result)->content == static_cast<var_single>(compare_result::equal))
					command_next = cmd_jump_if_equal.target;
			}

			break;
		}
		case command_type::jump_if_greater:
		{
			command_param_jump_if& cmd_jump_if_greater = command_current->cmd.param.jump_if;

			rule_var_pos pos_result = RULE_VAR_INVALID_POS;

			if (get_rule_var_pos_by_var(cmd_jump_if_greater.result, pos_result) == true)
			{
				if (get_rule_var_by_rule_var_pos_ptr(cmd_jump_if_greater.result.type, pos_result)->content == static_cast<var_single>(compare_result::greater))
					command_next = cmd_jump_if_greater.target;
			}

			break;
		}
		case command_type::jump_if_lower:
		{
			command_param_jump_if& cmd_jump_if_lower = command_current->cmd.param.jump_if;

			rule_var_pos pos_result = RULE_VAR_INVALID_POS;

			if (get_rule_var_pos_by_var(cmd_jump_if_lower.result, pos_result) == true)
			{
				if (get_rule_var_by_rule_var_pos_ptr(cmd_jump_if_lower.result.type, pos_result)->content == static_cast<var_single>(compare_result::lower))
					command_next = cmd_jump_if_lower.target;
			}

			break;
		}
		case command_type::jump_if_greater_or_equal:
		{
			command_param_jump_if& cmd_jump_if_greater_or_equal = command_current->cmd.param.jump_if;

			rule_var_pos pos_result = RULE_VAR_INVALID_POS;

			if (get_rule_var_pos_by_var(cmd_jump_if_greater_or_equal.result, pos_result) == true)
			{
				rule_var* var_target = get_rule_var_by_rule_var_pos_ptr(cmd_jump_if_greater_or_equal.result.type, pos_result);

				if (var_target->content == static_cast<var_single>(compare_result::equal) || var_target->content == static_cast<var_single>(compare_result::greater))
					command_next = cmd_jump_if_greater_or_equal.target;
			}

			break;
		}
		case command_type::jump_if_lower_or_equal:
		{
			command_param_jump_if& cmd_jump_if_lower_or_equal = command_current->cmd.param.jump_if;

			rule_var_pos pos_result = RULE_VAR_INVALID_POS;

			if (get_rule_var_pos_by_var(cmd_jump_if_lower_or_equal.result, pos_result) == true)
			{
				rule_var* var_target = get_rule_var_by_rule_var_pos_ptr(cmd_jump_if_lower_or_equal.result.type, pos_result);

				if (var_target->content == static_cast<var_single>(compare_result::equal) || var_target->content == static_cast<var_single>(compare_result::lower))
					command_next = cmd_jump_if_lower_or_equal.target;
			}

			break;
		}
		case command_type::add:
		{
			command_param_calculate& cmd_add = command_current->cmd.param.calculate;
			rule_var* var_save = get_rule_var_by_var_ptr(cmd_add.save);

			if (var_save == nullptr)
				break;

			if (cmd_add.left == cmd_add.right)
			{
				rule_var* var_left = get_rule_var_by_var_ptr(cmd_add.left);

				if (var_left == nullptr)
					break;

				var_save->content = var_left->content * 2;
			}
			else
			{
				rule_var* var_left = get_rule_var_by_var_ptr(cmd_add.left);

				if (var_left == nullptr)
					break;

				rule_var* var_right = get_rule_var_by_var_ptr(cmd_add.right);

				if (var_right == nullptr)
					break;

				var_save->content = var_left->content + var_right->content;
			}

			break;
		}
		case command_type::subtract:
		{
			command_param_calculate& cmd_subtract = command_current->cmd.param.calculate;
			rule_var* var_save = get_rule_var_by_var_ptr(cmd_subtract.save);

			if (var_save == nullptr)
				break;

			if (cmd_subtract.left == cmd_subtract.right)
			{
				var_save->content = 0;
			}
			else
			{
				rule_var* var_left = get_rule_var_by_var_ptr(cmd_subtract.left);

				if (var_left == nullptr)
					break;

				rule_var* var_right = get_rule_var_by_var_ptr(cmd_subtract.right);

				if (var_right == nullptr)
					break;

				var_save->content = var_left->content - var_right->content;
			}

			break;
		}
		case command_type::multiply:
		{
			command_param_calculate& cmd_multiply = command_current->cmd.param.calculate;
			rule_var* var_save = get_rule_var_by_var_ptr(cmd_multiply.save);

			if (var_save == nullptr)
				break;

			if (cmd_multiply.left == cmd_multiply.right)
			{
				rule_var* var_target = get_rule_var_by_var_ptr(cmd_multiply.left);

				if (var_target == nullptr)
					break;

				var_save->content = var_target->content * var_target->content;
			}
			else
			{
				rule_var* var_left = get_rule_var_by_var_ptr(cmd_multiply.left);

				if (var_left == nullptr)
					break;

				rule_var* var_right = get_rule_var_by_var_ptr(cmd_multiply.right);

				if (var_right == nullptr)
					break;

				var_save->content = var_left->content * var_right->content;
			}

			break;
		}
		case command_type::divide:
		{
			command_param_calculate& cmd_divide = command_current->cmd.param.calculate;
			rule_var* var_save = get_rule_var_by_var_ptr(cmd_divide.save);

			if (var_save == nullptr)
				break;

			rule_var* var_left = get_rule_var_by_var_ptr(cmd_divide.left);

			if (var_left == nullptr)
				break;

			rule_var* var_right = get_rule_var_by_var_ptr(cmd_divide.right);

			if (var_right == nullptr)
				break;

			if (var_right->content == 0)
				break;

			var_save->content = var_left->content / var_right->content;

			break;
		}
		case command_type::call:
		{
			command_param_call& cmd_call = command_current->cmd.param.call;

			new (&save) rule_run_result(command_current->next, &cmd_call);

			is_end = true;
			break;
		}
		case command_type::end:
		{
			command_param_end& cmd_end = command_current->cmd.param.end;

			rule_var_pos pos = RULE_VAR_INVALID_POS;

			if (get_rule_var_pos_by_var(cmd_end.out, pos) == false) new (&save) rule_run_result();
			else new (&save) rule_run_result(get_rule_var_by_rule_var_pos_ptr(cmd_end.out.type, pos)->content);

			is_end = true;
			break;
		}
		case command_type::write_command_write:
		{
			command_param_write_command_write& cmd_write_command_write = command_current->cmd.param.write_command_write;

			rule* target = ownership.get_rule(cmd_write_command_write.target_write.index);

			if (target != nullptr)
			{
				if (target->set_command(cmd_write_command_write.target_write.target, command_content(cmd_write_command_write.target_command)) == false)
				{
					new (&save) rule_run_result();
					is_end = true;
				}
			}
			else
			{
				new (&save) rule_run_result();
				is_end = true;
			}

			break;
		}
		case command_type::write_command_copy:
		{
			command_param_write_command_copy& cmd_write_command_copy = command_current->cmd.param.write_command_copy;

			rule* target = ownership.get_rule(cmd_write_command_copy.target_write.index);

			if (target != nullptr)
			{
				if (target->set_command(cmd_write_command_copy.target_write.target, command_content(cmd_write_command_copy.target_command)) == false)
				{
					new (&save) rule_run_result();
					is_end = true;
				}
			}
			else
			{
				new (&save) rule_run_result();
				is_end = true;
			}

			break;
		}
		case command_type::write_command_compare:
		{
			command_param_write_command_compare& cmd_write_command_compare = command_current->cmd.param.write_command_compare;

			rule* target = ownership.get_rule(cmd_write_command_compare.target_write.index);

			if (target != nullptr)
			{
				if (target->set_command(cmd_write_command_compare.target_write.target, command_content(cmd_write_command_compare.target_command)) == false)
				{
					new (&save) rule_run_result();
					is_end = true;
				}
			}
			else
			{
				new (&save) rule_run_result();
				is_end = true;
			}

			break;
		}
		case command_type::write_command_jump:
		{
			command_param_write_command_jump& cmd_write_command_jump = command_current->cmd.param.write_command_jump;

			rule* target = ownership.get_rule(cmd_write_command_jump.target_write.index);

			if (target != nullptr)
			{
				if (target->set_command(cmd_write_command_jump.target_write.target, command_content(cmd_write_command_jump.target_command)) == false)
				{
					new (&save) rule_run_result();
					is_end = true;
				}
			}
			else
			{
				new (&save) rule_run_result();
				is_end = true;
			}

			break;
		}
		case command_type::write_command_jump_if_equal:
		{
			command_param_write_command_jump_if& cmd_write_command_jump_if_equal = command_current->cmd.param.write_command_jump_if;

			rule* target = ownership.get_rule(cmd_write_command_jump_if_equal.target_write.index);

			if (target != nullptr)
			{
				if (target->set_command(cmd_write_command_jump_if_equal.target_write.target, command_content(command_type::jump_if_equal, cmd_write_command_jump_if_equal.target_command)) == false)
				{
					new (&save) rule_run_result();
					is_end = true;
				}
			}
			else
			{
				new (&save) rule_run_result();
				is_end = true;
			}

			break;
		}
		case command_type::write_command_jump_if_greater:
		{
			command_param_write_command_jump_if& cmd_write_command_jump_if_greater = command_current->cmd.param.write_command_jump_if;

			rule* target = ownership.get_rule(cmd_write_command_jump_if_greater.target_write.index);

			if (target != nullptr)
			{
				if (target->set_command(cmd_write_command_jump_if_greater.target_write.target, command_content(command_type::jump_if_greater, cmd_write_command_jump_if_greater.target_command)) == false)
				{
					new (&save) rule_run_result();
					is_end = true;
				}
			}
			else
			{
				new (&save) rule_run_result();
				is_end = true;
			}

			break;
		}
		case command_type::write_command_jump_if_lower:
		{
			command_param_write_command_jump_if& cmd_write_command_jump_if_lower = command_current->cmd.param.write_command_jump_if;

			rule* target = ownership.get_rule(cmd_write_command_jump_if_lower.target_write.index);

			if (target != nullptr)
			{
				if (target->set_command(cmd_write_command_jump_if_lower.target_write.target, command_content(command_type::jump_if_lower, cmd_write_command_jump_if_lower.target_command)) == false)
				{
					new (&save) rule_run_result();
					is_end = true;
				}
			}
			else
			{
				new (&save) rule_run_result();
				is_end = true;
			}

			break;
		}
		case command_type::write_command_jump_if_greater_or_equal:
		{
			command_param_write_command_jump_if& cmd_write_command_jump_if_greater_or_equal = command_current->cmd.param.write_command_jump_if;

			rule* target = ownership.get_rule(cmd_write_command_jump_if_greater_or_equal.target_write.index);

			if (target != nullptr)
			{
				if (target->set_command(cmd_write_command_jump_if_greater_or_equal.target_write.target, command_content(command_type::jump_if_greater_or_equal, cmd_write_command_jump_if_greater_or_equal.target_command)) == false)
				{
					new (&save) rule_run_result();
					is_end = true;
				}
			}
			else
			{
				new (&save) rule_run_result();
				is_end = true;
			}

			break;
		}
		case command_type::write_command_jump_if_lower_or_equal:
		{
			command_param_write_command_jump_if& cmd_write_command_jump_if_lower_or_equal = command_current->cmd.param.write_command_jump_if;

			rule* target = ownership.get_rule(cmd_write_command_jump_if_lower_or_equal.target_write.index);

			if (target != nullptr)
			{
				if (target->set_command(cmd_write_command_jump_if_lower_or_equal.target_write.target, command_content(command_type::jump_if_lower_or_equal, cmd_write_command_jump_if_lower_or_equal.target_command)) == false)
				{
					new (&save) rule_run_result();
					is_end = true;
				}
			}
			else
			{
				new (&save) rule_run_result();
				is_end = true;
			}

			break;
		}
		case command_type::write_command_add:
		{
			command_param_write_command_calculate& cmd_write_command_add = command_current->cmd.param.write_command_calculate;

			rule* target = ownership.get_rule(cmd_write_command_add.target_write.index);

			if (target != nullptr)
			{
				if (target->set_command(cmd_write_command_add.target_write.target, command_content(command_type::add, cmd_write_command_add.target_command)) == false)
				{
					new (&save) rule_run_result();
					is_end = true;
				}
			}
			else
			{
				new (&save) rule_run_result();
				is_end = true;
			}

			break;
		}
		case command_type::write_command_subtract:
		{
			command_param_write_command_calculate& cmd_write_command_subtract = command_current->cmd.param.write_command_calculate;

			rule* target = ownership.get_rule(cmd_write_command_subtract.target_write.index);

			if (target != nullptr)
			{
				if (target->set_command(cmd_write_command_subtract.target_write.target, command_content(command_type::subtract, cmd_write_command_subtract.target_command)) == false)
				{
					new (&save) rule_run_result();
					is_end = true;
				}
			}
			else
			{
				new (&save) rule_run_result();
				is_end = true;
			}

			break;
		}
		case command_type::write_command_multiply:
		{
			command_param_write_command_calculate& cmd_write_command_multiply = command_current->cmd.param.write_command_calculate;

			rule* target = ownership.get_rule(cmd_write_command_multiply.target_write.index);

			if (target != nullptr)
			{
				if (target->set_command(cmd_write_command_multiply.target_write.target, command_content(command_type::multiply, cmd_write_command_multiply.target_command)) == false)
				{
					new (&save) rule_run_result();
					is_end = true;
				}
			}
			else
			{
				new (&save) rule_run_result();
				is_end = true;
			}

			break;
		}
		case command_type::write_command_divide:
		{
			command_param_write_command_calculate& cmd_write_command_divide = command_current->cmd.param.write_command_calculate;

			rule* target = ownership.get_rule(cmd_write_command_divide.target_write.index);

			if (target != nullptr)
			{
				if (target->set_command(cmd_write_command_divide.target_write.target, command_content(command_type::divide, cmd_write_command_divide.target_command)) == false)
				{
					new (&save) rule_run_result();
					is_end = true;
				}
			}
			else
			{
				new (&save) rule_run_result();
				is_end = true;
			}

			break;
		}
		case command_type::write_command_call:
		{
			command_param_write_command_call& cmd_write_command_call = command_current->cmd.param.write_command_call;

			rule* target = ownership.get_rule(cmd_write_command_call.target_write.index);

			if (target != nullptr)
			{
				if (target->set_command(cmd_write_command_call.target_write.target, command_content(cmd_write_command_call.target_command)) == false)
				{
					new (&save) rule_run_result();
					is_end = true;
				}
			}
			else
			{
				new (&save) rule_run_result();
				is_end = true;
			}

			break;
		}
		case command_type::write_command_end:
		{
			command_param_write_command_end& cmd_write_command_end = command_current->cmd.param.write_command_end;

			rule* target = ownership.get_rule(cmd_write_command_end.target_write.index);

			if (target != nullptr)
			{
				if (target->set_command(cmd_write_command_end.target_write.target, command_content(cmd_write_command_end.target_command)) == false)
				{
					new (&save) rule_run_result();
					is_end = true;
				}
			}
			else
			{
				new (&save) rule_run_result();
				is_end = true;
			}

			break;
		}
		default: break;
		}

		if (is_end == true)
			break;

		if (test_validity_command_index(command_next, true) == false)
		{
			command_current = get_command_by_command_index_ptr(command_current->next, false);

			if (command_current == nullptr)
				break;
		}
		else
		{
			command_current = get_command_by_command_index_ptr(command_next, false);
			command_next = COMMAND_INVALID_INDEX;
		}
	}

	return true;
}


















