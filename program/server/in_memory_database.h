namespace database{
class in_memory_database : database{
	public:
		in_memory_database(){}
		~in_memory_database(){}
		void print_status(int i);
		void add_news_group(Newsgroup n);
		void remove_news_group(Newsgroup n);
	private:

};
}