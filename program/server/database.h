#ifndef DATABASE_H
#define DATABASE_H
#include <string>

namespace database{
class database{
	public:
	database();
	~database();
	virtual void print_status(int i) = 0;
	private:
};
}

#endif