#pragma once
#include <iostream>
#include <format>
#include "rule_rule.h"

struct rule_unit
{
	rule_index index;
	rule content;
	rule_unit();
	rule_unit(rule_index index_, const rule& content_);
	rule_unit(rule_index index_);
	rule_unit(const rule_unit& rule_unit_);
	rule_unit(rule_unit&& rule_unit_) noexcept;
	rule_unit& operator=(const rule_unit& other);
	~rule_unit();
};

struct rule_manager_stack
{
	rule_index index;
	command_index next;
	var_save save;
	rule_manager_stack();
	rule_manager_stack(rule_index index_, command_index next_, const var_save& save_);
	rule_manager_stack(const rule_manager_stack& rule_manager_stack_);
	rule_manager_stack(rule_manager_stack&& rule_manager_stack_) noexcept;
};

extern cmd_run_func func_base_run_mov;
extern cmd_run_func func_base_run_cmp;
extern cmd_run_func func_base_run_jmp;
extern cmd_run_func func_base_run_je;
extern cmd_run_func func_base_run_jne;
extern cmd_run_func func_base_run_ja;
extern cmd_run_func func_base_run_jae;
extern cmd_run_func func_base_run_jb;
extern cmd_run_func func_base_run_jbe;
extern cmd_run_func func_base_run_call;
extern cmd_run_func func_base_run_add;
extern cmd_run_func func_base_run_sub;
extern cmd_run_func func_base_run_mul;
extern cmd_run_func func_base_run_div;
extern cmd_run_func func_base_run_ret;
extern cmd_run_func func_base_run_acmd;
extern cmd_run_func func_base_run_scmd;
extern cmd_run_func func_base_run_icmd;
extern cmd_run_func func_base_run_dcmd;
extern cmd_run_func func_base_run_gcmd;
extern cmd_run_func func_base_run_gcmdnext;
extern cmd_run_func func_base_run_gcmdlast;
extern cmd_run_func func_base_run_arule;
extern cmd_run_func func_base_run_drule;
extern cmd_run_func func_base_run_acmdex;
extern cmd_run_func func_base_run_scmdex;
extern cmd_run_func func_base_run_icmdex;
extern cmd_run_func func_base_run_dcmdex;
extern cmd_run_func func_base_run_gcmdex;
extern cmd_run_func func_base_run_gcmdnextex;
extern cmd_run_func func_base_run_gcmdlastex;
extern cmd_run_func func_base_run_gstack;

extern cmd_op_func func_base_op_get_mov;
extern cmd_op_func func_base_op_get_cmp;
extern cmd_op_func func_base_op_get_jmp;
extern cmd_op_func func_base_op_get_je;
extern cmd_op_func func_base_op_get_jne;
extern cmd_op_func func_base_op_get_ja;
extern cmd_op_func func_base_op_get_jae;
extern cmd_op_func func_base_op_get_jb;
extern cmd_op_func func_base_op_get_jbe;
extern cmd_op_func func_base_op_get_call;
extern cmd_op_func func_base_op_get_add;
extern cmd_op_func func_base_op_get_sub;
extern cmd_op_func func_base_op_get_mul;
extern cmd_op_func func_base_op_get_div;
extern cmd_op_func func_base_op_get_ret;
extern cmd_op_func func_base_op_get_acmd;
extern cmd_op_func func_base_op_get_scmd;
extern cmd_op_func func_base_op_get_icmd;
extern cmd_op_func func_base_op_get_dcmd;
extern cmd_op_func func_base_op_get_gcmd;
extern cmd_op_func func_base_op_get_gcmdnext;
extern cmd_op_func func_base_op_get_gcmdlast;
extern cmd_op_func func_base_op_get_arule;
extern cmd_op_func func_base_op_get_drule;
extern cmd_op_func func_base_op_get_acmdex;
extern cmd_op_func func_base_op_get_scmdex;
extern cmd_op_func func_base_op_get_icmdex;
extern cmd_op_func func_base_op_get_dcmdex;
extern cmd_op_func func_base_op_get_gcmdex;
extern cmd_op_func func_base_op_get_gcmdnextex;
extern cmd_op_func func_base_op_get_gcmdlastex;
extern cmd_op_func func_base_op_get_gstack;

class rule_manager
{
public:
	rule_index begin;
	std::vector<rule_unit> rule_set;
	std::vector<rule_index> rule_set_free;
	std::unordered_map<size_t, cmd_funcs> funcs;
	std::vector<rule_manager_stack> stack;
public:
	bool test_validity_rule_index(rule_index index, bool is_test_del) const;
	std::optional<rule_unit*> get_rule_unit_ptr(rule_index index);
	std::optional<const rule_unit*> get_rule_unit_ptr_const(rule_index index) const;
	std::optional<rule_index> get_free();
	bool add_free(rule_index index);
	bool get_param(rule_index target_src, const rule_interrupt_value_call& src, std::vector<var_single>& save) const;
	void get_operand_text(const operand& op, std::string& save) const;
	void get_interrupt_text(rule_index index, const rule_interrupt& interrupt, std::string& save) const;
public:
	rule_manager();
	~rule_manager();
public:
	bool set_begin(rule_index index);
	bool set_param(rule_index index, std::span<const var_single> param);
	std::optional<rule_index> add_rule();
	bool del_rule(rule_index index);
	std::optional<rule*> get_rule(rule_index index);
	void set_add_command(size_t cmd_type, const cmd_funcs& funcs_new);
	bool run(std::vector<var_single>& result);
};
