#pragma once
#include "rule_operand.h"

enum class command_type :data_type_unsigned_base
{
	//-----------------------------------------------------------
	//           left           right
	//            a               b
	mov,// mov var/proxy , imm/var/proxy    a=b
	//-----------------------------------------------------------
	//           left              right         save
	//            a                  b            c
	cmp,// cmp imm/var/proxy , imm/var/proxy , var/proxy   a == b | a < b | a <= b | a > b | a >= b
	//-----------------------------------------------------------
	//           target
	jmp,// jmp imm/var/proxy
	//-----------------------------------------------------------
	//           target          compare
	je,//  je imm/var/proxy , imm/var/proxy
	jne,// jne imm/var/proxy , imm/var/proxy
	ja,//  ja imm/var/proxy , imm/var/proxy
	jae,// jae imm/var/proxy , imm/var/proxy
	jb,//  jb imm/var/proxy , imm/var/proxy
	jbe,// jbe imm/var/proxy , imm/var/proxy
	//-----------------------------------------------------------
	//              target        param begin   result save
	call,// call imm/var/proxy , imm/var/proxy , var/proxy
	//-----------------------------------------------------------
	//            left             right         save
	//             a                 b            c
	add,// add imm/var/proxy , imm/var/proxy , var/proxy    c=a+b
	sub,// sub imm/var/proxy , imm/var/proxy , var/proxy    c=a-b
	mul,// mul imm/var/proxy , imm/var/proxy , var/proxy    c=a*b
	div,// div imm/var/proxy , imm/var/proxy , var/proxy    c=a/b
	//-----------------------------------------------------------
	//         return content  return quantity
	ret,// ret imm/var/proxy , imm/var/proxy
	//-----------------------------------------------------------
	//             cmd type        op begin        save
	acmd,// acmd imm/var/proxy , imm/var/proxy , var/proxy
	//-----------------------------------------------------------
	//              target          cmd type        op begin
	scmd,// scmd imm/var/proxy , imm/var/proxy , imm/var/proxy
	//-----------------------------------------------------------
	//              target          cmd type        op begin       save
	icmd,// icmd imm/var/proxy , imm/var/proxy , imm/var/proxy , var/proxy
	//-----------------------------------------------------------
	//              target 
	dcmd,// dcmd imm/var/proxy
	//-----------------------------------------------------------
	//              target       save begin
	gcmd,// gcmd imm/var/proxy , var/proxy
	//-----------------------------------------------------------
	//                    target       idx save   save begin
	gcmdnext,// gcmdnxt imm/var/proxy , var/proxy ,var/proxy
	//-----------------------------------------------------------
	//                    target       idx save   save begin
	gcmdlast,// gcmdnxt imm/var/proxy , var/proxy ,var/proxy
	//-----------------------------------------------------------
	//                   type               op            run func
	//regcmd,// regcmd imm/var/proxy , imm/var/proxy , imm/var/proxy
	//-----------------------------------------------------------
	//               save
	arule,// arule var/proxy
	//-----------------------------------------------------------
	//                target
	drule,// drule imm/var/proxy
	//-----------------------------------------------------------
	//                  target          cmd type       op begin        save 
	acmdex,// acmdex imm/var/proxy , imm/var/proxy , imm/var/proxy , var/proxy
	//-----------------------------------------------------------
	//                target_rule      target_cmd       cmd type       op begin 
	scmdex,// scmdex imm/var/proxy , imm/var/proxy , imm/var/proxy , imm/var/proxy
	//-----------------------------------------------------------
	//              target_rule      target_cmd      cmd type        op begin       save
	icmdex,// icmd imm/var/proxy , imm/var/proxy ,imm/var/proxy , imm/var/proxy , var/proxy
	//-----------------------------------------------------------
	//              target_rule      target_cmd
	dcmdex,// dcmd imm/var/proxy , imm/var/proxy
	//-----------------------------------------------------------
	//                target rule     target cmd     save begin
	gcmdex,// gcmdex imm/var/proxy , imm/var/proxy , var/proxy
	//-----------------------------------------------------------
	//                       target rule     target cmd     idx save    save begin
	gcmdnextex,// gcmdnextex imm/var/proxy , imm/var/proxy , var/proxy , var/proxy
	//-----------------------------------------------------------
	//                       target rule     target cmd     idx save    save begin
	gcmdlastex,// gcmdlastex imm/var/proxy , imm/var/proxy , var/proxy , var/proxy


	invalid,
};

enum class compare_type :data_type_unsigned_base
{
	equal,
	greater,
	lower,
};

using command_operand = std::variant
<
	operand_set<oq::zero>,
	operand_set<oq::one>,
	operand_set<oq::two>,
	operand_set<oq::three>,
	operand_set<oq::four>,
	operand_set<oq::five>
>;

struct command
{
	command_type cmd;
	command_operand op;
	command();
	command(command_type cmd_, const command_operand& op_);
	template<typename... ops>
	command(command_type cmd_, ops&&... ops_) :
		cmd(cmd_), op(std::in_place_index<sizeof...(ops)>, std::forward<ops>(ops_)...)
	{
		static_assert(sizeof...(ops) <= OPERAND_QUANTITY_MAX, "超出最大操作数数量限制");
	}
	command(const command& command_);
	command(command&& command_) noexcept;
	command& operator=(const command& other);
	~command();
};

using command_index = data_type_unsigned_base;

static constexpr command_index COMMAND_INVALID_INDEX = 0xFFFFFFFFFFFFFFFFULL;

struct command_unit
{
	command content;
	command_index last;
	command_index next;
	command_unit();
	command_unit(const command& content_, command_index last_, command_index next_);
	command_unit(const command_unit& command_unit_);
	command_unit(command_unit&& command_unit_) noexcept;
	command_unit& operator=(const command_unit& other);
	~command_unit();
};

class rule;

class command_manager
{
private:
	command_index begin;
	command_index end;
	std::vector<command_unit> command_set;
	std::vector<command_index> command_set_free;
public:
	bool test_validity_command(const command& command_new) const;
	bool test_validity_command_index(command_index index, bool is_test_del) const;
	std::optional<command_unit*> get_command_unit_ptr(command_index index, bool is_save_del = false);
	std::optional<const command_unit*> get_command_unit_ptr_const(command_index index) const;
	std::optional<command_index> get_free();
	bool add_free(command_index index);
public:
	command_manager();
	~command_manager();
public:
	std::optional<command_index> get_begin() const;
	std::optional<command_index> get_end() const;
	bool set_begin(command_index index);
	bool set_end(command_index index);
	std::optional<command_index> add_command(const command& command_new);
	template<typename... operand>
	std::optional<command_index> add_command(command_type type, operand&&... op)
	{
		return add_command(command(type, std::forward<operand>(op)...));
	}
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
	std::optional<command> get_command(command_index index) const;
	std::optional<command> get_command_next(command_index index) const;
	std::optional<command> get_command_last(command_index index) const;
	std::optional<command*> get_command_ptr(command_index index);
	std::optional<command*> get_command_ptr_next(command_index index);
	std::optional<command*> get_command_ptr_last(command_index index);
	std::optional<command_index> get_command_index_next(command_index index) const;
	std::optional<command_index> get_command_index_last(command_index index) const;
	bool del_command(command_index index);
public:
	command_manager& operator=(const command_manager& other);
};
