#ifndef _MSG_HEAD_H_
#define _MSG_HEAD_H_

#include <arpa/inet.h>

struct msg_head_t
{
	enum {MSG_HEAD_LEN = 40};

	int32_t cmd_id;
	int32_t body_len;
	uint32_t uin;
	uint32_t cmd_seq;

	uint32_t msg_index;
	uint32_t msg_type; // notice or request

	uint32_t send_server_id;
	uint32_t recv_server_id;

	uint32_t tmp1;
	uint32_t tmp2;

	msg_head_t()
	{
		cmd_id = 0;
		body_len = 0;
		uin = 0;
		cmd_seq = 0;
		msg_index = 0;
		msg_type = 0;
		send_server_id = 0;
		recv_server_id = 0;
		tmp1 = 0;
		tmp2 = 0;
	}

	void init()
	{
		cmd_id = 0;
		body_len = 0;
		uin = 0;
		cmd_seq = 0;
		msg_index = 0;
		msg_type = 0;
		send_server_id = 0;
		recv_server_id = 0;
		tmp1 = 0;
		tmp2 = 0;
	}

	char *pack(char *buffer)
	{
		char *ptr = buffer;

		*((uint32_t *)ptr) = htonl(cmd_id);
		ptr += 4;

		*((uint32_t *)ptr) = htonl(body_len);
		ptr += 4;

		*((uint32_t *)ptr) = htonl(uin);
		ptr += 4;

		*((uint32_t *)ptr) = htonl(cmd_seq);
		ptr += 4;

		*((uint32_t *)ptr) = htonl(msg_index);
		ptr += 4;

		*((uint32_t *)ptr) = htonl(msg_type);
		ptr += 4;

		*((uint32_t *)ptr) = htonl(send_server_id);
		ptr += 4;

		*((uint32_t *)ptr) = htonl(recv_server_id);
		ptr += 4;

		*((uint32_t *)ptr) = htonl(tmp1);
		ptr += 4;

		*((uint32_t *)ptr) = htonl(tmp2);
		ptr += 4;

		return ptr;
	}

	char *unpack(char *buffer)
	{
		char *ptr = buffer;

		cmd_id = (int32_t)ntohl(*((uint32_t *)ptr));
		ptr += 4;

		body_len = (int32_t)ntohl(*((uint32_t *)ptr));
		ptr += 4;

		uin = (uint32_t)ntohl(*((uint32_t *)ptr));
		ptr += 4;

		cmd_seq = (uint32_t)ntohl(*((uint32_t *)ptr));
		ptr += 4;

		msg_index = (uint32_t)ntohl(*((uint32_t *)ptr));
		ptr += 4;

		msg_type = (uint32_t)ntohl((*(uint32_t *)ptr));
		ptr += 4;

		send_server_id = (uint32_t)ntohl(*((uint32_t *)ptr));
		ptr += 4;

		recv_server_id = (uint32_t)ntohl(*((uint32_t *)ptr));
		ptr += 4;

		tmp1 = (uint32_t)ntohl(*((uint32_t *)ptr));
		ptr += 4;

		tmp2 = (uint32_t)ntohl(*((uint32_t *)ptr));
		ptr += 4;

		return ptr;
	}
};

#endif
