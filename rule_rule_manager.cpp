#include "rule_rule_manager.h"


rule_unit::rule_unit() :
	index(RULE_INVALID_INDEX), content()
{

}
rule_unit::rule_unit(rule_index index_, const rule& content_) :
	index(index_), content(content_)
{

}
rule_unit::rule_unit(rule_index index_) :
	index(index_), content()
{

}
rule_unit::rule_unit(const rule_unit& rule_unit_) :
	index(rule_unit_.index), content(rule_unit_.content)
{

}
rule_unit::rule_unit(rule_unit&& rule_unit_) noexcept :
	index(rule_unit_.index), content(std::move(rule_unit_.content))
{
	rule_unit_.index = RULE_INVALID_INDEX;
}
rule_unit& rule_unit::operator=(const rule_unit& other)
{
	if (this != &other)
	{
		this->index = other.index;
		this->content = other.content;
	}

	return *this;
}
rule_unit::~rule_unit()
{
	this->index = RULE_INVALID_INDEX;
}



rule_manager_stack::rule_manager_stack() :
	index(RULE_INVALID_INDEX), next(COMMAND_INVALID_INDEX), save(std::in_place_index<static_cast<size_t>(operand_type::immediate)>, VAR_DEFAULT_CONTENT)
{

}
rule_manager_stack::rule_manager_stack(rule_index index_, command_index next_, const var_save& save_) :
	index(index_), next(next_), save(save_)
{

}
rule_manager_stack::rule_manager_stack(const rule_manager_stack& rule_manager_stack_) :
	index(rule_manager_stack_.index), next(rule_manager_stack_.next), save(rule_manager_stack_.save)
{

}
rule_manager_stack::rule_manager_stack(rule_manager_stack&& rule_manager_stack_) noexcept :
	index(rule_manager_stack_.index), next(rule_manager_stack_.next), save(std::move(rule_manager_stack_.save))
{
	rule_manager_stack_.index = RULE_INVALID_INDEX;
	rule_manager_stack_.next = COMMAND_INVALID_INDEX;
}



bool rule_manager::test_validity_rule_index(rule_index index, bool is_test_del) const
{
	if (index == RULE_INVALID_INDEX)
		return false;

	if (index >= this->rule_set.size())
		return false;

	return is_test_del == false ? true : this->rule_set[static_cast<size_t>(index)].index != COMMAND_INVALID_INDEX;
}
std::optional<rule_unit*> rule_manager::get_rule_unit_ptr(rule_index index)
{
	if (test_validity_rule_index(index, true) == false)
		return std::nullopt;

	return std::make_optional<rule_unit*>(&this->rule_set[index]);
}
std::optional<const rule_unit*> rule_manager::get_rule_unit_ptr_const(rule_index index) const
{
	if (test_validity_rule_index(index, true) == false)
		return std::nullopt;

	return std::make_optional<const rule_unit*>(&this->rule_set[index]);
}
std::optional<rule_index> rule_manager::get_free()
{
	if (this->rule_set_free.empty() == false)
	{
		rule_index index_free = this->rule_set_free.back();
		this->rule_set_free.pop_back();
		return std::make_optional<rule_index>(index_free);
	}

	return std::nullopt;
}
bool rule_manager::add_free(rule_index index)
{
	std::optional<rule_unit*> rule_uint_ptr_result = get_rule_unit_ptr(index);

	if (rule_uint_ptr_result.has_value() == false)
		return false;

	rule_uint_ptr_result.value()->~rule_unit();

	this->rule_set_free.emplace_back(index);
	return true;
}
bool rule_manager::get_param(rule_index target_src, const rule_interrupt_value_call& src, std::vector<var_single>& save) const
{
	save.clear();

	std::optional<const rule_unit*> rule_result = get_rule_unit_ptr_const(target_src);

	if (rule_result.has_value() == false)
		return false;

	const rule_unit* rule_target_src = rule_result.value();

	rule_result = get_rule_unit_ptr_const(src.target);

	if (rule_result.has_value() == false)
		return false;

	std::optional<size_t> param_quan_result = rule_result.value()->content.get_param_quantity();

	if (param_quan_result.has_value() == false)
		return false;

	size_t param_quan = param_quan_result.value();

	if (param_quan == 0)
		return true;

	save.resize(param_quan);

	switch (static_cast<rule_interrupt_call_type>(src.param_set.index()))
	{
	case rule_interrupt_call_type::immediate:
	{
		if (param_quan != 1)
		{
			save.clear();
			return false;
		}

		save.front() = std::get<immediate>(src.param_set).value;

		return true;
		break;
	}
	case rule_interrupt_call_type::proxy_common:
	{
		var_index var_proxy_index = std::get<proxy_common>(src.param_set).index;
		std::optional<var_single> var_target = rule_target_src->content.rule_var_common.get_var(var_proxy_index);

		if (var_target.has_value() == false)
			return false;

		return rule_target_src->content.rule_var_common.get_var(save, static_cast<var_index>(var_target.value()), param_quan);
		break;
	}
	case rule_interrupt_call_type::proxy_param:
	{
		var_index var_proxy_index = std::get<proxy_param>(src.param_set).index;
		std::optional<var_single> var_target = rule_target_src->content.rule_var_param.get_var(var_proxy_index);

		if (var_target.has_value() == false)
			return false;

		return rule_target_src->content.rule_var_param.get_var(save, static_cast<var_index>(var_target.value()), param_quan);
		break;
	}
	case rule_interrupt_call_type::variable_common:
	{
		var_index var_index = std::get<variable_common>(src.param_set).index;

		return rule_target_src->content.rule_var_common.get_var(save, var_index, param_quan);
		break;
	}
	case rule_interrupt_call_type::variable_param:
	{
		var_index var_index = std::get<variable_param>(src.param_set).index;

		return rule_target_src->content.rule_var_param.get_var(save, var_index, param_quan);
		break;
	}
	}

	return false;
}
void rule_manager::get_operand_text(const operand& op, std::string& save) const
{
	switch (static_cast<operand_type>(op.index()))
	{
	case operand_type::immediate:
	{
		save = std::format("<op type: immediae | value:{}>", static_cast<size_t>(std::get<immediate>(op).value));
		break;
	}
	case operand_type::variable_common:
	{
		save = std::format("<op type: variable_common | var index:{}>", static_cast<size_t>(std::get<variable_common>(op).index));
		break;
	}
	case operand_type::variable_param:
	{
		save = std::format("<op type: variable_param | var index:{}>", static_cast<size_t>(std::get<variable_param>(op).index));
		break;
	}
	case operand_type::proxy_common:
	{
		save = std::format("<op type: proxy_common | var index:{}>", static_cast<size_t>(std::get<proxy_common>(op).index));
		break;
	}
	case operand_type::proxy_param:
	{
		save = std::format("<op type: proxy_param | var index:{}>", static_cast<size_t>(std::get<proxy_param>(op).index));
		break;
	}
	default:
	{
		save = std::format("<op type: invalid | var index:NULL>");
		break;
	}
	}

	return;
}
void rule_manager::get_interrupt_text(rule_index index, const rule_interrupt& interrupt, std::string& save) const
{
	switch (interrupt.type)
	{
	case rule_interrupt_type::invalid:
	{
		save = std::format(
			"[rule_manager interrupt:{}] rule id:{} | info: {:s}\n",
			static_cast<size_t>(interrupt.type),
			static_cast<size_t>(index),
			interrupt.info
		);

		break;
	}
	case rule_interrupt_type::invalid_var:
	case rule_interrupt_type::invalid_cmd:
	{
		const rule_interrupt_value_invalid& error_target = std::get<rule_interrupt_value_invalid>(interrupt.value.value());

		if (interrupt.value.has_value() == true)
			get_operand_text(error_target.op_error, save);

		save = std::format(
			"[rule_manager interrupt:{}] rule id:{} | command id:{} | operand: {:s} | info: {:s}\n",
			static_cast<size_t>(interrupt.type),
			static_cast<size_t>(index),
			static_cast<size_t>(error_target.index),
			save,
			interrupt.info
		);

		break;
	}
	case rule_interrupt_type::call:
	{
		const rule_interrupt_value_call& call_target = std::get<rule_interrupt_value_call>(interrupt.value.value());

		get_operand_text(static_cast<operand>(call_target.param_set), save);

		save = std::format(
			"[rule_manager interrupt:{}] rule id:{} | target:{} | param: {:s} | info: {:s}\n",
			static_cast<size_t>(interrupt.type),
			static_cast<size_t>(index),
			static_cast<size_t>(call_target.target),
			save,
			interrupt.info
		);

		break;
	}
	case rule_interrupt_type::end:
	{

		save = std::format(
			"[rule_manager interrupt:{}] rule id:{} | info: {:s}\n",
			static_cast<size_t>(interrupt.type),
			static_cast<size_t>(index),
			interrupt.info
		);

		break;
	}
	default:
	{
		save = std::format(
			"[rule_manager interrupt:NULL] rule id:{} | info: {:s}\n",
			static_cast<size_t>(index),
			interrupt.info
		);

		break;
	}
	}

	return;
}
rule_manager::rule_manager() :
	begin(), rule_set(), rule_set_free(), funcs(), stack()
{
	this->funcs[static_cast<size_t>(command_type::mov)] = cmd_funcs(func_base_run_mov, func_base_op_get_mov);
	this->funcs[static_cast<size_t>(command_type::cmp)] = cmd_funcs(func_base_run_cmp, func_base_op_get_cmp);
	this->funcs[static_cast<size_t>(command_type::jmp)] = cmd_funcs(func_base_run_jmp, func_base_op_get_jmp);
	this->funcs[static_cast<size_t>(command_type::je)] = cmd_funcs(func_base_run_je, func_base_op_get_je);
	this->funcs[static_cast<size_t>(command_type::jne)] = cmd_funcs(func_base_run_jne, func_base_op_get_jne);
	this->funcs[static_cast<size_t>(command_type::ja)] = cmd_funcs(func_base_run_ja, func_base_op_get_ja);
	this->funcs[static_cast<size_t>(command_type::jae)] = cmd_funcs(func_base_run_jae, func_base_op_get_jae);
	this->funcs[static_cast<size_t>(command_type::jb)] = cmd_funcs(func_base_run_jb, func_base_op_get_jb);
	this->funcs[static_cast<size_t>(command_type::jbe)] = cmd_funcs(func_base_run_jbe, func_base_op_get_jbe);
	this->funcs[static_cast<size_t>(command_type::add)] = cmd_funcs(func_base_run_add, func_base_op_get_add);
	this->funcs[static_cast<size_t>(command_type::sub)] = cmd_funcs(func_base_run_sub, func_base_op_get_sub);
	this->funcs[static_cast<size_t>(command_type::mul)] = cmd_funcs(func_base_run_mul, func_base_op_get_mul);
	this->funcs[static_cast<size_t>(command_type::div)] = cmd_funcs(func_base_run_div, func_base_op_get_div);
	this->funcs[static_cast<size_t>(command_type::call)] = cmd_funcs(func_base_run_call, func_base_op_get_call);
	this->funcs[static_cast<size_t>(command_type::ret)] = cmd_funcs(func_base_run_ret, func_base_op_get_ret);
	this->funcs[static_cast<size_t>(command_type::acmd)] = cmd_funcs(func_base_run_acmd, func_base_op_get_acmd);
	this->funcs[static_cast<size_t>(command_type::icmd)] = cmd_funcs(func_base_run_icmd, func_base_op_get_icmd);
	this->funcs[static_cast<size_t>(command_type::scmd)] = cmd_funcs(func_base_run_scmd, func_base_op_get_scmd);
	this->funcs[static_cast<size_t>(command_type::dcmd)] = cmd_funcs(func_base_run_dcmd, func_base_op_get_dcmd);
	this->funcs[static_cast<size_t>(command_type::gcmd)] = cmd_funcs(func_base_run_gcmd, func_base_op_get_gcmd);
	this->funcs[static_cast<size_t>(command_type::gcmdnext)] = cmd_funcs(func_base_run_gcmdnext, func_base_op_get_gcmdnext);
	this->funcs[static_cast<size_t>(command_type::gcmdlast)] = cmd_funcs(func_base_run_gcmdlast, func_base_op_get_gcmdlast);
	this->funcs[static_cast<size_t>(command_type::arule)] = cmd_funcs(func_base_run_arule, func_base_op_get_arule);
	this->funcs[static_cast<size_t>(command_type::drule)] = cmd_funcs(func_base_run_drule, func_base_op_get_drule);
	this->funcs[static_cast<size_t>(command_type::acmdex)] = cmd_funcs(func_base_run_acmdex, func_base_op_get_acmdex);
	this->funcs[static_cast<size_t>(command_type::scmdex)] = cmd_funcs(func_base_run_scmdex, func_base_op_get_scmdex);
	this->funcs[static_cast<size_t>(command_type::icmdex)] = cmd_funcs(func_base_run_icmdex, func_base_op_get_icmdex);
	this->funcs[static_cast<size_t>(command_type::dcmdex)] = cmd_funcs(func_base_run_dcmdex, func_base_op_get_dcmdex);
	this->funcs[static_cast<size_t>(command_type::gcmdex)] = cmd_funcs(func_base_run_gcmdex, func_base_op_get_gcmdex);
	this->funcs[static_cast<size_t>(command_type::gcmdnextex)] = cmd_funcs(func_base_run_gcmdnextex, func_base_op_get_gcmdnextex);
	this->funcs[static_cast<size_t>(command_type::gcmdlastex)] = cmd_funcs(func_base_run_gcmdlastex, func_base_op_get_gcmdlastex);
	this->funcs[static_cast<size_t>(command_type::gstack)] = cmd_funcs(func_base_run_gstack, func_base_op_get_gstack);
}
rule_manager::~rule_manager()
{

}
bool rule_manager::set_begin(rule_index index)
{
	if (test_validity_rule_index(index, true) == false)
		return false;

	this->begin = index;
	return true;
}
bool rule_manager::set_param(rule_index index, std::span<const var_single> param)
{
	std::optional<rule_unit*> rule_target_result = get_rule_unit_ptr(index);

	return rule_target_result.has_value() == false ? false : rule_target_result.value()->content.set_param(param);
}
std::optional<rule_index> rule_manager::add_rule()
{
	std::optional<rule_index> index_free = get_free();
	rule_index index_new = index_free.has_value() == false ? static_cast<rule_index>(this->rule_set.size()) : index_free.value();

	if (index_free.has_value() == false) this->rule_set.emplace_back(rule_unit(index_new));
	else this->rule_set[static_cast<size_t>(index_new)] = rule_unit(index_new);

	return std::make_optional<rule_index>(index_new);
}
bool rule_manager::del_rule(rule_index index)
{
	return add_free(index);
}
std::optional<rule*> rule_manager::get_rule(rule_index index)
{
	std::optional<rule_unit*> result = get_rule_unit_ptr(index);
	std::optional<rule*> target = result.has_value() == false ? std::nullopt : std::make_optional<rule*>(&result.value()->content);

	return target;
}
void rule_manager::set_add_command(size_t cmd_type, const cmd_funcs& funcs_new)
{
	this->funcs[cmd_type] = funcs_new;
	return;
}
bool rule_manager::run(std::vector<var_single>& result)
{
	result.clear();

	if (test_validity_rule_index(this->begin, true) == false)
		return false;

	std::optional<rule_unit*> rule_current_result = get_rule_unit_ptr(this->begin);

	if (rule_current_result.has_value() == false)
		return false;

	rule_unit* rule_current = rule_current_result.value();
	std::optional<command_index> rule_current_enter = rule_current->content.get_begin();

	if (rule_current_enter.has_value() == false)
		return false;

	rule_interrupt run_result;
	std::optional<rule_index> rule_next = std::nullopt;
	std::vector<var_single> param_send;
	std::string bebug_txt;
	bool is_need_pop_stack = false;

	do
	{
		if (is_need_pop_stack == true)
		{
			this->stack.pop_back();
			is_need_pop_stack = false;
		}

		rule_current->content.run(*this, run_result, rule_current_enter, result, this->funcs);

		switch (run_result.type)
		{
		case rule_interrupt_type::invalid:
		{
			get_interrupt_text(rule_current->index, run_result, bebug_txt);
			std::cout << bebug_txt;
			result.clear();
			return false;
			break;
		}
		case rule_interrupt_type::invalid_var:
		{
			get_interrupt_text(rule_current->index, run_result, bebug_txt);
			std::cout << bebug_txt;
			result.clear();
			return false;
			break;
		}
		case rule_interrupt_type::invalid_cmd:
		{
			get_interrupt_text(rule_current->index, run_result, bebug_txt);
			std::cout << bebug_txt;
			result.clear();
			return false;
			break;
		}
		case rule_interrupt_type::call:
		{
			if (run_result.value.has_value() == false)
				break;

			rule_interrupt_value_call& call_info = std::get<rule_interrupt_value_call>(run_result.value.value());
			std::optional<rule_unit*> rule_target_result = get_rule_unit_ptr(call_info.target);

			if (rule_target_result.has_value() == false)
				break;

			if (get_param(rule_current->index, call_info, param_send) == false)
				break;

			if (rule_target_result.value()->content.set_param(param_send) == false)
				break;

			this->stack.emplace_back(rule_manager_stack(rule_current->index, call_info.next, call_info.save));
			rule_next.emplace(call_info.target);
			break;
		}
		case rule_interrupt_type::end:
		{
			if (this->stack.empty() == false)
			{
				const rule_manager_stack& last = this->stack.back();
				std::optional<rule*> rule_return = get_rule(last.index);

				if (rule_return.has_value() == false)
					break;

				rule_return.value()->set_var_value(static_cast<operand>(last.save), result);
			}

			break;
		}
		default:
		{

			break;
		}
		}

		if (rule_next.has_value() == false)
		{
			if (this->stack.empty() == false)
			{
				rule_manager_stack& last = this->stack.back();

				rule_current_result = get_rule_unit_ptr(last.index);

				if (rule_current_result.has_value() == false)
				{
					result.clear();
					return false;
				}

				rule_current = rule_current_result.value();
				rule_current_enter.emplace(last.next);
				is_need_pop_stack = true;
			}
		}
		else
		{
			rule_current_result = get_rule_unit_ptr(rule_next.value());

			if (rule_current_result.has_value() == false)
			{
				result.clear();
				return false;
			}

			rule_current = rule_current_result.value();
			rule_current_enter = rule_current->content.get_begin();
			rule_next = std::nullopt;
		}

	} while (this->stack.empty() == false);

	return true;
}
