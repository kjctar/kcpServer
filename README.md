# kcpServer
基于UDP+KCP+Epoll的消息转发服务器
- [x] 高并发udp消息转发
- [x] 双缓存日志
- [x] 定时器
- [x] kcp

├── base 
│   ├── doubleBufferLog.cpp //双缓存日志
│   ├── doubleBufferLog.h
│   ├── Epoll.cpp //epoll的封装
│   ├── Epoll.h
│   ├── Timer.cpp //timerfd定时器
│   ├── Timer.h
│   ├── timerQueue.cpp //定时任务队列
│   └── timerQueue.h
├── build.sh
├── kcp
│   ├── ikcp.c //kcp协议
│   └── ikcp.h
├── main
├── main.cpp
├── net
│   ├── kcpConnection.cpp //kcp连接，继承于udpConnection
│   ├── kcpConnection.h
│   ├── udpConnection.cpp //udp连接
│   ├── udpConnection.h
│   ├── udpServer.cpp //监听端口，端口复用 ，创建新的udpConnection或者kcpConnection
│   └── udpServer.h
├── README.md
└── test
    ├── kcpClient.cpp //测试kcp
    ├── testlog.cpp //测试日志
    ├── timerTest   
    ├── timerTest.cpp//测试定时
    ├── udpClient
    └── udpClient.cpp//测试udp收发