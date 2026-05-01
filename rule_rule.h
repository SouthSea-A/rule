#pragma once
#include <functional>
#include <string>
#include <unordered_map>
#include "rule_command.h"

using rule_index = data_type_unsigned_base;

constexpr rule_index RULE_INVALID_INDEX = 0xFFFFFFFFFFFFFFFFULL;

class rule_manager;

enum class rule_interrupt_type : data_type_unsigned_base
{
	end,
	call,

	invalid_var,
	invalid_cmd,

	invalid,
};

using var_save = operand;
using rule_interrupt_call = operand;
using rule_interrupt_call_type = operand_type;

struct rule_interrupt_value_call
{
	rule_index target;
	rule_interrupt_call param_set;
	var_save save;
	command_index next;
	rule_interrupt_value_call();
	rule_interrupt_value_call(rule_index target_, const rule_interrupt_call& param_set_, const rule_interrupt_call& save_, command_index next_);
	rule_interrupt_value_call(const rule_interrupt_value_call& rule_interrupt_value_call_);
	rule_interrupt_value_call(rule_interrupt_value_call&& rule_interrupt_value_call_) noexcept;
	rule_interrupt_value_call& operator=(const rule_interrupt_value_call& other);
};

struct rule_interrupt_value_invalid
{
	command_index index;
	operand op_error;
	rule_interrupt_value_invalid();
	rule_interrupt_value_invalid(command_index index_, const operand& op_error_);
	rule_interrupt_value_invalid(const rule_interrupt_value_invalid& rule_interrupt_value_invalid_);
	rule_interrupt_value_invalid(rule_interrupt_value_invalid&& rule_interrupt_value_invalid_) noexcept;
	rule_interrupt_value_invalid& operator=(const rule_interrupt_value_invalid& other);
};

using rule_interrupt_value = std::variant<rule_interrupt_value_call, rule_interrupt_value_invalid>;

struct rule_interrupt
{
	rule_interrupt_type type;
	std::optional<rule_interrupt_value> value;
	std::string info;
	rule_interrupt();
	rule_interrupt(rule_interrupt_type type_, const std::optional<rule_interrupt_value>& value_, const std::string& info_);
	rule_interrupt(const rule_interrupt& rule_interrupt_);
	rule_interrupt(rule_interrupt&& rule_interrupt_) noexcept;
	rule_interrupt& operator=(const rule_interrupt& other);
};

using cmd_run_func = std::function<void(rule&, rule_manager&, rule_interrupt&, command_index, command&, std::optional<command_index>&, std::vector<var_single>&, bool&)>;
using cmd_op_func = std::function<bool(const rule&, const operand&, command_operand&)>;

struct cmd_funcs
{
	cmd_run_func run;
	cmd_op_func op_get;
	cmd_funcs();
	cmd_funcs(const cmd_run_func& run_, const cmd_op_func& op_get_);
	cmd_funcs(const cmd_funcs& cmd_funs_);
	cmd_funcs(cmd_funcs&& cmd_funs_) noexcept;
	cmd_funcs& operator=(const cmd_funcs& other);
};

class rule
{
public:
	var_manager rule_var_param;
	var_manager rule_var_common;
	command_manager rule_command;
public:
	bool add_var_by_op_single(const operand& op);
	bool del_var_by_op_single(const operand& op);
	bool add_var_by_op(const command_operand& op);
	bool del_var_by_op(const command_operand& op);
	std::optional<var_single> get_var_value(const operand& op) const;
	bool get_var_value(const operand& op, size_t quan, std::vector<var_single>& save) const;
	bool set_var_value(const operand& op_src, var_single value, bool is_allow_dynamic_alloc = true);
	bool set_var_value(const operand& op_src, const std::vector<var_single>& value_all);
	bool get_op_by_op(const operand& op_begin, size_t quantity, std::vector<var_single>& save) const;
	bool get_op_by_var(var_single type, var_single content, operand& save, operand_type invalid_type = operand_type::invalid) const;
	bool get_op_by_cmd_op(command_type cmd_type, const operand& op, std::unordered_map<size_t, cmd_funcs>& cmd_set, command_operand& save) const;
	void get_info_by_cmd(const command& cmd_op, std::vector<var_single>& save) const;
protected:
	template<size_t index, typename op_set>
	void get_info_by_cmd_op_case_single(const op_set& op_set_, std::vector<var_single>& save) const
	{
		const operand& cmd_op = op_set_[static_cast<oi>(index)];
		std::optional<var_single> result = get_var_value(cmd_op);

		if (result.has_value() == true)
		{
			size_t base_idx = 1 + index * 2;
			save[base_idx] = static_cast<var_single>(static_cast<operand_type>(cmd_op.index()));
			save[base_idx + 1] = result.value();
		}
	}
	template<size_t index, typename op_set>
	void add_if_unique(const op_set& op_target, bool& add_result)
	{
		const operand& current = op_target[static_cast<oi>(index)];

		bool is_unique = [&]<size_t... index_place>(std::index_sequence<index_place...>)
		{
			return ((current != op_target[static_cast<oi>(index_place)]) && ...);
		}(std::make_index_sequence<index>{});

		if (is_unique == true)
			add_result = add_result && add_var_by_op_single(current);

		return;
	}
	template<size_t index, typename op_set>
	void del_if_unique(const op_set& op_target, bool& del_result)
	{
		const operand& current = op_target[static_cast<oi>(index)];

		bool is_unique = [&]<size_t... index_place>(std::index_sequence<index_place...>)
		{
			return ((current != op_target[static_cast<oi>(index_place)]) && ...);
		}(std::make_index_sequence<index>{});

		if (is_unique == true)
			del_result = del_result && del_var_by_op_single(current);

		return;
	}
public:
	rule();
	~rule();
public:
	bool set_param(const std::vector<var_single>& param);
	std::optional<size_t> get_param_quantity() const;
	std::optional<command_index> get_begin() const;
	std::optional<command_index> get_end() const;
	bool set_begin(command_index index);
	bool set_end(command_index index);
	std::optional<command> get_command(command_index index) const;
	std::optional<command_index> add_command(const command& command_new, command& ref_update, command_index index_update);
	std::optional<command_index> add_command(const command& command_new);
	template<typename... operand>
	std::optional<command_index> add_command(command_type type, operand&&... op)
	{
		return add_command(command(type, std::forward<operand>(op)...));
	}
	std::optional<command_index> insert_command(command_index index_insert, const command& command_new, command& ref_update, command_index index_update);
	std::optional<command_index> insert_command(command_index index_insert, const command& command_new);
	template<typename... operand>
	std::optional<command_index> insert_command(command_index index_insert, command_type type, operand&&... op)
	{
		return insert_command(index_insert, command(type, std::forward<operand>(op)...));
	}
	bool set_command(command_index index, const command& command_new);
	template<typename... operand>
	bool set_command(command_index index, command_type type, operand&&... op)
	{
		return set_command(index, command(type, std::forward<operand>(op)...));
	}
	bool del_command(command_index index, command& ref_update, command_index index_update);
	bool del_command(command_index index);
	void run(rule_manager& onwership, rule_interrupt& interrupt, const std::optional<command_index>& begin, std::vector<var_single>& result, std::unordered_map<size_t, cmd_funcs>& cmd_set);
public:
	rule& operator=(const rule& other);
};
