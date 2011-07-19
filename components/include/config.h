#ifndef config_h
#define config_h

#define WORD_SIZE 16
#define MEMORY_SIZE 32
#define RF_SIZE 6 //numero de registradores no RegisterFile

typedef enum UlaOperation_{
	ulaOperation_add = 1,
	ulaOperation_and, //2
	ulaOperation_multiply, //3
	ulaOperation_inc, //4
	ulaOperation_doNothing, //5
	ulaOperation_lt, //6
	ulaOperation_gt, //7
	ulaOperation_eq, //8
	ulaOperation_eqz, //9
}UlaOperation;

#endif
