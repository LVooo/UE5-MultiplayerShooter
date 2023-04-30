# UE5-MultiplayerShooter
## 开发日记：
---
## 一、项目创建
### 1. 加入插件

### 2. 资源导入

### 3. 动画重定向
使用骨骼约束系统（IK Rig）和骨骼重定向器（IK Retargeter）

### 4. 角色创建
移动方向：
- pitch：绕x轴（俯仰）
- yaw：绕z轴（左右方向旋转）
- roll：绕y轴（竖直方向旋转）

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

### 5. 远程过程调用（RPC）
1) Remote Procedure Call是指在客户端和服务器之间发送函数调用消息的机制。当客户端需要通知服务器执行某个函数时，客户端可以通过RPC将函数调用消息发送给服务器，并在服务器上执行该函数；服务器同理。  
2) RPC分为可靠的与不可靠的：
   - 可靠：确保RPC调用被成功接收并处理，如果网络出现问题会尝试重传RPC调用，直到成功处理。基于TCP协议，保证了数据的可靠性和正确性，比如玩家角色位置的同步
   - 不可靠：可能会在网络中被丢失。基于UDP协议，用于快速传输RPC调用，但不保证数据的可靠性和正确性；可能会出现数据丢失或顺序错乱等问题，比如玩家角色的伤害计算
3) 在UE中，对于声明了RPC的函数，需要在其后面添加一个函数名为原函数名+`_Implementation`的实现函数，用于处理服务器上实际执行该RPC所需的逻辑。这样设计的原因是：RPC系统基于反射机制（在程序运行时动态调用函数）实现的，使得RPC函数的调用和实现可以分离，从而更好的实现了代码解耦性和可维护性。

### 6. 装备武器动画姿势
因为上面仅在服务器端给武器类中的EquippedWeapon赋值了，客户端中的依然为null，这时就需要给武器类中的EquippedWeapon也设置为Replicated。  
一旦涉及这个过程都需要为该变量加上`UPROPERTY(Replicated)`，并重写`GetLifetimeReplicatedProps`虚函数。

### 7. 下蹲
UE中自带Crouch和UnCrouch内置函数（考虑了多人在线游戏因素），这个内置函数中`bIsCrouched`变量是可复制的所以可以复制到所有客户端，并且还实现了如**重置胶囊体**和移动速度切换等，大大减少了重复造轮子的时间。

Tilde符号即~，可以用作运行游戏时打开控制台的快捷键；能够查看和修改游戏状态，执行各种命令（如添加物体，碰撞检测等）

### 8. 瞄准
在`CombatComponent`中把诸如`EquippedWeapon`, `bAiming`等变量都设置为`Replicated`；这些属性将被自动在客户端上复制，从而实现服务器与客户端之间的同步。这里使用`ServerSetAiming`作为RPC会在客户端上调用服务器上的函数，服务器将更该应用于自己的状态并分发给所有已连接的客户端进行同步，因为该函数被标记为了`Server Reliable`所以只有服务器可以调用它并建立可靠的RPC服务。

>计算旋转偏移Yaw Offset过程：
1) 获取角色的基础瞄准旋转（Base Aim Rotation），即没有偏移时的瞄准旋转；
2) 获取角色的运动方向，通常是角色的速度向量，然后将其转换为一个旋转矢量；
3) 计算角色的运动方向与瞄准方向之间的旋转差（Delta Rotation）；
4) 将旋转差转换为一个浮点数值，通常是旋转差的Yaw轴（水平旋转轴）分量；
5) 使用差值算法（如线性插值或插值函数）逐渐调整Yaw Offset的值，使其向目标值靠近。

如：当玩家瞄准时，游戏会计算出身体需要旋转的角度，然后将这个角度缓慢地应用到角色的旋转中。这样，身体的旋转速度就可以逐渐与头部的旋转速度同步，从而保证射击时枪口不会偏离目标。  
在第三人称射击游戏中，角色的枪口和准心通常不在同一位置，而且准心的位置通常是固定的，不会随着角色的移动而变化。另外，角色身体的旋转和移动方向通常也不是完全与准心对齐的，这就导致了准心和角色的瞄准方向存在差异。为了让玩家能够更准确地瞄准，需要通过瞄准偏移来修正这种差异，让准心与实际瞄准方向对齐。

### 9. 横向移动以及身体倾斜
通过旋转root角度制作lean的动画，在混合空间blendspace中填入。  
为了配合身体的旋转，需要在服务端和客户端均禁用`OrientationRoration`（使用了RepNotify）  
计算身体倾斜lean时，记录了上一帧角色朝向，通过与当前帧的角色朝向的差值计算出朝向的变化率，即每秒的旋转角度；接着通过插值（Interpolation）计算出当前帧的身体倾斜状态并将其限制在-90到90度之间。对于横向则使用了`RInterpTo`使其有一个平滑的过渡。

### 10. 瞄准偏移
当人物处于装备武器站立状态时，此时人物上半身瞄准状态应该跟随鼠标移动，而下半身应保持静止状态。  
>当客户端发送Pitch（水平方向的旋转即俯仰视，限制在了-90到90度之间）到服务端时或服务器分发给客户端时，需要经过一个函数将角度压缩成size更小的short类型；因此负角度就会转换成[0->360)的度数，再通过与0xFFFF比特相与映射到[0->65536)，再传到服务器或其他客户端时再解压缩到[0->360)之间的度数，这就会造成当人物俯视时在其他客户端显示为仰视。

使用了Cashed Pose来分别存储我们装备武器后的身体以及瞄准姿态时的身体，使用layered blend按照spine_01将身体的上下部分分开；使用缓存姿势时，可以利用已经存在的动画资源，并通过设置适当的姿势转换规则来动态地生成动画，从而提高运行效率。另外，Cached Pose还支持在多个线程上进行计算，从而进一步提高效率。

**FABRIK IK（反向运动学）**，以使角色的左手到左肩这一部分（Solver）根据末端执行器右手（已Attachment绑定武器的transform以骨骼为参考系）能够自然地移动并正确地与手持枪械处于正确位置。在 Fabric IK 中，可以根据 end effector 的位置来调整 IK solver 的目标位置，以便更好地适应角色的运动和变化。 