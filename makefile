.SILENT:

PRESENTATION=presentation


BUILD_FOLDER = build
CC = g++ -DSUN -I$(PRESENTATION)
OBJS = $(BUILD_FOLDER)/presentation.o 


ALL: $(PROGRAMS)

$(BUILD_FOLDER)/dkjr:	dkjr.cpp $(OBJS)
	echo Creation de dkjr...
	$(CC) dkjr.cpp -o $(BUILD_FOLDER)/dkjr $(OBJS)  -lrt -lpthread -lSDL

$(BUILD_FOLDER)/presentation.o:	$(PRESENTATION)/presentation.c $(PRESENTATION)/presentation.h
		echo Creation de presentation.o ...
		$(CC) -c $(PRESENTATION)/presentation.c -o $(BUILD_FOLDER)/presentation.o

clean:
	@rm -r BUILD_FOLDER/
 

