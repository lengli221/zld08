# zld08

#### 介绍
MD 12A V1.4 提测通过版本--》V388/V389

#### 软件架构
软件架构说明


#### 安装教程

1.  xxxx
2.  xxxx
3.  xxxx

#### 使用说明

20210805--》（补）20210730
1.新增:博兰德充电器固件识别逻辑

20210812 --》11:14 --》修改美团需求:
1.升级过程中拔掉分控与主控的通讯线,充电柜界面应该显示分控失联(10S内)
	1.1 美团12A V1.5 20210812--增加分控进入bootloader检测是否失联,
		策略:唐工分控回复0x03,但数据长度限制在长度为0x01-->不更新状态数据
	1.2 修改分控失联时间:10S
2.下载/升级不可中断
	2.1 主控向分控已发起升级,下载电池固件,下载充电器固件--》不终止下载固件信息
	2.2 安卓--》向主控发起下载分控,电池固件,充电器固件,立即终止,否则一级告警信息无法响应
	2.3 安卓升级主控,不终止	
3.软件版本:
	CDG_MD_B_B1_B300_392.bin/CDG_MD_B_B1_B300_393.bin

#### 参与贡献

1.  Fork 本仓库
2.  新建 Feat_xxx 分支
3.  提交代码
4.  新建 Pull Request


#### 特技

1.  使用 Readme\_XXX.md 来支持不同的语言，例如 Readme\_en.md, Readme\_zh.md
2.  Gitee 官方博客 [blog.gitee.com](https://blog.gitee.com)
3.  你可以 [https://gitee.com/explore](https://gitee.com/explore) 这个地址来了解 Gitee 上的优秀开源项目
4.  [GVP](https://gitee.com/gvp) 全称是 Gitee 最有价值开源项目，是综合评定出的优秀开源项目
5.  Gitee 官方提供的使用手册 [https://gitee.com/help](https://gitee.com/help)
6.  Gitee 封面人物是一档用来展示 Gitee 会员风采的栏目 [https://gitee.com/gitee-stars/](https://gitee.com/gitee-stars/)
