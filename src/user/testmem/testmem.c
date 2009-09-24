/**
 * \file testmem.c
 * \date Jul 24, 2009
 * \author afomin
 * \details
 */

#include "shell_command.h"
#include "asm/cache.h"
#include "memory_tests.h"

#define COMMAND_NAME "testmem"
#define COMMAND_DESC_MSG "set of memory tests"
static const char *help_msg =
	#include "testmem_help.inc"
	;
#define HELP_MSG help_msg

DECLARE_SHELL_COMMAND_DESCRIPTOR(COMMAND_NAME, exec, COMMAND_DESC_MSG, HELP_MSG);

typedef void TEST_MEM_FUNC(WORD *addr, long int amount);

static int exec(int argsc, char **argsv) {
	int i;
	TEST_MEM_FUNC *test_mem_func;
	WORD *address = (WORD *) 0x70000000L;
	long int amount = 1000000L;
	int nextOption;
	getopt_init();
	do {
	        nextOption = getopt(argsc, argsv, "a:n:t:h");
	        switch(nextOption) {
	        case 'h':
	                show_help();
	                return 0;
	        case 'a':
	    		// Key -a for address
	                // Can be in hex and in decimal
	    		if ((optarg == NULL) || // addr empty
	                    ((!sscanf(optarg, "0x%x", &address)) // addr not in hex
	                    && (!sscanf(optarg, "%d", (int *) &address)))) { // addr not in decimal
	                        LOG_ERROR("testmem: -a: address value in hex or in decimal expected.\n");
	                        show_help();
	                        return -1;
	                    }
	                    // TODO remove next
	                    TRACE("Address: 0x%08x\n", address);
	    		    break;
	        case 'n':
    			// Key -n for number of
            		//   - times, starting loop in "loop" test
                        //   - bytes testing in other case
                        // The same as address can be either in hex or decimal
            		if (optarg == NULL) { // amount empty
                                LOG_ERROR("testmem: -n: amount value in hex or in decimal expected.\n");
                                show_help();
                                return -1;
                        }
                        if (!sscanf(optarg, "0x%x", &amount)) { // amount not in hex
                                if (!sscanf(optarg, "%d", (int *) &amount)) { // amount not in decimal
                        		LOG_ERROR("testmem: -n: amount value in hex or in decimal expected.\n");
                                        show_help();
                                        return -1;
                                }
                        }
                        break;
	        case 't':
	                // Key -t for test type
	                // If not presented - setting default type
	                // !! for "loop" different default value
	                // !!   for amount (in this case amount is
	                // !!   a counter of loop)
	                if (strcmp(optarg, "runzero") == 0) {
	                        test_mem_func = &memory_test_run0;
	                } else if (strcmp(optarg, "runone") == 0) {
	                        test_mem_func = &memory_test_run1;
	                } else if (strcmp(optarg, "loop") == 0) {
	                        amount = 0;
	                        test_mem_func = &memory_test_loop;
	                } else if (strcmp(optarg, "address") == 0) {
	                        test_mem_func = &memory_test_address;
	                } else if (strcmp(optarg, "chess") == 0) {
	                        test_mem_func = &memory_test_chess;
	                } else {
	                        LOG_ERROR("testmem: %s: no such test.\n", optarg);
	                	show_help();
	                        return -1;
	                }
	                break;
	        case -1:
	                break;
	        default:
	                return 0;
	        }
	} while(-1 != nextOption);

	if(test_mem_func == NULL) {
		LOG_ERROR("testmem: test name expected.\n");
		show_help();
		return -1;
	}

	cache_data_disable();
	TRACE("Before starting: address: 0x%08x, amount: 0x%08x\n", address, amount);
	(*test_mem_func)(address, amount);
	cache_data_enable();
	return 0;
}
