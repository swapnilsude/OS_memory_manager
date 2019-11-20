all:
		gcc mem_mgr.c -o mem_mgr
clean:
		rm mem_mgr
test:
		./mem_mgr test2.txt 100