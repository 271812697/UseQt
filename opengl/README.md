## Qt opengl

这是一个尝试在Qt下编写的OpenGL物理渲染器

**实现的功能有：**

- 使用实体组件系统ECS来管理对象
- 可以运行时加载和切换场景
- 实现了简单的骨骼动画
- 实现了简单的材质系统
- 实现了PBR 和IBL
- 使用计算着色器烘培IBL
- 使用计算着色器实现bloom效果
- 实现万向PCSS阴影效果
- 实现了可配置的线框模式绘制，避免了使用内置线框模式的锯齿现象
- 色调映射和gama矫正

**程序运行截图**

![image-20230307131554371](README.assets/image-20230307131554371-16781675923312.png)

![image-20230307131702018](README.assets/image-20230307131702018.png)