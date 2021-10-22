CC := cc 
C_FLAGS := -Wall 
C_FLAGS += -Werror 
INCLUDE_DIR := -I src 
SRC_DIR := src

SERVER_PROG := TCPEchoServer4
CLIENT_PROG := TCPEchoClient4

SERVER_SRC := ${SRC_DIR}/${SERVER_PROG}.c
CLIENT_SRC := ${SRC_DIR}/${CLIENT_PROG}.c 

SERVER_OBJ := $(patsubst %.c, %.o, ${SERVER_SRC})
CLIENT_OBJ := $(patsubst %.c, %.o, ${CLIENT_SRC})

DEPS_SRC := ${SRC_DIR}/AddressUtility.c \
						${SRC_DIR}/DieWithMessage.c \
						${SRC_DIR}/TCPServerUtility.c 

DEPS_OBJ := $(patsubst %.c, %.o, ${DEPS_SRC})

.PHONY : all clean 

all : $(SERVER_PROG) $(CLIENT_PROG)

${SERVER_PROG} : ${SERVER_OBJ} ${DEPS_OBJ} 
	$(CC) $(C_FLAGS) -o $@ $^

${CLIENT_PROG} : ${CLIENT_OBJ} ${DEPS_OBJ} 
	$(CC) $(C_FLAGS) -o $@ $^

${SERVER_OBJ} : ${SERVER_SRC}
	$(CC) $(INCLUDE_DIR) $(C_FLAGS) -c $^ -o $@

${CLIENT_OBJ} : ${CLIENT_SRC}
	$(CC) $(INCLUDE_DIR) $(C_FLAGS) -c $^ -o $@

${DEPS_OBJ} : %.o:%.c
	$(CC) $(INCLUDE_DIR) $(C_FLAGS) -c $^ -o $@

clean : 
	rm $(SRC_DIR)/*.o ${CLIENT_PROG} ${SERVER_PROG}
