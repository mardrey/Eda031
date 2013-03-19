#ifndef ARTICLE_H
#define ARTICLE_H

#include <string>


namespace database{
class article{
public:
	article(std::string cont, unsigned int i,std::string titl, std::string aut);
	~article();
	std::string get_content();
	unsigned int get_id();
	std::string get_title();
	std::string get_author;
	bool operator==(article other);
private:
	std::string content;
	std::string title;
	std::string author;
	unsigned int id;
};
}
#endif
