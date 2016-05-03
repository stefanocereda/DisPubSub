cplusplus {{
	#include <map>
	typedef std::vector<std::map<int, int>> ts_map_dict;
}}

class noncobject ts_map_dict;

message Message_msg{
    int topic=-1;
   	ts_map_dict ts_struct;
   	int senderId=-1;
   	char content = 'a'; 
}