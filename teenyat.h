/*
 * Name	   : teenyat.h
 * Author	 : William "Amos" Confer
 *
 * License	: Copyright (C) 2023 All rights reserved
 */

#ifndef __TEENYAT_H__
#define __TEENYAT_H__

#define TNY_CUSTOM_LOGGER

#ifndef __cplusplus

#include <stdbool.h>
#include <stdint.h>

#else  // __cplusplus
extern "C" {

#include <cstdint>

#endif // __cplusplus

typedef struct teenyat teenyat;

typedef uint16_t tny_uword;
typedef int16_t tny_sword;
typedef union tny_word tny_word;

/**
 * @brief
 *   System calllback function to handle TeenyAT read requests
 *
 * @param t
 *   The TeenyAT instance making the request
 *
 * @param addr
 *   Address of the read request
 *
 * @param[out] data
 *   Output the data result of the request.  If the requested address is not
 *   simulated or otherwise illegal (eg, not meant as an input address) it is
 *   up to the callback function what value to return here.
 *
 * @param delay
 *   Use this to tell the TeenyAT how many additional cycles of cost you want
 *   associated with this read request.  For example, if you want the device
 *   being read from to have a penalty of 9 clock cycles on top of the particular
 *   instruction used, pass 9 as the delay.  The TeenyAT will ensure this
 *   many cycles pass before the next instruction is executed.
 */
typedef void(*TNY_READ_FROM_BUS_FNPTR)(teenyat *t, tny_uword addr, tny_word *data, uint16_t *delay);

/**
 * @brief
 *   System calllback function to handle TeenyAT write requests
 *
 * @param t
 *   The TeenyAT instance making the request
 *
 * @param addr
 *   Address of the write request
 *
 * @param data
 *   The data to be sent to the address.  If the requested address is not
 *   simulated or otherwise illegal (eg, not meant as an input address) it is
 *   up to the callback function whether to ignore the request or do something
 * else.
 *
 * @param delay
 *   Use this to tell the TeenyAT how many additional cycles of cost you want
 *   associated with this write request.  For example, if you want the device
 *   being written to to have a penalty of 7 clock cycles on top of the particular
 *   instruction used, pass 7 as the delay.  The TeenyAT will ensure this
 *   many cycles pass before the next instruction is executed.
 */
typedef void(*TNY_WRITE_TO_BUS_FNPTR)(teenyat *t, tny_uword addr, tny_word data, uint16_t *delay);

/**
 * @brief
 *   System callback function to handle TeenyAT logging statements in a convienent way
 *   for the system writer requires the TNY_CUSTOM_LOGGER define
 *
 * @param t
 *   teenyat instance the custom logging callback will be added to
 * 
 * @param log_msg
 *   The message to be output as designed by the system writer
 */
typedef void(*TNY_LOG)(teenyat *t, char *log_msg);

/** While the TeenyAT has a 16 bit address space, RAM is only 32K words */
#define TNY_RAM_SIZE (1 << 15)
#define TNY_MAX_RAM_ADDRESS (TNY_RAM_SIZE - 1)
#define TNY_BUS_DELAY 3

union tny_word {
	struct {
		tny_sword immed4  : 4;
		tny_uword reg2   : 3;
		tny_uword reg1   : 3;
		tny_uword teeny  : 1;
		tny_uword opcode : 5;
	} instruction;

	struct {
		tny_uword greater  : 1;
		tny_uword less     : 1;
		tny_uword equals   : 1;
		tny_uword carry    : 1;
		tny_uword reserved : 12;
	} inst_flags;

	struct {
		tny_uword byte0 : 8;
		tny_uword byte1 : 8;
	} bytes;
	tny_uword u;
	tny_sword s;

	struct {
		tny_uword bit0  : 1;
		tny_uword bit1  : 1;
		tny_uword bit2  : 1;
		tny_uword bit3  : 1;
		tny_uword bit4  : 1;
		tny_uword bit5  : 1;
		tny_uword bit6  : 1;
		tny_uword bit7  : 1;
		tny_uword bit8  : 1;
		tny_uword bit9  : 1;
		tny_uword bit10 : 1;
		tny_uword bit11 : 1;
		tny_uword bit12 : 1;
		tny_uword bit13 : 1;
		tny_uword bit14 : 1;
		tny_uword bit15 : 1;
	} bits;
};

struct teenyat {
	/** Has this TeenyAT ever been initialized */
	bool initialized;
	/** Memory used for a program's code/data */
	tny_word ram[TNY_RAM_SIZE];
	/** copy of original bin file for resets */
	tny_word bin_image[TNY_RAM_SIZE];
	/**
	 * Registers...
	 *
	 * reg[0]: Program Counter (PC)
	 *
	 * reg[1]: Stack Pointer (SP)... address of the next top
	 *
	 * reg[2]: Zero Register (rZ)... always contains zero and is read only
	 *
	 * reg[3]-[7]: General Purpose Registers (rA -- rE)
	 */
	tny_word reg[8];
	/**
	 * Flag bits are set by CMP and all ALU instructions.
	 */
	struct {
		/**
		 * Carry is set/cleared by arithmetic, shift and rotate instructions.
		 * For shift/rotate instructions, the final bit shifted out of storage
		 * determines the flag.  If the shift length is zero, the flag is
		 * unchanged. */
		bool carry;
		/** Set/cleared if the result of a CMP or ALU instruction is 0 or not */
		bool equals;
		/** This flag is effectively the sign bit of the CMP or ALU result */
		bool less;
		/** Set if CMP or ALU result is positive and non-zero */
		bool greater;
	} flags;
	/**
	 * System callback function to handle TeenyAT read requests
	 */
	TNY_READ_FROM_BUS_FNPTR bus_read;
	/**
	 * System callback function to handle TeenyAT write requests
	 */
	TNY_WRITE_TO_BUS_FNPTR bus_write;
	/**
	 * System callback function to handle TeenyAT logging statements in a convenient way
	 * for the system writer.
	 */
	TNY_LOG system_log;
	/**
	 * The number of remaining cycles to delay to simulate the cost of the
	 * previous instruction.
	 */
	unsigned int delay_cycles;
	/**
	 * The number of cycles this instance has been running since initialization
	 * or reset.
	 */
	uint64_t cycle_cnt;
	/**
	 * An extra pointer for system developers so data can follow a TeenyAT
	 * instance through read/write callback functions, for example.
	 */
	void *ex_data;
};

#define TNY_OPCODE_SET 0
#define TNY_OPCODE_LOD 1
#define TNY_OPCODE_STR 2
#define TNY_OPCODE_PSH 3
#define TNY_OPCODE_POP 4
#define TNY_OPCODE_BTS 5
#define TNY_OPCODE_BTC 6
#define TNY_OPCODE_BTF 7
#define TNY_OPCODE_CAL 8
#define TNY_OPCODE_ADD 9
#define TNY_OPCODE_SUB 10
#define TNY_OPCODE_MPY 11
#define TNY_OPCODE_DIV 12
#define TNY_OPCODE_MOD 13
#define TNY_OPCODE_AND 14
#define TNY_OPCODE_OR  15
#define TNY_OPCODE_XOR 16
#define TNY_OPCODE_SHF 17
#define TNY_OPCODE_ROT 18
#define TNY_OPCODE_NEG 19
#define TNY_OPCODE_CMP 20
#define TNY_OPCODE_JMP 21
#define TNY_OPCODE_DJZ 22
#define TNY_OPCODE_DLY 23

#define TNY_REG_PC   0
#define TNY_REG_SP   1
#define TNY_REG_ZERO 2
#define TNY_REG_A    3
#define TNY_REG_B    4
#define TNY_REG_C    5
#define TNY_REG_D    6
#define TNY_REG_E    7

/**
 * @brief
 *   Initialize a TeenyAT instance and buffer the file for future resets.
 *
 * @param t
 *   The TeenyAT instance to initialize
 *
 * @param bin_file
 *   The pre-assembled .bin file to load and execute
 *
 * @param bus_read
 *   Callback function for handling read requests
 *
 * @param bus_write
 *   Callback function for handling write requests
 *
 * @return
 *   True on success, flase otherwise.
 *
 * @note
 *   Upon failed initialization, the t->initialized member can be assumed false,
 *   but the state of all other members is undefined.
 */
bool tny_init_from_file(teenyat *t, FILE *bin_file,
                        TNY_READ_FROM_BUS_FNPTR bus_read,
                        TNY_WRITE_TO_BUS_FNPTR bus_write);

/**
 * @brief
 *   Initialize a TeenyAT instance and buffer the file for future resets.
 *
 * @param t
 *   The TeenyAT instance to initialize
 *
 * @param bin
 *   unsigned char array containing the assembled binary of teenyat code
 *   to run.
 * 
 * @param bin_size
 *   size of the binary assembled teenyat code in bytes
 *
 * @param bus_read
 *   Callback function for handling read requests
 *
 * @param bus_write
 *   Callback function for handling write requests
 *
 * @return
 *   True on success, flase otherwise.
 *
 * @note
 *   Upon failed initialization, the t->initialized member can be assumed false,
 *   but the state of all other members is undefined.
 */
bool tny_init_from_unsigned_char_array(teenyat *t, unsigned char *bin, unsigned int bin_size,
										TNY_READ_FROM_BUS_FNPTR bus_read,
										TNY_WRITE_TO_BUS_FNPTR bus_write);

/**
 * @brief
 *   Initializes logging to use a systems custom logger
 * 
 * @param t
 *   The TeenyAT instance that uses this custom logging function
 * 
 * @param system_log
 * 	 Function pointer of type TNY_LOG that will be used from this point on
 *   for the teenyat instances logging.
*/
void tny_init_custom_log(teenyat *t, TNY_LOG system_log);

/**
 * @brief
 *   Reinitialize the TeenyAT
 *
 * Restore the TeenyAT to its initialized state as if it had just been done so
 * from the original .bin file
 *
 * @param t
 *   The TeenyAT instance to reset
 *
 * @return
 *   True on success, flase otherwise.
 *   Attempting to reset an unitialized TeenyAT will always return false.
 */
bool tny_reset(teenyat *t);

/**
 * @brief
 *   Advance the TeenyAT instance by one clock cycle
 *
 * This function provides the TeenyAT with something similar to a CPU clock
 * pulse.  The TeenyAT will handle all the internals of the traditional Fetch,
 * Decode, and Execute processes, but it is the caller's responsibility to
 * check whether an any bus requests were made after EVERY cycle.
 *
 * The caller MUST check t's bus state for whether the access is a read or
 * write.  Read/writes from/to external devices must be handled immediately...
 * before calling tny_clock() again.  Check the character in t->bus.state to
 * determine the type of bus access request.  'R' indicates t was trying to read
 * from the device at the address t->bus.address.  'W' indicates t was trying to
 * write the data in t->bus.data to the device at the address t->bus.address.
 * the state member may also contain 'X', indicating no request at all.
 *
 * On read requests, the caller MUST satisfy the request via tny_lod_result().
 *
 * On write requests, the caller MUST satisfy the request via tny_str_result().
 *
 * @param t
 *   The TeenyAT instance
 */
void tny_clock(teenyat *t);

#ifdef __cplusplus
}
#endif

#endif /* __TEENYAT_H__ */
