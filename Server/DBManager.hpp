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

struct Player{
	string id;
	string password;
	int x = 99;
	int y = 99;
	string name;
	int win;
	int lose;
	int level;
	double EXP;

	Player() {}
	Player(int x, int y)
		: x(x), y(y)
	{}

	bool isEmpty() {
		return id.empty();
	}
};

class DBManager {
private:
	// 클라에서 요청받은  id/ pswd 저장 멤버
	string id;
	string password;
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

	void setIdPassword(string _id , string _pswd) {
		id = _id;
		password = _pswd;
	}

	bool canLogin() {
		try {
			unique_ptr<Statement> stmt(conn->createStatement());
			unique_ptr<ResultSet> res(stmt->executeQuery("SELECT * FROM player WHERE login_id = '" + id + "'"));
			res->next();
			if (res->getString("login_pw") == password) {
				return true;
			}
			else { return false; }
		}
		catch (SQLException& e) {
			cout << "Query failed: " << e.what() << endl;
		}
	}

	/*Player loadPlayer(string _id) {
		try {
			unique_ptr<Statement> stmt(conn->createStatement());
			unique_ptr<ResultSet> res(stmt->executeQuery("SELECT * FROM player WHERE login_id = '" + _id + "'"));
			string p_id, p_password, p_name;
			int x, y, win, lose, level;
			double EXP;
			while (res->next())
			{
				p_id = res->getString("id");
				p_password = res->getString("password");
				x = res->getInt("x");
				y = res->getInt("y");
				p_name = res->getString("name");
				win = res->getInt("win");
				lose = res->getInt("lose");
				level = res->getInt("level");
				EXP = res->getDouble("EXP");
			}
			return Player(p_id, p_password, x, y, p_name, win, lose, level, EXP);
		}
		catch (SQLException& e) {
			cout << "Query failed: " << e.what() << endl;
		}
		return Player();
	}*/

};

