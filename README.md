# 2019CodeCraft
队名：西北小靓仔

文件介绍：
code1：初赛代码

map1：初赛地图

code2：复赛代码

map2：复赛地图


思路简单介绍：
1.根据地图信息创建双向图。

2.对车进行预处理（排序），确定发车顺序。

3.根据双向图利用Dijstra或者Floyd确定车的路径。

4.每一辆车经过一条道路则为此道路增加权重。

5.权重挥发。即除了道路本身所具有的的权重，后来增加的权重都具有挥发性，让其以一定速率进行挥发


具体分析请看我的博客：https://www.cnblogs.com/qiang-wei/p/10705503.html

https://www.cnblogs.com/qiang-wei/p/10705479.html
