#include "database.h"
#include <mysql.h>
#include <iostream>
using namespace std;

#define SCHEMA "boxman"
#define HOST "127.0.0.1"
#define PORT 3306
#define ADMIN "root"
#define ADMINPWD "qq010925"

//data structure
/*schema boxman

table players
id int NN PK
name varchar(45) NN
pwd varchar(45) NN
level int

table maps
id int NN PK
row_num int NN
col_num int NN
data varchar(1024) NN
next int

*/



static bool connect_boxman(MYSQL& mysql);

//mysql connection
bool connect_boxman(MYSQL& mysql)
{
	mysql_init(&mysql);
	mysql_options(&mysql, MYSQL_SET_CHARSET_NAME, "gbk");

	if (mysql_real_connect(&mysql, HOST, ADMIN, ADMINPWD, SCHEMA, PORT, NULL, 0) == NULL)
	{
		cout << "database connection failed:" << mysql_error(&mysql) << endl;
		exit(-1);
		return false;
	}

	return true;
}

//user login check
bool fetch_user_info(userinfo& user)
{
	MYSQL mysql;
	char sql[256];
	bool ret;
	MYSQL_RES* res;
	MYSQL_ROW row;

	//connect
	if (connect_boxman(mysql) == false)
	{
		return false;
	}

	//search sql
	snprintf(sql, 256, "select id, pwd, level from players where name='%s'",user.name.c_str());

	//get info
	ret = mysql_query(&mysql, sql);
	if (ret)
	{
		cout << "query failed" << mysql_error(&mysql);
		mysql_close(&mysql);
		return false;
	}

	res = mysql_store_result(&mysql);
	row = mysql_fetch_row(res);
	if (row == NULL)
	{
		mysql_free_result(res);
		mysql_close(&mysql);
		cout << "no such account" << endl;
		return false;
	}

	user.id = atoi(row[0]);
	user.pwd = row[1];
	user.level = atoi(row[2]);

	mysql_free_result(res);
	mysql_close(&mysql);

	return true;

}

//get map
bool fetch_map_info(mapinfo& map, int map_id)
{
	MYSQL mysql;
	char sql[256];
	bool ret;
	MYSQL_RES* res;
	MYSQL_ROW row;

	//connect
	if (connect_boxman(mysql) == false)
	{
		return false;
	}

	//search sql
	snprintf(sql, 256, "select row_num, col_num, data, next from maps where id='%d'", map_id);

	//get info
	ret = mysql_query(&mysql, sql);
	if (ret)
	{
		cout << "query failed" << mysql_error(&mysql);
		mysql_close(&mysql);
		return false;
	}

	res = mysql_store_result(&mysql);
	row = mysql_fetch_row(res);
	if (row == NULL)
	{
		mysql_free_result(res);
		mysql_close(&mysql);
		cout << "no such map" << endl;
		return false;
	}

	map.row = atoi(row[0]);
	map.col = atoi(row[1]);
	map.data = row[2];
	map.next = atoi(row[3]);

	mysql_free_result(res);
	mysql_close(&mysql);

	return true;
}