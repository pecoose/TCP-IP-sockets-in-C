CC := cc 
C_FLAGS := -Wall 
C_FLAGS += -Werror

INCLUDE_DIR := -I ./src
SRC_DIR := src 

DEP := AddressUtility.c \
			 DieWithMessage.c \
			 TCPServerUtility.c 

SRC_TCPEchoClient4 := $(SRC_DIR)/TCPEchoClient4.c 
SRC_TCPEchoServer4 := $(SRC_DIR)/TCPEchoServer4.c

OBJ_TCPEchoClient4 := $(patsubst %.c, %.o, $(SRC_TCPEchoClient4))
OBJ_TCPEchoServer4 := $(patsubst %.c, %.o, $(SRC_TCPEchoServer4))

DEP_SRC := $(SRC_DIR)/$(DEP)
DEP_OBJ := $(patsubst %.c, %.o, $(DEP_SRC))

TCPEcho4 : $(OBJ_TCPEchoServer4) $(OBJ_TCPEchoClient4) $(DEP_OBJ)
	$(CC) -o $@ $^

$(OBJ_TCPEchoClient4) : $(SRC_TCPEchoClient4)
	$(CC) $(C_FLAGS) $(INCLUDE_DIR) -c $^ -o $@

$(OBJ_TCPEchoServer4) : $(SRC_TCPEchoServer4)
	$(CC) $(C_FLAGS) $(INCLUDE_DIR) -c $^ -o $@

$(DEP_OBJ) : %.o:%.c
	$(CC) $(C_FLAGS) $(INCLUDE_DIR) -c $^ -o $@

clean:
	rm *.o