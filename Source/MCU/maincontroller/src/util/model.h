#ifndef MODEL_H
#define MODEL_H

#if defined (__MKL26Z64__)
#define TEENSY_MODEL 'LC'
#elif defined(__MK20DX128__)
#define TEENSY_MODEL '30'
#elif defined (__MK20DX256__) // 3.1 and 3.2 are the same
#define TEENSY_MODEL '32' 
#elif defined (__MK64FX512__)
#define TEENSY_MODEL '35'
#elif defined (__MK66FX1M0__)
#define TEENSY_MODEL '36'
#endif

#endif /* MODEL_H */