
use test;

DROP TABLE IF EXISTS test_table;

CREATE TABLE IF NOT EXISTS test_table (
	mid int(10) unsigned NOT NULL,
	username char(100) NOT NULL,
	password char(100) NOT NULL,
	num int(10) unsigned NOT NULL DEFAULT '0',
	PRIMARY KEY (mid),
	UNIQUE KEY username (username)
) ENGINE=MYISAM DEFAULT CHARSET=utf8
;

INSERT INTO test_table (mid, username, password, num) VALUES 
	(1, 'masha', '123456', 20)
,	(2, 'peter', 'qwerty', 40)
;
