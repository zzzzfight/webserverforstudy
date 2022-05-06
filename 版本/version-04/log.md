## day one
* 初步实现one loop per thread功能，但是没有考虑到安全性因素和一些线程信息的存储（暂时也没必要设计这些）
## day two
* 设计了事件分发
	* epoll对事件的监听进行增删改。
	* channel是每个事件fd的封装，负责管理事件fd及event状态，回调函数等。
	* httpdata类将事件包装成http协议下的一个事件类，负责业务层上的信息存储，管理http链接的各类信息，如缓存
	* EventLoop 负责对处理主逻辑，事件到达时的操作

* 问题
	* 有bug 无法建立连接进行正常的收发信息。