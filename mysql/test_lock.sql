USE test;
DROP TABLE IF EXISTS test_lock;
CREATE TABLE test_lock (
	id int NOT NULL,
	num int NOT NULL DEFAULT 0,
	PRIMARY KEY (id)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

INSERT INTO test_lock VALUES 
(1, 0), (2, 0), (3, 0), (4, 0), (5, 0), (6, 0), (7, 0), (8, 0), (9, 0),
(10, 0), (11, 0), (12, 0), (13, 0), (14, 0), (15, 0), (16, 0), (17, 0), (18, 0), (19, 0),
(20, 0), (21, 0), (22, 0), (23, 0), (24, 0), (25, 0), (26, 0), (27, 0), (28, 0), (29, 0),
(30, 0), (31, 0), (32, 0), (33, 0), (34, 0), (35, 0), (36, 0), (37, 0), (38, 0), (39, 0),
(40, 0), (41, 0), (42, 0), (43, 0), (44, 0), (45, 0), (46, 0), (47, 0), (48, 0), (49, 0),
(50, 0), (51, 0), (52, 0), (53, 0), (54, 0), (55, 0), (56, 0), (57, 0), (58, 0), (59, 0),
(60, 0), (61, 0), (62, 0), (63, 0), (64, 0), (65, 0), (66, 0), (67, 0), (68, 0), (69, 0),
(70, 0), (71, 0), (72, 0), (73, 0), (74, 0), (75, 0), (76, 0), (77, 0), (78, 0), (79, 0),
(80, 0), (81, 0), (82, 0), (83, 0), (84, 0), (85, 0), (86, 0), (87, 0), (88, 0), (89, 0),
(90, 0), (91, 0), (92, 0), (93, 0), (94, 0), (95, 0), (96, 0), (97, 0), (98, 0), (99, 0), (100, 0)
;

DROP PROCEDURE IF EXISTS update_lock_num;
DELIMITER $$
CREATE PROCEDURE update_lock_num(
IN in_id INT,
IN in_offset INT
)
BEGIN
    DECLARE out_count INT;
    DECLARE out_id INT;
    DECLARE out_num INT;
	SELECT id, count(*) INTO out_id, out_count FROM test_lock WHERE id=in_id LIMIT 1;
	IF out_count=0 THEN
		SELECT -1; -- id not exists
	ELSE
		SET AUTOCOMMIT=0;
		SELECT num INTO out_num FROM test_lock WHERE id=in_id FOR UPDATE; -- lock
		SET out_num=out_num+in_offset;
		UPDATE test_lock SET num=out_num WHERE id=in_id;
		COMMIT;
		SET AUTOCOMMIT=1;
		SELECT out_num;
	END IF;
END $$
DELIMITER ;
