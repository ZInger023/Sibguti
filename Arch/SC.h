#pragma once
#include <stdio.h>
#include <stdlib.h>
#define MEMORY 100
#define GETFLAG(k) (registr >> (k - 1)) & 0x1
#define SET1(K) registr | (1 << (k - 1))
#define SET0(k) registr &(~(1 << (k - 1)))
#define M 1 //Выход за границы
#define C 2 // Неверная команда
#define P 3 // Переполнение
#define F 4 // Ошибка файла
#define O 5 // Неправильный аргумент
int RAM[MEMORY];
int registr;
const int correct_operations[] = {0x10, 0x11, 0x21, 0x30, 0x31, 0x32, 0x33, 0x40,
                                  0x41, 0x42, 0x43, 0x51, 0x52, 0x53, 0x54, 0x55,
                                  0x56, 0x57, 0x58, 0x59, 0x60, 0x61, 0x62, 0x63,
                                  0x64, 0x65, 0x66, 0x67, 0x68, 0x68, 0x69, 0x70,
                                  0x71, 0x72, 0x73, 0x74, 0x75, 0x76};
const int operations_n = 38;

int cmp(const void *a, const void *b);
void sc_memoryInit();
int sc_memorySet(int address, int value);
int sc_memoryGet(int address, int *value);
int sc_memorySave(char *Filename);
int sc_memoryLoad(char *Filename);
int sc_regInit();
int sc_regSet(int value, int k);
int sc_regGet(int k, int *value);
int sc_commandEncode(int command, int operand, int *value);
int sc_commandDecode(int *operand, int *command, int value);