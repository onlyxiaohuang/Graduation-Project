### Vector DB Searching Engine & Visualization

大体设计思路：

* 1.后端：用cpp实现论文中的几个算法，再通过存入Json文件实现数据的存储。
* 2.前端：调用greuler库实现内容的实时展现。
* 3.优化算法&对比：后端实现多个算法，分别从召回率、性能等指标进行对比。

todo:

1、2 & 3

* -> 配置javascript & g++环境

* -> 看论文,实现算法

* -> 调研greuler库

On the way:
FINGER优化路由算法 two-stage-routing

Done:

DiskANN


hnswlib库：
https://github.com/nmslib/hnswlib/tree/master

greuler库:
https://github.com/mauriciopoppe/greuler

数据集：
http://corpus-texmex.irisa.fr/

压测:
https://github.com/wg/wrk