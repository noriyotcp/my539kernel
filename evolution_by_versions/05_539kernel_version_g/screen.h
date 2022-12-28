volatile unsigned char *video;

int nextTextPos;
int currLine;

void screen_init();
void print(char *str);
void println();
void printi(int number);
