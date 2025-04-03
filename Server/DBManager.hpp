#include <mysql/jdbc.h>

using namespace std;
using namespace sql;

#define SERVER_IP "127.0.0.1:3306"
#define USERNAME "root"
#define PASSWORD "12345"
#define DATABASE "Pokemon"

struct skill {
	int s_id;
	string name;
	float atk;
	int type;
};

struct Poketmon {
	int id;
	string name;
	string type;
	vector<skill> skills;
};

struct Player {
	string id;
	string password;
	int x = 2;
	int y = 39;
	string name;
	int win;
	int lose;
	int level;
	double EXP;
	string dir;

	Player() {}
	Player(int x, int y, string dir = "DOWN")
		: x(x), y(y), dir(dir)
	{
	}
	Player(string id, string password, int x, int y, string name, int win, int lose, int level, double EXP, string dir = "DOWN")
		: id(id)
		, password(password)
		, x(x)
		, y(y)
		, name(name)
		, win(win)
		, lose(lose)
		, level(level)
		, EXP(EXP)
		, dir(dir)
	{
	}
	bool isEmpty() {
		return id.empty();
	}
};

class DBManager {
private:

	// 기본 정보 로딩
	vector<Player> Players;
	vector<Poketmon> POKETMONS;
	// DB 연결 멤버
	unique_ptr<Connection> conn;
public:
	DBManager()
	{
		try {
			mysql::MySQL_Driver* driver = mysql::get_mysql_driver_instance();
			conn = unique_ptr<Connection>(driver->connect(SERVER_IP, USERNAME, PASSWORD));
			conn->setSchema(DATABASE);
			cout << "MySQL Connection success\n" << endl;
		}
		catch (SQLException& e) {
			cout << "MySQL Connection failed\n" << e.what() << endl;
		}


	}
	~DBManager() {
		cout << "MySQL Disconnect\n" << endl;
	}


	bool canLogin(string _id, string _pswd) {
		try
		{
			unique_ptr<Statement> stmt(conn->createStatement());
			unique_ptr<ResultSet> res(stmt->executeQuery("SELECT * FROM Player WHERE Login_ID = '" + _id + "'"));

			if (res->next()) {
				if (!res->isNull("Login_password")) {
					string db_pw = res->getString("Login_password");
					if (db_pw == _pswd) {
						return true;
					}
				}
			}
		}
		catch (SQLException& e) {
			cout << "Query failed: " << e.what() << endl;
		}
		return false;
	}

	Player loadPlayer(string _id) {
		try {
			unique_ptr<Statement> stmt(conn->createStatement());
			unique_ptr<ResultSet> res(stmt->executeQuery("SELECT * FROM Player WHERE Login_ID = '" + _id + "'"));

			if (res->next()) {
				string p_id = res->getString("Login_ID");
				string p_password = res->getString("Login_password");
				int x = res->getInt("player_x");
				int y = res->getInt("player_y");
				string p_name = res->getString("Player_name");
				int win = res->getInt("Win");
				int lose = res->getInt("Lose");
				int level = res->getInt("Level");
				double EXP = res->getDouble("EXP");

				return Player(p_id, p_password, x, y, p_name, win, lose, level, EXP);
			}
		}
		catch (SQLException& e) {
			cout << "Query failed: " << e.what() << endl;
		}
		return Player(); // 빈 플레이어 리턴
	}

	//--------------------------------------
	void savePlayer(const std::string& id, int x, int y, int win, int lose, int level, double EXP) {
		try {
			unique_ptr<Statement> stmt(conn->createStatement());
			std::string query = "UPDATE Player SET "
				"player_x = " + std::to_string(x) + ", "
				"player_y = " + std::to_string(y) + ", "
				"Win = " + std::to_string(win) + ", "
				"Lose = " + std::to_string(lose) + ", "
				"Level = " + std::to_string(level) + ", "
				"EXP = " + std::to_string(EXP) + " "
				"WHERE Login_ID = '" + id + "'";
			stmt->executeUpdate(query);
			std::cout << "[Saved data] " << "(" << x << ", " << y << ") " << "ID: " << id << ", "
				<< "Wins: " << win << ", Losses: " << lose << ", "
				<< "Level: " << level << ", EXP: " << EXP << "\n";
		}
		catch (SQLException& e) {
			std::cerr << "Failed to save player data: " << e.what() << "\n";
		}
	}
	//--------------------------------------

};