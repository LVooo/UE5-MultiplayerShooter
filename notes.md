# 基础AI与行为树

## UE中的AI系统
- Navigation Mesh `提供允许AI移动区域的数据（常用于寻路系统）`
- Behavior Tree `AI的大脑，执行顺序由上至下由左至右`
  - Selector：选中一个子节点执行（首先判断Decorators
  - Sequence：从左至右按顺序执行所有子节点
  - Task：动作行为比如攻击、移动
  - Service：重复执行背景任务
  - Decorator：条件检查、流程控制
- Blackboard `AI的存储区域，数据存储，无逻辑`
- Environment Query System `查找掩体位置等的空间查询`
- PawnSensing & AIPreception `视觉和听觉`