CC = g++
CFLAGS = -c -Wall -O3
INCLUDES = `pkg-config libxml++-2.6 --cflags` `curl-config --cflags`
LDFLAGS = `curl-config --libs` `pkg-config libxml++-2.6 --cflags --libs`
REMOVECMD = rm -rf *.o
SAMPLE_EXE = doclist
TESTS_EXE = run_tests

all: $(SAMPLE_EXE) $(TESTS_EXE)

tests: $(TESTS_EXE)

$(TESTS_EXE): atom_helper.o atom_helper_test.o
	$(CC) -Wall -O3 atom_helper.o *_test.o tests/run_tests.cpp $(LDFLAGS) -lcppunit -o $(TESTS_EXE)

clean:
	$(REMOVECMD) $(SAMPLE_EXE) $(TESTS_EXE)

$(SAMPLE_EXE): string_utils.o atom_helper.o service.o doc_list_service.o doclist_sample.o
	$(CC) -Wall -O3 *.o $(LDFLAGS) -o $(SAMPLE_EXE)

doclist_sample.o: doclist_sample.cc
	$(CC) $(CFLAGS) doclist_sample.cc $(INCLUDES)

doc_list_service.o: gdata/client/doc_list_service.cc gdata/client/doc_list_service.h
	$(CC) $(CFLAGS) gdata/client/doc_list_service.cc $(INCLUDES)

service.o: gdata/client/service.cc gdata/client/service.h
	$(CC) $(CFLAGS) gdata/client/service.cc $(INCLUDES)

atom_helper.o: gdata/atom_helper.cc gdata/atom_helper.h
	$(CC) $(CFLAGS) gdata/atom_helper.cc $(INCLUDES)

atom_helper_test.o: atom_helper.o
	$(CC) $(CFLAGS) tests/atom_helper_test.cc $(INCLUDES)

string_utils.o: gdata/util/string_utils.cc gdata/util/string_utils.h
	$(CC) $(CFLAGS) gdata/util/string_utils.cc
