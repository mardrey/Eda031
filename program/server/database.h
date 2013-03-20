#ifndef DATABASE_H
#define DATABASE_H
#include <string>
#include "news_group.h"
#include <vector>
namespace database{
class database{
	public:
	database();
	~database();
	void add_news_group(std::string& name);
	virtual void print_status(int i) = 0;
	private:
	unsigned int news_group_ids;
	std::vector<std::string> ngnames;
	std::vector<news_group> ngroups;
};
}

#endif
