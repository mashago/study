Callgrind:
1. get data
valgrind --tool=callgrind --separate-threads=yes ./callgrind_test
valgrind --tool=callgrind ./callgrind_test
--separate-threads=yes for multi threads

2. read data
callgrind_annotate --auto=yes callgrind.out.pid
callgrind_annotate --inclusive=yes callgrind.out.pid
callgrind_annotate --inclusive=yes --tree=both callgrind.out.pid



