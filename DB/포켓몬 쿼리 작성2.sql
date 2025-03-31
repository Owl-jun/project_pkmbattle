CREATE TABLE Player (
	Player_id INT AUTO_INCREMENT PRIMARY KEY,
    Player_name VARCHAR(50) NOT NULL,
	Login_ID VARCHAR(50) NOT NULL,
    Login_password VARCHAR(50) NOT NULL,
    Win INT DEFAULT 0,
    Lose INT DEFAULT 0,
    Level INT DEFAULT 1,
    EXP DOUBLE DEFAULT 0.0,
	player_x INT,
    player_y INT
);

CREATE TABLE Poketmon (
	PKM_id INT AUTO_INCREMENT PRIMARY KEY,
    Skill_SetID INT NOT NULL,
    PKM_name VARCHAR(50) NOT NULL,
    PKM_type VARCHAR(50) NOT NULL,
    HP FLOAT NOT NULL,
    FOREIGN KEY (Skill_SetID) REFERENCES PKM_Skill2(Skill_SetID)
);

CREATE TABLE Skill (
	Skill_id INT AUTO_INCREMENT PRIMARY KEY,
    Skill_name VARCHAR(50) NOT NULL,
    Attack Float,
    Skill_type VARCHAR(50) NOT NULL
);

CREATE TABLE PKM_Skill2 (
    Skill_SetID INT AUTO_INCREMENT PRIMARY KEY,
    Skill1_id INT,
    Skill2_id INT,
    Skill3_id INT,
    Skill4_id INT,
    FOREIGN KEY (Skill1_id) REFERENCES Skill(Skill_id),
    FOREIGN KEY (Skill2_id) REFERENCES Skill(Skill_id),
    FOREIGN KEY (Skill3_id) REFERENCES Skill(Skill_id),
    FOREIGN KEY (Skill4_id) REFERENCES Skill(Skill_id)
);

SELECT * FROM Player;

INSERT INTO Player (Player_id, Player_name, Login_id, Login_password) VALUES (1, '강석준', 'ksj', 1111)
																		   , (2, '최보정', 'cbj', 2222)
                                                                           , (3, '이형우', 'lhw', 3333)
																		   , (4, '황석준', 'hsj', 4444)
                                                                           , (5, '김찬수', 'kcs', 5555)
                                                                           , (6, '우동관', 'wdk', 6666);
                                                                           
SELECT * FROM Skill;

INSERT INTO Skill (Skill_id, Skill_name, Attack, Skill_type) VALUES (1, '몸통박치기', 20, '노말')
																  , (2, '전광석화', 20, '노말')
                                                                  , (3, '100만볼트', 30, '전기')
                                                                  , (4, '번개', 40, '전기')
                                                                  , (5, '불꽃세례', 30, '불')
                                                                  , (6, '불대문자', 40, '불')
                                                                  , (7, '물대포', 30, '물')
                                                                  , (8, '하이드로펌프', 40, '물')
                                                                  , (9, '덩클채찍', 30, '풀')
                                                                  , (10, '잎날가르기', 40, '풀');
																	
SELECT * FROM Poketmon;

INSERT INTO Poketmon (PKM_id, Skill_SetID, PKM_name, PKM_type, HP) VALUES (1, 1, '피까츄', '전기', 100)
													   , (2, 2, '파이리', '불', 100)
                                                       , (3, 3, '꼬북이', '물', 100)
                                                       , (4, 4, '이상해씨', '풀', 100)
                                                       , (5, 5, '칠색조', '불', 100)
                                                       , (6, 6, '고라파덕', '물', 100)
                                                       , (7, 7, '코일', '전기', 100)
                                                       , (8, 8, '치코리타', '풀', 100)
                                                       , (9, 9, '브케인', '불', 100)
                                                       , (10, 10, '리아코', '물', 100);
                                                       
SELECT * FROM PKM_Skill;

INSERT INTO PKM_Skill (Skill_SetID, Skill1_id, Skill2_id, Skill3_id, Skill4_id) VALUES (1, 1, 2, 3, 4)
																					  , (2, 1, 2, 5, 6)
                                                                                      , (3, 1, 2, 7, 8)
                                                                                      , (4, 1, 2, 9, 10)
                                                                                      , (5, 1, 2, 5, 6)
                                                                                      , (6, 1, 2, 7, 8)
                                                                                      , (7, 1, 2, 3, 4)
                                                                                      , (8, 1, 2, 9, 10)
                                                                                      , (9, 1, 2, 5, 6)
                                                                                      , (10, 1, 2, 7, 8);
