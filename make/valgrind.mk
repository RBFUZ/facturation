valgrind-debug: debug/facturation
	G_SLICE=always-malloc G_DEBUG=gc-friendly valgrind --tool=memcheck --leak-check=full --track-origins=yes --suppressions=make/valgrind-suppressions.txt  --gen-suppressions=no debug/facturation $(VALGRIND) 2>&1 

valgrind-release: release/facturation
	G_SLICE=always-malloc G_DEBUG=gc-friendly valgrind --tool=memcheck --leak-check=full --track-origins=yes --suppressions=make/valgrind-suppressions.txt  --gen-suppressions=no release/facturation $(VALGRIND) 2>&1

valgrind-debug-nogui: debug/facturation
	G_SLICE=always-malloc G_DEBUG=gc-friendly valgrind --tool=memcheck --leak-check=full --track-origins=yes --suppressions=make/valgrind-suppressions.txt  --gen-suppressions=no debug/facturation disable-gui $(VALGRIND) 2>&1 

valgrind-release-nogui: release/facturation
	G_SLICE=always-malloc G_DEBUG=gc-friendly valgrind --tool=memcheck --leak-check=full --track-origins=yes --suppressions=make/valgrind-suppressions.txt  --gen-suppressions=no release/facturation disable-gui $(VALGRIND) 2>&1
	
valgrind-debug-stripped: debug/facturation
	G_SLICE=always-malloc G_DEBUG=gc-friendly valgrind --tool=memcheck --leak-check=full --track-origins=yes --suppressions=make/valgrind-suppressions.txt  --gen-suppressions=no debug/facturation disable-gui silent-tests disable-dump-usage $(VALGRIND) 2>&1 

valgrind-release-stripped: release/facturation
	G_SLICE=always-malloc G_DEBUG=gc-friendly valgrind --tool=memcheck --leak-check=full --track-origins=yes --suppressions=make/valgrind-suppressions.txt  --gen-suppressions=no release/facturation disable-gui  silent-tests disable-dump-usage $(VALGRIND) 2>&1