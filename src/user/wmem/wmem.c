/**
 * \file wmem.c
 *
 * \date 20.02.2009
 * \author Alexey Fomin
 */
#include "shell_command.h"

#define COMMAND_NAME "wmem"
#define COMMAND_DESC_MSG "writes to memory"
static const char *help_msg =
	#include "wmem_help.inc"
	;
#define HELP_MSG help_msg

DECLARE_SHELL_COMMAND_DESCRIPTOR(COMMAND_NAME, exec, COMMAND_DESC_MSG, HELP_MSG);

static int exec(int argsc, char **argsv) {
	int i, flag = 0;
	int nextOption;
	WORD *address;
	WORD new_value;

	getopt_init();
        do {
                nextOption = getopt(argsc, argsv, "a:v:h");
                switch(nextOption) {
                case 'h':
                        show_help();
                        return 0;
                case 'a':
                        if ((optarg != NULL) && (!sscanf(optarg, "0x%x", &address))
                                        && (!sscanf(optarg, "%d", (int *) &address))) {
                                LOG_ERROR("wmem: hex value expected.\n");
                                show_help();
                                return -1;
                        }
                        if (address != address & 0xFFFFFFFC) {
                                LOG_ERROR("wmem: address in wrong format (last 2 bits must be 0)\n");
                                show_help();
                                return -1;
                        }
                        flag++;
                        break;
                case 'v':
            		if ((optarg != NULL) && (!sscanf(optarg, "0x%x", &new_value))
                                        && (!sscanf(optarg, "%d", (int *) &new_value))) {
                                LOG_ERROR("wmem: hex value expected.\n");
                                show_help();
                                return -1;
                        }
                        flag++;
                        break;
		case -1:
	                break;
	        default:
	                return 0;
	        }
	} while(-1 != nextOption);

	if(flag != 2) {
		LOG_ERROR("wmem: -a and -v keys expected!\n");
		return -1;
	}

	*((WORD *) address) = new_value;
}
