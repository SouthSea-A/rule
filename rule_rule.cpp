#include "rule_rule.h"


rule_interrupt_value_call::rule_interrupt_value_call() :
	target(RULE_INVALID_INDEX), param_set(std::in_place_index<static_cast<size_t>(operand_type::immediate)>, VAR_DEFAULT_CONTENT), save(std::in_place_index<static_cast<size_t>(operand_type::immediate)>, VAR_DEFAULT_CONTENT), next(COMMAND_INVALID_INDEX)
{

}
rule_interrupt_value_call::rule_interrupt_value_call(rule_index target_, const rule_interrupt_call& param_set_, const rule_interrupt_call& save_, command_index next_) :
	target(target_), param_set(param_set_), save(save_), next(next_)
{

}
rule_interrupt_value_call::rule_interrupt_value_call(const rule_interrupt_value_call& rule_interrupt_value_call_) :
	target(rule_interrupt_value_call_.target), param_set(rule_interrupt_value_call_.param_set), save(rule_interrupt_value_call_.save), next(rule_interrupt_value_call_.next)
{

}
rule_interrupt_value_call::rule_interrupt_value_call(rule_interrupt_value_call&& rule_interrupt_value_call_) noexcept :
	target(rule_interrupt_value_call_.target), param_set(std::move(rule_interrupt_value_call_.param_set)), save(std::move(rule_interrupt_value_call_.save)), next(rule_interrupt_value_call_.next)
{
	rule_interrupt_value_call_.next = COMMAND_INVALID_INDEX;
}
rule_interrupt_value_call& rule_interrupt_value_call::operator=(const rule_interrupt_value_call& other)
{
	if (this != &other)
	{
		this->target = other.target;
		this->param_set = other.param_set;
		this->save = other.save;
		this->next = other.next;
	}

	return *this;
}




rule_interrupt_value_invalid::rule_interrupt_value_invalid() :
	index(COMMAND_INVALID_INDEX), op_error(std::in_place_index<static_cast<size_t>(operand_type::immediate)>)
{

}
rule_interrupt_value_invalid::rule_interrupt_value_invalid(command_index index_, const operand& op_error_) :
	index(index_), op_error(op_error_)
{

}
rule_interrupt_value_invalid::rule_interrupt_value_invalid(const rule_interrupt_value_invalid& rule_interrupt_value_invalid_) :
	index(rule_interrupt_value_invalid_.index), op_error(rule_interrupt_value_invalid_.op_error)
{

}
rule_interrupt_value_invalid::rule_interrupt_value_invalid(rule_interrupt_value_invalid&& rule_interrupt_value_invalid_) noexcept :
	index(std::move(rule_interrupt_value_invalid_.index)), op_error(std::move(rule_interrupt_value_invalid_.op_error))
{
	rule_interrupt_value_invalid_.index = COMMAND_INVALID_INDEX;
}
rule_interrupt_value_invalid& rule_interrupt_value_invalid::operator=(const rule_interrupt_value_invalid& other)
{
	if (this != &other)
	{
		this->index = other.index;
		this->op_error = other.op_error;
	}

	return *this;
}




rule_interrupt::rule_interrupt() :
	type(rule_interrupt_type::invalid), value(std::nullopt), info()
{

}
rule_interrupt::rule_interrupt(rule_interrupt_type type_, const std::optional<rule_interrupt_value>& value_, const std::string& info_) :
	type(type_), value(value_), info(info_)
{

}
rule_interrupt::rule_interrupt(const rule_interrupt& rule_interrupt_) :
	type(rule_interrupt_.type), value(rule_interrupt_.value), info(rule_interrupt_.info)
{

}
rule_interrupt::rule_interrupt(rule_interrupt&& rule_interrupt_) noexcept :
	type(rule_interrupt_.type), value(std::move(rule_interrupt_.value)), info(std::move(rule_interrupt_.info))
{
	rule_interrupt_.type = rule_interrupt_type::invalid;
}
rule_interrupt& rule_interrupt::operator=(const rule_interrupt& other)
{
	if (this != &other)
	{
		this->type = other.type;
		this->value = other.value;
		this->info = other.info;
	}

	return *this;
}



cmd_funcs::cmd_funcs() :
	run(), op_get()
{

}
cmd_funcs::cmd_funcs(const cmd_run_func& run_, const cmd_op_func& op_get_) :
	run(run_), op_get(op_get_)
{

}
cmd_funcs::cmd_funcs(const cmd_funcs& cmd_funs_) :
	run(cmd_funs_.run), op_get(cmd_funs_.op_get)
{

}
cmd_funcs::cmd_funcs(cmd_funcs&& cmd_funs_) noexcept :
	run(std::move(cmd_funs_.run)), op_get(std::move(cmd_funs_.op_get))
{

}
cmd_funcs& cmd_funcs::operator=(const cmd_funcs& other)
{
	if (this != &other)
	{
		this->run = other.run;
		this->op_get = other.op_get;
	}

	return *this;
}


bool rule::add_var_by_op_single(const operand& op)
{
	bool add_result = true;

	switch (static_cast<operand_type>(op.index()))
	{
	case operand_type::immediate:
	{
		add_result = true;
		break;
	}
	case operand_type::variable_param:
	{
		add_result&& this->rule_var_param.add_var(var(std::get<variable_param>(op).index, VAR_DEFAULT_CONTENT));
		break;
	}
	case operand_type::proxy_param:
	{
		add_result&& this->rule_var_param.add_var(var(std::get<proxy_param>(op).index, VAR_DEFAULT_CONTENT));
		break;
	}
	case operand_type::variable_common:
	{
		add_result&& this->rule_var_common.add_var(var(std::get<variable_common>(op).index, VAR_DEFAULT_CONTENT));
		break;
	}
	case operand_type::proxy_common:
	{
		add_result&& this->rule_var_common.add_var(var(std::get<proxy_common>(op).index, VAR_DEFAULT_CONTENT));
		break;
	}
	default:
	{
		add_result = false;
		break;
	}
	}

	return add_result;
}
bool rule::del_var_by_op_single(const operand& op)
{
	bool del_result = true;

	switch (static_cast<operand_type>(op.index()))
	{
	case operand_type::immediate:
	{
		del_result = true;
		break;
	}
	case operand_type::variable_param:
	{
		del_result&& this->rule_var_param.del_var(std::get<variable_param>(op).index);
		break;
	}
	case operand_type::proxy_param:
	{
		del_result&& this->rule_var_param.del_var(std::get<proxy_param>(op).index);
		break;
	}
	case operand_type::variable_common:
	{
		del_result&& this->rule_var_common.del_var(std::get<variable_common>(op).index);
		break;
	}
	case operand_type::proxy_common:
	{
		del_result&& this->rule_var_common.del_var(std::get<proxy_common>(op).index);
		break;
	}
	default:
	{
		del_result = false;
		break;
	}
	}

	return del_result;
}
bool rule::add_var_by_op(const command_operand& op)
{
	bool add_result = true;

	std::visit(
		[&](const auto& op_target)
		{
			constexpr size_t op_quan = std::tuple_size_v<decltype(op_target.set)>;

			[&] <size_t... index>(std::index_sequence<index...>)
			{
				(add_if_unique<index>(op_target, add_result), ...);
			}(std::make_index_sequence<op_quan>{});

		},
		op);

	return add_result;
}
bool rule::del_var_by_op(const command_operand& op)
{
	bool del_result = true;

	std::visit(
		[&](const auto& op_target)
		{
			constexpr size_t op_quan = std::tuple_size_v<decltype(op_target.set)>;

			[&] <size_t... index>(std::index_sequence<index...>)
			{
				(del_if_unique<index>(op_target, del_result), ...);
			}(std::make_index_sequence<op_quan>{});

		},
		op);

	return del_result;
}
std::optional<var_single> rule::get_var_value(const operand& op) const
{
	std::optional<var_single> result_value;

	switch (static_cast<operand_type>(op.index()))
	{
	case operand_type::immediate:
	{
		result_value.emplace(std::get<immediate>(op).value);
		break;
	}
	case operand_type::variable_common:
	{
		result_value = this->rule_var_common.get_var(std::get<variable_common>(op).index);
		break;
	}
	case operand_type::variable_param:
	{
		result_value = this->rule_var_param.get_var(std::get<variable_param>(op).index);
		break;
	}
	case operand_type::proxy_common:
	{
		result_value = this->rule_var_common.get_var(std::get<proxy_common>(op).index);

		if (result_value.has_value() == false)
		{
			result_value = std::nullopt;
			break;
		}

		result_value = this->rule_var_common.get_var(static_cast<var_index>(result_value.value()));
		break;
	}
	case operand_type::proxy_param:
	{
		result_value = this->rule_var_param.get_var(std::get<proxy_param>(op).index);

		if (result_value.has_value() == false)
		{
			result_value = std::nullopt;
			break;
		}

		result_value = this->rule_var_param.get_var(static_cast<var_index>(result_value.value()));

		break;
	}
	default:
	{
		result_value = std::nullopt;
		break;
	}
	}

	return result_value;
}

bool rule::get_var_value(const operand& op, size_t quan, std::vector<var_single>& save) const
{
	switch (static_cast<operand_type>(op.index()))
	{
	case operand_type::immediate:
	{
		save.resize(1);
		save.back() = std::get<immediate>(op).value;
		break;
	}
	case operand_type::variable_common:
	{
		if (this->rule_var_common.get_var(save, std::get<variable_common>(op).index, quan) == false)
			return false;
		
		break;
	}
	case operand_type::variable_param:
	{
		if (this->rule_var_param.get_var(save, std::get<variable_common>(op).index, quan) == false)
			return false;

		break;
	}
	case operand_type::proxy_common:
	{
		std::optional<var_single> result_value = this->rule_var_common.get_var(std::get<proxy_common>(op).index);

		if (result_value.has_value() == false)
		{
			return false;
			break;
		}

		if (this->rule_var_common.get_var(save, static_cast<var_index>(result_value.value()), quan) == false)
			return false;

		break;
	}
	case operand_type::proxy_param:
	{
		std::optional<var_single> result_value = this->rule_var_param.get_var(std::get<proxy_param>(op).index);

		if (result_value.has_value() == false)
		{
			return false;
			break;
		}

		if (this->rule_var_param.get_var(save, static_cast<var_index>(result_value.value()), quan) == false)
			return false;

		break;
	}
	default:
	{
		return false;
		break;
	}
	}

	return true;

}

bool rule::set_var_value(const operand& op_src, var_single value, bool is_allow_dynamic_alloc)
{
	bool result = false;

	switch (static_cast<operand_type>(op_src.index()))
	{
	case operand_type::immediate:
	{
		result = false;
		break;
	}
	case operand_type::variable_common:
	{
		const variable_common& vc = std::get<variable_common>(op_src);

		result = this->rule_var_common.set_var(vc.index, value);

		if (result == false && is_allow_dynamic_alloc == true)
		{
			if (add_var_by_op_single(vc) == false)
			{
				result = false;
				break;
			}

			result = this->rule_var_common.set_var(vc.index, value);
		}

		break;
	}
	case operand_type::variable_param:
	{
		const variable_param& vp = std::get<variable_param>(op_src);

		result = this->rule_var_param.set_var(vp.index, value);

		if (result == false && is_allow_dynamic_alloc == true)
		{
			if (add_var_by_op_single(vp) == false)
			{
				result = false;
				break;
			}

			result = this->rule_var_param.set_var(vp.index, value);
		}

		break;
	}
	case operand_type::proxy_common:
	{
		std::optional<var_single> var_proxy = this->rule_var_common.get_var(std::get<proxy_common>(op_src).index);

		if (var_proxy.has_value() == false)
		{
			result = false;
			break;
		}

		var_index target = static_cast<var_index>(var_proxy.value());

		result = this->rule_var_common.set_var(target, value);

		if (result == false && is_allow_dynamic_alloc == true)
		{
			if (add_var_by_op_single(variable_common(target)) == false)
			{
				result = false;
				break;
			}

			result = this->rule_var_common.set_var(target, value);
		}

		break;
	}
	case operand_type::proxy_param:
	{
		std::optional<var_single> var_proxy = this->rule_var_param.get_var(std::get<proxy_param>(op_src).index);

		if (var_proxy.has_value() == false)
		{
			result = false;
			break;
		}

		var_index target = static_cast<var_index>(var_proxy.value());

		result = this->rule_var_param.set_var(target, value);

		if (result == false && is_allow_dynamic_alloc == true)
		{
			if (add_var_by_op_single(variable_param(target)) == false)
			{
				result = false;
				break;
			}

			result = this->rule_var_param.set_var(target, value);
		}

		break;
	}
	default:
	{
		result = false;
		break;
	}
	}

	return result;
}
bool rule::set_var_value(const operand& op_src, std::span<const var_single> value_all)
{
	bool result = false;

	switch (static_cast<operand_type>(op_src.index()))
	{
	case operand_type::immediate:
	{
		result = false;
		break;
	}
	case operand_type::variable_common:
	{
		const variable_common& vc = std::get<variable_common>(op_src);

		result = this->rule_var_common.set_var(vc.index, value_all);
		break;
	}
	case operand_type::variable_param:
	{
		const variable_param& vp = std::get<variable_param>(op_src);

		result = this->rule_var_param.set_var(vp.index, value_all);
		break;
	}
	case operand_type::proxy_common:
	{
		std::optional<var_single> var_proxy = this->rule_var_common.get_var(std::get<proxy_common>(op_src).index);

		if (var_proxy.has_value() == false)
		{
			result = false;
			break;
		}

		var_index target = static_cast<var_index>(var_proxy.value());

		result = this->rule_var_common.set_var(target, value_all);
		break;
	}
	case operand_type::proxy_param:
	{
		std::optional<var_single> var_proxy = this->rule_var_param.get_var(std::get<proxy_param>(op_src).index);

		if (var_proxy.has_value() == false)
		{
			result = false;
			break;
		}

		var_index target = static_cast<var_index>(var_proxy.value());

		result = this->rule_var_param.set_var(target, value_all);
		break;
	}
	default:
	{
		result = false;
		break;
	}
	}

	return result;
}
bool rule::get_op_by_op(const operand& op_begin, size_t quantity, std::vector<var_single>& save) const
{
	if (quantity == 0)
	{
		save.resize(0);
		return true;
	}

	save.resize(quantity);

	switch (static_cast<operand_type>(op_begin.index()))
	{
	case operand_type::immediate:
	{
		if (quantity > 1)
			return false;

		std::optional<var_single> result = get_var_value(op_begin);

		if (result.has_value() == false)
			return false;

		save.front() = result.value();

		break;
	}
	case operand_type::variable_common:
	{
		if (this->rule_var_common.get_var(save, static_cast<var_index>(std::get<variable_common>(op_begin).index), quantity) == false)
			return false;

		break;
	}
	case operand_type::proxy_common:
	{
		std::optional<var_single> result = get_var_value(op_begin);

		if (result.has_value() == false)
			return false;

		if (this->rule_var_common.get_var(save, static_cast<var_index>(result.value()), quantity) == false)
			return false;

		break;
	}
	case operand_type::variable_param:
	{
		if (this->rule_var_param.get_var(save, static_cast<var_index>(std::get<variable_param>(op_begin).index), quantity) == false)
			return false;

		break;
	}
	case operand_type::proxy_param:
	{
		std::optional<var_single> result = get_var_value(op_begin);

		if (result.has_value() == false)
			return false;

		if (this->rule_var_param.get_var(save, static_cast<var_index>(result.value()), quantity) == false)
			return false;

		break;
	}
	default:
	{
		return false;
		break;
	}
	}

	return true;
}
bool rule::get_op_by_var(var_single type, var_single content, operand& save, operand_type invalid_type) const
{
	operand_type type_target = static_cast<operand_type>(type % 5);

	if (type_target == invalid_type)
		return false;

	switch (static_cast<operand_type>(type % 5))
	{
	case operand_type::immediate:
	{
		save.emplace<immediate>(content);
		break;
	}
	case operand_type::variable_common:
	{
		save.emplace<variable_common>(content);
		break;
	}
	case operand_type::variable_param:
	{
		save.emplace<variable_param>(content);
		break;
	}
	case operand_type::proxy_common:
	{
		save.emplace<proxy_common>(content);
		break;
	}
	case operand_type::proxy_param:
	{
		save.emplace<proxy_param>(content);
		break;
	}
	default:
	{
		return false;
		break;
	}
	}

	return true;
}
bool rule::get_op_by_cmd_op(command_type cmd_type, const operand& op, std::unordered_map<size_t, cmd_funcs>& cmd_set, command_operand& save) const
{
	return cmd_set[static_cast<size_t>(cmd_type)].op_get(*this, op, save);
}
void rule::get_info_by_cmd(const command& cmd_op, std::vector<var_single>& save) const
{
	std::visit(
		[&](const auto& op_set)->auto
		{
			constexpr size_t op_quan = std::tuple_size_v<decltype(op_set.set)>;

			save.assign(1 + op_quan * 2, static_cast<var_single>(operand_type::invalid));
			save.front() = static_cast<var_single>(cmd_op.cmd);

			[&]<size_t... op_case>(std::index_sequence<op_case...>)
			{
				(... , get_info_by_cmd_op_case_single<op_case>(op_set, save));
			}(std::make_index_sequence<op_quan>{});

		},
		cmd_op.op);

	return;
}
rule::rule() :
	rule_var_param(), rule_var_common(), rule_command()
{

}
rule::~rule()
{

}
bool rule::set_param(std::span<const var_single> param)
{
	return this->rule_var_param.set_var(param);
}
std::optional<size_t> rule::get_param_quantity() const
{
	return this->rule_var_param.get_quantity_var();
}
std::optional<command_index> rule::get_begin() const
{
	return this->rule_command.get_begin();
}
std::optional<command_index> rule::get_end() const
{
	return this->rule_command.get_end();
}
bool rule::set_begin(command_index index)
{
	return this->rule_command.set_begin(index);
}
bool rule::set_end(command_index index)
{
	return this->rule_command.set_end(index);
}
std::optional<command> rule::get_command(command_index index) const
{
	return this->rule_command.get_command(index);
}
std::optional<command_index> rule::add_command(const command& command_new, command& ref_update, command_index index_update)
{
	std::optional<command_index> result = add_command(command_new);
	std::optional<command*> cmd_new = this->rule_command.get_command_ptr(index_update);

	if (cmd_new.has_value() == true)
		ref_update = *cmd_new.value();

	return result;
}
std::optional<command_index> rule::add_command(const command& command_new)
{
	std::optional<command_index> command_index_new = this->rule_command.add_command(command_new);

	if (command_index_new.has_value() == false)
		return std::nullopt;

	if (add_var_by_op(command_new.op) == false)
	{
		this->rule_command.del_command(command_index_new.value());
		return std::nullopt;
	}

	return std::make_optional<command_index>(command_index_new.value());
}
std::optional<command_index> rule::insert_command(command_index index_insert, const command& command_new, command& ref_update, command_index index_update)
{
	std::optional<command_index> result = insert_command(index_insert, command_new);
	std::optional<command*> cmd_new = this->rule_command.get_command_ptr(index_update);

	if (cmd_new.has_value() == true)
		ref_update = *cmd_new.value();

	return result;
}
std::optional<command_index> rule::insert_command(command_index index_insert, const command& command_new)
{
	std::optional<command_index> command_index_new = this->rule_command.insert_command(index_insert, command_new);

	if (command_index_new.has_value() == false)
		return std::nullopt;

	if (add_var_by_op(command_new.op) == false)
	{
		this->rule_command.del_command(command_index_new.value());
		return std::nullopt;
	}

	return std::make_optional<command_index>(command_index_new.value());
}
bool rule::set_command(command_index index, const command& command_new)
{
	std::optional<command> target = this->rule_command.get_command(index);

	if (target.has_value() == false)
		return false;

	if (del_var_by_op(target.value().op) == false)
		return false;

	if (this->rule_command.set_command(index, command_new) == false)
	{
		add_var_by_op(target.value().op);
		return false;
	}

	if (add_var_by_op(command_new.op) == false)
	{
		this->rule_command.set_command(index, target.value());
		add_var_by_op(target.value().op);
		return false;
	}

	return true;
}
bool rule::del_command(command_index index, command& ref_update, command_index index_update)
{
	bool result = del_command(index);
	std::optional<command*> cmd_new = this->rule_command.get_command_ptr(index_update);

	if (cmd_new.has_value() == true)
		ref_update = *cmd_new.value();

	return result;
}
bool rule::del_command(command_index index)
{
	std::optional<command> command_del_target = this->rule_command.get_command(index);

	if (command_del_target.has_value() == false)
		return false;

	if (this->rule_command.del_command(index) == false)
		return false;

	if (del_var_by_op(command_del_target.value().op) == false)
		return false;

	return true;
}
void rule::run(rule_manager& onwership, rule_interrupt& interrupt, const std::optional<command_index>& begin, std::vector<var_single>& result, std::unordered_map<size_t, cmd_funcs>& cmd_set)
{
	interrupt = rule_interrupt(rule_interrupt_type::end, std::nullopt, "null");

	std::optional<command_index> command_begin_result = begin.has_value() == false ? this->rule_command.get_begin() : begin.value();

	if (command_begin_result.has_value() == false)
	{
		interrupt = rule_interrupt(
			rule_interrupt_type::invalid,
			std::nullopt,
			"起始指令不存在"
		);
		return;
	}

	if (this->rule_command.test_validity_command_index(command_begin_result.value(), true) == false)
	{
		interrupt = rule_interrupt(
			rule_interrupt_type::invalid,
			std::nullopt,
			"起始指令不存在"
		);
		return;
	}

	bool is_end = false;
	std::optional<command_index> command_index_next = std::nullopt;
	command_index command_index_current = command_begin_result.value();
	std::optional<command*> command_current_result = this->rule_command.get_command_ptr(command_index_current);

	while (is_end == false)
	{
		if (command_current_result.has_value() == false)
		{
			interrupt = rule_interrupt(
				rule_interrupt_type::invalid_cmd,
				std::make_optional<rule_interrupt_value_invalid>(
					command_index_current,
					immediate(static_cast<var_single>(command_index_current))
				),
				"无法找到对应指令"
			);
			return;
		}

		command* command_current = command_current_result.value();

		cmd_set[static_cast<size_t>(command_current->cmd)].run(
			*this,
			onwership,
			interrupt,
			command_index_current,
			*command_current,
			command_index_next,
			result,
			is_end
		);

		if (is_end == true)
			break;

		if (command_index_next.has_value() == false)
		{
			std::optional<command_index> command_index_next_result = this->rule_command.get_command_index_next(command_index_current);

			if (command_index_next_result.has_value() == false)
			{
				interrupt = rule_interrupt(
					rule_interrupt_type::end,
					std::nullopt,
					"单规则执行完成"
				);
				return;
			}

			command_index_current = command_index_next_result.value();
		}
		else
		{
			command_index_current = command_index_next.value();
			command_index_next = std::nullopt;
		}

		command_current_result = this->rule_command.get_command_ptr(command_index_current);
	}

	return;

}
rule& rule::operator=(const rule& other)
{
	if (this != &other)
	{
		this->rule_var_param = other.rule_var_param;
		this->rule_var_common = other.rule_var_common;
		this->rule_command = other.rule_command;
	}

	return *this;
}

