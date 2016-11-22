
#include <string>

class TennisPlayer {
private:
	std::string first_name;
	std::string last_name;
	bool has_table;
public:
	TennisPlayer();
	TennisPlayer(const std::string &first_name, const std::string &last_name, bool has_table);

	void show();
	void reset_table(bool flag);
};

class RatedPlayer : public TennisPlayer {
private:
	int rate;
public:
	RatedPlayer(int rate=0, const std::string &first_name="nope", const std::string &last_name="nope", bool has_table=false);
	RatedPlayer(int rate, const TennisPlayer &tp);

	void reset_rate(int r);	
	int get_rate();
	void reset_name(const std::string &first_name, const std::string &last_name);
};
