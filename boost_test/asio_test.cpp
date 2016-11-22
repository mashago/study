//下面是一个异步模式的简单的Tcp echo服务器
#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/smart_ptr.hpp>

using boost::asio::ip::tcp;
using boost::system::error_code;

struct CHelloWorld_Service
{
    //类的初始化创建：设置io_service, 设置1000端口
    CHelloWorld_Service(boost::asio::io_service &iosev)
         :m_acceptor(iosev, tcp::endpoint(tcp::v4(), 7710))
    {
    }
    
    //创建一个tcp的socket；且还是侦听
    void start()
    {
        // 开始等待连接（非阻塞）
         boost::shared_ptr<tcp::socket> psocket(new tcp::socket(m_acceptor.get_io_service()));

        // 触发的事件只有error_code参数，所以用boost::bind把socket绑定进去
         m_acceptor.async_accept(*psocket, boost::bind(&CHelloWorld_Service::accept_handler, this, psocket, _1) );
     }

    // 有客户端连接时accept_handler触发
    void accept_handler(boost::shared_ptr<tcp::socket> psocket, error_code ec)
     {
        if(ec) return;

        // 继续等待连接
         start();

        // 显示远程IP
         std::cout << psocket->remote_endpoint().address() << std::endl;

        // 发送信息(非阻塞)
         boost::shared_ptr<std::string> pstr(new std::string("hello async world!\n"));
         psocket->async_write_some(boost::asio::buffer(*pstr),
             boost::bind(&CHelloWorld_Service::write_handler, this, pstr, _1, _2)
             );
     }

    // 异步写操作完成后write_handler触发
    void write_handler(boost::shared_ptr<std::string> pstr, error_code ec, size_t bytes_transferred)
     {
        if(ec)
             std::cout<< "发送失败!" << std::endl;
        else
             std::cout<< *pstr << " 已发送" << std::endl;
     }

private:
     tcp::acceptor m_acceptor;
};


int main(int argc, char* argv[])
{
     //建立io服务器
     boost::asio::io_service iosev;

     CHelloWorld_Service sev(iosev);

    //开始侦听socket的连接；和开始接收远程数据
     sev.start();

     //开始执行回调函数
     iosev.run();

    return 0;
}
