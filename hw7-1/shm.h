#define	MY_ID		546 // 내 ID 쓰기, 다 다른 Key값 생성을 위해
#define	SHM_KEY		(0x9000 + MY_ID)
#define	SHM_SIZE	1024
#define	SHM_MODE	(SHM_R | SHM_W | IPC_CREAT)