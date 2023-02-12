#define	MAX_FIFO_NAME	32

#define	SERV_FIFO	"./.fifo" // Well-known FIFO

typedef struct  {
	char	returnFifo[MAX_FIFO_NAME]; // Client-specific FIFO
	char	data[128]; // req or res data
}
	MsgType;