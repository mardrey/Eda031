#include <vector>
#include <string>
#include "news_group.h"
#include "article.h"
#include "database.h"
#include <dirent.h>

namespace database{
	class on_disc_database : public database{
		public:
			on_disc_database();
			~on_disc_database();
			int add_news_group(std::string& name);
			std::vector<news_group> list_news_groups();
			bool delete_news_group(int id);
			bool add_article(int id, std::string& title, std::string& author, std::string& content);
			int delete_article(int group_id, int article_id);
			article get_article(int group_id, int article_id);
			news_group get_news_group(int id);
		private:
			std::string path;
			DIR *root;
	};

}