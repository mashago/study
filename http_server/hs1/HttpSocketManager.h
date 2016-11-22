#ifndef __HTTPSOCKET_H__
#define __HTTPSOCKET_H__
#include <netinet/in.h>
#include <string>

class HttpSocketManager
{
private:
	int main_fd;
	int client_fd;
	struct sockaddr_in sin;
	HttpSocketManager();
	~HttpSocketManager();
	static HttpSocketManager *m_pThis;
public:
	static HttpSocketManager * GetThis() { return m_pThis; };
	static void Create() { m_pThis = new HttpSocketManager; };
	static void Destory() { delete m_pThis; m_pThis=NULL; };
	int Init(std::string addr, int port);
	int Wait(int timeout);
	int Recv(char *buff, int maxSize);
	int Send(const char *buff, int size);
	int Disconnect();
};
#endif
