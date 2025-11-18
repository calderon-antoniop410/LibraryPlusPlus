PRAGMA foreign_keys=OFF;
BEGIN TRANSACTION;
CREATE TABLE IF NOT EXISTS "Users" (
	"username"	TEXT,
	"password"	TEXT,
	PRIMARY KEY("username")
);
INSERT INTO Users VALUES('admin','password');
INSERT INTO Users VALUES('test','test');
INSERT INTO Users VALUES('username','password');
INSERT INTO Users VALUES('test2','test2');
INSERT INTO Users VALUES('test3','test3');
COMMIT;
