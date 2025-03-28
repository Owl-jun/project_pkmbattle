#define ASIO_STANDALONE
#include <asio.hpp>
#include <windows.h>
#include <iostream>
#include <mysql/jdbc.h>

using asio::ip::tcp;

using namespace std;
using namespace sql;

#define SERVER_IP "127.0.0.1:3306"
#define USERNAME "root"
#define PASSWORD "12345"
#define DATABASE "Pokemon"

class Player {
private:
	int id;
	string name;
	string login_id;
	string login_pw;
	bool isCon;

public:
	// 생성자
	Player(int _id, const string& _name, const string& _login_id, const string& _login_pw)
		: id(_id), name(_name), login_id(_login_id), login_pw(_login_pw) {
		isCon = true;
	}

	// Getter 함수
	int getId() const { return id; }
	string getName() const { return name; }
	string getLoginId() const { return login_id; }
	string getLoginPw() const { return login_pw; }
	bool getIsCon() const { return isCon; }
	string toString() const {
		return "ID: " + to_string(id) + ", Name: " + name + ", Login ID: " + login_id + ", PW: " + login_pw;
	}
};

class MySQLConnector {
private:
	string server;
	string username;
	string password;
	string database;
	unique_ptr<Connection> conn;
public:
	MySQLConnector(const string& serv, const string& user, const string& pw, const string& db)
		: server(serv), username(user), password(pw), database(db) {
		try {

			mysql::MySQL_Driver* driver = mysql::get_mysql_driver_instance();
			conn = unique_ptr<Connection>(driver->connect(server, username, password));

			conn->setSchema(database);


			cout << "MySQL Connection success\n" << endl;
		}
		catch (SQLException& e) {
			cout << "MySQL Connection failed\n" << e.what() << endl;
		}
	}
	~MySQLConnector() {
		cout << "MySQL Disconnect\n" << endl;
	}

	vector<Player> getAllPlayers() {
		vector<Player> players;
		try {
			unique_ptr<Statement> stmt(conn->createStatement());
			unique_ptr<ResultSet> res(stmt->executeQuery("SELECT * FROM player"));

			while (res->next()) {
				int id = res->getInt("id");
				string name = res->getString("name");
				string login_id = res->getString("login_id");
				string login_pw = res->getString("login_pw");

				Player player(id, name, login_id, login_pw);
				players.push_back(player);
			}
		}
		catch (SQLException& e) {
			cout << "Query failed: " << e.what() << endl;
		}
		return players;
	}

	//============================================

	Player getPlayerById(const string& login_id, const string& login_pw) {
		try {
			unique_ptr<PreparedStatement> pstmt(
				conn->prepareStatement("SELECT * FROM player WHERE login_id = ? AND login_pw = ?"));
			pstmt->setString(1, login_id);
			pstmt->setString(2, login_pw);
			unique_ptr<ResultSet> res(pstmt->executeQuery());

			if (res->next()) {
				return Player(
					res->getInt("id"),
					res->getString("name"),
					res->getString("login_id"),
					res->getString("login_pw")
				);
			}
			else {
				throw runtime_error(u8"해당 ID의 플레이어가 없습니다.");
			}
		}
		catch (SQLException& e) {
			throw runtime_error(u8"DB 오류: "s + e.what());
		}
	}
};



int main() {
	SetConsoleOutputCP(CP_UTF8);
	SetConsoleCP(CP_UTF8);
	MySQLConnector db(SERVER_IP, USERNAME, PASSWORD, DATABASE);
	vector<Player> players = db.getAllPlayers();

	for (vector<Player>::const_iterator i = players.begin(); i != players.end(); ++i) {
		const Player& p = *i;

		cout << "ID: " << p.getId()
			<< ", Name: " << p.getName()
			<< ", Login ID: " << p.getLoginId()
			<< ", Login PW: " << p.getLoginPw() << endl;
	}

	try {
		asio::io_context io_context;
		tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), 9000));
		cout << "server loading... (127.0.0.1:9000)\n";

		while (true) {
			tcp::socket socket(io_context);
			acceptor.accept(socket); // 클라이언트 연결 수락

			try {
				//============================================
				char recvBuf[128] = { 0 };
				size_t recvLen = socket.read_some(asio::buffer(recvBuf));
				string recvStr(recvBuf, recvLen);

				recvStr.erase(remove(recvStr.begin(), recvStr.end(), '\n'), recvStr.end());
				recvStr.erase(remove(recvStr.begin(), recvStr.end(), '\r'), recvStr.end());

				size_t delimPos = recvStr.find('|');
				if (delimPos == string::npos) throw runtime_error(u8"잘못된 로그인 형식입니다");

				string loginId = recvStr.substr(0, delimPos);
				string loginPw = recvStr.substr(delimPos + 1);

				cout << "login_id: " << loginId << ", login_pw: " << loginPw << endl;

				Player p = db.getPlayerById(loginId, loginPw);
				string data = "False";

				// 로그인 성공
				if (p.getIsCon()) {
					data = "TRUE";
				}

				asio::write(socket, asio::buffer(data));
				cout << "login complete!\n";
			}
			catch (exception& e) {
				string err = string("FALSE");
				asio::write(socket, asio::buffer(err));
				cout << "login failed!\n";
			}
		}
	}
	catch (exception& e) {
		cout << "server error" << endl;
	}



	//============================================



		//// 데이터 전송 (플레이어 목록)
		//for (const Player& p : players) {
		//	string data = p.getId() + "\n";
		//	asio::write(socket, asio::buffer(data));
		//}


	//	string data = "ID: " + to_string(p.getId()) + ", Login ID: " + p.getLoginId() + ", PW: " + p.getLoginPw();
	//	asio::write(socket, asio::buffer(data));
	//}

	return 0;
}