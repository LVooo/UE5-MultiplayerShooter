# UE5-MultiplayerShooter
## 开发日记：
---
## 一、项目创建
### 1. 加入插件

### 2. 资源导入

### 3. 动画重定向

### 4. 角色创建

### 5. 角色动画

### 6. 无缝传送和大厅
1) Non-Seamless Travel(非无缝传送)：客户端从服务器端断开连接，客户端重连至相同服务器。发生情况：第一次加载地图，第一次连接至服务器，当结束或开始一个多人游戏。  
2) Seamless Travel（无缝切换）：客户端无需从服务器断开连接，避免了重联时遇到的网络问题，更加丝滑。在`Game Mode`中启用，需要一个transition map，这个地图需要已经被加载完成（这样就省去了同时存在两张大地图的资源消耗）。 

### 7. 网络角色
分别有以下几种不同的网络角色：
- `ENetRole::ROLE_Authority`: 服务器上的权威角色
- `ENetRole::ROLE_SimulatedProxy`: 客户端上看到的其他玩家控制的角色
- `ENetRole::ROLE_AutonomousProxy`: 自己客户端上可控制的角色
- `ENetRole::ROLE_None`: 没有定义角色的actor

local role和remote role的区别：
- local role：在客户端上显示的为控制的角色为`Autonomous`和其他客户端上的角色`Simulated`；而在服务器端角色显示都为`Authority`
- remote role：在客户端上显示的为在服务器端的网络角色即`Authority`；而在服务器端则显示控制的角色为`Autonomous`和相应的`Simulated`


## 二、武器
### 1. 创建武器类

### 2. 拾取武器显示

### 3. 武器复制变量
在网络系统中注册了可复制的变量，解决了只能在服务端显示拾取武器控件的问题，能通过服务端复制武器到客户端，使用`DOREPLIFETIME_CONDITION`来限制只在有所有权的服务端或客户端复制武器，其他客户端不会同步该武器。  
并且通过委托函数（通过宏设置`ReplicatedUsing`）来触发回调函数`OnRep_`（里面能够通过参数保存原属性的旧值）来设置人物与武器重叠时的拾取武器控件的显示与关闭。

### 4. 装备武器
- 使用了Actor Component来创建一个在人物角色上的新组件用来装备武器
- 为`CombatComponent`添加人物角色的友元类以将所有访问权限暴露给角色类
- 在多个类中为了获取私有变量，创建内联函数`FORCEINLINE`的get或set函数来减少性能开销。在内联函数中，函数体的代码被肢解插入到调用该函数的地方，因此不需要进行函数调用的相关开销，如压栈、跳转、返回值处理等。相比于函数调用，内联函数的执行速度更快。