#ifndef _HTTP_T_
#define _HTTP_T_
#include<cstring>
#include<cstdio>
class http_conn
{
public:
	http_conn(){
		//初始化
		this->check_curr = 0;
		this->check_index = 0;
		this->curr_status = CHECK_LINE;
		this->m_method = GET;
		this->my_url = 0;
	}
private:
	enum LINE_STATUS{LINE_OK = 0, LINE_OPEN, LINE_BAD};
	enum REQUEST_STATUS{NO_REQUEST = 0, GET_REQUEST, BAD_REQUEST};
	enum CHECK_STATUS{CHECK_LINE = 0, CHECK_HEAD};
	enum METHOD{GET, POST};
	char* my_url;
private:
	int check_curr;
	int check_index;
	CHECK_STATUS curr_status;
	METHOD m_method;
	char* m_connection;
	char* m_host;
public:
	LINE_STATUS parse_line(char *buffer, int &check_curr, int &check_index){
		// char temp;
		for(; check_curr < check_index; check_curr++){
			// temp = buffer[check_curr];
			if(buffer[check_curr] == '\r'){
				if(check_curr + 1 < check_index){
					if(buffer[check_curr + 1] == '\n'){
						buffer[check_curr++] = '\0';
						buffer[check_curr++] = '\0';
						return LINE_OK;
					}
					else{
						return LINE_BAD;
					}
				}else{
					return LINE_OPEN;
				}

			}
			if(buffer[check_curr] == '\n'){
				if(check_curr >= 1 && buffer[check_curr - 1] == '\r'){
					buffer[check_curr - 1] = '\0';
					buffer[check_curr++] = '\0';
					return LINE_OK;
				}
				else{
					return LINE_BAD;
				}
			}
		}
		return LINE_OPEN;
	}	


public:
	REQUEST_STATUS parse_request_line(char* buffer, CHECK_STATUS &curr_status){
		my_url = strpbrk(buffer, " \t");
		if(my_url == NULL){
			printf("http请求行格式错误\n");
			return BAD_REQUEST;
		}
		*my_url++ = '\0';
		char* method = buffer;
		if(strcasecmp(method, "get") == 0){
			m_method = GET;
		}
		else if(strcasecmp(method, "post") == 0){
			m_method = POST;
		}
		else{
			return BAD_REQUEST;
		}
		my_url += strspn(my_url, " \t");
		char* version = strpbrk(my_url, " \t");
		if(version == NULL){
			printf("http请求行格式错误\n");
			return BAD_REQUEST;
		}
		*version++ == '\0';			
		version += strspn(version, " \t");
		if(strcasecmp(version, "HTTP/1.1") != 0){
			return BAD_REQUEST;
		}

		if(strncasecmp(my_url, "http://", 7) == 0){
			my_url += 7;
			my_url = strchr(my_url, '/');
		}else if(strncasecmp(my_url, "https://", 8) == 0){
			my_url += 8;
			my_url = strchr(my_url, '/');
		}
		if(!my_url || my_url[0] != '\0'){
			return BAD_REQUEST;
		}

		curr_status = CHECK_HEAD;
		return NO_REQUEST;
	}	

public:
	REQUEST_STATUS parse_request_head(char* temp){
		if(temp[0] = '\0')
		{
			printf("获取了一个完整的报文头");
			return GET_REQUEST;
		}
		if(strncasecmp(temp, "CONNECTION:", 11) == 0){
			temp += 11;
			temp += strspn(temp, " \t");
			m_connection = temp;
			return NO_REQUEST;
		}
		if(strncasecmp(temp, "HOST:", 5) == 0){
			temp += 5;
			temp += strspn(temp, " \t");
			m_host = temp;
			return NO_REQUEST;
		}else{
			printf("未知请求头字段\n");
		}
		

		return NO_REQUEST;
	}
	

};

#endif