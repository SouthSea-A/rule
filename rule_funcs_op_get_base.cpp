#include "rule.h"


cmd_op_func func_base_op_get_mov = [](
	const rule& owner, 
	const operand& op, 
	command_operand& save
	)->bool
	{
		std::vector<var_single> param_get;

		if (owner.get_op_by_op(op, 4, param_get) == false)
			return false;

		operand left;
		operand right;

		if (owner.get_op_by_var(param_get[0], param_get[1], left, operand_type::immediate) == false)
			return false;

		if (owner.get_op_by_var(param_get[2], param_get[3], right) == false)
			return false;

		save.emplace<static_cast<size_t>(oq::two)>(left, right);

		return true;
	};

cmd_op_func func_base_op_get_cmp = [](
	const rule& owner, 
	const operand& op, 
	command_operand& save
	)->bool
	{
		std::vector<var_single> param_get;

		if (owner.get_op_by_op(op, 6, param_get) == false)
			return false;

		operand left;
		operand right;
		operand cmp_save;

		if (owner.get_op_by_var(param_get[0], param_get[1], left) == false)
			return false;

		if (owner.get_op_by_var(param_get[2], param_get[3], right) == false)
			return false;

		if (owner.get_op_by_var(param_get[4], param_get[5], cmp_save, operand_type::immediate) == false)
			return false;

		save.emplace<static_cast<size_t>(oq::three)>(left, right, cmp_save);

		return true;
	};

cmd_op_func func_base_op_get_jmp = [](
	const rule& owner, 
	const operand& op, 
	command_operand& save
	)->bool
	{
		std::vector<var_single> param_get;

		if (owner.get_op_by_op(op, 2, param_get) == false)
			return false;

		operand target;

		if (owner.get_op_by_var(param_get[0], param_get[1], target) == false)
			return false;

		save.emplace<static_cast<size_t>(oq::one)>(target);

		return true;
	};

cmd_op_func func_base_op_get_je = [](
	const rule& owner,
	const operand& op,
	command_operand& save
	)->bool
	{
		std::vector<var_single> param_get;

		if (owner.get_op_by_op(op, 4, param_get) == false)
			return false;

		operand target;
		operand test;

		if (owner.get_op_by_var(param_get[0], param_get[1], target) == false)
			return false;

		if (owner.get_op_by_var(param_get[2], param_get[3], test) == false)
			return false;

		save.emplace<static_cast<size_t>(oq::two)>(target, test);

		return true;
	};

cmd_op_func func_base_op_get_jne = func_base_op_get_je;
cmd_op_func func_base_op_get_ja = func_base_op_get_je;
cmd_op_func func_base_op_get_jae = func_base_op_get_je;
cmd_op_func func_base_op_get_jb = func_base_op_get_je;
cmd_op_func func_base_op_get_jbe = func_base_op_get_je;

cmd_op_func func_base_op_get_call = [](
	const rule& owner,
	const operand& op,
	command_operand& save
	)->bool
	{
		std::vector<var_single> param_get;

		if (owner.get_op_by_op(op, 6, param_get) == false)
			return false;

		operand target;
		operand param_begin;
		operand result_save;

		if (owner.get_op_by_var(param_get[0], param_get[1], target) == false)
			return false;

		if (owner.get_op_by_var(param_get[2], param_get[3], param_begin) == false)
			return false;

		if (owner.get_op_by_var(param_get[4], param_get[5], result_save, operand_type::immediate) == false)
			return false;

		save.emplace<static_cast<size_t>(oq::three)>(target, param_begin, result_save);

		return true;
	};

cmd_op_func func_base_op_get_add = [](
	const rule& owner,
	const operand& op,
	command_operand& save
	)->bool
	{
		std::vector<var_single> param_get;

		if (owner.get_op_by_op(op, 6, param_get) == false)
			return false;

		operand left;
		operand right;
		operand calc_save;

		if (owner.get_op_by_var(param_get[0], param_get[1], left) == false)
			return false;

		if (owner.get_op_by_var(param_get[2], param_get[3], right) == false)
			return false;

		if (owner.get_op_by_var(param_get[4], param_get[5], calc_save, operand_type::immediate) == false)
			return false;

		save.emplace<static_cast<size_t>(oq::three)>(left, right, calc_save);

		return true;
	};

cmd_op_func func_base_op_get_sub = func_base_op_get_add;
cmd_op_func func_base_op_get_mul = func_base_op_get_add;
cmd_op_func func_base_op_get_div = func_base_op_get_add;

cmd_op_func func_base_op_get_ret = [](
	const rule& owner,
	const operand& op,
	command_operand& save
	)->bool
	{
		std::vector<var_single> param_get;

		if (owner.get_op_by_op(op, 4, param_get) == false)
			return false;

		operand save_target;
		operand save_quan;

		if (owner.get_op_by_var(param_get[0], param_get[1], save_target) == false)
			return false;

		if (owner.get_op_by_var(param_get[2], param_get[3], save_quan) == false)
			return false;

		save.emplace<static_cast<size_t>(oq::two)>(save_target, save_quan);

		return true;
	};

cmd_op_func func_base_op_get_acmd = [](
	const rule& owner,
	const operand& op,
	command_operand& save
	)->bool
	{
		std::vector<var_single> param_get;

		if (owner.get_op_by_op(op, 6, param_get) == false)
			return false;

		operand type;
		operand op_begin;
		operand idx_save;

		if (owner.get_op_by_var(param_get[0], param_get[1], type) == false)
			return false;

		if (owner.get_op_by_var(param_get[2], param_get[3], op_begin) == false)
			return false;

		if (owner.get_op_by_var(param_get[4], param_get[5], idx_save, operand_type::immediate) == false)
			return false;

		save.emplace<static_cast<size_t>(oq::three)>(type, op_begin, idx_save);

		return true;
	};

cmd_op_func func_base_op_get_scmd = [](
	const rule& owner,
	const operand& op,
	command_operand& save
	)->bool
	{
		std::vector<var_single> param_get;

		if (owner.get_op_by_op(op, 6, param_get) == false)
			return false;

		operand target;
		operand type;
		operand op_begin;

		if (owner.get_op_by_var(param_get[0], param_get[1], target) == false)
			return false;

		if (owner.get_op_by_var(param_get[2], param_get[3], type) == false)
			return false;

		if (owner.get_op_by_var(param_get[4], param_get[5], op_begin) == false)
			return false;

		save.emplace<static_cast<size_t>(oq::three)>(target, type, op_begin);

		return true;
	};

cmd_op_func func_base_op_get_icmd = [](
	const rule& owner,
	const operand& op,
	command_operand& save
	)->bool
	{
		std::vector<var_single> param_get;

		if (owner.get_op_by_op(op, 8, param_get) == false)
			return false;

		operand target;
		operand type;
		operand op_begin;
		operand idx_save;

		if (owner.get_op_by_var(param_get[0], param_get[1], target) == false)
			return false;

		if (owner.get_op_by_var(param_get[2], param_get[3], type) == false)
			return false;

		if (owner.get_op_by_var(param_get[4], param_get[5], op_begin) == false)
			return false;

		if (owner.get_op_by_var(param_get[6], param_get[7], idx_save, operand_type::immediate) == false)
			return false;

		save.emplace<operand_set<oq::four>>(target, type, op_begin, idx_save);

		return true;
	};

cmd_op_func func_base_op_get_dcmd = [](
	const rule& owner,
	const operand& op,
	command_operand& save
	)->bool
	{
		std::vector<var_single> param_get;

		if (owner.get_op_by_op(op, 2, param_get) == false)
			return false;

		operand target;

		if (owner.get_op_by_var(param_get[0], param_get[1], target) == false)
			return false;

		save.emplace<static_cast<size_t>(oq::one)>(target);

		return true;
	};

cmd_op_func func_base_op_get_gcmd = [](
	const rule& owner,
	const operand& op,
	command_operand& save
	)->bool
	{
		std::vector<var_single> param_get;

		if (owner.get_op_by_op(op, 4, param_get) == false)
			return false;

		operand target;
		operand op_save;

		if (owner.get_op_by_var(param_get[0], param_get[1], target) == false)
			return false;

		if (owner.get_op_by_var(param_get[2], param_get[3], op_save, operand_type::immediate) == false)
			return false;

		save.emplace<static_cast<size_t>(oq::two)>(target, op_save);

		return true;
	};

cmd_op_func func_base_op_get_gcmdnext = [](
	const rule& owner,
	const operand& op,
	command_operand& save
	)->bool
	{
		std::vector<var_single> param_get;

		if (owner.get_op_by_op(op, 6, param_get) == false)
			return false;

		operand target;
		operand index_save;
		operand op_save;

		if (owner.get_op_by_var(param_get[0], param_get[1], target) == false)
			return false;

		if (owner.get_op_by_var(param_get[2], param_get[3], index_save) == false)
			return false;

		if (owner.get_op_by_var(param_get[4], param_get[5], op_save, operand_type::immediate) == false)
			return false;

		save.emplace<static_cast<size_t>(oq::three)>(target, index_save, op_save);

		return true;
	};

cmd_op_func func_base_op_get_gcmdlast = func_base_op_get_gcmdnext;

cmd_op_func func_base_op_get_arule = [](
	const rule& owner,
	const operand& op,
	command_operand& save
	)->bool
	{
		std::vector<var_single> param_get;

		if (owner.get_op_by_op(op, 2, param_get) == false)
			return false;

		operand index_save;

		if (owner.get_op_by_var(param_get[0], param_get[1], index_save, operand_type::immediate) == false)
			return false;

		save.emplace<static_cast<size_t>(oq::one)>(index_save);

		return true;
	};

cmd_op_func func_base_op_get_drule = [](
	const rule& owner,
	const operand& op,
	command_operand& save
	)->bool
	{
		std::vector<var_single> param_get;

		if (owner.get_op_by_op(op, 2, param_get) == false)
			return false;

		operand index_del;

		if (owner.get_op_by_var(param_get[0], param_get[1], index_del) == false)
			return false;

		save.emplace<static_cast<size_t>(oq::one)>(index_del);

		return true;
	};

cmd_op_func func_base_op_get_acmdex = [](
	const rule& owner,
	const operand& op,
	command_operand& save
	)->bool
	{
		std::vector<var_single> param_get;

		if (owner.get_op_by_op(op, 8, param_get) == false)
			return false;

		operand target;
		operand type;
		operand op_begin;
		operand idx_save;

		if (owner.get_op_by_var(param_get[0], param_get[1], target) == false)
			return false;

		if (owner.get_op_by_var(param_get[2], param_get[3], type) == false)
			return false;

		if (owner.get_op_by_var(param_get[4], param_get[5], op_begin) == false)
			return false;

		if (owner.get_op_by_var(param_get[6], param_get[7], idx_save, operand_type::immediate) == false)
			return false;

		save.emplace<static_cast<size_t>(oq::four)>(target, type, op_begin, idx_save);

		return true;
	};

cmd_op_func func_base_op_get_scmdex = [](
	const rule& owner,
	const operand& op,
	command_operand& save
	)->bool
	{
		std::vector<var_single> param_get;

		if (owner.get_op_by_op(op, 8, param_get) == false)
			return false;

		operand target_rule;
		operand target_cmd;
		operand type;
		operand op_begin;

		if (owner.get_op_by_var(param_get[0], param_get[1], target_rule) == false)
			return false;

		if (owner.get_op_by_var(param_get[2], param_get[3], target_cmd) == false)
			return false;

		if (owner.get_op_by_var(param_get[4], param_get[5], type) == false)
			return false;

		if (owner.get_op_by_var(param_get[6], param_get[7], op_begin) == false)
			return false;

		save.emplace<static_cast<size_t>(oq::four)>(target_rule, target_cmd, type, op_begin);

		return true;
	};

cmd_op_func func_base_op_get_icmdex = [](
	const rule& owner,
	const operand& op,
	command_operand& save
	)->bool
	{
		std::vector<var_single> param_get;

		if (owner.get_op_by_op(op, 10, param_get) == false)
			return false;

		operand target_rule;
		operand target_cmd;
		operand type;
		operand op_begin;
		operand index_save;

		if (owner.get_op_by_var(param_get[0], param_get[1], target_rule) == false)
			return false;

		if (owner.get_op_by_var(param_get[2], param_get[3], target_cmd) == false)
			return false;

		if (owner.get_op_by_var(param_get[4], param_get[5], type) == false)
			return false;

		if (owner.get_op_by_var(param_get[6], param_get[7], op_begin) == false)
			return false;

		if (owner.get_op_by_var(param_get[8], param_get[9], index_save, operand_type::immediate) == false)
			return false;

		save.emplace<static_cast<size_t>(oq::five)>(target_rule, target_cmd, type, op_begin, index_save);

		return true;
	};

cmd_op_func func_base_op_get_dcmdex = [](
	const rule& owner,
	const operand& op,
	command_operand& save
	)->bool
	{
		std::vector<var_single> param_get;

		if (owner.get_op_by_op(op, 4, param_get) == false)
			return false;

		operand target_rule;
		operand target_cmd;

		if (owner.get_op_by_var(param_get[0], param_get[1], target_rule) == false)
			return false;

		if (owner.get_op_by_var(param_get[2], param_get[3], target_cmd) == false)
			return false;

		save.emplace<static_cast<size_t>(oq::two)>(target_rule, target_cmd);

		return true;
	};

cmd_op_func func_base_op_get_gcmdex = [](
	const rule& owner,
	const operand& op,
	command_operand& save
	)->bool
	{
		std::vector<var_single> param_get;

		if (owner.get_op_by_op(op, 6, param_get) == false)
			return false;

		operand target_rule;
		operand target_cmd;
		operand op_save;

		if (owner.get_op_by_var(param_get[0], param_get[1], target_rule) == false)
			return false;

		if (owner.get_op_by_var(param_get[2], param_get[3], target_cmd) == false)
			return false;

		if (owner.get_op_by_var(param_get[4], param_get[5], op_save, operand_type::immediate) == false)
			return false;

		save.emplace<static_cast<size_t>(oq::three)>(target_rule, target_cmd, op_save);

		return true;
	};

cmd_op_func func_base_op_get_gcmdnextex = [](
	const rule& owner,
	const operand& op,
	command_operand& save
	)->bool
	{
		std::vector<var_single> param_get;

		if (owner.get_op_by_op(op, 8, param_get) == false)
			return false;

		operand target_rule;
		operand target_cmd;
		operand index_save;
		operand op_save;

		if (owner.get_op_by_var(param_get[0], param_get[1], target_rule) == false)
			return false;

		if (owner.get_op_by_var(param_get[2], param_get[3], target_cmd) == false)
			return false;

		if (owner.get_op_by_var(param_get[4], param_get[5], index_save) == false)
			return false;

		if (owner.get_op_by_var(param_get[6], param_get[7], op_save, operand_type::immediate) == false)
			return false;

		save.emplace<static_cast<size_t>(oq::four)>(target_rule, target_cmd, index_save, op_save);

		return true;
	};

cmd_op_func func_base_op_get_gcmdlastex = func_base_op_get_gcmdnextex;
