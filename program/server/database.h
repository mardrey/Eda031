#ifndef DATABASE_H
#define DATABASE_H
#include <string>


namespace database{
class database{
	public:
	database(){};
	~database(){};
	//virtual void add_news_group(std::string& name)=0;
	virtual void print_status(int i);
	virtual void add_news_group(std::string& name);
	virtual void list_news_groups();
	private:

};
}

#endif
