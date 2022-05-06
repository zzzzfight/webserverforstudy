#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <errno.h>
#include <sys/stat.h>
#include <string.h>
#include <stdio.h>
#include <iostream>
// #include<unistd.h>
// #include<sys/uio.h>
#include<sys/mman.h>
#include<fcntl.h>
using namespace std;
class http_conn
{
public:
	enum METHOD
	{
		GET = 0,
	};
	enum CHECK_STATE
	{
		CHECK_STATE_REQUESTLINE = 0,
		CHECK_STATE_HEADER,
		CHECK_STATE_CONTENT
	};
	enum HTTP_CODE
	{
		NO_REQUEST,
		GET_REQUEST,
		BAD_REQUEST,
		NO_RESOURCE,
		FORBIDDEN_REQUEST,
		FILE_REQUEST,
		INTERNAL_ERROR,
		// CLOSED_CONNECTION
	};
	enum LINE_STATUS
	{
		LINE_OK = 0,
		LINE_BAD,
		LINE_OPEN
	};

public:
	http_conn() = default;
	~http_conn() = default;

	HTTP_CODE parse_request_line(char *text);
	HTTP_CODE parse_headers(char *text);
	HTTP_CODE parse_content(char *text);
	HTTP_CODE do_request();
	HTTP_CODE process_read();
	HTTP_CODE do_request();
	bool read_once();

	void init(int sockfd);
	// char *get_line()
	// {
	// 	return m_read_buf + m_start_line;
	// }
	LINE_STATUS parse_line();

public:
	// private:
	static const int READ_BUFFER_SIZE = 1024;
	static const int WRITE_BUFFER_SIZE = 1024;
	static const int FILENAME_LEN = 200;
	// int m_checked_id = 0;
	// char m_read_buf[READ_BUFFER_SIZE];
	/*该HTTP连接的socket和对方的socket地址*/
	int m_sockfd;
	sockaddr_in m_address;
	/*读缓冲区*/
	char m_read_buf[READ_BUFFER_SIZE];
	/*标识读缓冲中已经读入的客户数据的最后一个字节的下一个位置*/
	int m_read_idx;
	/*当前正在分析的字符在读缓冲区中的位置*/
	int m_checked_idx;
	/*当前正在解析的行的起始位置*/
	int m_start_line;
	/*写缓冲区*/
	char m_write_buf[WRITE_BUFFER_SIZE];
	/*写缓冲区中待发送的字节数*/
	int m_write_idx;
	/*主状态机当前所处的状态*/
	CHECK_STATE m_check_state;
	/*请求方法*/
	METHOD m_method;
	/*客户请求的目标文件的完整路径，其内容等于doc_root+m_url，doc_root是网站
	根目录*/
	char m_real_file[FILENAME_LEN];
	/*客户请求的目标文件的文件名*/
	char *m_url;
	/*HTTP协议版本号，我们仅支持HTTP/1.1*/
	char *m_version;
	/*主机名*/
	char *m_host;
	/*HTTP请求的消息体的长度*/
	int m_content_length;
	/*HTTP请求是否要求保持连接*/
	bool m_linger;
	/*客户请求的目标文件被mmap到内存中的起始位置*/
	char *m_file_address;
	/*目标文件的状态。通过它我们可以判断文件是否存在、是否为目录、是否可读，并获
	取文件大小等信息*/
	struct stat m_file_stat;
	/*我们将采用writev来执行写操作，所以定义下面两个成员，其中m_iv_count表示
	被写内存块的数量*/
	struct iovec m_iv[2];
	int m_iv_count;

private:
	int m_TRIGMode = 0;
	char *get_line() { return m_read_buf + m_start_line; };
	char *doc_root;
};

void http_conn::init(int sockfd = 0)
{
	m_checked_idx = 0;
	m_read_idx = 0;
	m_start_line = 0;
	m_sockfd = sockfd;
	doc_root = "./root";

	memset(m_read_buf, '\0', READ_BUFFER_SIZE);
	memset(m_write_buf, '\0', WRITE_BUFFER_SIZE);
	memset(m_real_file, '\0', FILENAME_LEN);
}

http_conn::LINE_STATUS http_conn::parse_line()
{
	char temp;
	for (; m_checked_idx < m_read_idx; ++m_checked_idx)
	{
		temp = m_read_buf[m_checked_idx];
		if (temp == '\r')
		{
			if ((m_checked_idx + 1) == m_read_idx)
				return LINE_OPEN;
			else if (m_read_buf[m_checked_idx + 1] == '\n')
			{
				m_read_buf[m_checked_idx++] = '\0';
				m_read_buf[m_checked_idx++] = '\0';
				return LINE_OK;
			}
			return LINE_BAD;
		}
		else if (temp == '\n')
		{
			if (m_checked_idx > 1 && m_read_buf[m_checked_idx - 1] == '\r')
			{
				m_read_buf[m_checked_idx - 1] = '\0';
				m_read_buf[m_checked_idx++] = '\0';
				return LINE_OK;
			}
			return LINE_BAD;
		}
	}
	return LINE_OPEN;
}
http_conn::HTTP_CODE http_conn::parse_request_line(char *text)
{
	m_url = strpbrk(text, " \t");
	if (!m_url)
	{
		return BAD_REQUEST;
	}
	*m_url++ = '\0';

	// printf("%s\n",m_url);

	char *method = text;
	if (strcasecmp(method, "GET") == 0)
	{
		m_method = GET;
	}
	else
	{
		return BAD_REQUEST;
	}
	m_url += strspn(m_url, " \t");
	m_version = strpbrk(m_url, " \t");
	if (!m_version)
	{
		return BAD_REQUEST;
	}
	*m_version++ = '\0';
	m_version += strspn(m_url, " \t");
	if (strcasecmp(m_version, "HTTP/1.1") != 0)
	{
		return BAD_REQUEST;
	}
	if (strncasecmp(m_url, "http://", 7) == 0)
	{
		m_url += 7;
		m_url = strchr(m_url, '/');
	}
	if (!m_url || m_url[0] != '/')
	{
		return BAD_REQUEST;
	}
	if (strlen(m_url) == 1)
	{
		strcat(m_url, "judge.html");
	}
	m_check_state = CHECK_STATE_HEADER;
	return NO_REQUEST;
}

http_conn::HTTP_CODE http_conn::process_read()
{
	LINE_STATUS line_status = LINE_OK;
	HTTP_CODE ret = NO_REQUEST;
	char *text = 0;

	while ((m_check_state == CHECK_STATE_CONTENT && line_status == LINE_OK) || ((line_status = parse_line()) == LINE_OK))
	{
		text = get_line();
		m_start_line = m_checked_idx;
		// LOG_INFO("%s", text);
		switch (m_check_state)
		{
		case CHECK_STATE_REQUESTLINE:
		{
			ret = parse_request_line(text);
			if (ret == BAD_REQUEST)
				return BAD_REQUEST;
			break;
		}
		case CHECK_STATE_HEADER:
		{
			ret = parse_headers(text);
			if (ret == BAD_REQUEST)
				return BAD_REQUEST;
			else if (ret == GET_REQUEST)
			{
				// cout<<"do request"<<endl;
				return do_request();
			}
			break;
		}
		// case CHECK_STATE_CONTENT:
		// {
		// 	ret = parse_content(text);
		// 	if (ret == GET_REQUEST)
		// 		return do_request();
		// 	line_status = LINE_OPEN;
		// 	break;
		// }
		default:
		{
			cout << "default" << endl;
			// return;
		}
			return INTERNAL_ERROR;
		}
	}
	return NO_REQUEST;
}

http_conn::HTTP_CODE http_conn::parse_headers(char *text)
{
	if (text[0] == '\0')
	{
		if (m_content_length != 0)
		{
			m_check_state = CHECK_STATE_CONTENT;
			return NO_REQUEST;
		}
		return GET_REQUEST;
	}
	else if (strncasecmp(text, "Connection:", 11) == 0)
	{
		text += 11;
		text += strspn(text, " \t");
		if (strcasecmp(text, "keep-alive") == 0)
		{
			m_linger = true;
		}
	}
	else if (strncasecmp(text, "Content-length:", 15) == 0)
	{
		text += 15;
		text += strspn(text, " \t");
		m_content_length = atol(text);
	}
	else if (strncasecmp(text, "Host:", 5) == 0)
	{
		text += 5;
		text += strspn(text, " \t");
		m_host = text;
	}
	else
	{
		// LOG_INFO("oop!unknow header: %s", text);
	}
	return NO_REQUEST;
}

bool http_conn::read_once()
{
	if (m_read_idx >= READ_BUFFER_SIZE)
	{
		return false;
	}
	int bytes_read = 0;

	// LT读取数据
	if (0 == m_TRIGMode)
	{
		bytes_read = recv(m_sockfd, m_read_buf + m_read_idx, READ_BUFFER_SIZE - m_read_idx, 0);
		m_read_buf[bytes_read] = 0;
		m_read_idx += bytes_read;

		if (bytes_read <= 0)
		{
			return false;
		}

		return true;
	}
	// ET读数据
	else
	{
		while (true)
		{
			bytes_read = recv(m_sockfd, m_read_buf + m_read_idx, READ_BUFFER_SIZE - m_read_idx, 0);
			if (bytes_read == -1)
			{
				if (errno == EAGAIN || errno == EWOULDBLOCK)
					break;
				return false;
			}
			else if (bytes_read == 0)
			{
				return false;
			}
			m_read_idx += bytes_read;
		}
		return true;
	}
}

http_conn::HTTP_CODE http_conn::do_request()
{
	strcpy(m_real_file, doc_root);
	int len = strlen(doc_root);
	// const char*p = strrchr(m_url, '/');

	if (0)
	{
	}
	else
	{
		strncpy(m_real_file + len, m_url, FILENAME_LEN - len - 1);
	}
	if (stat(m_real_file, &m_file_stat) < 0)
		return NO_RESOURCE;

	if (!(m_file_stat.st_mode & S_IROTH))
		return FORBIDDEN_REQUEST;

	if (S_ISDIR(m_file_stat.st_mode))
		return BAD_REQUEST;

	int fd = open(m_real_file, O_RDONLY);
	m_file_address = (char *)mmap(0, m_file_stat.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
	close(fd);
	return FILE_REQUEST;
}