CC = g++
CFLAGS = -c -Wall -O3
INCLUDES = `pkg-config libxml++-2.6 --cflags` `curl-config --cflags`
LDFLAGS = `curl-config --libs` `pkg-config libxml++-2.6 --cflags --libs`
REMOVECMD = rm -rf *.o
EXECUTABLE = doclist

all: $(EXECUTABLE)

$(EXECUTABLE): string_utils.o atom_helper.o service.o doc_list_service.o doclist_sample.o
	$(CC) -Wall -O3 *.o $(LDFLAGS) -o $(EXECUTABLE)

doclist_sample.o: doclist_sample.cc
	$(CC) $(CFLAGS) doclist_sample.cc $(INCLUDES)

doc_list_service.o: gdata/client/doc_list_service.cc gdata/client/doc_list_service.h
	$(CC) $(CFLAGS) gdata/client/doc_list_service.cc $(INCLUDES)

service.o: gdata/client/service.cc gdata/client/service.h
	$(CC) $(CFLAGS) gdata/client/service.cc $(INCLUDES)

atom_helper.o: gdata/atom_helper.cc gdata/atom_helper.h
	$(CC) $(CFLAGS) gdata/atom_helper.cc $(INCLUDES)

string_utils.o: gdata/util/string_utils.cc gdata/util/string_utils.h
	$(CC) $(CFLAGS) gdata/util/string_utils.cc

clean:
	$(REMOVECMD) $(EXECUTABLE)
