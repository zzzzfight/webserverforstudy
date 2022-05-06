#include<mysql/mysql.h>
#include<iostream>
#include<assert.h>
#include<list>
#include"./test/locker.h"

class mysql_conn_info{
public:
	char* m_host = "127.0.0.1";			 //主机地址
	char* m_User = "root";		 //登陆数据库用户名
	char* m_PassWord = "myNewPwd@123";	 //登陆数据库密码
	char* m_DatabaseName = "mysql"; //使用数据库名
	unsigned int m_Port = 3306;		 //数据库端口号
	char* m_unix_socket = nullptr;
	unsigned long m_client_flag = 0;	
};

class sqlConn{
private:
	MYSQL* conn;
	static sqlConn* sqlconn1;
	sqlConn(mysql_conn_info info):m_info(info){
		conn = mysql_init(conn);
		assert(conn>=0);
		conn = mysql_real_connect(conn, m_info.m_host, 
		m_info.m_User, m_info.m_PassWord, m_info.m_DatabaseName,
		m_info.m_Port, m_info.m_unix_socket, m_info.m_client_flag);
	}
	mysql_conn_info m_info;

public:
	static sqlConn* sqlInit(mysql_conn_info info);
	~sqlConn(){

	}
	MYSQL* getConnetion(){
		return conn;
	}
	void clossConnection(){
		mysql_close(conn);
		std::cout<<"closed"<<std::endl;
	}
};

sqlConn* sqlConn::sqlconn1 = nullptr;
sqlConn* sqlConn::sqlInit(mysql_conn_info info){
	if(sqlconn1 == nullptr){
		sqlconn1 = new sqlConn(info);
		assert(sqlconn1 >= 0);
	}
	return sqlconn1;
}








int main(){
	sqlConn* test = sqlConn::sqlInit(mysql_conn_info());
	if(!(test->getConnetion())){
		std::cout<<"true\n"<<std::endl;
	}
	test->clossConnection();
	delete test;
}