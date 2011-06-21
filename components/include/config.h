#ifndef config_h
#define config_h

#define WORD_SIZE 16
#define MEMORY_SIZE 32
#define RF_SIZE 6 //numero de registradores no RegisterFile

typedef enum UlaOperation_{
	ulaOperation_add = 1,
	ulaOperation_and,
	ulaOperation_multiply,
	ulaOperation_inc,
	ulaOperation_doNothing,
	ulaOperation_lt,
	ulaOperation_gt,
	ulaOperation_eq,
	ulaOperation_eqz,
}UlaOperation;

#endif
