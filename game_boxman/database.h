#pragma once
#include <string>
#ifndef DATABASE_H


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

struct userinfo
{
	int id;
	std::string name;
	std::string pwd;
	int level;
};

struct mapinfo {
	int id;
	int row;
	int col;
	std::string data;
	int next;
};

bool fetch_user_info(userinfo& user);

bool fetch_map_info(mapinfo& map,int map_id);



#endif
