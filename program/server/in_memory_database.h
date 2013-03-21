#include <vector>
#include <string>
#include "news_group.h"
#include "database.h"
#include <utility>
#include "article.h"
namespace database{
class in_memory_database : database{
	public:

		in_memory_database();
		~in_memory_database();
		void print_status(int i);
		int add_news_group(std::string& name);
		std::vector<news_group> list_news_groups();
		bool delete_news_group(unsigned int id);
		bool add_article(unsigned int id, std::string& title, std::string& author, std::string& content);
		int delete_article(unsigned int group_id, unsigned int article_id);
		article* get_article(unsigned int group_id, unsigned int article_id);
	private:
	unsigned int news_group_ids;
	std::vector<std::string> ngnames;
	std::vector<news_group> ngroups;
};
}
