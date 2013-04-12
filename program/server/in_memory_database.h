#include <vector>
#include <string>
#include "news_group.h"
#include "database.h"
#include <utility>
#include "article.h"
namespace database{
class in_memory_database : public database{
	public:

		in_memory_database();
		~in_memory_database();
		int add_news_group(std::string& name);
		std::vector<news_group> list_news_groups();
		bool delete_news_group(int id);
		bool add_article(int id, std::string& title, std::string& author, std::string& content);
		int delete_article(int group_id, int article_id);
		article get_article(int group_id, int article_id);
		news_group get_news_group(int id);
	private:
	unsigned int news_group_ids;
	std::vector<std::string> ngnames;
	std::vector<news_group> ngroups;
};
}
