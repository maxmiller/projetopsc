#ifndef config_h
#define config_h

#define WORD_SIZE 16
#define MEMORY_SIZE 32
#define RF_SIZE 6 //numero de registradores no RegisterFile

typedef enum UlaOperation_{
	ulaOperation_add,
	ulaOperation_and,
	ulaOperation_multiply,
	ulaOperation_inc,
	ulaOperation_doNothing,
}UlaOperation;

#endif
