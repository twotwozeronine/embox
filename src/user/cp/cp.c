 /**
 * \file cp.c
 *
 * \date 22.08.2009
 * \author zoomer
 */
#include "shell_command.h"
#include "file_new.h"

#define COMMAND_NAME "cp"
#define COMMAND_DESC_MSG "cp file"
static const char *help_msg =
	    #include "cp_help.inc"
	    ;
#define HELP_MSG help_msg

DECLARE_SHELL_COMMAND_DESCRIPTOR(COMMAND_NAME, exec, COMMAND_DESC_MSG, HELP_MSG);

static int exec(int argsc, char **argsv) {
	int nextOption;
	const char *src_path,*dst_path;
	getopt_init();
	do {
		nextOption = getopt(argsc, argsv, "h");
	        switch(nextOption) {
	    	case 'h':
	        	show_help();
	    		return 0;
                case -1:
	                break;
	        default:
	                return 0;
	        }
	} while(-1 != nextOption);

	if (argsc < 3) {
		show_help();
		return -1;
	}

	src_path = argsv[argsc - 2];
	dst_path = argsv[argsc - 1];

	FDESC src = open(src_path, NULL);
	if (src==FDESC_INVALID)  {
		TRACE("can't open file %s\n",src_path);
		return -1;
	}

	FDESC dst = open(dst_path, O_CREAT);
	if (dst==FDESC_INVALID) {
		TRACE("can't open file %s\n",dst_path);
		return -1;
	}

	// buf optimized for whole block write
	char buf[0x40000];
	int bytesread = 0;
	while ((bytesread = read(src,buf,0x40000)) > 0)
		if (write(dst,buf,bytesread)<=0) {
			TRACE ("WR ERR!\n");
			break;
		}

	if (!fsync(dst_path))
		return -1;

	return 0;
}
