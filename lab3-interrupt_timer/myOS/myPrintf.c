#define KERNEL  //needed by vsprintf.c
#include "types.h" //needed by vsprintf.c
#include "vsprintf.c"

//=========================simple output===========================================
#define VGA_BASE 0xb8000
#define ROWS 24   // row25 is for system
#define COLS 80

unsigned char * vgaPtr(int _row, int _col){
    unsigned char * ptr = (unsigned char *)VGA_BASE;
    return ptr + _row * COLS * 2 + _col *2;
}

void put_char(char c, char color, int _row, int _col) {
	unsigned char *ptr = vgaPtr(_row,_col);

	*ptr++ = c; 
    *ptr   = color;	
}

int put_chars(char *msg, char color, int _row, int _col){
	char *ptr=msg;
	char c;	
    int n=0;
	
	c = *ptr;
	while (c!='\0'){
        n++;
	    if ( _col==80 ) {	_col = 0;	_row ++;	}
	    if ( _row==25 ) _row = 0;

	    put_char(c, color, _row, _col++);	 

	    c = *(++ptr);  //next char
	}
	return n;
}

void clear_char(int _row, int _col) {
	unsigned char *ptr = vgaPtr(_row,_col);

	*ptr++ = 0; 
    *ptr   = 0;	
}

void clearLastRow(void){
    int _col;
    for(_col=0; _col<COLS; _col++)
        clear_char(ROWS-1, _col);
}

void scrollOneRow(void){        
    int i;
    unsigned char * ptr = (unsigned char *)VGA_BASE;
    unsigned char * nextRowPtr = vgaPtr(1,0);

    for (i=0; i< (ROWS-1) * COLS; i++) {
        *ptr++ = *nextRowPtr++;
        *ptr++ = *nextRowPtr++;
    }
    clearLastRow();        
}

int col=0, row=0;
void append2screen(char *str,int color){ 
    char *current =str ;
    while(*current) {        

        if (*current !='\n') {
            put_char(*current,color,row,col++);
        } else { // for '\n'            
            col = 0; row ++; 
        }        
        // what if col and row too big                
        if (col >= COLS) { col = 0; row++; }
        if (row >= ROWS) {
            //row=0;
            scrollOneRow(); 
            row=ROWS-1;
        }

        current++;
    }  
}

#undef short
static void _outb(unsigned short port, unsigned char value){
    asm volatile ("outb %1, %0" : : "dN" (port), "a" (value));
}

static void move_cursor(void){
    int cursorLocation = row*80 + col;

    _outb(0x3D4, 14);                    
    _outb(0x3D5, cursorLocation >> 8);   
    _outb(0x3D4, 15);                    
    _outb(0x3D5, cursorLocation && 0xff);        
}

void clear_screen(void) {	
	unsigned char *ptr = (unsigned char *)VGA_BASE;  
    int _col,_row;  
	for(_row=0; _row< ROWS; _row++) {
	    for (_col=0; _col<COLS; _col++) {
            (*ptr++) = 0;  //first char
            (*ptr++) = 0;  //second char
	    }
	}
    row=0; col=0;
    move_cursor();
    return;
}

char buf[400];  //TODO: fix me
int myPrintf(int color,const char *format, ...){
    va_list args;
    int n;

    va_start(args, format);
    n = vsprintf(buf, format, args);
    //if (n>400) while(1);
    append2screen(buf,color);
    move_cursor();
    va_end(args);  

    return n; 
}