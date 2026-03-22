# rule
一个基于 C++(C++11及以上) 的规则执行引擎，支持指令式规则定义、变量管理、规则间调用以及运行时动态修改任何其他规则中的指令
## 特性
- **指令驱动**：内置 15 种基础指令（算术、比较、跳转、调用、结束等），支持顺序、条件、循环执行
- **变量系统**：区分参数变量和普通变量，支持引用计数自动回收；提供代理变量实现间接访问
- **规则调用**：规则之间可以相互调用，通过栈保存上下文，支持返回值传递
- **动态修改**：通过 `write_command_*` 系列指令，可在运行时修改任意规则中的任意指令
- **高效存储**：指令和变量均使用 `std::vector` 存储，空闲索引复用
- **无第三方依赖**：仅使用 C++ 标准库
- ## 简单示例
```
#include "rule.h"

int main()
{
    rule_manager manager;
    rule_index idx = manager.add_rule();
    rule* r = manager.get_rule(idx);

    var common_var(var_type::common, 0);
    var param_var(var_type::param, 0);

    r->add_command_write(common_var, 0);
    r->add_command_copy(common_var, param_var);
    r->add_command_end(common_var);

    std::vector<rule_var_info> params = { rule_var_info(0, 42) };
    var_single result;
    manager.run(idx, params, result);   // result == 42

    return 0;
}
```
## 核心架构
### 类架构
- `rule`：单条规则，包含指令链表、参数变量集、普通变量集和空闲指令池
- `rule_manager`：管理所有规则，提供规则创建、删除、查找以及全局执行入口
### 变量管理
- 参数变量（`var_type::param`）和普通变量（`var_type::common`）分别存储在有序 `vector` 中，按 `index` 排序，使用二分查找
- 每个变量有引用计数，指令引用时增加计数，删除时减少；计数归零时自动释放
- 代理变量（`*_proxy`）的 `content` 存储被代理变量的索引，读写时自动解引用
### 指令链表
- 指令存储在 `command_set` 中，每个指令包含内容和 `next` 索引，形成单向链表
- 删除指令时将其索引放入 `command_index_free` 池，创建新指令时优先复用池中索引
## 指令集详解
|指令|参数|说明|
|-|-|-|
|`write`|`target`(var), `content`(var_single)|将常量值写入变量|
|`copy`|`target`(var), `src`(var)|将源变量的值复制到目标变量|
|`compare`|`one`(var), `sec`(var), `save`(var)|比较两个变量，结果存入 `save`（0=相等, 1=大于, 2=小于）|
|`jump`|`target`(command_index)|无条件跳转到指定指令|
|`jump_if_equal`|`result`(var), `target`(command_index)|若比较结果为相等则跳转|
|`jump_if_greater`|`result`(var), `target`(command_index)|若比较结果为大于则跳转|
|`jump_if_lower`|`result`(var), `target`(command_index)|若比较结果为小于则跳转|
|`jump_if_greater_or_equal`|`result`(var), `target`(command_index)|若比较结果为大于或等于则跳转|
|`jump_if_lower_or_equal`|`result`(var), `target`(command_index)|若比较结果为小于或等于则跳转|
|`add`|`left`(var), `right`(var), `save`(var)|加法，`save = left + right`|
|`subtract`|`left`(var), `right`(var), `save`(var)|减法，`save = left - right`|
|`multiply`|`left`(var), `right`(var), `save`(var)|乘法，`save = left * right`|
|`divide`|`left`(var), `right`(var), `save`(var)|除法，`save = left / right`（除零时跳过）|
|`call`|`target`(rule_index), `param`(vector<rule_var_info>), `save`(var)|调用另一条规则，返回值存入 `save`|
|`end`|`out`(var)|	结束当前规则，将 `out` 的值作为返回值|
|`write_command_write`|`target_rule`(rule_index), `target_cmd`(command_index), `target_var`(var), `content`(var_single)|将目标规则中指定位置的指令修改为 `write` 指令，写入指定变量和常量值|
|`write_command_copy`|`target_rule`(rule_index), `target_cmd`(command_index), `target_var`(var), `src`(var)|修改为 `copy` 指令，从源变量复制到目标变量|
|`write_command_compare`|`target_rule`(rule_index), `target_cmd`(command_index), `one`(var), `sec`(var), `save`(var)|修改为 `compare` 指令|
|`write_command_jump`|`target_rule`(rule_index), `target_cmd`(command_index), `jump_target`(command_index)|修改为 jump 指令，跳转到指定位置|
|`write_command_jump_if_equal`|`target_rule`(rule_index), `target_cmd`(command_index), `result`(var), `jump_target`(command_index)|修改为 `jump_if_equal` 指令|
|`write_command_jump_if_greater`|`target_rule`(rule_index), `target_cmd`(command_index), `result`(var), `jump_target`(command_index)|修改为 `jump_if_greater` 指令|
|`write_command_jump_if_lower`|`target_rule`(rule_index), `target_cmd`(command_index), `result`(var), `jump_target`(command_index)|修改为 `jump_if_lower` 指令|
|`write_command_jump_if_greater_or_equal`|`target_rule`(rule_index), `target_cmd`(command_index), `result`(var), `jump_target`(command_index)|修改为 `jump_if_greater_or_equal` 指令|
|`write_command_jump_if_lower_or_equal`|`target_rule`(rule_index), `target_cmd`(command_index), `result`(var), `jump_target`(command_index)|修改为 `jump_if_lower_or_equal` 指令|
|`write_command_add`|`target_rule`(rule_index), `target_cmd`(command_index), `left`(var), `right`(var), `save`(var)|修改为 `add` 指令|
|`write_command_subtract`|`target_rule`(rule_index), `target_cmd`(command_index), `left`(var), `right`(var), `save`(var)|修改为 `subtract` 指令|
|`write_command_multiply`|`target_rule`(rule_index), `target_cmd`(command_index), `left`(var), `right`(var), `save`(var)|修改为 `multiply` 指令|
|`write_command_divide`|`target_rule`(rule_index), `target_cmd`(command_index), `left`(var), `right`(var), `save`(var)|修改为 `divide` 指令|
|`write_command_call`|`target_rule`(rule_index), `target_cmd`(command_index), `call_target`(rule_index), `param`(vector<rule_var_info>), `save`(var)|修改为 `call` 指令|
|`write_command_end`|`target_rule`(rule_index), `target_cmd`(command_index), `out`(var)|修改为 `end` 指令|
## API 参考
### 类型定义
- `using var_single = uint64_t`：变量存储的值类型
- `using command_index = uint64_t`：指令索引
- `using rule_index = uint64_t`：规则索引
- `using var_index = uint64_t`：变量索引
### 枚举
- `enum class command_type`：指令类型枚举，包含所有基础指令和 `write_command_*` 变体
- `enum class var_type`：变量类型，包括 `param`、`common`、`param_proxy`、`common_proxy`
- `enum class compare_result`：比较结果，值为 `equal`(0)、`greater`(1)、`lower`(2)
- `enum class interrupt_type`：规则运行中断类型，`call` 或 `end`
### 结构体
`var`
变量标识符
```
struct var
{
    var_type type;
    var_index index;
};
```
`rule_var_info`
参数传递结构，可以是常量值或变量引用
```
struct rule_var_info
{
    var_index index;      // 变量索引（用于目标规则中）
    bool is_var;          // true 表示使用变量，false 表示常量
    union
    {
        var_single content;  // 常量值
        var varible;         // 变量引用
    } pram;
};
```
`command_content`
指令内容，使用联合体存储不同指令的参数
`command`
指令节点，包含 `command_content cmd` 和 `command_index next`
`rule_run_result`
规则执行结果，由 `rule::run` 返回，用于规则管理器处理调用和结束
### 类`rule`
#### 公共成员函数
|函数签名|描述|
|-|-|
|`size_t get_param_quantity() const`|返回规则参数数量|
|`bool set_param(const std::vector<rule_var_info>& param)`|设置参数值。参数列表必须与 `param_set` 中变量的数量一致，且每个元素必须是常量（`is_var == false`）。成功返回 `true`，否则返回 `false`|
|`command_index get_begin() const`|返回首条指令索引|
|`bool get_var(const var& target, var_single& save, bool is_get_proxy = false) const`|获取变量的值。若 `is_get_proxy` 为 `true` 且 `target` 为代理变量，则先解引用。成功返回 `true`，失败返回 `false`|
|`bool set_var(const var& target, var_single content, bool is_write_proxy = false)`|设置变量的值。若 `is_write_proxy` 为 true 且 `target` 为代理变量，则写入被代理变量。成功返回 `true`|
|`command_index add_command(const command_content& new_command_content)`|在规则末尾添加指令，返回新指令索引|
|`bool del_command(command_index index)`|删除指定指令，自动更新链表。成功返回 `true`|
|`bool set_command(command_index index, const command_content& new_command_content)`|修改指定指令的内容。成功返回 `true`|
|`command_index insert_command(command_index index_insert, const command_content& new_command_content)`|在指定指令前插入新指令，返回新指令索引|
|`bool get_command(command_index index, command_content& save) const`|获取指定指令的内容，保存到 `save`（通过 `placement new`）。成功返回 `true`|
|`bool run(rule_manager& ownership, rule_run_result& save, command_index begin = COMMAND_INVALID_INDEX)`|执行规则。通常由 `rule_manager` 调用，不直接使用|
### 类`rule_manager`
#### 公共成员函数
|函数签名|描述|
|-|-|
|`rule_index add_rule()`|创建一条新规则，返回其索引|
|`bool del_rule(rule_index index)`|删除指定规则|
|`rule* get_rule(rule_index index)`|获取规则指针，若索引无效或规则已被删除则返回 `nullptr`|
|`const rule* get_const_rule(rule_index index) const`|获取规则常量指针|
|`bool set_begin(rule_index index)`|设置起始规则（默认运行入口）|
|`bool run(rule_index begin_index, const std::vector<rule_var_info>& param_begin, var_single& save)`|从指定规则开始执行。`param_begin` 为参数列表，结果存入 `save`。成功返回 `true`，失败返回 `false`|
## 使用示例
### 算术运算与条件跳转（计算斐波那契数列）
```
#include "rule.h"

int main()
{
    rule_manager mgr;
    rule_index idx = mgr.add_rule();
    rule* r = mgr.get_rule(idx);

    // 定义变量
    var param_n(var_type::param, 0);
    var a(var_type::common, 0);
    var b(var_type::common, 1);
    var i(var_type::common, 2);
    var temp(var_type::common, 3);
    var cmp_result(var_type::common, 4);
    var const_one(var_type::common, 5);

    // 初始化常量 1
    r->add_command_write(const_one, 1);
    // 初始化 a = 0, b = 1, i = 2
    r->add_command_write(a, 0);
    r->add_command_write(b, 1);
    r->add_command_write(i, 2);

    // 比较指令（循环条件）
    command_index cmp_idx = r->add_command_compare(i, param_n, cmp_result);

    // 条件跳转指令（目标临时填0）
    command_index cond_jump_idx = r->add_command_jump_if_greater(cmp_result, 0);

    // 循环体
    r->add_command_add(a, b, temp);
    r->add_command_copy(a, b);
    r->add_command_copy(b, temp);
    r->add_command_add(i, const_one, i);

    // 无条件跳回比较指令
    r->add_command_jump(cmp_idx);

    // 结束指令
    command_index end_idx = r->add_command_end(b);

    // 修正条件跳转的目标为结束指令
    r->set_command(cond_jump_idx, command_content(command_type::jump_if_greater, command_param_jump_if(cmp_result, end_idx)));

    // 运行，计算第10项
    std::vector<rule_var_info> params = { rule_var_info(0, 10) };
    var_single result;
    mgr.run(idx, params, result);   // result == 55
}
```
### 规则间调用
```
#include "rule.h"

int main()
{
    rule_manager mgr;

    // 子规则：将传入参数加1后返回
    rule_index sub_idx = mgr.add_rule();
    rule* sub = mgr.get_rule(sub_idx);
    var p_sub(var_type::param, 0);
    var t_sub(var_type::common, 0);
    sub->add_command_copy(t_sub, p_sub);
    sub->add_command_add(t_sub, var(var_type::common, 1), t_sub);
    sub->add_command_end(t_sub);

    // 主规则：调用子规则，输出结果
    rule_index main_idx = mgr.add_rule();
    rule* main = mgr.get_rule(main_idx);
    var p_main(var_type::param, 0);
    var res_main(var_type::common, 0);
    std::vector<rule_var_info> call_params = { rule_var_info(0, p_main) };
    main->add_command_call(sub_idx, call_params, res_main);
    main->add_command_end(res_main);

    // 执行主规则，传入参数5，应返回6
    std::vector<rule_var_info> main_params = { rule_var_info(0, 5) };
    var_single final;
    mgr.run(main_idx, main_params, final);   // final == 6

    return 0;
}
```
### 动态修改指令
```
#include "rule.h"

int main()
{
    rule_manager mgr;

    // 目标规则：写入0并返回
    rule_index target_idx = mgr.add_rule();
    rule* target = mgr.get_rule(target_idx);
    var target_var(var_type::common, 0);
    target->add_command_write(target_var, 0);
    target->add_command_end(target_var);

    // 修改器规则：将目标的第一条指令改为写入42
    rule_index mod_idx = mgr.add_rule();
    rule* modifier = mgr.get_rule(mod_idx);
    var dummy(var_type::common, 0);
    modifier->add_command_write_command_write(target_idx, 0, target_var, 42);
    modifier->add_command_end(dummy);

    var_single res;

    // 第一次执行目标，得到0
    mgr.run(target_idx, {}, res);   // res == 0

    // 执行修改器，修改目标规则
    mgr.run(mod_idx, {}, res);      // 修改生效

    // 再次执行目标，得到42
    mgr.run(target_idx, {}, res);   // res == 42

    return 0;
}
```
## 注意事项
- 异常安全：代码未使用异常，所有错误通过返回值报告。调用者应检查返回值
- 线程安全：类非线程安全，多线程环境需外部同步
- 自修改风险：write_command_* 指令可任意修改其他规则，应谨慎使用
- 代理变量：仅支持单层间接引用，避免循环引用
## 未来计划
- 加入更多的内置函数
- 加入自定义指令的拓展支持





























