
typedef struct cmd_s {

	char *cmd;
	void (*func)(char *);
	float *fVar;
	int *iVar;

} cmd_t;

extern cmd_t cmd[];

void cmdExecute(char *command);
int cmdFind(char *string);
char *cmdGetCommand(int i);

void cmdQuit(char *args);
void cmdPause(char *args);
void cmdPlay(char *args);
void cmdRestart(char *arg);
void cmdSaveFrame(char *arg);
void cmdSaveFrameDump(char *arg);
void cmdLoadFrameDump(char *arg);
void cmdRecord(char *arg);
void cmdStop(char *arg);
void cmdStart(char *arg);
void cmdFps(char *arg);
void cmdSpawn(char *arg);
void cmdStatus(char *arg);
