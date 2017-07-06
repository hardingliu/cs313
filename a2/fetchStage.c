#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <inttypes.h>
#include <stdlib.h>
#include <errno.h>
#include "printInternalReg.h"

#define ERROR_RETURN -1
#define SUCCESS 0

// This method is a helper funciton for checking the validity of a register, i.e., rA or rB.
// The register is passed as an int and this method will return an int, either
// 0 or 1. If the returned value is 0, suggesting that the passed register is
// invalid; else suggesting that the passed register is valid.
int checkReg(int regs) {
    int result = 1;
    switch (regs) {
    case 0:
        break;
    case 1:
        break;
    case 2:
        break;
    case 3:
        break;
    case 4:
        break;
    case 5:
        break;
    case 6:
        break;
    case 7:
        break;
    case 8:
        break;
    case 9:
        break;
    case 10:
        break;
    case 11:
        break;
    case 12:
        break;
    case 13:
        break;
    case 14:
        break;
    default:
        result = 0;
        break;
    }
    return result;
}


// This method is a helper function to check the validities of two registers of each instruction.
// Three parameters are passed to this function: iCode (to decide what kind of registers the instruction
// should have); regA (i.e., the regA); regB (i.e. the regB). It would return an int, either 0 or 1. 
// If it returns 0, suggesting that one of the registers is invalid. Otherwise, both registers are valid. 
// This method uses checkReg method to check each register individually.
int checkRegs(unsigned char iCode, int regA, int regB) {
    int result = 1;
    switch (iCode) {
    case 0x2:
        result = checkReg(regA);
        if (result == 0) {
            break;
        }
        else {
            result = checkReg(regB);
            break;
        }

    case 0x3:
        if (regA != 15) {
            result = 0;
            break;
        }
        else {
            result = checkReg(regB);
            break;
        }

    case 0x4:
        result = checkReg(regA);
        if (result == 0) {
            break;
        }
        else {
            result = checkReg(regB);
            break;
        }

    case 0x5:
        result = checkReg(regA);
        if (result == 0) {
            break;
        }
        else {
            result = checkReg(regB);
            break;
        }

    case 0x6:
        result = checkReg(regA);
        if (result == 0) {
            break;
        }
        else {
            result = checkReg(regB);
            break;
        }

    case 0xa:
        if (regB != 15) {
            result = 0;
            break;
        }
        else {
            result = checkReg(regA);
            break;
        }

    case 0xb:
        if (regB != 15) {
            result = 0;
            break;
        }
        else {
            result = checkReg(regA);
            break;
        }

    default:
        break;
    }

    return result;
}

int main(int argc, char **argv) {

    int machineCodeFD = -1; // File descriptor of file with object code
    uint64_t PC = 0;        // The program counter
    struct fetchRegisters fReg;

    // Verify that the command line has an appropriate number
    // of arguments

    if (argc < 2 || argc > 3) {
        printf("Usage: %s InputFilename [startingOffset]\n", argv[0]);
        return ERROR_RETURN;
    }

    // First argument is the file to open, attempt to open it
    // for reading and verify that the open did occur.
    machineCodeFD = open(argv[1], O_RDONLY);

    if (machineCodeFD < 0) {
        printf("Failed to open: %s\n", argv[1]);
        return ERROR_RETURN;
    }

    // If there is a 2nd argument present, it is an offset so
    // convert it to a value. This offset is the initial value the
    // program counter is to have. The program will seek to that location
    // in the object file and begin fetching instructions from there
    if (3 == argc) {
        // See man page for strtol() as to why
        // we check for errors by examining errno
        errno = 0;
        PC = strtol(argv[2], NULL, 0);
        if (errno != 0) {
            perror("Invalid offset on command line");
            return ERROR_RETURN;
        }
    }

    printf("Opened %s, starting offset 0x%016llX\n", argv[1], PC);

    // Start adding your code here and comment out the line the #define EXAMPLESON line

    FILE *fp;
    fp = fopen(argv[1], "r");
    fReg.PC = PC;

    // To keep track the number of consecutive halt instructions seen, initialized to be zero before entering the loop.
    int haltCheck = 0;

    // An infinite loop that reads instructions line by line. The loop terminates when EOF/error occurs.
    for (;;) {

        fseek(fp, fReg.PC, SEEK_SET);
        unsigned char opCode[1];
        int size = 1;
        fread(opCode, sizeof(char), size, fp);

        if (feof(fp)) {
            printf("%s \n", "Normal termination");
            fclose(fp);
            break;
        }

        // This is the opcode of current instruction.
        unsigned char iCode = opCode[0] >> 4;
        // This is the function code of current instruction.
        unsigned char iFun = opCode[0] << 4;
        iFun >>= 4;

        // This is used to check the validity of the opcode. If the opcode is still 1 after the switch(iCode),
        // suggesting the opcode is valid, else invalid.
        int checkCODE = 1;

        // This is used to check the validity of the function code. If the opcode is still 1 after the switch(iCode),
        // suggesting the function code is valid, else invalid.
        int checkFUN = 1;

        // This switch statement is to check the validities of opcode and function code of current instruction.
        switch (iCode) {
        case 0x0:
            if (iFun == 0x0) {
                fReg.icode = 0;
                fReg.ifun = 0;
                fReg.instr = "halt";
                fReg.valP = fReg.PC + 1;
                size = 0;
                break;
            }
            else {
                checkFUN = 0;
                break;
            }

        case 0x1:
            if (iFun == 0x0) {
                fReg.icode = 1;
                fReg.ifun = 0;
                fReg.instr = "nop";
                fReg.valP = fReg.PC + 1;
                size = 0;
                break;
            }
            else {
                checkFUN = 0;
                break;
            }

        case 0x2:
            switch (iFun) {
            case 0x0:
                fReg.icode = 2;
                fReg.ifun = 0;
                fReg.instr = "rrmovq";
                fReg.valP = fReg.PC + 2;
                size = 1;
                break;

            case 0x1:
                fReg.icode = 2;
                fReg.ifun = 1;
                fReg.instr = "cmovle";
                fReg.valP = fReg.PC + 2;
                size = 1;
                break;

            case 0x2:
                fReg.icode = 2;
                fReg.ifun = 2;
                fReg.instr = "cmovl";
                fReg.valP = fReg.PC + 2;
                size = 1;
                break;

            case 0x3:
                fReg.icode = 2;
                fReg.ifun = 3;
                fReg.instr = "cmove";
                fReg.valP = fReg.PC + 2;
                size = 1;
                break;

            case 0x4:
                fReg.icode = 2;
                fReg.ifun = 4;
                fReg.instr = "cmovn";
                fReg.valP = fReg.PC + 2;
                size = 1;
                break;

            case 0x5:
                fReg.icode = 2;
                fReg.ifun = 5;
                fReg.instr = "cmovge";
                fReg.valP = fReg.PC + 2;
                size = 1;
                break;

            case 0x6:
                fReg.icode = 2;
                fReg.ifun = 6;
                fReg.instr = "cmovg";
                fReg.valP = fReg.PC + 2;
                size = 1;
                break;

            default:
                checkFUN = 0;
                break;
            }
            break;

        case 0x3:
            if (iFun == 0) {
                fReg.icode = 3;
                fReg.ifun = 0;
                fReg.instr = "irmovq";
                fReg.valP = fReg.PC + 10;
                size = 9;
                break;
            }
            else {
                checkFUN = 0;
                break;
            }

        case 0x4:
            if (iFun == 0) {
                fReg.icode = 4;
                fReg.ifun = 0;
                fReg.instr = "rmmovq";
                fReg.valP = fReg.PC + 10;
                size = 9;
                break;
            }
            else {
                checkFUN = 0;
                break;
            }

        case 0x5:
            if (iFun == 0) {
                fReg.icode = 5;
                fReg.ifun = 0;
                fReg.instr = "mrmovq";
                fReg.valP = fReg.PC + 10;
                size = 9;
                break;
            }
            else {
                checkFUN = 0;
                break;
            }

        case 0x6:
            switch (iFun) {
            case 0x0:
                fReg.icode = 6;
                fReg.ifun = 0;
                fReg.instr = "addq";
                fReg.valP = fReg.PC + 2;
                size = 1;
                break;

            case 0x1:
                fReg.icode = 6;
                fReg.ifun = 1;
                fReg.instr = "subq";
                fReg.valP = fReg.PC + 2;
                size = 1;
                break;

            case 0x2:
                fReg.icode = 6;
                fReg.ifun = 2;
                fReg.instr = "andq";
                fReg.valP = fReg.PC + 2;
                size = 1;
                break;

            case 0x3:
                fReg.icode = 6;
                fReg.ifun = 3;
                fReg.instr = "xorq";
                fReg.valP = fReg.PC + 2;
                size = 1;
                break;

            case 0x4:
                fReg.icode = 6;
                fReg.ifun = 4;
                fReg.instr = "mulq";
                fReg.valP = fReg.PC + 2;
                size = 1;
                break;

            case 0x5:
                fReg.icode = 6;
                fReg.ifun = 5;
                fReg.instr = "divq";
                fReg.valP = fReg.PC + 2;
                size = 1;
                break;

            case 0x6:
                fReg.icode = 6;
                fReg.ifun = 6;
                fReg.instr = "modq";
                fReg.valP = fReg.PC + 2;
                size = 1;
                break;

            default:
                checkFUN = 0;
                break;
            }
            break;

        case 0x7:
            switch (iFun) {
            case 0x0:
                fReg.icode = 7;
                fReg.ifun = 0;
                fReg.instr = "jmp";
                fReg.valP = fReg.PC + 9;
                size = 8;
                break;

            case 0x1:
                fReg.icode = 7;
                fReg.ifun = 1;
                fReg.instr = "jle";
                fReg.valP = fReg.PC + 9;
                size = 8;
                break;

            case 0x2:
                fReg.icode = 7;
                fReg.ifun = 2;
                fReg.instr = "jl";
                fReg.valP = fReg.PC + 9;
                size = 8;
                break;

            case 0x3:
                fReg.icode = 7;
                fReg.ifun = 3;
                fReg.instr = "je";
                fReg.valP = fReg.PC + 9;
                size = 8;
                break;

            case 0x4:
                fReg.icode = 7;
                fReg.ifun = 4;
                fReg.instr = "jn";
                fReg.valP = fReg.PC + 9;
                size = 8;
                break;

            case 0x5:
                fReg.icode = 7;
                fReg.ifun = 5;
                fReg.instr = "jge";
                fReg.valP = fReg.PC + 9;
                size = 8;
                break;

            case 0x6:
                fReg.icode = 7;
                fReg.ifun = 6;
                fReg.instr = "jg";
                fReg.valP = fReg.PC + 9;
                size = 8;
                break;

            default:
                checkFUN = 0;
                break;
            }
            break;

        case 0x8:
            if (iFun == 0) {
                fReg.icode = 8;
                fReg.ifun = 0;
                fReg.instr = "call";
                fReg.valP = fReg.PC + 9;
                size = 8;
                break;
            }
            else {
                checkFUN = 0;
                break;
            }

        case 0x9:
            if (iFun == 0) {
                fReg.icode = 9;
                fReg.ifun = 0;
                fReg.instr = "ret";
                fReg.valP = fReg.PC + 1;
                size = 0;
                break;
            }
            else {
                checkFUN = 0;
                break;
            }

        case 0xa:
            if (iFun == 0) {
                fReg.icode = 0xa;
                fReg.ifun = 0;
                fReg.instr = "pushq";
                fReg.valP = fReg.PC + 2;
                size = 1;
                break;
            }
            else {
                checkFUN = 0;
                break;
            }

        case 0xb:
            if (iFun == 0) {
                fReg.icode = 0xb;
                fReg.ifun = 0;
                fReg.instr = "popq";
                fReg.valP = fReg.PC + 2;
                size = 1;
                break;
            }
            else {
                checkFUN = 0;
                break;
            }

        default:
            checkCODE = 0;
            break;
        }

        // If the opcode is invalid, prints an error and terminates the program.
        if (checkCODE == 0) {
            printf("%s 0x%02x %s 0x%03x \n", "Invalid opcode", opCode[0], "at", (unsigned int)fReg.PC);
            fclose(fp);
            break;
        }

        // If the function code is invalid, prints an error and terminates the program.
        if (checkFUN == 0) {
            printf("%s 0x%02x %s 0x%03x \n", "Invalid function code", opCode[0], "at", (unsigned int)fReg.PC);
            fclose(fp);
            break;
        }

        // This long nested "if ... else if" block is to process the remaining instruction after setting 
        // the opcode and function code, will go to corressponding if statement given the length of instruction
        // after subtracting the 1 byte of opcode and function code. After processoing the instruction, go to the 
        // next iteration. If an error occurs, break the loop and terminates.
        if (size == 0) {
            if (iCode == 0 && iFun == 0) {
                haltCheck += 1;
                if (haltCheck > 2) {
                    fReg.PC = fReg.valP;
                }
                else {
                    fReg.regsValid = 0;
                    fReg.valCValid = 0;
                    printRegS(&fReg);
                    fReg.PC = fReg.valP;
                }
            }
            else {
                haltCheck = 0;
                fReg.regsValid = 0;
                fReg.valCValid = 0;
                printRegS(&fReg);
                fReg.PC = fReg.valP;
            }
        }
        else if (size == 1) {
            haltCheck = 0;
            unsigned char buffer[1];
            unsigned long size_t = fread(buffer, sizeof(char), 1, fp);

            if (size_t < size) {
                fReg.regsValid = 0;
                fReg.valCValid = 0;
                printRegS(&fReg);
                printf("%s 0x%03x %s %d %s %s %d %s \n", "Memory access error at", (unsigned int)fReg.PC,
                ", required", size + 1, "bytes," ," read", (int)size_t + 1, "bytes.");
                fclose(fp);
                break;
            }
            else {
                fReg.rA = buffer[0] >> 4;
                fReg.rB = buffer[0] << 4;
                fReg.rB >>= 4;
                if (!checkRegs(iCode, (int)fReg.rA, (int)fReg.rB)) {
                    printf("%s %02x %s 0x%03x \n", "Invalid registers", (unsigned int)buffer[0], "at", (unsigned int)fReg.PC);
                    fclose(fp);
                    break;
                }
                else {
                    fReg.regsValid = 1;
                    fReg.valCValid = 0;
                    printRegS(&fReg);
                    fReg.PC = fReg.valP;
                }
            }
        }
        else if (size == 8) {
            haltCheck = 0;
            unsigned char buffer[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
            unsigned long size_t = fread(buffer, sizeof(char), 8, fp);
            if (size_t < size && size_t > 0) {
                fReg.regsValid = 0;
                fReg.valCValid = 1;
                fReg.byte0 = buffer[0];
                fReg.byte1 = buffer[1];
                fReg.byte2 = buffer[2];
                fReg.byte3 = buffer[3];
                fReg.byte4 = buffer[4];
                fReg.byte5 = buffer[5];
                fReg.byte6 = buffer[6];
                fReg.byte7 = buffer[7];
                fReg.valC = ((uint64_t)fReg.byte7) << 56 | ((uint64_t)fReg.byte6) << 48 | ((uint64_t)fReg.byte5) << 40 | ((uint64_t)fReg.byte4) << 32 | 
                ((uint64_t)fReg.byte3) << 24 | ((uint64_t)fReg.byte2) << 16 | ((uint64_t)fReg.byte1) << 8 | fReg.byte0;
                printRegS(&fReg);
                printf("%s 0x%03x %s %d %s %s %d %s \n", "Memory access error at", (unsigned int)fReg.PC,
                ", required", size + 1, "bytes,"," read", (int)size_t + 1, "bytes.");
                fclose(fp);
                break;
            }
            else if (size_t < size && size_t <= 0) {
                fReg.regsValid = 0;
                fReg.valCValid = 0;
                printRegS(&fReg);
                printf("%s 0x%03x %s %d %s %s %d %s \n", "Memory access error at", (unsigned int)fReg.PC,
                ", required", size + 1, "bytes," ," read", (int)size_t + 1, "bytes.");
                fclose(fp);
                break;
            }
            else {
                fReg.regsValid = 0;
                fReg.valCValid = 1;
                fReg.byte0 = buffer[0];
                fReg.byte1 = buffer[1];
                fReg.byte2 = buffer[2];
                fReg.byte3 = buffer[3];
                fReg.byte4 = buffer[4];
                fReg.byte5 = buffer[5];
                fReg.byte6 = buffer[6];
                fReg.byte7 = buffer[7];
                fReg.valC = ((uint64_t)fReg.byte7) << 56 | ((uint64_t)fReg.byte6) << 48 | ((uint64_t)fReg.byte5) << 40 | ((uint64_t)fReg.byte4) << 32 
                | ((uint64_t)fReg.byte3) << 24 | ((uint64_t)fReg.byte2) << 16 | ((uint64_t)fReg.byte1) << 8 | fReg.byte0;
                printRegS(&fReg);
                fReg.PC = fReg.valP;
            }
        }
        else {
            haltCheck = 0;
            unsigned char buffer[9] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
            unsigned long size_t = fread(buffer, sizeof(char), size, fp);
            if (size_t < size) {
                if (size_t == 1) {
                    fReg.rA = buffer[0] >> 4;
                    fReg.rB = buffer[0] << 4;
                    fReg.rB >>= 4;
                    if (!checkRegs(iCode, (int)fReg.rA, (int)fReg.rB)) {
                        printf("%s %02x %s 0x%03x \n", "Invalid registers", (unsigned int)buffer[0],
                        "at", (unsigned int)fReg.PC);
                        fclose(fp);
                        break;
                    }
                    else {
                        fReg.regsValid = 1;
                        fReg.valCValid = 0;
                        printRegS(&fReg);
                        printf("%s 0x%03x %s %d %s %s %d %s \n", "Memory access error at", (unsigned int)fReg.PC,
                        ", required", size + 1, "bytes," ," read", (int)size_t + 1, "bytes.");
                        break;
                    }
                }
                else if (size_t > 1) {
                    fReg.rA = buffer[0] >> 4;
                    fReg.rB = buffer[0] << 4;
                    fReg.rB >>= 4;
                    if (!checkRegs(iCode, (int)fReg.rA, (int)fReg.rB)) {
                        printf("%s %02x %s 0x%03x \n", "Invalid registers", (unsigned int)buffer[0],
                        "at", (unsigned int)fReg.PC);
                        fclose(fp);
                        break;
                    }
                    else {
                        fReg.regsValid = 1;
                        fReg.valCValid = 1;
                        fReg.byte0 = buffer[1];
                        fReg.byte1 = buffer[2];
                        fReg.byte2 = buffer[3];
                        fReg.byte3 = buffer[4];
                        fReg.byte4 = buffer[5];
                        fReg.byte5 = buffer[6];
                        fReg.byte6 = buffer[7];
                        fReg.byte7 = buffer[8];
                        fReg.valC = ((uint64_t)fReg.byte7) << 56 | ((uint64_t)fReg.byte6) << 48 | ((uint64_t)fReg.byte5) << 40 | ((uint64_t)fReg.byte4) << 32 |
                         ((uint64_t)fReg.byte3) << 24 | ((uint64_t)fReg.byte2) << 16 | ((uint64_t)fReg.byte1) << 8 | fReg.byte0;
                        printRegS(&fReg);
                        printf("%s 0x%03x %s %d %s %s %d %s \n", "Memory access error at", (unsigned int)fReg.PC,
                        ", required", size + 1, "bytes," ," read ", (int)size_t + 1, "bytes.");
                        break;
                    }
                }
                else {
                    fReg.regsValid = 0;
                    fReg.valCValid = 0;
                    printRegS(&fReg);
                    printf("%s 0x%03x %s %d %s %s %d %s \n", "Memory access error at", (unsigned int)fReg.PC,
                    ", required", size + 1, "bytes," ," read", (int)size_t + 1, "bytes.");
                    fclose(fp);
                    break;
                }
            }
            else {
                fReg.rA = buffer[0] >> 4;
                fReg.rB = buffer[0] << 4;
                fReg.rB >>= 4;
                if (!checkRegs(iCode, (int)fReg.rA, (int)fReg.rB)) {
                    printf("%s %02x %s 0x%03x \n", "Invalid registers", (unsigned int)buffer[0],
                    "at", (unsigned int)fReg.PC);
                    fclose(fp);
                    break;
                }
                else {
                    fReg.regsValid = 1;
                    fReg.valCValid = 1;
                    fReg.byte0 = buffer[1];
                    fReg.byte1 = buffer[2];
                    fReg.byte2 = buffer[3];
                    fReg.byte3 = buffer[4];
                    fReg.byte4 = buffer[5];
                    fReg.byte5 = buffer[6];
                    fReg.byte6 = buffer[7];
                    fReg.byte7 = buffer[8];
                    fReg.valC = ((uint64_t)fReg.byte7) << 56 | ((uint64_t)fReg.byte6) << 48 | ((uint64_t)fReg.byte5) << 40 | 
                    ((uint64_t)fReg.byte4) << 32 | ((uint64_t)fReg.byte3) << 24 | ((uint64_t)fReg.byte2) << 16 | 
                    ((uint64_t)fReg.byte1) << 8 | fReg.byte0;
                    printRegS(&fReg);
                    fReg.PC = fReg.valP;
                }
            }
        }
    }

//#define EXAMPLESON 1
#ifdef EXAMPLESON
    // The next few lines are examples of various types of output. In the comments is
    // an instruction, the address it is at and the associated binary code that would
    // be found in the object code file at that address (offset). Your program
    // will read that binary data and then pull it appart just like the fetch stage.
    // Once everything has been pulled apart then a call to printRegS is made to
    // have the output printed. Read the comments in printInternalReg.h for what
    // the various fields of the structure represent. Note: Do not fill in fields
    // int the structure that aren't used by that instruction.

    /*************************************************
     irmovq $1, %rsi   0x008: 30f60100000000000000
     ***************************************************/

    fReg.PC = 8;
    fReg.icode = 3;
    fReg.ifun = 0;
    fReg.regsValid = 1;
    fReg.rA = 15;
    fReg.rB = 6;
    fReg.valCValid = 1;
    fReg.valC = 1;
    fReg.byte0 = 1;
    fReg.byte1 = 0;
    fReg.byte2 = 0;
    fReg.byte3 = 0;
    fReg.byte4 = 0;
    fReg.byte5 = 0;
    fReg.byte6 = 0;
    fReg.byte7 = 0;
    fReg.valP = 8 + 10;
    fReg.instr = "irmovq";

    printRegS(&fReg);

    /*************************************************
     je target   x034: 733f00000000000000     Note target is a label

     ***************************************************/

    fReg.PC = 0x34;
    fReg.icode = 7;
    fReg.ifun = 3;
    fReg.regsValid = 0;
    fReg.valCValid = 1;
    fReg.valC = 0x3f;
    fReg.byte0 = 0x3f;
    fReg.byte1 = 0;
    fReg.byte2 = 0;
    fReg.byte3 = 0;
    fReg.byte4 = 0;
    fReg.byte5 = 0;
    fReg.byte6 = 0;
    fReg.byte7 = 0;
    fReg.valP = 0x34 + 9;
    fReg.instr = "je";

    printRegS(&fReg);
    /*************************************************
     nop  x03d: 10

     ***************************************************/

    fReg.PC = 0x3d;
    fReg.icode = 1;
    fReg.ifun = 0;
    fReg.regsValid = 0;
    fReg.valCValid = 0;
    fReg.valP = 0x3d + 1;
    fReg.instr = "nop";

    printRegS(&fReg);

    /*************************************************
     addq %rsi,%rdx  0x03f: 6062

     ***************************************************/

    fReg.PC = 0x3f;
    fReg.icode = 6;
    fReg.ifun = 0;
    fReg.regsValid = 1;
    fReg.rA = 6;
    fReg.rB = 2;
    fReg.valCValid = 0;
    fReg.valP = 0x3f + 2;
    fReg.instr = "add";

    printRegS(&fReg);
#endif

    return SUCCESS;
}
