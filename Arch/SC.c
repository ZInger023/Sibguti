#include "SC.h"

int cmp(const void *a, const void *b)
{
    if (a > b)
        return 1;
    else if (a == b)
        return 0;
    else
        return -1;
}

void sc_memoryInit()
{
    for (int i = 0; i < MEMORY; i++)
    {
        RAM[i] = 0;
    }
}

int sc_memorySet(int address, int value)
{
    if (address < MEMORY)
    {
        RAM[address] = value;
        return 0;
    }
    sc_regSet(1, M);
    return -1;
}

int sc_memoryGet(int address, int *value)
{
    if (address < MEMORY)
    {
        *value = RAM[address];
        return 0;
    }
    sc_regSet(1, M);
    return -1;
}

int sc_memorySave(char *Filename)
{
    FILE *file;
    file = fopen(Filename, "wb");
    if (file == NULL)
    {
        sc_regSet(1, F);
        return -1;
    }
    fwrite(RAM, sizeof(int), MEMORY, file);

    fclose(file);
    return 0;
}

int sc_memoryLoad(char *Filename)
{
    FILE *file;
    file = fopen(Filename, "rb");
    if (file == NULL)
    {
        sc_regSet(1, F);
        return -1;
    }
    fread(RAM, sizeof(int), MEMORY, file);

    fclose(file);
    return 0;
}

int sc_regInit()
{
    registr = 0;
    return 0;
}

int sc_regSet(int value, int k)
{
    if (value == 1)
    {
        registr = SET1(k);
    }
    else if (value == 0)
    {
        registr = SET0(k);
    }
    else
    {
        sc_regSet(1, O);
        return -1;
    }
    return 0;
}

int sc_regGet(int k, int *value)
{
    if (k < 33 && k > 0)
    {
        *value = GETFLAG(k);
        return 0;
    }
    else
    {
        sc_regSet(1, M);
        return -1;
    }
}

int sc_commandEncode(int command, int operand, int *value)
{
    int *command_ptr = NULL;

    command_ptr = bsearch(&command, correct_operations, operations_n, sizeof(int), cmp);

    if (command_ptr != NULL)
    {
        *value = (command << 7) | operand;
        return 0;
    }
    else
    {
        sc_regSet(1, C);
        return -1;
    }
}

int sc_commandDecode(int *operand, int *command, int value)
{
    void *correct_command;
    int check;
    int tmp_command, tmp_operand;

    check = (value >> 14) & 1;
    if (check == 0)
    {
        tmp_command = (value >> 7) & 0x7F;
        tmp_operand = value & 0x7F;
        correct_command = bsearch(&tmp_command, correct_operations, operations_n, sizeof(int), cmp);
        if (correct_command != NULL)
        {
            *command = tmp_command;
            *operand = tmp_operand;
        }
        else
        {
            sc_regSet(1, C);
            return -1;
        }
    }
    else
    {
        sc_regSet(1, C);
        return -1;
    }

    return 0;
}

int main()
{
    sc_memoryInit();
    sc_regInit();
    for (int i = 0; i < 5; i++)
    {
        if (sc_memorySet(i, rand() % 100) == -1)
        {
            printf("Error memorySet: code -1\n");
            return -1;
        }
    }
    int *value = malloc(sizeof(int));
    sc_memoryGet(2, value);
    printf("%d\n", *value);
    char filename[] = "file.bin";
    if (sc_memorySave(filename) == -1)
    {
        printf("file not open! code -1\n");
    }
    sc_memoryInit();
    if (sc_memoryLoad(filename) == -1)
    {
        printf("file not open! code -1\n");
    }
    sc_memoryGet(2, value);
    printf("Load %d\n", *value);
    sc_regSet(1, 2);
    int *flag = calloc(1, sizeof(int));
    sc_regGet(2, flag);
    printf("Flag = %d\n", *flag);
    return 0;
}
