#include<mysql/mysql.h>
#include<list>
#include<assert.h>
#include<iostream>
#include"../locker/locker.h"
using namespace std;

class mysqlconn_Info{
public:
	char* host = "localhost";			 //主机地址
	char* user = "root";		 //登陆数据库用户名
	char* pwd = "myNewPwd@123";	 //登陆数据库密码
	char* db = "mysql"; //使用数据库名
	unsigned int port = 3306;		 //数据库端口号
	char* unix_socket = nullptr;
	unsigned long client_flag = 0;	
};

class sqlconn{
private:
	int conn_avail = 0; //连接池中可用连接
	int conn_used = 0;   //连接池中当前已使用的连接
	static int m_maxnum;
	
	mysqlconn_Info m_info;	
	list<MYSQL*>* sqlconn_List;

	static sqlconn* connPool;
	sem reserve;
	locker lock;

public:
	static sqlconn* GetInstance(mysqlconn_Info info, int maxnum);

	void sqlconn_Init(){
		conn_used = 0;
		conn_avail = 0;
		sqlconn_List = new list<MYSQL*>(); //初始化list
		assert(sqlconn_List!=nullptr);
		// lterator<> it;
		// MYSQL* con[m_maxnum] = {nullptr};
		// MYSQL* con = nullptr;
		MYSQL *con = nullptr;
		for(int i = 0; i < m_maxnum; i++){
			// con = mysql_init(con);
			con = nullptr;	
			con = mysql_init(con);
			assert(con != nullptr);
			
			// con = mysql_real_connect(con, m_info.host, m_info.user, m_info.pwd, m_info.db, m_info.port, m_info.unix_socket, m_info.client_flag);
			mysql_real_connect(con, m_info.host, m_info.user, m_info.pwd, m_info.db, m_info.port, m_info.unix_socket, m_info.client_flag);
			// assert(&con != nullptr);
			(*sqlconn_List).push_back(con);
			++conn_avail;
		}
		// m_maxnum = maxnum;	
		reserve = sem(conn_avail);
		std::cout<<"init success"<<std::endl;
	}

	MYSQL* sqlconn_GetConnection(){
		MYSQL* temp_con = nullptr; 
		if(0 == (*sqlconn_List).size()){
			return nullptr;
		}
		// reserve.front();
		reserve.wait();
		lock.lock();
		temp_con = (*sqlconn_List).front();
		(*sqlconn_List).pop_front();
		--conn_avail;
		++conn_used;
		lock.unlock();
		std::cout<<"get success"<<std::endl;
		return temp_con;
	}

	bool sqlconn_ReleaseConnection(MYSQL* con){
		if(nullptr == con){
			return false;
		}

		lock.lock();

		(*sqlconn_List).push_back(con);
		++conn_avail;
		--conn_used;

		lock.unlock();
		reserve.post();

		std::cout<<"release success"<<std::endl;
		return true;
	}

	~sqlconn(){
		// delete sqlconn_List;
		sqlconn_DestroyPool();
		std::cout<<"delete success"<<std::endl;
	}
	

private:
	void sqlconn_DestroyPool(){
		lock.lock();
		if(!sqlconn_List){
			return;
		}
		list<MYSQL*>::iterator it;
		for(it = (*sqlconn_List).begin(); it != (*sqlconn_List).end(); it++)
		{ 	
			// MYSQL temp = it;
			mysql_close(*it);
		}
		conn_avail = 0;
		conn_used = 0;
		(*sqlconn_List).clear();
		delete sqlconn_List;
		lock.unlock();
	}

	sqlconn(mysqlconn_Info info): m_info(info){

	}

public:



};

sqlconn* sqlconn::connPool = nullptr;
sqlconn* sqlconn::GetInstance(mysqlconn_Info info, int maxnum){
	if(!connPool)
		connPool = new sqlconn(info);
		sqlconn::m_maxnum = maxnum;
		std::cout<<"construct success"<<std::endl;
	return connPool;
}
int sqlconn::m_maxnum = 0;

//test
// int main(){
// 	mysqlconn_Info info;
// 	sqlconn* testpool = sqlconn::GetInstance(info, 8);
// 	testpool->sqlconn_Init();
// 	MYSQL* testcon = testpool->sqlconn_GetConnection();
// 	MYSQL* testcon01 = testpool->sqlconn_GetConnection();
// 	testpool->sqlconn_ReleaseConnection(testcon01);
// 	testpool->sqlconn_ReleaseConnection(testcon);
// 	delete testpool;
// }