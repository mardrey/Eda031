#ifndef ARTICLE_H
#define ARTICLE_H

#include <string>


namespace database{
class article{
public:
	article(std::string s, unsigned int i);
	~article();
	std::string get_content();
	unsigned int get_id();
	bool operator==(article other);
private:
	std::string content;
	unsigned int id;
};
}
#endif
