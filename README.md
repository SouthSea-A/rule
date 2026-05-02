# 一个支持自修改、自定义规则的规则执行器虚拟机
## 执行器虚拟机特性简介
- 这类似一个汇编指令执行器虚拟机，能按照 “指令 + 操作数” 的形式按序执行指令流水。但不同的是，这里的操作数不再是 “寄存器 + 地址” 的组合，而是变成任意设置的变量组合，更为方便简洁。
- 内置元编程指令，允许运行时的反射和动态修改、添加和删除指令。且不同于汇编的是，这里的动态修改更为安全简单，无需考虑指令的长度问题。
- 允许变量的随意设置，无需预先申请、声明，即写即用，自动分配，就像真实中打草稿一般。删除时也会自动管理其生命周期。
- 允许自定义指令的加入，可编译前加入，也可运行时加入。同时，也允许对已有指令进行重载操作，基本指令的定义和实现也接受外部的重载。
- 允许制定多个规则，同时也支持规则间的相互调用，就像普通函数的创建和调用一般。同时，也支持规则的动态生成和调用。
---
## 执行器虚拟机模块及各公开接口
1. **数据类型模块(rule_base.h)**
- 目的：统一所有数据的类型和长度
  |数据别名|原类型|
  |-|-|
  |data_type_unsigned_base|unsigned long long|
---
2. **变量模块(rule_var.h 、 rule_var.cpp)**
- 目的：简化变量的使用，将变量与具体名称解耦，与数字编号绑定。达到像手写草稿般地随意设置变量，同时由内部自动管理变量。
- 类型别名
  |数据别名|原类型|含义|
  |-|-|-|
  |var_single|data_type_unsigned_base|储存变量内容|
  |var_index|data_type_unsigned_base|变量编号|
  |var_ref|data_type_unsigned_base|变量引用计数|
- 常量
  |常量定义|数值|含义|
  |-|-|-|
  |static constexpr var_single VAR_DEFAULT_CONTENT|0x0000000000000000ULL|变量默认内容|
  |static constexpr var_index VAR_INVALID_INDEX|0xFFFFFFFFFFFFFFFFULL|无效的变量编号|
  |static constexpr var_ref VAR_INVALID_REF|0xFFFFFFFFFFFFFFFFULL|无效的变量引用计数|
- 结构 **var**
  |字段|含义|
  |-|-|
  |var_index index|变量编号|
  |var_single content|变量内容|
- 结构 **var_count**
  |字段|含义|
  |-|-|
  |var variable|变量信息|
  |var_ref ref|变量引用次数|
- 类 **var_manager** 公开接口
  |接口|作用|
  |-|-|
  |bool add_var(const var& var_new)|按照提供的 **var** 结构中的变量信息，创建新变量。如已有相同编号的变量，则增加其引用次数。成功返回 **true**，否则为 **false**|
  |bool del_var(var_index index)|提供变量编号 **index**，减少一次对应变量的引用次数。如次数减为0，则真正删除。成功返回 **true**，否则为 **false**|
  |std::optional<var_single> get_var(var_index index) const|提供变量编号 **index** ,取得变量中的内容，返回 **std::optional<var_single>**。如变量不存在，则为 **std::nullopt**|
  |std::optional<var_ref> get_var_ref(var_index index) const|提供变量编号 **index** ,取得变量的引用次数，返回 **std::optional<var_ref>**。如变量不存在，则为 **std::nullopt**|
  |bool get_var(std::vector<var_single>& save, var_index start, size_t quantity) const|提供变量内容保存容器 **save** 、起始变量编号 **index** 和 读取数量 **quantity**，从变量 **index** 开始，按序逐个读取往后比其编号大的变量，直到满足数量要求，并将结果按序保存在 **save** 中。如果读取数量大于实际数量，则返回 **false** ，反之为 **true**|
  |bool set_var(const var& var_change)|按照提供的 **var** 结构中的变量信息，设置变量新内容。如果变量不存在，返回 **false** ，反之为 **true**|
  |bool set_var(var_index index, var_single content)|提供变量编号 **index** 和 变量内容**content** ，设置变量新内容。如果变量不存在，返回 **false** ，反之为 **true**|
  |bool set_var(const std::vector<var_single>& var_all)|提供有序的变量内容容器 **var_all**，从此容器的第一个元素开始，按序从实际储存的最小编号的变量开始，逐个往后比前一个编号大的变量进行内容覆写。如果写入的变量数量大于实际储存的变量数量，则返回 **false** ，反之为 **true**|
  |bool set_var(var_index index_begin, const std::vector<var_single>& var_all)|提供起始变量编号 **index** 和有序的变量内容容器 **var_all**，从此容器的第一个元素开始，按序从编号为 **index** 的变量开始开始，逐个往后比前一个编号大的变量进行内容覆写。如果写入的变量数量大于实际储存的变量数量，则返回 **false** ，反之为 **true**|
  |std::optional<size_t> get_quantity_var() const|获取变量的数量，返回 **std::optional<size_t>** ，如果没有变量，则返回 **std::nullopt**|
  |var_manager& operator=(const var_manager& other)|本类的 **=** 操作符重载|
---
3. **操作数模块(rule_operand.h、rule_operand.cpp)**
- 目的：定义指令所需的操作数类型和操作数集合，支持立即数、多种变量引用以及间接寻址（代理），使指令能够灵活访问数据。
- 枚举类 **operand_type**
  |类型|原类型|含义|
  |-|-|-|
  |immediate|data_type_unsigned_base|立即数，直接使用此内容|
  |variable_param|data_type_unsigned_base|参数变量，主要用于规则间调用|
  |variable_common|data_type_unsigned_base|普通变量，主要用于规则内使用|
  |proxy_param|data_type_unsigned_base|参数代理变量，类似于指针，将此变量中的内容当作变量编号再读取一遍得到变量内容，主要用于规则间调用|
  |proxy_common|data_type_unsigned_base|普通代理变量，类似于指针，将此变量中的内容当作变量编号再读取一遍得到变量内容，主要用于规则内使用|
  |invalid|data_type_unsigned_base|无效的变量类型|
- 枚举类 **operand_quantity**
  |类型|原类型|含义|
  |-|-|-|
  |zero|data_type_unsigned_base|0个操作数|
  |one|data_type_unsigned_base|1个操作数|
  |two|data_type_unsigned_base|2个操作数|
  |three|data_type_unsigned_base|3个操作数|
  |four|data_type_unsigned_base|4个操作数|
  |five|data_type_unsigned_base|5个操作数|
- 枚举类 **operand_index**
  |类型|原类型|含义|
  |-|-|-|
  |first|data_type_unsigned_base|第一个操作数|
  |second|data_type_unsigned_base|第二个操作数|
  |third|data_type_unsigned_base|第三个操作数|
  |fourth|data_type_unsigned_base|第四个操作数|
  |fifth|data_type_unsigned_base|第五个操作数|
- 结构 **immediate**
  |字段|含义|
  |-|-|
  |var_single value|立即数内容|
- 结构 **variable_param**
  |字段|含义|
  |-|-|
  |var_index index|参数变量编号|
- 结构 **variable_common**
  |字段|含义|
  |-|-|
  |var_index index|普通变量编号|
- 结构 **proxy_param**
  |字段|含义|
  |-|-|
  |var_index index|参数代理变量编号|
- 结构 **proxy_common**
  |字段|含义|
  |-|-|
  |var_index index|普通代理变量编号|
- 类型别名
  |数据别名|原类型|含义|
  |-|-|-|
  |operand|std::variant<immediate, variable_param, variable_common, proxy_param, proxy_common>|实现操作数的多态，同时容纳5个操作数的类型|
  |oq|operand_quantity|操作数数量|
  |oi|operand_index|操作数编号|
- 结构 **template\<oq quan> struct operand_set**
  |字段|含义|
  |-|-|
  |std::array<operand, static_cast<size_t>(quan)> set|储存 **quan** 个相同或不同的操作数|
- 常量
  |常量定义|数值|含义|
  |-|-|-|
  |static constexpr size_t OPERAND_QUANTITY_MAX|5|最大操作数数量|
---
4. **指令模块(rule_command.h、rule_command.cpp)**
- 目的：集中管理各样的指令及其相对顺序，同时定义基础指令集
- 枚举类 **command_type**
  |类型|原类型|含义|
  |-|-|-|
  |mov|data_type_unsigned_base|移动数据|
  |cmp|data_type_unsigned_base|比较数据|
  |jmp|data_type_unsigned_base|跳转|
  |je|data_type_unsigned_base|等于跳转|
  |jne|data_type_unsigned_base|不等于跳转|
  |ja|data_type_unsigned_base|大于跳转|
  |jae|data_type_unsigned_base|大于等于跳转|
  |jb|data_type_unsigned_base|小于跳转|
  |jbe|data_type_unsigned_base|小于等于跳转|
  |add|data_type_unsigned_base|相加数据|
  |sub|data_type_unsigned_base|相减数据|
  |mul|data_type_unsigned_base|相乘数据|
  |div|data_type_unsigned_base|相除数据|
  |call|data_type_unsigned_base|调用规则|
  |ret|data_type_unsigned_base|规则返回|
  |acmd|data_type_unsigned_base|添加指令|
  |scmd|data_type_unsigned_base|修改指令|
  |icmd|data_type_unsigned_base|插入指令|
  |dcmd|data_type_unsigned_base|删除指令|
  |gcmd|data_type_unsigned_base|查询指令|
  |gcmdnext|data_type_unsigned_base|查询下一条指令|
  |gcmdlast|data_type_unsigned_base|查询上一条指令|
  |arule|data_type_unsigned_base|添加规则|
  |acmdex|data_type_unsigned_base|对外部规则添加指令|
  |scmdex|data_type_unsigned_base|对外部规则修改指令|
  |icmdex|data_type_unsigned_base|对外部规则插入指令|
  |dcmdex|data_type_unsigned_base|对外部规则删除指令|
  |gcmdex|data_type_unsigned_base|向外部规则查询指令|
  |gcmdnextex|data_type_unsigned_base|向外部规则查询下一条指令|
  |gcmdlastex|data_type_unsigned_base|向外部规则查询上一条指令|
  |invalid|data_type_unsigned_base|无效指令|
- 枚举类 **compare_type**
  |类型|原类型|含义|
  |-|-|-|
  |equal|data_type_unsigned_base|比较指令比较结果为相等|
  |greater|data_type_unsigned_base|比较指令比较结果为大于|
  |lower|data_type_unsigned_base|比较指令比较结果为小于|
- 类型别名
  |数据别名|原类型|含义|
  |-|-|-|
  |command_operand| std::variant<operand_set\<oq::zero>,operand_set\<oq::one>,operand_set\<oq::two>,operand_set\<oq::three>,operand_set\<oq::four>,operand_set\<oq::five>>|储存1-5个操作数|
  |command_index|data_type_unsigned_base|指令编号|
- 结构 **command**
  |字段|含义|
  |-|-|
  |command_type cmd|指令类型|
  |command_operand op|指令的操作数|
- 常量
  |常量定义|数值|含义|
  |-|-|-|
  |static constexpr command_index COMMAND_INVALID_INDEX|0xFFFFFFFFFFFFFFFFULL|无效的指令编号|
- 结构 **command_unit**
  |字段|含义|
  |-|-|
  |command content|指令内容信息|
  |command_index last|上一条指令编号|
  |command_index next|下一条指令编号|
- 类 **command_manager** 公开接口
  |接口|作用|
  |-|-|
  |bool test_validity_command(const command& command_new) const|按照提供的 **command_new** 结构中的检测此指令是否有效，有效返回 **true** ，否则为 **false**|
  |bool test_validity_command_index(command_index index, bool is_test_del) const|按照提供的 **index** 指令编号以及 **is_test_del** 的值，检测指令是否有效。如果指令不存在或 **is_test_del**为真且当前指令已被删除的话，则返回 **false**，反之为 **true**|
  |std::optional<command_unit\*> get_command_unit_ptr(command_index index, bool is_save_del = false)|按照提供的 **index** 指令编号以及 **is_save_del** 的值，获取指令的 **command_unit\*** 结构指针，如果指令不存在或 **is_save_del** 为真且当前指令已被删除的话，则返回 **std::nullopt**；否则，**is_save_del** 为假且当前指令已被删除的话，依旧返回指令的 **command_unit\*** 结构指针。同时， **is_save_del** 默认为真|
  |std::optional<const command_unit\*> get_command_unit_ptr_const(command_index index) const|按照提供的 **index** 指令编号获取指令的 **command_unit\*** 结构常量指针。如果指令不存在或被删除，则返回 **std::nullopt**|
  |std::optional<command_index> get_free()|获取空闲的已删除指令的编号(获取的是最近删除的)，如果没有，则返回 **std::nullopt**|
  |bool add_free(command_index index)|按照提供的 **index** 指令编号删除指定的指令，并添加到空闲列表中|
  |std::optional<command_index> get_begin() const|获取起始指令编号，如果不存在，则返回 **std::nullopt**|
  |std::optional<command_index> get_end() const|获取结束指令编号，如果不存在，则返回 **std::nullopt**|
  |bool set_begin(command_index index)|设置起始指令编号，失败返回 **false** ，反之为 **true**|
  |bool set_end(command_index index)|设置结束指令编号，失败返回 **false** ，反之为 **true**|
  |std::optional<command_index> add_command(const command& command_new)|按照提供的 **command_new** 结构中的信息，添加新的指令，并返回新指令的编号。如果添加失败，则返回 **std::nullopt**。如果是首条指令，这此次指令将被自动设置为起始指令，往后每添加一条指令都将自动设置为结束指令|
  |template<typename... operand> std::optional<command_index> add_command(command_type type, operand&&... op)|按照提供的 **type** 的指令类型以及 **op** 中的操作数参数包自动打包并调用 **std::optional<command_index> add_command(const command& command_new)** 添加指令。添加成功返回新指令的编号，否则返回 **std::nullopt**。同样地，如果是首条指令，这此次指令将被自动设置为起始指令，往后每添加一条指令都将自动设置为结束指令|
  |std::optional<command_index> insert_command(command_index index_insert, const command& command_new)|按照提供的 **index_insert** 插入指令编号以及 **command_new** 结构中的信息，在指定插入的地方代替原指令的顺序插入新的指令，旧指令的顺序变为在新指令之后，并返回新指令的编号。如果插入失败，则返回 **std::nullopt**。如果插入的是首条指令，这此次指令将被自动设置为起始指令|
  |template<typename... operand> bool set_command(command_index index, command_type type, operand&&... op)|按照提供的 **index** 的指令类型以及 **op** 中的操作数参数包自动打包并调用 **std::optional<command_index> insert_command(command_index index_insert, const command& command_new)** 在指定插入的地方代替原指令的顺序插入新的指令。旧指令的顺序变为在新指令之后，并返回新指令的编号。如果插入失败，则返回 **std::nullopt**。同样地，如果插入的是首条指令，这此次指令将被自动设置为起始指令|
  |std::optional<command> get_command(command_index index) const|按照提供的 **index** 指令编号获取指令 **command** 结构的拷贝。如果指令不存在或被删除，则返回 **std::nullopt**|
  |std::optional<command> get_command_next(command_index index) const|按照提供的 **index** 指令编号获取此指令的下一条指令 **command** 结构的拷贝。如果指令不存在或被删除，则返回 **std::nullopt**|
  |std::optional<command> get_command_last(command_index index) const|按照提供的 **index** 指令编号获取此指令的上一条指令 **command** 结构的拷贝。如果指令不存在或被删除，则返回 **std::nullopt**|
  |std::optional<command*> get_command_ptr(command_index index)|按照提供的 **index** 指令编号获取 **command** 结构的指针，如果指令不存在或被删除，则返回 **std::nullopt**|
  |std::optional<command\*> get_command_ptr_next(command_index index)|按照提供的 **index** 指令编号获取此指令的下一条指令 **command** 结构的指针，如果指令不存在或被删除，则返回 **std::nullopt**|
  |std::optional<command\*> get_command_ptr_last(command_index index)|按照提供的 **index** 指令编号获取此指令的上一条指令 **command** 结构的指针，如果指令不存在或被删除，则返回 **std::nullopt**|
  |std::optional<command_index> get_command_index_next(command_index index) const|按照提供的 **index** 指令编号获取此指令的下一条指令的编号，如果指令不存在或被删除，则返回 **std::nullopt**|
  |std::optional<command_index> get_command_index_last(command_index index) const|按照提供的 **index** 指令编号获取此指令的上一条指令的编号，如果指令不存在或被删除，则返回 **std::nullopt**|
  |bool del_command(command_index index)|按照提供的 **index** 指令编号删除指令，如果指令不存在或被删除，则返回 **false** ，反之为 **true**|
  |command_manager& operator=(const command_manager& other)|本类的 **=** 操作符重载|
---
5. **规则模块(rule_rule.h、rule_rule.cpp)**
- 目的：将变量与指令封装为一个“规则”，代表一个可独立执行的代码单元。支持参数设置、代码执行、中断响应，以及指令与变量生命周期的自动关联。
- 枚举类 **rule_interrupt_type**
  |类型|原类型|含义|
  |-|-|-|
  |end|data_type_unsigned_base|指令运行完成的中断|
  |call|data_type_unsigned_base|调用其他规则的中断|
  |invalid_var|data_type_unsigned_base|无法读取/写入变量的中断|
  |invalid_cmd|data_type_unsigned_base|无法读取/运行指令的中断|
  |invalid|data_type_unsigned_base|无效中断|
- 类型别名
  |数据别名|原类型|含义|
  |-|-|-|
  |rule_index|data_type_unsigned_base|规则编号|
  |var_save|operand|保存变量的类型|
  |rule_interrupt_call|operand|调用中断的参数变量类型|
  |rule_interrupt_call_type|operand_type|调用中断的参数类型|
  |rule_interrupt_value|std::variant<rule_interrupt_value_call, rule_interrupt_value_invalid>|储存不同的中断返回值类型|
  |cmd_run_func|std::function<void(rule&, rule_manager&, rule_interrupt&, command_index, command&, std::optional<command_index>&, std::vector<var_single>&, bool&)>|指令执行函数类型|
  |cmd_op_func|std::function<bool(const rule&, const operand&, command_operand&)>|指令操作数提取函数类型|
- 结构 **rule_interrupt_value_call**
  |字段|含义|
  |-|-|
  |rule_index target|调用的目标规则|
  |rule_interrupt_call param_set|参数传递的起始变量|
  |var_save save|保存被调用函数执行完成后的返回值的变量|
  |command_index next|执行调用指令后的下一条指令|
- 结构 **rule_interrupt_value_call**
  |字段|含义|
  |-|-|
  |command_index index|出错的指令编号|
  |operand op_error|出错的操作数|
- 结构 **rule_interrupt**
  |字段|含义|
  |-|-|
  |rule_interrupt_type type|中断类型|
  |std::optional<rule_interrupt_value> value|中断内容|
  |std::string info|中断文本|
- 结构 **cmd_funcs**
  |字段|含义|
  |-|-|
  |cmd_run_func run|指令执行函数|
  |cmd_op_func op_get|指令操作数获取函数|
- 类 **command_manager** 公开接口
  |接口|作用|
  |-|-|
  |bool add_var_by_op_single(const operand& op)|按照提供的单个操作数 **op**，根据其类型和编号在对应变量池中创建变量(增加引用计数)。成功返回 **true** ，否则为 **false**|
  |bool del_var_by_op_single(const operand& op)|按照提供的单个操作数 **op**，根据其类型和编号在对应变量池中删除变量(减少引用计数)。成功返回 **true** ，否则为 **false**|
  |bool add_var_by_op(const command_operand& op)|按照提供的指令操作数集合 **op**，遍历其中的每一个操作数并去重后，在对应变量池中创建变量。成功返回 **true**，否则为 **false**|
  |bool del_var_by_op(const command_operand& op)|按照提供的指令操作数集合 **op**，遍历其中的每一个操作数并去重后，在对应变量池中删除变量。成功返回 **true**，否则为 **false**|
  |std::optional<var_single> get_var_value(const operand& op) const|按照提供的操作数 **op**，获取其指向的变量内容。支持立即数直接返回、普通变量直接读取、代理变量间接寻址后读取。如果变量不存在，则返回 **std::nullopt**|
  |bool get_var_value(const operand& op, size_t quan, std::vector<var_single>& save) const|按照提供的操作数 **op**、读取数量 **quan** 和保存容器 **save**，从操作数指向的变量开始，连续读取 **quan** 个变量内容到 **save** 中。支持立即数(只能读1个)、普通变量、代理变量等。如果读取失败或数量不足，返回 **false**，反之为 **true**|
  |bool set_var_value(const operand& op_src, var_single value, bool is_allow_dynamic_alloc = true)|按照提供的目标操作数 **op_src** 和内容 **value**，设置变量内容。如果 **is_allow_dynamic_alloc** 为 **true** 且目标变量不存在，则自动创建后再写入；为 **false** 则只写入已存在的变量。支持代理变量的间接寻址写入。写入成功返回 **true**，否则为 **false**|
  |bool set_var_value(const operand& op_src, const std::vector<var_single>& value_all)|按照提供的目标操作数 **op_src** 和内容序列 **value_all**，从目标变量开始，连续写入多个变量内容。支持代理变量的间接寻址。写入成功返回 **true**，否则为 **false**|
  |bool get_op_by_op(const operand& op_begin, size_t quantity, std::vector<var_single>& save) const|按照提供的起始操作数 **op_begin** 和数量 **quantity**，从该操作数指向的变量开始，连续读取 **quantity** 个变量内容到 **save** 中。用于将一段连续的变量数据作为操作数参数来源。成功返回 **true**，否则为 **false**|
  |bool get_op_by_var(var_single type, var_single content, operand& save, operand_type invalid_type = operand_type::invalid) const|按照提供的类型码 **type** 和内容 **content**，构造一个操作数并保存到 **save** 中。**type** 对5取模后映射到操作数类型(0=立即数，1=参数变量，2=普通变量，3=参数代理，4=普通代理)。如果映射后的类型等于 **invalid_type** ，则返回 **false**|
  |bool get_op_by_cmd_op(command_type cmd_type, const operand& op, std::unordered_map<size_t, cmd_funcs>& cmd_set, command_operand& save) const|按照提供的指令类型 **cmd_type** 和起始操作数 **op**，调用对应指令的操作数提取函数，从变量中解析出完整的操作数集合并保存到 **save** 中。成功返回 **true**，否则为 **false**|
  |void get_info_by_cmd(const command& cmd_op, std::vector<var_single>& save) const|按照提供的指令 **cmd_op**，提取其完整信息(指令类型码 + 每个操作数的类型码和内容)并按序保存到 **save** 中。主要用于查询指令时的信息导出|
  |bool set_param(const std::vector<var_single>& param)|按照提供的参数内容序列 **param**，设置当前规则的参数变量池的内容。成功返回 **true**，否则为 **false**|
  |std::optional<size_t> get_param_quantity() const|获取当前规则的参数数量，返回 **std::optional<size_t>**。如果没有参数，则返回 **std::nullopt**|
  |std::optional<command_index> get_begin() const|获取当前规则的起始指令编号，如果不存在，则返回 **std::nullopt**|
  |std::optional<command_index> get_end() const|获取当前规则的结束指令编号，如果不存在，则返回 **std::nullopt**|
  |bool set_begin(command_index index)|设置当前规则的起始指令编号，失败返回 **false**，反之为 **true**|
  |bool set_end(command_index index)|设置当前规则的结束指令编号，失败返回 **false**，反之为 **true**|
  |std::optional<command> get_command(command_index index) const|按照提供的 **index** 指令编号获取指令的拷贝。如果指令不存在或被删除，则返回 **std::nullopt**|
  |std::optional<command_index> add_command(const command& command_new, command& ref_update, command_index index_update)|按照提供的 **command_new** 添加新指令，同时用 **ref_update** 返回 **index_update** 编号处的指令的最新状态(用于指令自修改时的引用更新)。添加成功返回新指令的编号，否则返回 **std::nullopt**|
  |std::optional<command_index> add_command(const command& command_new)|按照提供的 command_new 添加新指令，同时自动为新指令中引用的变量增加引用计数。添加成功返回新指令的编号，如果添加失败或变量引用失败(会回滚删除指令)，则返回 **std::nullopt**|
  |template<typename... operand> std::optional<command_index> add_command(command_type type, operand&&... op)|按照提供的指令类型 **type** 和操作数参数包 **op** ，自动打包并调用 **add_command** 添加指令。添加成功返回新指令的编号，否则返回 **std::nullopt**|
  |std::optional<command_index> insert_command(command_index index_insert, const command& command_new, command& ref_update, command_index index_update)|按照提供的 **index_insert** 插入位置和 **command_new** 新指令信息，在指定位置前插入新指令，同时用 **ref_update** 返回 **index_update** 编号处的指令的最新状态。插入成功返回新指令的编号，否则返回 **std::nullopt**|
  |std::optional<command_index> insert_command(command_index index_insert, const command& command_new)|按照提供的 **index_insert** 插入位置和 **command_new** 新指令信息，在指定位置前插入新指令，同时自动为新指令中引用的变量增加引用计数。插入成功返回新指令的编号，如果插入失败或变量引用失败(会回滚删除指令)，则返回 **std::nullopt**|
  |template<typename... operand> std::optional<command_index> insert_command(command_index index_insert, command_type type, operand&&... op)|按照提供的插入位置 **index_insert**、指令类型 **type** 和操作数参数包 **op**，自动打包并调用 **insert_command** 插入指令。插入成功返回新指令的编号，否则返回 **std::nullopt**|
  |bool set_command(command_index index, const command& command_new)|按照提供的 **index** 指令编号和 **command_new** 新指令信息，替换指定位置的指令。该操作是事务性的：先移除旧指令的变量引用，再设置新指令并添加新变量引用，任何一步失败都会回滚到原始状态。成功返回 **true** ，否则为 **false**|
  |template<typename... operand> bool set_command(command_index index, command_type type, operand&&... op)|按照提供的指令编号 **index**、指令类型 **type** 和操作数参数包 **op**，自动打包并调用 **set_command** 修改指令。成功返回 **true**，否则为 **false**|
  |bool del_command(command_index index, command& ref_update, command_index index_update)|按照提供的 **index** 指令编号删除指令，同时用 **ref_update** 返回 **index_update** 编号处的指令的最新状态。成功返回 **true**，否则为 **false**|
  |bool del_command(command_index index)|按照提供的 **index** 指令编号删除指令，同时自动移除该指令对变量的引用计数。成功返回 **true** ，否则为 **false**|
  |void run(rule_manager& onwership, rule_interrupt& interrupt, const std::optional<command_index>& begin, std::vector<var_single>& result, std::unordered_map<size_t, cmd_funcs>& cmd_set)|从指定的起始指令 **begin** (若为空则从规则起始指令开始)开始，按序执行当前规则中的指令。执行过程中通过 **interrupt** 返回中断信息，通过 **result**返回执行结果，通过 **cmd_set** 查找指令对应的执行函数。执行循环会持续到遇到结束中断或错误中断为止|
  |rule& operator=(const rule& other)|本类的 = 操作符重载|
- 类 **command_manager** 公开成员变量
  |成员字段|含义|
  |-|-|
  |var_manager rule_var_param|参数变量池，用于规则间调用时接收参数|
  |var_manager rule_var_common|普通变量池，用于规则内部的通用计算|
  |command_manager rule_command|指令管理器，储存当前规则的所有指令|
---
6. **规则管理器模块(rule_rule_manager.h、rule_rule_manager.cpp)**
- 目的：集中管理所有规则的集合，提供规则的增删查改功能。同时作为整个执行器的顶层调度器，管理调用栈，处理跨规则调用的参数传递、上下文切换和返回值保存。
- 结构 **rule_unit**
  |字段|含义|
  |-|-|
  |rule_index index|规则编号|
  |rule content|规则内容|
- 结构 **rule_manager_stack**
  |字段|含义|
  |-|-|
  |rule_index index|调用者的规则编号，用于调用返回时恢复上下文|
  |command_index next|调用者的下一条指令编号，用于调用返回后继续执行|
  |var_save save|保存被调用规则返回值的起始变量信息|
- 类 **command_manager** 公开接口
  |接口|作用|
  |-|-|
  |bool test_validity_rule_index(rule_index index, bool is_test_del) const|按照提供的 **index** 规则编号以及 **is_test_del** 的值，检测规则是否有效。如果规则不存在或 **is_test_del** 为真且当前规则已被删除，则返回 **false**，反之为 **true**|
  |std::optional<rule_unit*> get_rule_unit_ptr(rule_index index)|按照提供的 **index** 规则编号获取规则的 **rule_unit\*** 结构指针。如果规则不存在或已被删除，则返回 **std::nullopt**|
  |std::optional<const rule_unit\*> get_rule_unit_ptr_const(rule_index index) const|按照提供的 **index** 规则编号获取规则的 **rule_unit\*** 结构常量指针。如果规则不存在或已被删除，则返回 **std::nullopt**|
  |std::optional<rule_index> get_free()|获取空闲的已删除规则的编号(获取的是最近删除的)，如果没有，则返回 **std::nullopt**|
  |bool add_free(rule_index index)|按照提供的 **index** 规则编号删除指定的规则，并添加到空闲列表中|
  |bool get_param(rule_index target_src, const rule_interrupt_value_call& src, std::vector<var_single>& save) const|按照提供的参数来源规则编号 **target_src** 和调用中断信息 **src**，从来源规则的变量池中提取参数内容并保存到 **save** 中。支持立即数、普通变量、代理变量等多种参数传递方式。成功返回 **true**，否则为 **false**|
  |void get_operand_text(const operand& op, std::string& save) const|按照提供的操作数 **op**，将其信息格式化为可读的文本并保存到 **save** 中。主要用于调试输出|
  |void get_interrupt_text(rule_index index, const rule_interrupt& interrupt, std::string& save) const|按照提供的规则编号 **index** 和中断信息 **interrupt**，将中断的详细信息格式化为可读的文本并保存到 **save** 中。主要用于调试输出|
  |bool set_begin(rule_index index)|设置起始规则编号，失败返回 **false**，反之为 **true**|
  |bool set_param(rule_index index, const std::vector<var_single>& param)|按照提供的规则编号 **index** 和参数内容序列 **param**，设置目标规则的参数变量池。成功返回 **true**，否则为 **false**|
  |std::optional<rule_index> add_rule()|创建一个新的空白规则，返回新规则的编号。如果创建失败，则返回 **std::nullopt**|
  |bool del_rule(rule_index index)|按照提供的 **index** 规则编号删除规则，如果规则不存在或已被删除，则返回 **false**，反之为 **true**|
  |std::optional<rule\*> get_rule(rule_index index)|按照提供的 **index** 规则编号获取规则的指针。如果规则不存在或已被删除，则返回 **std::nullopt**|
  |void set_add_command(size_t cmd_type, const cmd_funcs& funcs_new)|按照提供的指令类型码 **cmd_type** 和指令函数对 **funcs_new**，注册或重载一条指令的执行函数和操作数提取函数。可在编译前或运行时动态添加|
  |bool run(std::vector<var_single>& result)|从起始规则开始执行整个规则系统。执行过程中自动管理调用栈：遇到 **call** 中断时保存上下文并切换到目标规则；遇到 **end** 中断时从栈中弹出栈帧，恢复调用者上下文并设置返回值。执行结果保存在 **result** 中。成功返回 **true**，否则为 **false**|
- 类 **command_manager** 公开成员变量
  |成员字段|含义|
  |-|-|
  |rule_index begin||
  |std::vector<rule_unit> rule_set|起始规则的编号，执行器的入口点|
  |std::vector<rule_unit> rule_set|所有规则的集合|
  |std::vector<rule_index> rule_set_free|已删除规则的空闲编号列表，用于复用|
  |std::unordered_map<size_t, cmd_funcs> funcs|指令类型码到指令函数对(执行函数 + 操作数提取函数)的映射表|
  |std::vector<rule_manager_stack> stack|调用栈，用于管理跨规则调用的上下文保存和恢复|
---
7. **指令函数模块(rule_funcs_run_base.cpp、rule_funcs_op_get_base.cpp)**
- 目的：定义每条基本指令的具体执行逻辑和操作数提取逻辑。所有指令函数的声明在 **rule_rule_manager.h** 中通过 **extern** 导出，实现分布在对应的 **.cpp** 文件中，便于外部重载和自定义扩展。
- **基础指令的操作数格式**
  |指令|操作数数量|操作数含义(按序)|
  |-|-|-|
  |mov|2|目标变量, 来源变量|
  |cmp|3|左比较变量, 右比较变量, 结果保存变量|
  |jmp|1|跳转目标变量|
  |je/jne/ja/jae/jb/jbe|2|跳转目标变量, 比较结果变量|
  |add/sub/mul/div|3|左操作变量, 右操作变量, 结果保存变量|
  |call|3|目标规则变量, 参数起始变量, 返回值保存变量|
  |ret|2|返回内容起始变量, 返回数量变量|
  |acmd|3|指令类型变量, 操作数起始变量, 新指令编号保存变量|
  |scmd|3|目标指令编号变量, 新指令类型变量, 操作数起始变量|
  |imcd|4|目标指令编号变量, 新指令类型变量, 操作数起始变量, 新指令编号保存变量|
  |dcmd|1|目标指令编号变量|
  |gcmd|2|目标指令编号变量, 指令信息保存变量|
  |gcmdnext|3|目标指令编号变量, 下一条指令编号保存变量, 指令信息保存变量|
  |gcmdlast|3|目标指令编号变量, 上一条指令编号保存变量, 指令信息保存变量|
  |arule|1|新规则编号保存变量|
  |drule|1|目标规则编号变量|
  |acmdex|4|目标规则编号变量, 指令类型变量, 操作数起始变量, 新指令编号保存变量|
  |scmdex|4|目标规则编号变量, 目标指令编号变量, 新指令类型变量, 操作数起始变量|
  |icmdex|5|目标规则编号变量, 目标指令编号变量, 新指令类型变量, 操作数起始变量, 新指令编号保存变量|
  |dmcdex|2|目标规则编号变量, 目标指令编号变量|
  |gcmdex|3|目标规则编号变量, 目标指令编号变量, 指令信息保存变量|
  |gcmdnextex|4|目标规则编号变量, 目标指令编号变量, 下一条指令编号保存变量, 指令信息保存变量|
  |gcmdlastex|4|目标规则编号变量, 目标指令编号变量, 上一条指令编号保存变量, 指令信息保存变量|
- 所有指令执行函数遵循统一的函数签名
  ```
  void func_name
  (
    rule& owner_rule,           // 当前执行的规则
    rule_manager& owner_rule_manager, // 规则管理器
    rule_interrupt& interrupt,  // 中断信息输出
    command_index cmd_index_current, // 当前指令编号
    command& cmd_current,       // 当前指令内容
    std::optional<command_index>& cmd_index_next, // 下一条指令编号(可为空，为空则自动取当前指令的下一条)
    std::vector<var_single>& result, // 执行结果输出
    bool& is_end                // 是否结束执行
  );
  ```
- 所有指令操作数提取函数遵循统一的函数签名
  ```
  bool func_name
  (
    const rule& owner,          // 当前规则(只读)
    const operand& op,          // 起始操作数
    command_operand& save       // 解析出的完整操作数集合
  );
  ```
## 使用方法
1. 基础准备——创建规则管理器并添加规则
- 所有操作都从 **rule_manager** 开始。首先创建一个管理器实例，然后添加规则。
  ```
  #include "rule.h"

  int main()
  {
    // 1. 创建规则管理器
    rule_manager manager;

    // 2. 添加一个新规则，获取规则编号
    std::optional<rule_index> rule_id = manager.add_rule();
    if (rule_id.has_value() == false)
        return -1; // 创建失败

    // 3. 设置此规则为起始规则（执行入口）
    manager.set_begin(rule_id.value());

    // 4. 获取规则指针，方便后续操作
    std::optional<rule*> rule_ptr = manager.get_rule(rule_id.value());
    if (rule_ptr.has_value() == false)
        return -1;

    rule* my_rule = rule_ptr.value();

    // ... 后续添加指令等操作 ...

    return 0;
  }
  ```
2. **添加指令——构建指令流水**  
通过 **rule** 的 **add_command** 方法添加指令。每条指令由 **指令类型** 和 **操作数** 组成。  
- 2.1 **最简单的指令——移动数据 (mov)**
  ```
  // 功能：将立即数 42 移动到 1 号普通变量中
  // mov variable_common(1) , immediate(42)
  // 即：变量1 = 42
  my_rule->add_command(
      command_type::mov,
      variable_common(1),   // 目标：1号普通变量
      immediate(42)          // 来源：立即数42
  ); 
  ```
- **2.2 算术运算——加法 (add)**
  ```
  // 预先设置两个变量的值
  // 变量10 = 100
  my_rule->add_command(command_type::mov, variable_common(10), immediate(100));
  // 变量20 = 200
  my_rule->add_command(command_type::mov, variable_common(20), immediate(200));

  // 加法：变量30 = 变量10 + 变量20
  // add variable_common(10) , variable_common(20) , variable_common(30)
  my_rule->add_command(
      command_type::add,
      variable_common(10),   // 左操作数：变量10
      variable_common(20),   // 右操作数：变量20
      variable_common(30)    // 结果保存到：变量30
  );
  ```
- **2.3 比较与条件跳转 (cmp + je)**
  ```
  // 设置变量 1 = 10, 变量 2 = 10
  my_rule->add_command(command_type::mov, variable_common(1), immediate(10));
  my_rule->add_command(command_type::mov, variable_common(2), immediate(10));

  // 比较：变量3 = 比较结果(变量1 vs 变量2)
  // cmp variable_common(1) , variable_common(2) , variable_common(3)
  my_rule->add_command(
      command_type::cmp,
      variable_common(1),    // 左比较数
      variable_common(2),    // 右比较数
      variable_common(3)     // 结果保存（0=相等, 1=大于, 2=小于）
  );

  // 等于则跳转：如果变量3 == equal，则跳转到变量4指向的指令
  // je immediate(目标指令编号) , variable_common(3)
  // 此处假设目标指令编号为 100（需要通过变量存储）
  my_rule->add_command(command_type::mov, variable_common(4), immediate(100));
  my_rule->add_command(
      command_type::je,
      variable_common(4),    // 跳转目标（变量4的值 = 100）
      variable_common(3)     // 比较结果
  );
  ```
3. **使用代理变量（间接寻址）**
代理变量的内容是另一个变量的编号，读取时会自动二次寻址。
```
  // 1. 设置变量 5 = 999
my_rule->add_command(command_type::mov, variable_common(5), immediate(999));

// 2. 设置代理变量 10 的内容为 5（即指向变量5）
my_rule->add_command(command_type::mov, variable_common(10), immediate(5));

// 3. 通过代理变量读取：proxy_common(10) → 先读变量10得5 → 再读变量5得999
// mov variable_common(20) , proxy_common(10)
// 结果：变量20 = 999
my_rule->add_command(
    command_type::mov,
    variable_common(20),   // 目标
    proxy_common(10)       // 来源（代理，实际读取变量5的内容）
);
```
4. **运行时自修改——动态添加/修改/删除指令**  
这是本执行器的核心特性，指令可以在运行时修改自身或其他指令。
- 4.1 **动态添加指令 (acmd)**
  ```
  // 假设变量 50 存储了新指令类型（mov = 0），
  // 变量 51、52 存储了操作数信息（连续存放）
  // 变量 60 用于保存新指令的编号

  // 先准备好"指令模板"变量
  my_rule->add_command(command_type::mov, variable_common(51), immediate(0));         // 类型码：0 = mov
  my_rule->add_command(command_type::mov, variable_common(52), immediate(1));         // 类型码：1 = variable_common
  my_rule->add_command(command_type::mov, variable_common(53), immediate(100));       // 内容：变量编号 100
  my_rule->add_command(command_type::mov, variable_common(54), immediate(0));         // 类型码：0 = immediate
  my_rule->add_command(command_type::mov, variable_common(55), immediate(888));       // 内容：立即数 888

  // acmd：在本规则末尾添加一条指令
  // 参数：指令类型变量, 操作数起始变量, 新指令编号保存变量
  my_rule->add_command(
      command_type::acmd,
      variable_common(51),   // 指令类型（mov）
      variable_common(52),   // 操作数起始位置（从变量52开始连续读取）
      variable_common(60)    // 新指令编号保存到变量60
  );
  // 执行后，规则末尾会多一条：mov variable_common(100), immediate(888)
  // 变量60 中存储了新指令的编号
  ```
- 4.2 **动态修改指令 (scmd)**
  ```
  // 假设要修改编号为 10 的指令，将其改为 mov variable_common(200), immediate(777)
  // 先准备好操作数变量
  my_rule->add_command(command_type::mov, variable_common(70), immediate(10));  // 目标指令编号
  my_rule->add_command(command_type::mov, variable_common(71), immediate(0));   // 新指令类型：mov
  my_rule->add_command(command_type::mov, variable_common(72), immediate(1));   // 类型码：variable_common
  my_rule->add_command(command_type::mov, variable_common(73), immediate(200)); // 变量编号 200
  my_rule->add_command(command_type::mov, variable_common(74), immediate(0));   // 类型码：immediate
  my_rule->add_command(command_type::mov, variable_common(75), immediate(777)); // 立即数 777

  // scmd：修改本规则的指定指令
  // 参数：目标指令编号变量, 新指令类型变量, 操作数起始变量
  my_rule->add_command(
      command_type::scmd,
      variable_common(70),   // 目标指令编号
      variable_common(71),   // 新指令类型
      variable_common(72)    // 操作数起始位置
  );
  ```
- 4.3 **动态插入指令 (icmd)**
  ```
  // 在编号为 5 的指令之前插入一条新指令
  my_rule->add_command(command_type::mov, variable_common(80), immediate(5));   // 插入位置（在指令5之前）

  // icmd：在指定位置前插入指令
  // 参数：目标位置指令编号变量, 新指令类型变量, 操作数起始变量, 新指令编号保存变量
  my_rule->add_command(
      command_type::icmd,
      variable_common(80),   // 插入位置
      variable_common(71),   // 新指令类型
      variable_common(72),   // 操作数起始位置
      variable_common(61)    // 新指令编号保存到变量61
  );
  ```
- 4.4 **动态删除指令 (dcmd)**
  ```
  // 删除编号为 10 的指令
  my_rule->add_command(command_type::mov, variable_common(90), immediate(10));

  my_rule->add_command(
      command_type::dcmd,
      variable_common(90)    // 要删除的指令编号
  );
  ```
- 4.5 **查询指令信息 (gcmd)**
  ```
  // 查询编号为 10 的指令的完整信息
  my_rule->add_command(command_type::mov, variable_common(91), immediate(10));

  // gcmd：查询指令信息，结果保存到指定变量开始的连续区域
  // 保存格式：[指令类型, 操作数1类型, 操作数1内容, 操作数2类型, 操作数2内容, ...]
  my_rule->add_command(
      command_type::gcmd,
      variable_common(91),   // 目标指令编号
      variable_common(92)    // 查询结果保存的起始变量编号
  );
  ```
5. **跨规则调用——类似函数调用**
- 5.1 **创建被调用规则**
  ```
  // 创建一个新规则作为"子函数"
  std::optional<rule_index> sub_rule_id = manager.add_rule();
  std::optional<rule*> sub_rule_ptr = manager.get_rule(sub_rule_id.value());
  rule* sub_rule = sub_rule_ptr.value();

  // 子规则的逻辑：将参数1和参数2相加，结果存到参数3
  // 参数变量用 variable_param，普通变量用 variable_common
  // variable_param(0) = 第一个参数，variable_param(1) = 第二个参数
  sub_rule->add_command(
      command_type::add,
      variable_param(0),     // 第一个参数
      variable_param(1),     // 第二个参数
      variable_param(2)      // 第三个参数（结果）
  );

  // 返回：ret 返回内容起始变量, 返回数量
  // 返回2个变量：结果值 和 一个标志
  sub_rule->add_command(
      command_type::ret,
      variable_param(2),     // 从参数3开始
      immediate(1)           // 返回1个变量
  );
  ```
- 5.2 **主规则调用子规则**
  ```
  // 准备参数：设置变量 100=10, 101=20 作为传入参数
  my_rule->add_command(command_type::mov, variable_common(100), immediate(10));
  my_rule->add_command(command_type::mov, variable_common(101), immediate(20));

  // call：调用目标规则
  // 参数：目标规则编号, 参数起始变量, 返回值保存变量
  my_rule->add_command(
      command_type::call,
      immediate(static_cast<var_single>(sub_rule_id.value())),  // 目标规则编号
      variable_common(100),  // 参数起始（从变量100开始连续读取）
      variable_common(200)   // 返回值保存到变量200开始的位置
  );
  // 调用后，变量200中存储了子规则的返回结果（10+20=30）
  ```
6. **跨规则自修改——修改其他规则的指令 (acmdex)**
```
// 在外部规则（sub_rule）的末尾添加一条指令
my_rule->add_command(command_type::mov, variable_common(110),
    immediate(static_cast<var_single>(sub_rule_id.value())));  // 目标规则编号
my_rule->add_command(command_type::mov, variable_common(111), immediate(0));  // 指令类型：mov
my_rule->add_command(command_type::mov, variable_common(112), immediate(1));  // 操作数类型：variable_common
my_rule->add_command(command_type::mov, variable_common(113), immediate(50)); // 变量编号 50
my_rule->add_command(command_type::mov, variable_common(114), immediate(0));  // 操作数类型：immediate
my_rule->add_command(command_type::mov, variable_common(115), immediate(123)); // 立即数 123

// acmdex：对外部规则添加指令
// 参数：目标规则编号变量, 指令类型变量, 操作数起始变量, 新指令编号保存变量
my_rule->add_command(
    command_type::acmdex,
    variable_common(110),  // 目标规则编号
    variable_common(111),  // 指令类型
    variable_common(112),  // 操作数起始位置
    variable_common(120)   // 新指令编号保存到变量120
);
// 执行后，sub_rule 末尾新增：mov variable_common(50), immediate(123)
```
7. **动态创建和删除规则 (arule / drule)**
```
// arule：创建一个新规则，编号保存到变量150
my_rule->add_command(
    command_type::arule,
    variable_common(150)   // 新规则编号保存位置
);

// drule：删除一个规则
// 先设置要删除的规则编号
my_rule->add_command(command_type::mov, variable_common(151),
    immediate(static_cast<var_single>(some_rule_id)));

my_rule->add_command(
    command_type::drule,
    variable_common(151)   // 要删除的规则编号
);
```
8. **执行规则**
所有指令添加完成后，调用 rule_manager 的 run 方法开始执行。
```
std::vector<var_single> result;
bool success = manager.run(result);

if (success)
{
    // 执行成功，result 中包含最终返回值
    std::cout << "执行成功" << std::endl;
}
else
{
    // 执行失败，查看控制台输出的中断信息定位问题
    std::cout << "执行失败" << std::endl;
}
```
9. **自定义指令**  
可以通过 set_add_command 方法注册自定义指令或重载已有指令。
```
// 1. 定义自定义指令的执行函数
cmd_run_func my_custom_run = [](
    rule& owner_rule,
    rule_manager& owner_rule_manager,
    rule_interrupt& interrupt,
    command_index cmd_index_current,
    command& cmd_current,
    std::optional<command_index>& cmd_index_next,
    std::vector<var_single>& result,
    bool& is_end
) -> void
{
    // 自定义逻辑：例如将第一个操作数乘以2
    auto& op_set = std::get<operand_set<oq::one>>(cmd_current.op);
    std::optional<var_single> val = owner_rule.get_var_value(op_set[oi::first]);
    
    if (val.has_value())
    {
        owner_rule.set_var_value(op_set[oi::first], val.value() * 2);
    }
    
    is_end = false;
    cmd_index_next = std::nullopt; // 自动执行下一条
};

// 2. 定义自定义指令的操作数提取函数
cmd_op_func my_custom_op_get = [](
    const rule& owner,
    const operand& op,
    command_operand& save
) -> bool
{
    // 从变量中提取操作数
    std::vector<var_single> param_get;
    if (owner.get_op_by_op(op, 2, param_get) == false)
        return false;
    
    operand target;
    if (owner.get_op_by_var(param_get[0], param_get[1], target) == false)
        return false;
    
    save.emplace<static_cast<size_t>(oq::one)>(target);
    return true;
};

// 3. 注册自定义指令（使用 command_type 未占用的枚举值，或重载已有指令）
// 假设自定义指令的类型码为 100
manager.set_add_command(100, cmd_funcs(my_custom_run, my_custom_op_get));
```
10. **完整示例——计算 1+2 并动态修改指令**
```
#include "rule.h"
#include <iostream>

int main()
{
    rule_manager manager;

    // 创建主规则
    auto rule_id = manager.add_rule();
    manager.set_begin(rule_id.value());
    auto rule_ptr = manager.get_rule(rule_id.value());
    rule* r = rule_ptr.value();

    // 步骤1：变量1=1，变量2=2
    r->add_command(command_type::mov, variable_common(1), immediate(1));
    r->add_command(command_type::mov, variable_common(2), immediate(2));

    // 步骤2：变量3 = 变量1 + 变量2 → 结果应为3
    r->add_command(command_type::add,
        variable_common(1), variable_common(2), variable_common(3));

    // 步骤3：将 add 指令改为 sub（减法），验证自修改
    // 先找到 add 指令的编号（假设为2，即第3条指令，编号从0开始）
    // 准备 scmd 的操作数
    r->add_command(command_type::mov, variable_common(10), immediate(2));   // 目标指令编号
    r->add_command(command_type::mov, variable_common(11), immediate(3));   // 新类型：sub
    r->add_command(command_type::mov, variable_common(12), immediate(1));   // 操作数类型
    r->add_command(command_type::mov, variable_common(13), immediate(1));   // 变量1
    r->add_command(command_type::mov, variable_common(14), immediate(1));   // 操作数类型
    r->add_command(command_type::mov, variable_common(15), immediate(2));   // 变量2
    r->add_command(command_type::mov, variable_common(16), immediate(1));   // 操作数类型
    r->add_command(command_type::mov, variable_common(17), immediate(4));   // 结果存到变量4

    r->add_command(command_type::scmd,
        variable_common(10),    // 目标指令编号
        variable_common(11),    // 新指令类型（sub）
        variable_common(12)     // 操作数起始
    );

    // 步骤4：再次执行 add 位置（现已被改为 sub）
    // 但由于指令已被修改，实际执行的是 sub
    // 添加一条跳转指令回到那被修改的指令重新执行
    r->add_command(command_type::mov, variable_common(5), immediate(2));
    r->add_command(command_type::jmp, variable_common(5));

    // 执行
    std::vector<var_single> result;
    if (manager.run(result))
    {
        // 第一次执行 add：变量3 = 1+2 = 3
        // 修改指令后第二次执行 sub：变量4 = 1-2 = -1
        std::cout << "执行成功" << std::endl;
    }
    else
    {
        std::cout << "执行失败" << std::endl;
    }

    return 0;
}

```
11. **接口使用注意事项**
- 变量无需声明：变量在首次被指令引用时自动创建，无需预先定义。
- 变量编号策略：建议参数变量使用小编号(如 0-99)，普通变量使用大编号(如 100+)，便于区分。
- 操作数顺序：每条指令的操作数都有固定顺序和含义，请参考指令函数模块中的操作数格式说明。
- 指令编号：指令编号从 **0** 开始递增，也可通过 **acmd/icmd** 返回的新编号获取。
- 错误处理：执行过程中的错误会通过控制台输出中断信息，包含规则编号、指令编号、出错操作数等详细定位信息。
- 自定义指令：使用 **set_add_command** 注册时，类型码建议使用较大数值(如 100 以上)，避免与内置指令冲突。
## 注意事项
- **C++20**：此项目需要C++20及以上的支持。
- **单线程执行**：当前不支持多线程并行执行，所有规则在单线程内顺序调度。
- **无类型系统**：所有数据均为 **unsigned long long**，不区分整数、浮点数、字符串等类型。
- **无持久化**：当前不支持将规则和指令序列保存到文件或从文件加载。
- **无外部 I/O 指令**：内置指令集中没有输入输出指令(可通过自定义指令扩展)。
- **错误恢复有限**：遇到运行时错误会立即终止执行，暂无错误恢复机制。







  
  

