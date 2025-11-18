PRAGMA foreign_keys=OFF;
BEGIN TRANSACTION;
CREATE TABLE IF NOT EXISTS "books" (
	"book"	varchar(10),
	"author"	varchar(20),
	"ISBN"	integer,
	"is_borrowed"	INTEGER DEFAULT 0
);
INSERT INTO books VALUES('The Great Gatsby','F. Scott Fitzgerald',273565,0);
INSERT INTO books VALUES('1984','George Orwell',524935,0);
INSERT INTO books VALUES('To Kill a Mockingbird','Harper Lee',120084,0);
COMMIT;
