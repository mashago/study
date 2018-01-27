callgrind:
1. get data
valgrind --tool=callgrind --separate-threads=yes ./a.out
valgrind --tool=callgrind ./a.out
--separate-threads=yes for multi threads

2. profile data
callgrind_annotate --auto=yes callgrind.out.pid
callgrind_annotate --inclusive=yes callgrind.out.pid
callgrind_annotate --inclusive=yes --tree=both callgrind.out.pid
--auto=yes
--inclusive=yes
--tree=both

memcheck:

