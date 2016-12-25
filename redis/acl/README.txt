MAIN OPERATION

1.create redis connect
a.normal mode
acl::redis_client client(char * addr, int conn_timeout, int rw_timeout);
acl::redis_string str_op(&client);
acl::redis_key key_op(&client);

b.cluster mode
acl::redis_client_cluster client(char * addr, int conn_timeout, int rw_timeout);
acl::redis_string str_op(&client, int max_conns);
acl::redis_key key_op(&client, int max_conns);

2.set k/v
bool str_op.set(char *key, char *value);
bool str_op.set(char *key, size_t key_len, char *value, size_t value_len);

3.get k/v
bool str_op.get(char *key, acl::string value);
bool str_op.get(char *key, size_t key_len, acl::string value);

4.exist key
bool key_op.exists(char *key);

5.delete
int num = key_op.del_one(char *key);
int num = key_op.del(char **keys, size_t size);
