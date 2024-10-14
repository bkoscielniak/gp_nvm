CC = gcc
CFLAGS = -Wall

TARGET = test_gp_nvm
SOURCES = gp_nvm.c test_gp_nvm.c

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCES)

clean:
	rm -f $(TARGET) *.o