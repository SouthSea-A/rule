#include "rule.h"

cmd_run_func func_base_run_mov = [](
	rule& owner_rule,
	rule_manager& owner_rule_manager,
	rule_interrupt& interrupt,
	command_index cmd_index_current,
	command& cmd_current,
	std::optional<command_index>& cmd_index_next,
	std::vector<var_single>& result,
	bool& is_end
	)->void
	{
		cmd_index_next = std::nullopt;
		is_end = false;
		std::optional<var_single> content_src = owner_rule.get_var_value(std::get<operand_set<oq::two>>(cmd_current.op)[oi::second]);

		if (content_src.has_value() == false)
		{
			interrupt = rule_interrupt(
				rule_interrupt_type::invalid_var,
				std::make_optional<rule_interrupt_value_invalid>(
					cmd_index_current,
					std::get<operand_set<oq::two>>(cmd_current.op)[oi::second]
				),
				"右操作数无效，无法获取"
			);
			is_end = true;
			return;
		}

		if (owner_rule.set_var_value(std::get<operand_set<oq::two>>(cmd_current.op)[oi::first], content_src.value()) == false)
		{
			interrupt = rule_interrupt(
				rule_interrupt_type::invalid_var,
				std::make_optional<rule_interrupt_value_invalid>(
					cmd_index_current,
					std::get<operand_set<oq::two>>(cmd_current.op)[oi::first]
				),
				"左操作数无效，无法写入"
			);
			is_end = true;

		}

		return;
	};

cmd_run_func func_base_run_cmp = [](
	rule& owner_rule,
	rule_manager& owner_rule_manager,
	rule_interrupt& interrupt,
	command_index cmd_index_current,
	command& cmd_current,
	std::optional<command_index>& cmd_index_next,
	std::vector<var_single>& result,
	bool& is_end
	)->void
	{
		cmd_index_next = std::nullopt;
		is_end = false;
		std::optional<var_single> left = owner_rule.get_var_value(std::get<operand_set<oq::three>>(cmd_current.op)[oi::first]);

		if (left.has_value() == false)
		{
			interrupt = rule_interrupt(
				rule_interrupt_type::invalid_var,
				std::make_optional<rule_interrupt_value_invalid>(
					cmd_index_current,
					std::get<operand_set<oq::three>>(cmd_current.op)[oi::first])
				,
				"左比较操作数无效，无法获取"
			);
			is_end = true;
			return;
		}

		std::optional<var_single> right = owner_rule.get_var_value(std::get<operand_set<oq::three>>(cmd_current.op)[oi::second]);

		if (right.has_value() == false)
		{
			interrupt = rule_interrupt(
				rule_interrupt_type::invalid_var,
				std::make_optional<rule_interrupt_value_invalid>(
					cmd_index_current,
					std::get<operand_set<oq::three>>(cmd_current.op)[oi::second]
				),
				"右比较操作数无效，无法获取"
			);
			is_end = true;

			return;
		}

		compare_type result_cmp = compare_type::equal;

		if (left.value() == right.value()) result_cmp = compare_type::equal;
		if (left.value() > right.value()) result_cmp = compare_type::greater;
		if (left.value() < right.value()) result_cmp = compare_type::lower;

		if (owner_rule.set_var_value(std::get<operand_set<oq::three>>(cmd_current.op)[oi::third], static_cast<var_single>(result_cmp)) == false)
		{
			interrupt = rule_interrupt(
				rule_interrupt_type::invalid_var,
				std::make_optional<rule_interrupt_value_invalid>(
					cmd_index_current,
					std::get<operand_set<oq::three>>(cmd_current.op)[oi::third]
				),
				"比较结果保存操作数无效，无法写入"
			);
			is_end = true;
		}

		return;
	};

cmd_run_func func_base_run_jmp = [](
	rule& owner_rule,
	rule_manager& owner_rule_manager,
	rule_interrupt& interrupt,
	command_index cmd_index_current,
	command& cmd_current,
	std::optional<command_index>& cmd_index_next,
	std::vector<var_single>& result,
	bool& is_end
	)->void
	{
		cmd_index_next = std::nullopt;
		is_end = false;
		std::optional<var_single> command_index_target = owner_rule.get_var_value(std::get<operand_set<oq::one>>(cmd_current.op)[oi::first]);

		if (command_index_target.has_value() == false)
		{
			interrupt = rule_interrupt(
				rule_interrupt_type::invalid_var,
				std::make_optional<rule_interrupt_value_invalid>(
					cmd_index_current,
					std::get<operand_set<oq::one>>(cmd_current.op)[oi::first]
				),
				"跳转操作数无效，无法获取目的指令位置"
			);
			is_end = true;

			return;
		}

		command_index command_target = static_cast<command_index>(command_index_target.value());

		if (owner_rule.rule_command.test_validity_command_index(command_target, true) == false)
		{
			interrupt = rule_interrupt(
				rule_interrupt_type::invalid_cmd,
				std::make_optional<rule_interrupt_value_invalid>(
					cmd_index_current,
					immediate(static_cast<var_single>(command_target))
				),
				"跳转操作数无效，目标指令不存在"
			);
			is_end = true;

			return;
		}

		cmd_index_next.emplace(command_target);
		return;
	};

cmd_run_func func_base_run_je = [](
	rule& owner_rule,
	rule_manager& owner_rule_manager,
	rule_interrupt& interrupt,
	command_index cmd_index_current,
	command& cmd_current,
	std::optional<command_index>& cmd_index_next,
	std::vector<var_single>& result,
	bool& is_end
	)->void
	{
		cmd_index_next = std::nullopt;
		is_end = false;
		std::optional<var_single> compare_result = owner_rule.get_var_value(std::get<operand_set<oq::two>>(cmd_current.op)[oi::second]);

		if (compare_result.has_value() == false)
		{
			interrupt = rule_interrupt(
				rule_interrupt_type::invalid_var,
				std::make_optional<rule_interrupt_value_invalid>(
					cmd_index_current,
					std::get<operand_set<oq::two>>(cmd_current.op)[oi::second]
				),
				"比较结果保存操作数无效，无法获取"
			);
			is_end = true;

			return;
		}

		std::optional<var_single> command_index_target = owner_rule.get_var_value(std::get<operand_set<oq::two>>(cmd_current.op)[oi::first]);

		if (command_index_target.has_value() == false)
		{
			interrupt = rule_interrupt(
				rule_interrupt_type::invalid_var,
				std::make_optional<rule_interrupt_value_invalid>(
					cmd_index_current,
					std::get<operand_set<oq::two>>(cmd_current.op)[oi::first]
				),
				"跳转操作数无效，无法获取目的指令位置"
			);
			is_end = true;

			return;
		}

		command_index jump_target = static_cast<command_index>(command_index_target.value());

		if (owner_rule.rule_command.test_validity_command_index(jump_target, true) == false)
		{
			interrupt = rule_interrupt(
				rule_interrupt_type::invalid_cmd,
				std::make_optional<rule_interrupt_value_invalid>(
					cmd_index_current,
					immediate(static_cast<var_single>(jump_target))
				),
				"跳转操作数无效，目标指令不存在"
			);
			is_end = true;

			return;
		}

		if (static_cast<compare_type>(compare_result.value()) == compare_type::equal)
			cmd_index_next.emplace(jump_target);

		return;
	};

cmd_run_func func_base_run_jne = [](
	rule& owner_rule,
	rule_manager& owner_rule_manager,
	rule_interrupt& interrupt,
	command_index cmd_index_current,
	command& cmd_current,
	std::optional<command_index>& cmd_index_next,
	std::vector<var_single>& result,
	bool& is_end
	)->void
	{
		cmd_index_next = std::nullopt;
		is_end = false;
		std::optional<var_single> compare_result = owner_rule.get_var_value(std::get<operand_set<oq::two>>(cmd_current.op)[oi::second]);

		if (compare_result.has_value() == false)
		{
			interrupt = rule_interrupt(
				rule_interrupt_type::invalid_var,
				std::make_optional<rule_interrupt_value_invalid>(
					cmd_index_current,
					std::get<operand_set<oq::two>>(cmd_current.op)[oi::second]
				),
				"比较结果保存操作数无效，无法获取"
			);
			is_end = true;

			return;
		}

		std::optional<var_single> command_index_target = owner_rule.get_var_value(std::get<operand_set<oq::two>>(cmd_current.op)[oi::first]);

		if (command_index_target.has_value() == false)
		{
			interrupt = rule_interrupt(
				rule_interrupt_type::invalid_var,
				std::make_optional<rule_interrupt_value_invalid>(
					cmd_index_current,
					std::get<operand_set<oq::two>>(cmd_current.op)[oi::first]
				),
				"跳转操作数无效，无法获取目的指令位置"
			);
			is_end = true;

			return;
		}

		command_index jump_target = static_cast<command_index>(command_index_target.value());

		if (owner_rule.rule_command.test_validity_command_index(jump_target, true) == false)
		{
			interrupt = rule_interrupt(
				rule_interrupt_type::invalid_cmd,
				std::make_optional<rule_interrupt_value_invalid>(
					cmd_index_current,
					immediate(static_cast<var_single>(jump_target))
				),
				"跳转操作数无效，目标指令不存在"
			);
			is_end = true;

			return;
		}

		if (static_cast<compare_type>(compare_result.value()) != compare_type::equal)
			cmd_index_next.emplace(jump_target);

		return;
	};

cmd_run_func func_base_run_ja = [](
	rule& owner_rule,
	rule_manager& owner_rule_manager,
	rule_interrupt& interrupt,
	command_index cmd_index_current,
	command& cmd_current,
	std::optional<command_index>& cmd_index_next,
	std::vector<var_single>& result,
	bool& is_end
	)->void
	{
		cmd_index_next = std::nullopt;
		is_end = false;
		std::optional<var_single> compare_result = owner_rule.get_var_value(std::get<operand_set<oq::two>>(cmd_current.op)[oi::second]);

		if (compare_result.has_value() == false)
		{
			interrupt = rule_interrupt(
				rule_interrupt_type::invalid_var,
				std::make_optional<rule_interrupt_value_invalid>(
					cmd_index_current,
					std::get<operand_set<oq::two>>(cmd_current.op)[oi::second]
				),
				"比较结果保存操作数无效，无法获取"
			);
			is_end = true;

			return;
		}

		std::optional<var_single> command_index_target = owner_rule.get_var_value(std::get<operand_set<oq::two>>(cmd_current.op)[oi::first]);

		if (command_index_target.has_value() == false)
		{
			interrupt = rule_interrupt(
				rule_interrupt_type::invalid_var,
				std::make_optional<rule_interrupt_value_invalid>(
					cmd_index_current,
					std::get<operand_set<oq::two>>(cmd_current.op)[oi::first]
				),
				"跳转操作数无效，无法获取目的指令位置"
			);
			is_end = true;

			return;
		}

		command_index jump_target = static_cast<command_index>(command_index_target.value());

		if (owner_rule.rule_command.test_validity_command_index(jump_target, true) == false)
		{
			interrupt = rule_interrupt(
				rule_interrupt_type::invalid_cmd,
				std::make_optional<rule_interrupt_value_invalid>(
					cmd_index_current,
					immediate(static_cast<var_single>(jump_target))
				),
				"跳转操作数无效，目标指令不存在"
			);
			is_end = true;

			return;
		}

		if (static_cast<compare_type>(compare_result.value()) == compare_type::greater)
			cmd_index_next.emplace(jump_target);

		return;
	};

cmd_run_func func_base_run_jae = [](
	rule& owner_rule,
	rule_manager& owner_rule_manager,
	rule_interrupt& interrupt,
	command_index cmd_index_current,
	command& cmd_current,
	std::optional<command_index>& cmd_index_next,
	std::vector<var_single>& result,
	bool& is_end
	)->void
	{
		cmd_index_next = std::nullopt;
		is_end = false;
		std::optional<var_single> compare_result = owner_rule.get_var_value(std::get<operand_set<oq::two>>(cmd_current.op)[oi::second]);

		if (compare_result.has_value() == false)
		{
			interrupt = rule_interrupt(
				rule_interrupt_type::invalid_var,
				std::make_optional<rule_interrupt_value_invalid>(
					cmd_index_current,
					std::get<operand_set<oq::two>>(cmd_current.op)[oi::second]
				),
				"比较结果保存操作数无效，无法获取"
			);
			is_end = true;

			return;
		}

		std::optional<var_single> command_index_target = owner_rule.get_var_value(std::get<operand_set<oq::two>>(cmd_current.op)[oi::first]);

		if (command_index_target.has_value() == false)
		{
			interrupt = rule_interrupt(
				rule_interrupt_type::invalid_var,
				std::make_optional<rule_interrupt_value_invalid>(
					cmd_index_current,
					std::get<operand_set<oq::two>>(cmd_current.op)[oi::first]
				),
				"跳转操作数无效，无法获取目的指令位置"
			);
			is_end = true;

			return;
		}

		command_index jump_target = static_cast<command_index>(command_index_target.value());

		if (owner_rule.rule_command.test_validity_command_index(jump_target, true) == false)
		{
			interrupt = rule_interrupt(
				rule_interrupt_type::invalid_cmd,
				std::make_optional<rule_interrupt_value_invalid>(
					cmd_index_current,
					immediate(static_cast<var_single>(jump_target))
				),
				"跳转操作数无效，目标指令不存在"
			);
			is_end = true;

			return;
		}

		compare_type compare_result_type = static_cast<compare_type>(compare_result.value());

		if (compare_result_type == compare_type::greater || compare_result_type == compare_type::equal)
			cmd_index_next.emplace(jump_target);

		return;
	};

cmd_run_func func_base_run_jb = [](
	rule& owner_rule,
	rule_manager& owner_rule_manager,
	rule_interrupt& interrupt,
	command_index cmd_index_current,
	command& cmd_current,
	std::optional<command_index>& cmd_index_next,
	std::vector<var_single>& result,
	bool& is_end
	)->void
	{
		cmd_index_next = std::nullopt;
		is_end = false;
		std::optional<var_single> compare_result = owner_rule.get_var_value(std::get<operand_set<oq::two>>(cmd_current.op)[oi::second]);

		if (compare_result.has_value() == false)
		{
			interrupt = rule_interrupt(
				rule_interrupt_type::invalid_var,
				std::make_optional<rule_interrupt_value_invalid>(
					cmd_index_current,
					std::get<operand_set<oq::two>>(cmd_current.op)[oi::second]
				),
				"比较结果保存操作数无效，无法获取"
			);
			is_end = true;

			return;
		}

		std::optional<var_single> command_index_target = owner_rule.get_var_value(std::get<operand_set<oq::two>>(cmd_current.op)[oi::first]);

		if (command_index_target.has_value() == false)
		{
			interrupt = rule_interrupt(
				rule_interrupt_type::invalid_var,
				std::make_optional<rule_interrupt_value_invalid>(
					cmd_index_current,
					std::get<operand_set<oq::two>>(cmd_current.op)[oi::first]
				),
				"跳转操作数无效，无法获取目的指令位置"
			);
			is_end = true;

			return;
		}

		command_index jump_target = static_cast<command_index>(command_index_target.value());

		if (owner_rule.rule_command.test_validity_command_index(jump_target, true) == false)
		{
			interrupt = rule_interrupt(
				rule_interrupt_type::invalid_cmd,
				std::make_optional<rule_interrupt_value_invalid>(
					cmd_index_current,
					immediate(static_cast<var_single>(jump_target))
				),
				"跳转操作数无效，目标指令不存在"
			);
			is_end = true;

			return;
		}

		if (static_cast<compare_type>(compare_result.value()) == compare_type::lower)
			cmd_index_next.emplace(jump_target);

		return;
	};

cmd_run_func func_base_run_jbe = [](
	rule& owner_rule,
	rule_manager& owner_rule_manager,
	rule_interrupt& interrupt,
	command_index cmd_index_current,
	command& cmd_current,
	std::optional<command_index>& cmd_index_next,
	std::vector<var_single>& result,
	bool& is_end
	)->void
	{
		cmd_index_next = std::nullopt;
		is_end = false;
		std::optional<var_single> compare_result = owner_rule.get_var_value(std::get<operand_set<oq::two>>(cmd_current.op)[oi::second]);

		if (compare_result.has_value() == false)
		{
			interrupt = rule_interrupt(
				rule_interrupt_type::invalid_var,
				std::make_optional<rule_interrupt_value_invalid>(
					cmd_index_current,
					std::get<operand_set<oq::two>>(cmd_current.op)[oi::second]
				),
				"比较结果保存操作数无效，无法获取"
			);
			is_end = true;

			return;
		}

		std::optional<var_single> command_index_target = owner_rule.get_var_value(std::get<operand_set<oq::two>>(cmd_current.op)[oi::first]);

		if (command_index_target.has_value() == false)
		{
			interrupt = rule_interrupt(
				rule_interrupt_type::invalid_var,
				std::make_optional<rule_interrupt_value_invalid>(
					cmd_index_current,
					std::get<operand_set<oq::two>>(cmd_current.op)[oi::first]
				),
				"跳转操作数无效，无法获取目的指令位置"
			);
			is_end = true;

			return;
		}

		command_index jump_target = static_cast<command_index>(command_index_target.value());

		if (owner_rule.rule_command.test_validity_command_index(jump_target, true) == false)
		{
			interrupt = rule_interrupt(
				rule_interrupt_type::invalid_cmd,
				std::make_optional<rule_interrupt_value_invalid>(
					cmd_index_current,
					immediate(static_cast<var_single>(jump_target))
				),
				"跳转操作数无效，目标指令不存在"
			);
			is_end = true;

			return;
		}

		compare_type compare_result_type = static_cast<compare_type>(compare_result.value());

		if (compare_result_type == compare_type::lower || compare_result_type == compare_type::equal)
			cmd_index_next.emplace(jump_target);

		return;
	};

cmd_run_func func_base_run_call = [](
	rule& owner_rule,
	rule_manager& owner_rule_manager,
	rule_interrupt& interrupt,
	command_index cmd_index_current,
	command& cmd_current,
	std::optional<command_index>& cmd_index_next,
	std::vector<var_single>& result,
	bool& is_end
	)->void
	{
		cmd_index_next = std::nullopt;
		is_end = false;
		std::optional<var_single> rule_index_target = owner_rule.get_var_value(std::get<operand_set<oq::three>>(cmd_current.op)[oi::first]);

		if (rule_index_target.has_value() == false)
		{
			interrupt = rule_interrupt(
				rule_interrupt_type::invalid_var,
				std::make_optional<rule_interrupt_value_invalid>(
					cmd_index_current,
					std::get<operand_set<oq::three>>(cmd_current.op)[oi::first]
				),
				"调用跳转操作数无效，无法获取目的指令位置"
			);
			is_end = true;

			return;
		}

		rule_index call_target = static_cast<rule_index>(rule_index_target.value());

		if (owner_rule_manager.test_validity_rule_index(call_target, true) == false)
		{
			interrupt = rule_interrupt(
				rule_interrupt_type::invalid_cmd,
				std::make_optional<rule_interrupt_value_invalid>(
					cmd_index_current,
					immediate(static_cast<var_single>(call_target))
				),
				"调用跳转操作数无效，目标规则不存在"
			);
			is_end = true;

			return;
		}

		std::optional<var_single> command_index_next_result = owner_rule.rule_command.get_command_index_next(cmd_index_current);

		command_index command_index_next = command_index_next_result.has_value() == false ? COMMAND_INVALID_INDEX : static_cast<command_index>(command_index_next_result.value());

		interrupt = rule_interrupt(
			rule_interrupt_type::call,
			std::make_optional<rule_interrupt_value>(
				rule_interrupt_value_call(
					call_target,
					std::get<operand_set<oq::three>>(cmd_current.op)[oi::second],
					std::get<operand_set<oq::three>>(cmd_current.op)[oi::third],
					command_index_next
				)),
			"规则调用返回"
		);

		is_end = true;
		return;
	};

cmd_run_func func_base_run_add = [](
	rule& owner_rule,
	rule_manager& owner_rule_manager,
	rule_interrupt& interrupt,
	command_index cmd_index_current,
	command& cmd_current,
	std::optional<command_index>& cmd_index_next,
	std::vector<var_single>& result,
	bool& is_end
	)->void
	{
		cmd_index_next = std::nullopt;
		is_end = false;
		std::optional<var_single> left = owner_rule.get_var_value(std::get<operand_set<oq::three>>(cmd_current.op)[oi::first]);

		if (left.has_value() == false)
		{
			interrupt = rule_interrupt(
				rule_interrupt_type::invalid_var,
				std::make_optional<rule_interrupt_value_invalid>(
					cmd_index_current,
					std::get<operand_set<oq::three>>(cmd_current.op)[oi::first]
				),
				"运算左操作数无效，无法获取"
			);
			is_end = true;

			return;
		}


		std::optional<var_single> right = owner_rule.get_var_value(std::get<operand_set<oq::three>>(cmd_current.op)[oi::second]);

		if (right.has_value() == false)
		{
			interrupt = rule_interrupt(
				rule_interrupt_type::invalid_var,
				std::make_optional<rule_interrupt_value_invalid>(
					cmd_index_current,
					std::get<operand_set<oq::three>>(cmd_current.op)[oi::second]
				),
				"运算右操作数无效，无法获取"
			);
			is_end = true;

			return;
		}

		var_single calculate_result = left.value() + right.value();

		if (owner_rule.set_var_value(std::get<operand_set<oq::three>>(cmd_current.op)[oi::third], calculate_result) == false)
		{
			interrupt = rule_interrupt(
				rule_interrupt_type::invalid_var,
				std::make_optional<rule_interrupt_value_invalid>(
					cmd_index_current,
					std::get<operand_set<oq::three>>(cmd_current.op)[oi::third]
				),
				"运算结果保存操作数无效，无法写入"
			);
			is_end = true;

			return;
		}

		return;
	};

cmd_run_func func_base_run_sub = [](
	rule& owner_rule,
	rule_manager& owner_rule_manager,
	rule_interrupt& interrupt,
	command_index cmd_index_current,
	command& cmd_current,
	std::optional<command_index>& cmd_index_next,
	std::vector<var_single>& result,
	bool& is_end
	)->void
	{
		cmd_index_next = std::nullopt;
		is_end = false;
		std::optional<var_single> left = owner_rule.get_var_value(std::get<operand_set<oq::three>>(cmd_current.op)[oi::first]);

		if (left.has_value() == false)
		{
			interrupt = rule_interrupt(
				rule_interrupt_type::invalid_var,
				std::make_optional<rule_interrupt_value_invalid>(
					cmd_index_current,
					std::get<operand_set<oq::three>>(cmd_current.op)[oi::first]
				),
				"运算左操作数无效，无法获取"
			);
			is_end = true;

			return;
		}


		std::optional<var_single> right = owner_rule.get_var_value(std::get<operand_set<oq::three>>(cmd_current.op)[oi::second]);

		if (right.has_value() == false)
		{
			interrupt = rule_interrupt(
				rule_interrupt_type::invalid_var,
				std::make_optional<rule_interrupt_value_invalid>(
					cmd_index_current,
					std::get<operand_set<oq::three>>(cmd_current.op)[oi::second]
				),
				"运算右操作数无效，无法获取"
			);
			is_end = true;

			return;
		}

		var_single calculate_result = left.value() - right.value();

		if (owner_rule.set_var_value(std::get<operand_set<oq::three>>(cmd_current.op)[oi::third], calculate_result) == false)
		{
			interrupt = rule_interrupt(
				rule_interrupt_type::invalid_var,
				std::make_optional<rule_interrupt_value_invalid>(
					cmd_index_current,
					std::get<operand_set<oq::three>>(cmd_current.op)[oi::third]
				),
				"运算结果保存操作数无效，无法写入"
			);
			is_end = true;

			return;
		}

		return;
	};

cmd_run_func func_base_run_mul = [](
	rule& owner_rule,
	rule_manager& owner_rule_manager,
	rule_interrupt& interrupt,
	command_index cmd_index_current,
	command& cmd_current,
	std::optional<command_index>& cmd_index_next,
	std::vector<var_single>& result,
	bool& is_end
	)->void
	{
		cmd_index_next = std::nullopt;
		is_end = false;
		std::optional<var_single> left = owner_rule.get_var_value(std::get<operand_set<oq::three>>(cmd_current.op)[oi::first]);

		if (left.has_value() == false)
		{
			interrupt = rule_interrupt(
				rule_interrupt_type::invalid_var,
				std::make_optional<rule_interrupt_value_invalid>(
					cmd_index_current,
					std::get<operand_set<oq::three>>(cmd_current.op)[oi::first]
				),
				"运算左操作数无效，无法获取"
			);
			is_end = true;

			return;
		}


		std::optional<var_single> right = owner_rule.get_var_value(std::get<operand_set<oq::three>>(cmd_current.op)[oi::second]);

		if (right.has_value() == false)
		{
			interrupt = rule_interrupt(
				rule_interrupt_type::invalid_var,
				std::make_optional<rule_interrupt_value_invalid>(
					cmd_index_current,
					std::get<operand_set<oq::three>>(cmd_current.op)[oi::second]
				),
				"运算右操作数无效，无法获取"
			);
			is_end = true;

			return;
		}

		var_single calculate_result = left.value() * right.value();

		if (owner_rule.set_var_value(std::get<operand_set<oq::three>>(cmd_current.op)[oi::third], calculate_result) == false)
		{
			interrupt = rule_interrupt(
				rule_interrupt_type::invalid_var,
				std::make_optional<rule_interrupt_value_invalid>(
					cmd_index_current,
					std::get<operand_set<oq::three>>(cmd_current.op)[oi::third]
				),
				"运算结果保存操作数无效，无法写入"
			);
			is_end = true;

			return;
		}

		return;
	};

cmd_run_func func_base_run_div = [](
	rule& owner_rule,
	rule_manager& owner_rule_manager,
	rule_interrupt& interrupt,
	command_index cmd_index_current,
	command& cmd_current,
	std::optional<command_index>& cmd_index_next,
	std::vector<var_single>& result,
	bool& is_end
	)->void
	{
		cmd_index_next = std::nullopt;
		is_end = false;
		std::optional<var_single> left = owner_rule.get_var_value(std::get<operand_set<oq::three>>(cmd_current.op)[oi::first]);

		if (left.has_value() == false)
		{
			interrupt = rule_interrupt(
				rule_interrupt_type::invalid_var,
				std::make_optional<rule_interrupt_value_invalid>(
					cmd_index_current,
					std::get<operand_set<oq::three>>(cmd_current.op)[oi::first]
				),
				"运算左操作数无效，无法获取"
			);
			is_end = true;

			return;
		}

		std::optional<var_single> right = owner_rule.get_var_value(std::get<operand_set<oq::three>>(cmd_current.op)[oi::second]);

		if (right.has_value() == false)
		{
			interrupt = rule_interrupt(
				rule_interrupt_type::invalid_var,
				std::make_optional<rule_interrupt_value_invalid>(
					cmd_index_current,
					std::get<operand_set<oq::three>>(cmd_current.op)[oi::second]
				),
				"运算右操作数无效，无法获取"
			);
			is_end = true;

			return;
		}

		if (right.value() == 0)
		{
			interrupt = rule_interrupt(
				rule_interrupt_type::invalid_var,
				std::make_optional<rule_interrupt_value_invalid>(
					cmd_index_current,
					immediate(static_cast<var_single>(cmd_index_current))
				),
				"除法的右操作数不能为0"
			);
			is_end = true;
			return;
		}

		var_single calculate_result = left.value() / right.value();

		if (owner_rule.set_var_value(std::get<operand_set<oq::three>>(cmd_current.op)[oi::third], calculate_result) == false)
		{
			interrupt = rule_interrupt(
				rule_interrupt_type::invalid_var,
				std::make_optional<rule_interrupt_value_invalid>(
					cmd_index_current,
					std::get<operand_set<oq::three>>(cmd_current.op)[oi::third]
				),
				"运算结果保存操作数无效，无法写入"
			);
			is_end = true;

			return;
		}

		return;
	};

cmd_run_func func_base_run_ret = [](
	rule& owner_rule,
	rule_manager& owner_rule_manager,
	rule_interrupt& interrupt,
	command_index cmd_index_current,
	command& cmd_current,
	std::optional<command_index>& cmd_index_next,
	std::vector<var_single>& result,
	bool& is_end
	)->void
	{
		cmd_index_next = std::nullopt;
		is_end = false;

		const operand& var_return_begin = std::get<operand_set<oq::two>>(cmd_current.op)[oi::first];
		const operand& var_return_quan = std::get<operand_set<oq::two>>(cmd_current.op)[oi::second];
		std::optional<var_single> var_quan = owner_rule.get_var_value(var_return_quan);

		if (var_quan.has_value() == false)
		{
			interrupt = rule_interrupt(
				rule_interrupt_type::invalid_var,
				std::make_optional<rule_interrupt_value_invalid>(
					cmd_index_current,
					var_return_quan
				),
				"返回数量操作数无效，无法获取"
			);
			is_end = true;

			return;
		}

		if (owner_rule.get_var_value(var_return_begin, static_cast<size_t>(var_quan.value()), result) == false)
		{
			interrupt = rule_interrupt(
				rule_interrupt_type::invalid_var,
				std::make_optional<rule_interrupt_value_invalid>(
					cmd_index_current,
					var_return_begin
				),
				"返回起始变量操作数无效，无法完整获取"
			);
			is_end = true;

			return;
		}

		is_end = true;
		return;
	};

cmd_run_func func_base_run_acmd = [](
	rule& owner_rule,
	rule_manager& owner_rule_manager,
	rule_interrupt& interrupt,
	command_index cmd_index_current,
	command& cmd_current,
	std::optional<command_index>& cmd_index_next,
	std::vector<var_single>& result,
	bool& is_end
	)->void
	{
		cmd_index_next = std::nullopt;
		is_end = false;
		std::optional<var_single> cmd_type = owner_rule.get_var_value(std::get<operand_set<oq::three>>(cmd_current.op)[oi::first]);

		if (cmd_type.has_value() == false)
		{
			interrupt = rule_interrupt(
				rule_interrupt_type::invalid_var,
				std::make_optional<rule_interrupt_value_invalid>(
					cmd_index_current,
					std::get<operand_set<oq::three>>(cmd_current.op)[oi::first]
				),
				"指令添加类型操作数无效，无法获取"
			);
			is_end = true;

			return;
		}

		command_operand op_new;
		command_type cmd_type_new = static_cast<command_type>(cmd_type.value());
		const operand& op_get = std::get<operand_set<oq::three>>(cmd_current.op)[oi::second];

		if (owner_rule.get_op_by_cmd_op(cmd_type_new, op_get, owner_rule_manager.funcs, op_new) == false)
		{
			interrupt = rule_interrupt(
				rule_interrupt_type::invalid_var,
				std::make_optional<rule_interrupt_value_invalid>(
					cmd_index_current,
					op_get
				),
				"指令添加的指定操作数无效，无法获取"
			);
			is_end = true;

			return;
		}

		std::optional<command_index> cmd_index_new = owner_rule.add_command(
			command(cmd_type_new, static_cast<const command_operand&>(op_new)), 
			cmd_current,
			cmd_index_current
			);

		if (cmd_index_new.has_value() == false)
		{
			interrupt = rule_interrupt(
				rule_interrupt_type::invalid_var,
				std::make_optional<rule_interrupt_value_invalid>(
					cmd_index_current,
					op_get
				),
				"指令添加失败"
			);
			is_end = true;

			return;
		}

		const operand& index_save = std::get<operand_set<oq::three>>(cmd_current.op)[oi::third];
		command_index idx = cmd_index_new.value();

		if (owner_rule.set_var_value(index_save, static_cast<var_single>(idx)) == false)
		{
			owner_rule.del_command(idx);

			interrupt = rule_interrupt(
				rule_interrupt_type::invalid_var,
				std::make_optional<rule_interrupt_value_invalid>(
					cmd_index_current,
					index_save
				),
				"指令添加的保存操作数无效，无法写入"
			);
			is_end = true;

			return;
		}

		return;
	};

cmd_run_func func_base_run_scmd = [](
	rule& owner_rule,
	rule_manager& owner_rule_manager,
	rule_interrupt& interrupt,
	command_index cmd_index_current,
	command& cmd_current,
	std::optional<command_index>& cmd_index_next,
	std::vector<var_single>& result,
	bool& is_end
	)->void
	{
		cmd_index_next = std::nullopt;
		is_end = false;

		std::optional<var_single> target_set = owner_rule.get_var_value(std::get<operand_set<oq::three>>(cmd_current.op)[oi::first]);

		if (target_set.has_value() == false)
		{
			interrupt = rule_interrupt(
				rule_interrupt_type::invalid_var,
				std::make_optional<rule_interrupt_value_invalid>(
					cmd_index_current,
					std::get<operand_set<oq::three>>(cmd_current.op)[oi::first]
				),
				"指令修改目标操作数无效，无法获取"
			);
			is_end = true;

			return;
		}

		std::optional<var_single> cmd_type = owner_rule.get_var_value(std::get<operand_set<oq::three>>(cmd_current.op)[oi::second]);

		if (cmd_type.has_value() == false)
		{
			interrupt = rule_interrupt(
				rule_interrupt_type::invalid_var,
				std::make_optional<rule_interrupt_value_invalid>(
					cmd_index_current,
					std::get<operand_set<oq::three>>(cmd_current.op)[oi::second]
				),
				"指令修改类型操作数无效，无法获取"
			);
			is_end = true;

			return;
		}

		command_operand op_new;
		command_type cmd_type_new = static_cast<command_type>(cmd_type.value());
		const operand& op_get = std::get<operand_set<oq::three>>(cmd_current.op)[oi::third];

		if (owner_rule.get_op_by_cmd_op(cmd_type_new, op_get, owner_rule_manager.funcs, op_new) == false)
		{
			interrupt = rule_interrupt(
				rule_interrupt_type::invalid_var,
				std::make_optional<rule_interrupt_value_invalid>(
					cmd_index_current,
					op_get
				),
				"指令修改的指定操作数无效，无法获取"
			);
			is_end = true;

			return;
		}

		if (owner_rule.set_command(static_cast<command_index>(target_set.value()), command(cmd_type_new, static_cast<const command_operand&>(op_new))) == false)
		{
			interrupt = rule_interrupt(
				rule_interrupt_type::invalid_var,
				std::make_optional<rule_interrupt_value_invalid>(
					cmd_index_current,
					op_get
				),
				"指令修改失败"
			);
			is_end = true;

			return;
		}

		return;
	};

cmd_run_func func_base_run_icmd = [](
	rule& owner_rule,
	rule_manager& owner_rule_manager,
	rule_interrupt& interrupt,
	command_index cmd_index_current,
	command& cmd_current,
	std::optional<command_index>& cmd_index_next,
	std::vector<var_single>& result,
	bool& is_end
	)->void
	{
		cmd_index_next = std::nullopt;
		is_end = false;

		std::optional<var_single> target_set = owner_rule.get_var_value(std::get<operand_set<oq::four>>(cmd_current.op)[oi::first]);

		if (target_set.has_value() == false)
		{
			interrupt = rule_interrupt(
				rule_interrupt_type::invalid_var,
				std::make_optional<rule_interrupt_value_invalid>(
					cmd_index_current,
					std::get<operand_set<oq::four>>(cmd_current.op)[oi::first]
				),
				"指令插入目标操作数无效，无法获取"
			);
			is_end = true;

			return;
		}

		std::optional<var_single> cmd_type = owner_rule.get_var_value(std::get<operand_set<oq::four>>(cmd_current.op)[oi::second]);

		if (cmd_type.has_value() == false)
		{
			interrupt = rule_interrupt(
				rule_interrupt_type::invalid_var,
				std::make_optional<rule_interrupt_value_invalid>(
					cmd_index_current,
					std::get<operand_set<oq::four>>(cmd_current.op)[oi::second]
				),
				"指令插入类型操作数无效，无法获取"
			);
			is_end = true;

			return;
		}

		command_operand op_new;
		command_type cmd_type_new = static_cast<command_type>(cmd_type.value());
		const operand& op_get = std::get<operand_set<oq::four>>(cmd_current.op)[oi::third];

		if (owner_rule.get_op_by_cmd_op(cmd_type_new, op_get, owner_rule_manager.funcs, op_new) == false)
		{
			interrupt = rule_interrupt(
				rule_interrupt_type::invalid_var,
				std::make_optional<rule_interrupt_value_invalid>(
					cmd_index_current,
					op_get
				),
				"指令插入的指定操作数无效，无法获取"
			);
			is_end = true;

			return;
		}

		std::optional<command_index> command_new_index = owner_rule.insert_command(
			static_cast<command_index>(target_set.value()), 
			command(cmd_type_new, static_cast<const command_operand&>(op_new)),
			cmd_current,
			cmd_index_current
		);

		if (command_new_index.has_value() == false)
		{
			interrupt = rule_interrupt(
				rule_interrupt_type::invalid_var,
				std::make_optional<rule_interrupt_value_invalid>(
					cmd_index_current,
					op_get
				),
				"指令插入失败"
			);
			is_end = true;

			return;
		}

		const operand& index_save = std::get<operand_set<oq::four>>(cmd_current.op)[oi::fourth];
		command_index idx = command_new_index.value();

		if (owner_rule.set_var_value(index_save, static_cast<var_single>(idx)) == false)
		{
			owner_rule.del_command(idx);

			interrupt = rule_interrupt(
				rule_interrupt_type::invalid_var,
				std::make_optional<rule_interrupt_value_invalid>(
					cmd_index_current,
					index_save
				),
				"指令插入的保存操作数无效，无法写入"
			);
			is_end = true;

			return;
		}


		return;
	};

cmd_run_func func_base_run_dcmd = [](
	rule& owner_rule,
	rule_manager& owner_rule_manager,
	rule_interrupt& interrupt,
	command_index cmd_index_current,
	command& cmd_current,
	std::optional<command_index>& cmd_index_next,
	std::vector<var_single>& result,
	bool& is_end
	)->void
	{
		cmd_index_next = std::nullopt;
		is_end = false;

		operand op_del = std::get<operand_set<oq::one>>(cmd_current.op)[oi::first];
		std::optional<var_single> target_del = owner_rule.get_var_value(op_del);

		if (target_del.has_value() == false)
		{
			interrupt = rule_interrupt(
				rule_interrupt_type::invalid_var,
				std::make_optional<rule_interrupt_value_invalid>(
					cmd_index_current,
					op_del
				),
				"指令删除目标操作数无效，无法获取"
			);
			is_end = true;

			return;
		}

		if (owner_rule.del_command(static_cast<command_index>(target_del.value())) == false)
		{
			interrupt = rule_interrupt(
				rule_interrupt_type::invalid_var,
				std::nullopt,
				"指令删除失败"
			);
			is_end = true;

			return;
		}

		return;
	};

cmd_run_func func_base_run_gcmd = [](
	rule& owner_rule,
	rule_manager& owner_rule_manager,
	rule_interrupt& interrupt,
	command_index cmd_index_current,
	command& cmd_current,
	std::optional<command_index>& cmd_index_next,
	std::vector<var_single>& result,
	bool& is_end
	)->void
	{
		cmd_index_next = std::nullopt;
		is_end = false;

		const operand& op_target = std::get<operand_set<oq::two>>(cmd_current.op)[oi::first];
		std::optional<var_single> index_target = owner_rule.get_var_value(op_target);

		if (index_target.has_value() == false)
		{
			interrupt = rule_interrupt(
				rule_interrupt_type::invalid_var,
				std::make_optional<rule_interrupt_value_invalid>(
					cmd_index_current,
					op_target
				),
				"指令查询目标操作数无效，无法获取"
			);
			is_end = true;

			return;
		}

		std::optional<command*> cmd_target = owner_rule.rule_command.get_command_ptr(static_cast<command_index>(index_target.value()));

		if (cmd_target.has_value() == false)
		{
			interrupt = rule_interrupt(
				rule_interrupt_type::invalid_var,
				std::make_optional<rule_interrupt_value_invalid>(
					cmd_index_current,
					op_target
				),
				"指令查询目标无效，无法获取，指令不存在"
			);
			is_end = true;

			return;
		}

		std::vector<var_single> cmd_param_write;

		owner_rule.get_info_by_cmd(*cmd_target.value(), cmd_param_write);

		const operand& op_save = std::get<operand_set<oq::two>>(cmd_current.op)[oi::second];

		if (owner_rule.set_var_value(op_save, cmd_param_write) == false)
		{
			interrupt = rule_interrupt(
				rule_interrupt_type::invalid_var,
				std::make_optional<rule_interrupt_value_invalid>(
					cmd_index_current,
					op_save
				),
				"指令查询保存操作数无效，无法写入"
			);
			is_end = true;

			return;
		}

		return;
	};

cmd_run_func func_base_run_gcmdnext = [](
	rule& owner_rule,
	rule_manager& owner_rule_manager,
	rule_interrupt& interrupt,
	command_index cmd_index_current,
	command& cmd_current,
	std::optional<command_index>& cmd_index_next,
	std::vector<var_single>& result,
	bool& is_end
	)->void
	{
		cmd_index_next = std::nullopt;
		is_end = false;

		const operand& op_target = std::get<operand_set<oq::three>>(cmd_current.op)[oi::first];
		std::optional<var_single> index_target = owner_rule.get_var_value(op_target);

		if (index_target.has_value() == false)
		{
			interrupt = rule_interrupt(
				rule_interrupt_type::invalid_var,
				std::make_optional<rule_interrupt_value_invalid>(
					cmd_index_current,
					op_target
				),
				"指令查询目标操作数无效，无法获取"
			);
			is_end = true;

			return;
		}

		command_index command_index_this = static_cast<command_index>(index_target.value());
		std::optional<command*> cmd_target = owner_rule.rule_command.get_command_ptr_next(command_index_this);
		std::optional<command_index> cmd_index_target = owner_rule.rule_command.get_command_index_next(command_index_this);

		if (cmd_target.has_value() == false || cmd_index_target.has_value() == false)
		{
			interrupt = rule_interrupt(
				rule_interrupt_type::invalid_var,
				std::make_optional<rule_interrupt_value_invalid>(
					cmd_index_current,
					op_target
				),
				"指令查询目标无效，无法获取，指令不存在"
			);
			is_end = true;

			return;
		}

		const operand& cmd_index_save = std::get<operand_set<oq::three>>(cmd_current.op)[oi::second];

		if (owner_rule.set_var_value(cmd_index_save, cmd_index_target.value()) == false)
		{
			interrupt = rule_interrupt(
				rule_interrupt_type::invalid_var,
				std::make_optional<rule_interrupt_value_invalid>(
					cmd_index_current,
					cmd_index_save
				),
				"指令编号查询保存操作数无效，无法写入"
			);
			is_end = true;

			return;
		}

		std::vector<var_single> cmd_param_write;

		owner_rule.get_info_by_cmd(*cmd_target.value(), cmd_param_write);

		const operand& op_save = std::get<operand_set<oq::three>>(cmd_current.op)[oi::third];

		if (owner_rule.set_var_value(op_save, cmd_param_write) == false)
		{
			interrupt = rule_interrupt(
				rule_interrupt_type::invalid_var,
				std::make_optional<rule_interrupt_value_invalid>(
					cmd_index_current,
					op_save
				),
				"指令查询保存操作数无效，无法写入"
			);
			is_end = true;

			return;
		}

		return;
	};

cmd_run_func func_base_run_gcmdlast = [](
	rule& owner_rule,
	rule_manager& owner_rule_manager,
	rule_interrupt& interrupt,
	command_index cmd_index_current,
	command& cmd_current,
	std::optional<command_index>& cmd_index_next,
	std::vector<var_single>& result,
	bool& is_end
	)->void
	{
		cmd_index_next = std::nullopt;
		is_end = false;

		const operand& op_target = std::get<operand_set<oq::three>>(cmd_current.op)[oi::first];
		std::optional<var_single> index_target = owner_rule.get_var_value(op_target);

		if (index_target.has_value() == false)
		{
			interrupt = rule_interrupt(
				rule_interrupt_type::invalid_var,
				std::make_optional<rule_interrupt_value_invalid>(
					cmd_index_current,
					op_target
				),
				"指令查询目标操作数无效，无法获取"
			);
			is_end = true;

			return;
		}

		command_index command_index_this = static_cast<command_index>(index_target.value());
		std::optional<command*> cmd_target = owner_rule.rule_command.get_command_ptr_last(command_index_this);
		std::optional<command_index> cmd_index_target = owner_rule.rule_command.get_command_index_last(command_index_this);

		if (cmd_target.has_value() == false || cmd_index_target.has_value() == false)
		{
			interrupt = rule_interrupt(
				rule_interrupt_type::invalid_var,
				std::make_optional<rule_interrupt_value_invalid>(
					cmd_index_current,
					op_target
				),
				"指令查询目标无效，无法获取，指令不存在"
			);
			is_end = true;

			return;
		}

		const operand& cmd_index_save = std::get<operand_set<oq::three>>(cmd_current.op)[oi::second];

		if (owner_rule.set_var_value(cmd_index_save, cmd_index_target.value()) == false)
		{
			interrupt = rule_interrupt(
				rule_interrupt_type::invalid_var,
				std::make_optional<rule_interrupt_value_invalid>(
					cmd_index_current,
					cmd_index_save
				),
				"指令编号查询保存操作数无效，无法写入"
			);
			is_end = true;

			return;
		}

		std::vector<var_single> cmd_param_write;

		owner_rule.get_info_by_cmd(*cmd_target.value(), cmd_param_write);

		const operand& op_save = std::get<operand_set<oq::three>>(cmd_current.op)[oi::third];

		if (owner_rule.set_var_value(op_save, cmd_param_write) == false)
		{
			interrupt = rule_interrupt(
				rule_interrupt_type::invalid_var,
				std::make_optional<rule_interrupt_value_invalid>(
					cmd_index_current,
					op_save
				),
				"指令查询保存操作数无效，无法写入"
			);
			is_end = true;

			return;
		}

		return;
	};


cmd_run_func func_base_run_arule = [](
	rule& owner_rule,
	rule_manager& owner_rule_manager,
	rule_interrupt& interrupt,
	command_index cmd_index_current,
	command& cmd_current,
	std::optional<command_index>& cmd_index_next,
	std::vector<var_single>& result,
	bool& is_end
	)->void
	{
		cmd_index_next = std::nullopt;
		is_end = false;

		std::optional<rule_index> rule_new = owner_rule_manager.add_rule();

		if (rule_new.has_value() == false)
		{
			interrupt = rule_interrupt(
				rule_interrupt_type::invalid,
				std::nullopt,
				"规则添加失败"
			);
			is_end = true;

			return;
		}

		const operand& op_save = std::get<operand_set<oq::one>>(cmd_current.op)[oi::first];

		if (owner_rule.set_var_value(op_save, static_cast<var_single>(rule_new.value())) == false)
		{
			interrupt = rule_interrupt(
				rule_interrupt_type::invalid_var,
				std::make_optional<rule_interrupt_value_invalid>(
					cmd_index_current,
					op_save
				),
				"规则添加编号保存操作数无效，无法写入"
			);
			is_end = true;

			return;
		}

		return;
	};

cmd_run_func func_base_run_drule = [](
	rule& owner_rule,
	rule_manager& owner_rule_manager,
	rule_interrupt& interrupt,
	command_index cmd_index_current,
	command& cmd_current,
	std::optional<command_index>& cmd_index_next,
	std::vector<var_single>& result,
	bool& is_end
	)->void
	{
		cmd_index_next = std::nullopt;
		is_end = false;

		const operand& op_del = std::get<operand_set<oq::one>>(cmd_current.op)[oi::first];
		std::optional<var_single> rule_target = owner_rule.get_var_value(op_del);

		if (rule_target.has_value() == false)
		{
			interrupt = rule_interrupt(
				rule_interrupt_type::invalid_var,
				std::make_optional<rule_interrupt_value_invalid>(
					cmd_index_current,
					op_del
				),
				"规则删除编号操作数无效，无法获取"
			);
			is_end = true;

			return;
		}

		if (owner_rule_manager.del_rule(static_cast<rule_index>(rule_target.value())) == false)
		{
			interrupt = rule_interrupt(
				rule_interrupt_type::invalid_var,
				std::nullopt,
				"规则失败，规则不存在"
			);
			is_end = true;

			return;
		}

		return;
	};

cmd_run_func func_base_run_acmdex = [](
	rule& owner_rule,
	rule_manager& owner_rule_manager,
	rule_interrupt& interrupt,
	command_index cmd_index_current,
	command& cmd_current,
	std::optional<command_index>& cmd_index_next,
	std::vector<var_single>& result,
	bool& is_end
	)->void
	{
		cmd_index_next = std::nullopt;
		is_end = false;

		const operand& op_rule_target = std::get<operand_set<oq::four>>(cmd_current.op)[oi::first];
		std::optional<var_single> rule_target = owner_rule.get_var_value(op_rule_target);

		if (rule_target.has_value() == false)
		{
			interrupt = rule_interrupt(
				rule_interrupt_type::invalid_var,
				std::make_optional<rule_interrupt_value_invalid>(
					cmd_index_current,
					op_rule_target
				),
				"指令添加目标规则操作数无效，无法获取"
			);
			is_end = true;

			return;
		}

		std::optional<rule*> rule_add_target = owner_rule_manager.get_rule(static_cast<rule_index>(rule_target.value()));

		if (rule_add_target.has_value() == false)
		{
			interrupt = rule_interrupt(
				rule_interrupt_type::invalid_var,
				std::make_optional<rule_interrupt_value_invalid>(
					cmd_index_current,
					op_rule_target
				),
				"指令添加目标规则无效，规则不存在"
			);
			is_end = true;

			return;
		}
	
		std::optional<var_single> cmd_type = owner_rule.get_var_value(std::get<operand_set<oq::four>>(cmd_current.op)[oi::second]);

		if (cmd_type.has_value() == false)
		{
			interrupt = rule_interrupt(
				rule_interrupt_type::invalid_var,
				std::make_optional<rule_interrupt_value_invalid>(
					cmd_index_current,
					std::get<operand_set<oq::four>>(cmd_current.op)[oi::second]
				),
				"指令添加类型操作数无效，无法获取"
			);
			is_end = true;

			return;
		}

		command_operand op_new;
		command_type cmd_type_new = static_cast<command_type>(cmd_type.value());
		const operand& op_get = std::get<operand_set<oq::four>>(cmd_current.op)[oi::third];

		if (owner_rule.get_op_by_cmd_op(cmd_type_new, op_get, owner_rule_manager.funcs, op_new) == false)
		{
			interrupt = rule_interrupt(
				rule_interrupt_type::invalid_var,
				std::make_optional<rule_interrupt_value_invalid>(
					cmd_index_current,
					op_get
				),
				"指令添加的指定操作数无效，无法获取"
			);
			is_end = true;

			return;
		}

		std::optional<command_index> cmd_index_new = rule_add_target.value()->add_command(
			command(cmd_type_new, static_cast<const command_operand&>(op_new)),
			cmd_current,
			cmd_index_current
		);

		if (cmd_index_new.has_value() == false)
		{
			interrupt = rule_interrupt(
				rule_interrupt_type::invalid_var,
				std::make_optional<rule_interrupt_value_invalid>(
					cmd_index_current,
					op_get
				),
				"指令添加失败"
			);
			is_end = true;

			return;
		}

		const operand& index_save = std::get<operand_set<oq::four>>(cmd_current.op)[oi::fourth];
		command_index idx = cmd_index_new.value();

		if (owner_rule.set_var_value(index_save, static_cast<var_single>(idx)) == false)
		{
			owner_rule.del_command(idx);

			interrupt = rule_interrupt(
				rule_interrupt_type::invalid_var,
				std::make_optional<rule_interrupt_value_invalid>(
					cmd_index_current,
					index_save
				),
				"指令添加的保存操作数无效，无法写入"
			);
			is_end = true;

			return;
		}

		return;
	};

cmd_run_func func_base_run_scmdex = [](
	rule& owner_rule,
	rule_manager& owner_rule_manager,
	rule_interrupt& interrupt,
	command_index cmd_index_current,
	command& cmd_current,
	std::optional<command_index>& cmd_index_next,
	std::vector<var_single>& result,
	bool& is_end
	)->void
	{
		cmd_index_next = std::nullopt;
		is_end = false;

		const operand& op_rule_target = std::get<operand_set<oq::four>>(cmd_current.op)[oi::first];
		std::optional<var_single> rule_target = owner_rule.get_var_value(op_rule_target);

		if (rule_target.has_value() == false)
		{
			interrupt = rule_interrupt(
				rule_interrupt_type::invalid_var,
				std::make_optional<rule_interrupt_value_invalid>(
					cmd_index_current,
					op_rule_target
				),
				"指令修改目标规则操作数无效，无法获取"
			);
			is_end = true;

			return;
		}

		std::optional<rule*> rule_set_target = owner_rule_manager.get_rule(static_cast<rule_index>(rule_target.value()));

		if (rule_set_target.has_value() == false)
		{
			interrupt = rule_interrupt(
				rule_interrupt_type::invalid_var,
				std::make_optional<rule_interrupt_value_invalid>(
					cmd_index_current,
					op_rule_target
				),
				"指令修改目标规则无效，规则不存在"
			);
			is_end = true;

			return;
		}

		const operand& op_cmd_target = std::get<operand_set<oq::four>>(cmd_current.op)[oi::second];
		std::optional<var_single> cmd_target = owner_rule.get_var_value(op_cmd_target);

		if (rule_target.has_value() == false)
		{
			interrupt = rule_interrupt(
				rule_interrupt_type::invalid_var,
				std::make_optional<rule_interrupt_value_invalid>(
					cmd_index_current,
					op_rule_target
				),
				"指令修改目标指令操作数无效，无法获取"
			);
			is_end = true;

			return;
		}

		std::optional<var_single> cmd_type = owner_rule.get_var_value(std::get<operand_set<oq::four>>(cmd_current.op)[oi::third]);

		if (cmd_type.has_value() == false)
		{
			interrupt = rule_interrupt(
				rule_interrupt_type::invalid_var,
				std::make_optional<rule_interrupt_value_invalid>(
					cmd_index_current,
					std::get<operand_set<oq::four>>(cmd_current.op)[oi::third]
				),
				"指令修改类型操作数无效，无法获取"
			);
			is_end = true;

			return;
		}

		command_operand op_new;
		command_type cmd_type_new = static_cast<command_type>(cmd_type.value());
		const operand& op_get = std::get<operand_set<oq::four>>(cmd_current.op)[oi::third];

		if (owner_rule.get_op_by_cmd_op(cmd_type_new, op_get, owner_rule_manager.funcs, op_new) == false)
		{
			interrupt = rule_interrupt(
				rule_interrupt_type::invalid_var,
				std::make_optional<rule_interrupt_value_invalid>(
					cmd_index_current,
					op_get
				),
				"指令修改的指定操作数无效，无法获取"
			);
			is_end = true;

			return;
		}

		if (rule_set_target.value()->set_command(static_cast<command_index>(cmd_target.value()), command(cmd_type_new, static_cast<const command_operand&>(op_new))) == false)
		{
			interrupt = rule_interrupt(
				rule_interrupt_type::invalid_var,
				std::make_optional<rule_interrupt_value_invalid>(
					cmd_index_current,
					op_get
				),
				"指令修改失败"
			);
			is_end = true;

			return;
		}

		return;
	};

cmd_run_func func_base_run_icmdex = [](
	rule& owner_rule,
	rule_manager& owner_rule_manager,
	rule_interrupt& interrupt,
	command_index cmd_index_current,
	command& cmd_current,
	std::optional<command_index>& cmd_index_next,
	std::vector<var_single>& result,
	bool& is_end
	)->void
	{
		cmd_index_next = std::nullopt;
		is_end = false;

		const operand& op_rule_target = std::get<operand_set<oq::five>>(cmd_current.op)[oi::first];
		std::optional<var_single> rule_target = owner_rule.get_var_value(op_rule_target);

		if (rule_target.has_value() == false)
		{
			interrupt = rule_interrupt(
				rule_interrupt_type::invalid_var,
				std::make_optional<rule_interrupt_value_invalid>(
					cmd_index_current,
					op_rule_target
				),
				"指令插入目标规则操作数无效，无法获取"
			);
			is_end = true;

			return;
		}

		std::optional<rule*> rule_insert_target = owner_rule_manager.get_rule(static_cast<rule_index>(rule_target.value()));

		if (rule_insert_target.has_value() == false)
		{
			interrupt = rule_interrupt(
				rule_interrupt_type::invalid_var,
				std::make_optional<rule_interrupt_value_invalid>(
					cmd_index_current,
					op_rule_target
				),
				"指令插入目标规则无效，规则不存在"
			);
			is_end = true;

			return;
		}

		const operand& op_cmd_target = std::get<operand_set<oq::five>>(cmd_current.op)[oi::second];
		std::optional<var_single> cmd_target = owner_rule.get_var_value(op_cmd_target);

		if (rule_target.has_value() == false)
		{
			interrupt = rule_interrupt(
				rule_interrupt_type::invalid_var,
				std::make_optional<rule_interrupt_value_invalid>(
					cmd_index_current,
					op_rule_target
				),
				"指令插入目标指令操作数无效，无法获取"
			);
			is_end = true;

			return;
		}

		std::optional<var_single> cmd_type = owner_rule.get_var_value(std::get<operand_set<oq::five>>(cmd_current.op)[oi::third]);

		if (cmd_type.has_value() == false)
		{
			interrupt = rule_interrupt(
				rule_interrupt_type::invalid_var,
				std::make_optional<rule_interrupt_value_invalid>(
					cmd_index_current,
					std::get<operand_set<oq::five>>(cmd_current.op)[oi::third]
				),
				"指令插入类型操作数无效，无法获取"
			);
			is_end = true;

			return;
		}

		command_operand op_new;
		command_type cmd_type_new = static_cast<command_type>(cmd_type.value());
		const operand& op_get = std::get<operand_set<oq::five>>(cmd_current.op)[oi::fourth];

		if (owner_rule.get_op_by_cmd_op(cmd_type_new, op_get, owner_rule_manager.funcs, op_new) == false)
		{
			interrupt = rule_interrupt(
				rule_interrupt_type::invalid_var,
				std::make_optional<rule_interrupt_value_invalid>(
					cmd_index_current,
					op_get
				),
				"指令插入的指定操作数无效，无法获取"
			);
			is_end = true;

			return;
		}

		std::optional<command_index> command_new_index = rule_insert_target.value()->insert_command(
			static_cast<command_index>(cmd_target.value()),
			command(cmd_type_new, static_cast<const command_operand&>(op_new)),
			cmd_current,
			cmd_index_current
		);

		if (command_new_index.has_value() == false)
		{
			interrupt = rule_interrupt(
				rule_interrupt_type::invalid_var,
				std::make_optional<rule_interrupt_value_invalid>(
					cmd_index_current,
					op_get
				),
				"指令插入失败"
			);
			is_end = true;

			return;
		}

		const operand& index_save = std::get<operand_set<oq::five>>(cmd_current.op)[oi::fifth];
		command_index idx = command_new_index.value();

		if (owner_rule.set_var_value(index_save, static_cast<var_single>(idx)) == false)
		{
			owner_rule.del_command(idx);

			interrupt = rule_interrupt(
				rule_interrupt_type::invalid_var,
				std::make_optional<rule_interrupt_value_invalid>(
					cmd_index_current,
					index_save
				),
				"指令插入的保存操作数无效，无法写入"
			);
			is_end = true;

			return;
		}

		return;
	};

cmd_run_func func_base_run_dcmdex = [](
	rule& owner_rule,
	rule_manager& owner_rule_manager,
	rule_interrupt& interrupt,
	command_index cmd_index_current,
	command& cmd_current,
	std::optional<command_index>& cmd_index_next,
	std::vector<var_single>& result,
	bool& is_end
	)->void
	{
		cmd_index_next = std::nullopt;
		is_end = false;

		const operand& op_rule_target = std::get<operand_set<oq::two>>(cmd_current.op)[oi::first];
		std::optional<var_single> rule_target = owner_rule.get_var_value(op_rule_target);

		if (rule_target.has_value() == false)
		{
			interrupt = rule_interrupt(
				rule_interrupt_type::invalid_var,
				std::make_optional<rule_interrupt_value_invalid>(
					cmd_index_current,
					op_rule_target
				),
				"指令删除目标规则操作数无效，无法获取"
			);
			is_end = true;

			return;
		}

		std::optional<rule*> rule_del_target = owner_rule_manager.get_rule(static_cast<rule_index>(rule_target.value()));

		if (rule_del_target.has_value() == false)
		{
			interrupt = rule_interrupt(
				rule_interrupt_type::invalid_var,
				std::make_optional<rule_interrupt_value_invalid>(
					cmd_index_current,
					op_rule_target
				),
				"指令删除目标规则无效，规则不存在"
			);
			is_end = true;

			return;
		}

		const operand& op_cmd_target = std::get<operand_set<oq::two>>(cmd_current.op)[oi::second];
		std::optional<var_single> cmd_target = owner_rule.get_var_value(op_cmd_target);

		if (rule_target.has_value() == false)
		{
			interrupt = rule_interrupt(
				rule_interrupt_type::invalid_var,
				std::make_optional<rule_interrupt_value_invalid>(
					cmd_index_current,
					op_rule_target
				),
				"指令删除目标指令操作数无效，无法获取"
			);
			is_end = true;

			return;
		}

		if (rule_del_target.value()->del_command(static_cast<command_index>(cmd_target.value())) == false)
		{
			interrupt = rule_interrupt(
				rule_interrupt_type::invalid,
				std::nullopt,
				"指令删除失败"
			);
			is_end = true;

			return;
		}

		return;
	};

cmd_run_func func_base_run_gcmdex = [](
	rule& owner_rule,
	rule_manager& owner_rule_manager,
	rule_interrupt& interrupt,
	command_index cmd_index_current,
	command& cmd_current,
	std::optional<command_index>& cmd_index_next,
	std::vector<var_single>& result,
	bool& is_end
	)->void
	{
		cmd_index_next = std::nullopt;
		is_end = false;

		const operand& op_rule_target = std::get<operand_set<oq::three>>(cmd_current.op)[oi::first];
		std::optional<var_single> rule_target = owner_rule.get_var_value(op_rule_target);

		if (rule_target.has_value() == false)
		{
			interrupt = rule_interrupt(
				rule_interrupt_type::invalid_var,
				std::make_optional<rule_interrupt_value_invalid>(
					cmd_index_current,
					op_rule_target
				),
				"指令查询目标规则操作数无效，无法获取"
			);
			is_end = true;

			return;
		}

		std::optional<rule*> rule_get_target = owner_rule_manager.get_rule(static_cast<rule_index>(rule_target.value()));

		if (rule_get_target.has_value() == false)
		{
			interrupt = rule_interrupt(
				rule_interrupt_type::invalid_var,
				std::make_optional<rule_interrupt_value_invalid>(
					cmd_index_current,
					op_rule_target
				),
				"指令查询目标规则无效，规则不存在"
			);
			is_end = true;

			return;
		}

		const operand& op_target = std::get<operand_set<oq::three>>(cmd_current.op)[oi::second];
		std::optional<var_single> index_target = owner_rule.get_var_value(op_target);

		if (index_target.has_value() == false)
		{
			interrupt = rule_interrupt(
				rule_interrupt_type::invalid_var,
				std::make_optional<rule_interrupt_value_invalid>(
					cmd_index_current,
					op_target
				),
				"指令查询目标指令操作数无效，无法获取"
			);
			is_end = true;

			return;
		}

		std::optional<command*> cmd_target = rule_get_target.value()->rule_command.get_command_ptr(static_cast<command_index>(index_target.value()));

		if (cmd_target.has_value() == false)
		{
			interrupt = rule_interrupt(
				rule_interrupt_type::invalid_var,
				std::make_optional<rule_interrupt_value_invalid>(
					cmd_index_current,
					op_target
				),
				"指令查询目标指令无效，无法获取，指令不存在"
			);
			is_end = true;

			return;
		}

		std::vector<var_single> cmd_param_write;

		rule_get_target.value()->get_info_by_cmd(*cmd_target.value(), cmd_param_write);

		const operand& op_save = std::get<operand_set<oq::three>>(cmd_current.op)[oi::third];

		if (owner_rule.set_var_value(op_save, cmd_param_write) == false)
		{
			interrupt = rule_interrupt(
				rule_interrupt_type::invalid_var,
				std::make_optional<rule_interrupt_value_invalid>(
					cmd_index_current,
					op_save
				),
				"指令查询保存操作数无效，无法写入"
			);
			is_end = true;

			return;
		}

		return;
	};

cmd_run_func func_base_run_gcmdnextex = [](
	rule& owner_rule,
	rule_manager& owner_rule_manager,
	rule_interrupt& interrupt,
	command_index cmd_index_current,
	command& cmd_current,
	std::optional<command_index>& cmd_index_next,
	std::vector<var_single>& result,
	bool& is_end
	)->void
	{
		cmd_index_next = std::nullopt;
		is_end = false;

		const operand& op_rule_target = std::get<operand_set<oq::four>>(cmd_current.op)[oi::first];
		std::optional<var_single> rule_target = owner_rule.get_var_value(op_rule_target);

		if (rule_target.has_value() == false)
		{
			interrupt = rule_interrupt(
				rule_interrupt_type::invalid_var,
				std::make_optional<rule_interrupt_value_invalid>(
					cmd_index_current,
					op_rule_target
				),
				"指令查询目标规则操作数无效，无法获取"
			);
			is_end = true;

			return;
		}

		std::optional<rule*> rule_get_target = owner_rule_manager.get_rule(static_cast<rule_index>(rule_target.value()));

		if (rule_get_target.has_value() == false)
		{
			interrupt = rule_interrupt(
				rule_interrupt_type::invalid_var,
				std::make_optional<rule_interrupt_value_invalid>(
					cmd_index_current,
					op_rule_target
				),
				"指令查询目标规则无效，规则不存在"
			);
			is_end = true;

			return;
		}

		const operand& op_target = std::get<operand_set<oq::four>>(cmd_current.op)[oi::second];
		std::optional<var_single> index_target = owner_rule.get_var_value(op_target);

		if (index_target.has_value() == false)
		{
			interrupt = rule_interrupt(
				rule_interrupt_type::invalid_var,
				std::make_optional<rule_interrupt_value_invalid>(
					cmd_index_current,
					op_target
				),
				"指令查询目标操作数无效，无法获取"
			);
			is_end = true;

			return;
		}

		command_index command_index_this = static_cast<command_index>(index_target.value());
		std::optional<command*> cmd_target = rule_get_target.value()->rule_command.get_command_ptr_next(command_index_this);
		std::optional<command_index> cmd_index_target = rule_get_target.value()->rule_command.get_command_index_next(command_index_this);

		if (cmd_target.has_value() == false || cmd_index_target.has_value() == false)
		{
			interrupt = rule_interrupt(
				rule_interrupt_type::invalid_var,
				std::make_optional<rule_interrupt_value_invalid>(
					cmd_index_current,
					op_target
				),
				"指令查询目标无效，无法获取，指令不存在"
			);
			is_end = true;

			return;
		}

		const operand& cmd_index_save = std::get<operand_set<oq::four>>(cmd_current.op)[oi::third];

		if (owner_rule.set_var_value(cmd_index_save, cmd_index_target.value()) == false)
		{
			interrupt = rule_interrupt(
				rule_interrupt_type::invalid_var,
				std::make_optional<rule_interrupt_value_invalid>(
					cmd_index_current,
					cmd_index_save
				),
				"指令编号查询保存操作数无效，无法写入"
			);
			is_end = true;

			return;
		}

		std::vector<var_single> cmd_param_write;

		rule_get_target.value()->get_info_by_cmd(*cmd_target.value(), cmd_param_write);

		const operand& op_save = std::get<operand_set<oq::four>>(cmd_current.op)[oi::fourth];

		if (owner_rule.set_var_value(op_save, cmd_param_write) == false)
		{
			interrupt = rule_interrupt(
				rule_interrupt_type::invalid_var,
				std::make_optional<rule_interrupt_value_invalid>(
					cmd_index_current,
					op_save
				),
				"指令查询保存操作数无效，无法写入"
			);
			is_end = true;

			return;
		}

		return;
	};

cmd_run_func func_base_run_gcmdlastex = [](
	rule& owner_rule,
	rule_manager& owner_rule_manager,
	rule_interrupt& interrupt,
	command_index cmd_index_current,
	command& cmd_current,
	std::optional<command_index>& cmd_index_next,
	std::vector<var_single>& result,
	bool& is_end
	)->void
	{
		cmd_index_next = std::nullopt;
		is_end = false;

		const operand& op_rule_target = std::get<operand_set<oq::four>>(cmd_current.op)[oi::first];
		std::optional<var_single> rule_target = owner_rule.get_var_value(op_rule_target);

		if (rule_target.has_value() == false)
		{
			interrupt = rule_interrupt(
				rule_interrupt_type::invalid_var,
				std::make_optional<rule_interrupt_value_invalid>(
					cmd_index_current,
					op_rule_target
				),
				"指令查询目标规则操作数无效，无法获取"
			);
			is_end = true;

			return;
		}

		std::optional<rule*> rule_get_target = owner_rule_manager.get_rule(static_cast<rule_index>(rule_target.value()));

		if (rule_get_target.has_value() == false)
		{
			interrupt = rule_interrupt(
				rule_interrupt_type::invalid_var,
				std::make_optional<rule_interrupt_value_invalid>(
					cmd_index_current,
					op_rule_target
				),
				"指令查询目标规则无效，规则不存在"
			);
			is_end = true;

			return;
		}

		const operand& op_target = std::get<operand_set<oq::four>>(cmd_current.op)[oi::second];
		std::optional<var_single> index_target = owner_rule.get_var_value(op_target);

		if (index_target.has_value() == false)
		{
			interrupt = rule_interrupt(
				rule_interrupt_type::invalid_var,
				std::make_optional<rule_interrupt_value_invalid>(
					cmd_index_current,
					op_target
				),
				"指令查询目标操作数无效，无法获取"
			);
			is_end = true;

			return;
		}

		command_index command_index_this = static_cast<command_index>(index_target.value());
		std::optional<command*> cmd_target = rule_get_target.value()->rule_command.get_command_ptr_last(command_index_this);
		std::optional<command_index> cmd_index_target = rule_get_target.value()->rule_command.get_command_index_last(command_index_this);

		if (cmd_target.has_value() == false || cmd_index_target.has_value() == false)
		{
			interrupt = rule_interrupt(
				rule_interrupt_type::invalid_var,
				std::make_optional<rule_interrupt_value_invalid>(
					cmd_index_current,
					op_target
				),
				"指令查询目标无效，无法获取，指令不存在"
			);
			is_end = true;

			return;
		}

		const operand& cmd_index_save = std::get<operand_set<oq::four>>(cmd_current.op)[oi::third];

		if (owner_rule.set_var_value(cmd_index_save, cmd_index_target.value()) == false)
		{
			interrupt = rule_interrupt(
				rule_interrupt_type::invalid_var,
				std::make_optional<rule_interrupt_value_invalid>(
					cmd_index_current,
					cmd_index_save
				),
				"指令编号查询保存操作数无效，无法写入"
			);
			is_end = true;

			return;
		}

		std::vector<var_single> cmd_param_write;

		rule_get_target.value()->get_info_by_cmd(*cmd_target.value(), cmd_param_write);

		const operand& op_save = std::get<operand_set<oq::four>>(cmd_current.op)[oi::fourth];

		if (owner_rule.set_var_value(op_save, cmd_param_write) == false)
		{
			interrupt = rule_interrupt(
				rule_interrupt_type::invalid_var,
				std::make_optional<rule_interrupt_value_invalid>(
					cmd_index_current,
					op_save
				),
				"指令查询保存操作数无效，无法写入"
			);
			is_end = true;

			return;
		}

		return;
	};

cmd_run_func func_base_run_gstack = [](
	rule& owner_rule,
	rule_manager& owner_rule_manager,
	rule_interrupt& interrupt,
	command_index cmd_index_current,
	command& cmd_current,
	std::optional<command_index>& cmd_index_next,
	std::vector<var_single>& result,
	bool& is_end
	)->void
	{
		cmd_index_next = std::nullopt;
		is_end = false;

		const operand& op_get_quan = std::get<operand_set<oq::two>>(cmd_current.op)[oi::first];
		std::optional<var_single> rule_target = owner_rule.get_var_value(op_get_quan);

		if (rule_target.has_value() == false)
		{
			interrupt = rule_interrupt(
				rule_interrupt_type::invalid_var,
				std::make_optional<rule_interrupt_value_invalid>(
					cmd_index_current,
					op_get_quan
				),
				"调用栈查询数量操作数无效，无法获取"
			);
			is_end = true;

			return;
		}

		size_t quan = static_cast<size_t>(rule_target.value());

		if (quan > owner_rule_manager.stack.size())
		{
			interrupt = rule_interrupt(
				rule_interrupt_type::invalid,
                std::nullopt,
				"调用栈查询数量超过当前上限，无法获取"
			);
			is_end = true;

			return;
		}

		std::vector<var_single> stack_info_write;
		std::vector<rule_manager_stack>::const_iterator it = owner_rule_manager.stack.end();

		stack_info_write.assign(quan * 2, static_cast<var_single>(RULE_INVALID_INDEX));

		for (size_t index = 0; index < quan; index += 2)
		{
			rule_index rule_index_last = (it - index - 1)->index;

			std::optional<rule*> rule_last = owner_rule_manager.get_rule(rule_index_last);

			if (rule_last.has_value() == false)
			{
				interrupt = rule_interrupt(
					rule_interrupt_type::invalid,
					std::nullopt,
					"调用栈查询规则失败"
				);
				is_end = true;

				return;
			}

			std::optional<command_index> cmd_index_last = rule_last.value()->rule_command.get_command_index_last(rule_index_last);

			if (cmd_index_last.has_value() == false)
			{
				interrupt = rule_interrupt(
					rule_interrupt_type::invalid,
					std::nullopt,
					"调用栈查询指令失败"
				);
				is_end = true;

				return;
			}


			stack_info_write[index] = static_cast<var_single>(rule_index_last);
			stack_info_write[index + 1] = static_cast<var_single>(cmd_index_last.value());
		}

		const operand& op_save = std::get<operand_set<oq::two>>(cmd_current.op)[oi::second];

		if (owner_rule.set_var_value(op_save, stack_info_write) == false)
		{
			interrupt = rule_interrupt(
				rule_interrupt_type::invalid_var,
				std::make_optional<rule_interrupt_value_invalid>(
					cmd_index_current,
					op_save
				),
				"调用栈查询保存操作数无效，无法写入"
			);
			is_end = true;

			return;
		}

		return;
	};
