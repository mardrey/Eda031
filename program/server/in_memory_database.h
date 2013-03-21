#include <vector>
#include <string>
#include "news_group.h"
#include "database.h"
#include <utility>

namespace database{
class in_memory_database : database{
	public:
		in_memory_database();
		~in_memory_database();
		void print_status(int i);
		void add_news_group(std::string& name);
		std::vector<news_group> list_news_groups();
	private:
	unsigned int news_group_ids;
	std::vector<std::string> ngnames;
	std::vector<news_group> ngroups;
};
}