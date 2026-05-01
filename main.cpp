#include "rule.h"
#include <iostream>

int main()
{
    rule_manager test;

    std::optional<rule_index> rule1 = test.add_rule();

    if (rule1.has_value() == false)
        return 0;

    std::optional<rule*> rule_ptr_result = test.get_rule(rule1.value());

    if (rule_ptr_result.has_value() == false)
        return 0;

    rule* rule_1_ptr = rule_ptr_result.value();

    rule_1_ptr->add_command(
        command_type::mov,
        variable_common(0),
        immediate(static_cast<var_single>(operand_type::variable_common))
    );
    rule_1_ptr->add_command(
        command_type::mov,
        variable_common(1),
        immediate(6)
    );
    rule_1_ptr->add_command(
        command_type::mov,
        variable_common(2),
        immediate(static_cast<var_single>(operand_type::immediate))
    );
    rule_1_ptr->add_command(
        command_type::mov,
        variable_common(3),
        immediate(999897)
    );
    rule_1_ptr->add_command(
        command_type::mov,
        variable_common(4),
        immediate(static_cast<var_single>(operand_type::variable_common))
    );
    rule_1_ptr->add_command(
        command_type::mov,
        variable_common(5),
        immediate(6)
    );
    rule_1_ptr->add_command(
        command_type::mov,
        variable_common(6),
        immediate(static_cast<var_single>(operand_type::immediate))
    );
    rule_1_ptr->add_command(
        command_type::mov,
        variable_common(7),
        immediate(1)
    );
    rule_1_ptr->add_command(
        command_type::acmd,
        immediate(static_cast<size_t>(command_type::mov)),
        variable_common(0),
        variable_common(9)
    );
    rule_1_ptr->add_command(
        command_type::acmd,
        immediate(static_cast<size_t>(command_type::ret)),
        variable_common(4),
        variable_common(9)
    );

    test.set_begin(rule1.value());

    std::vector<var_single> result;
    test.run(result);

    return 0;
}