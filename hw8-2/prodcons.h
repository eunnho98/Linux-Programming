#define	MY_ID			498
#define	SHM_KEY			(0x9000 + MY_ID)
#define	SHM_MODE		(SHM_R | SHM_W | IPC_CREAT)

#define	REQ_MUTEX_SEM_KEY	(0x6000 + MY_ID)
#define	RES_MUTEX_SEM_KEY	(0x7000 + MY_ID)
#define	SHM_SIZE	1024