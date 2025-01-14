/**
 * @file
 *
 * @date May 24, 2018
 * @author Anton Bondarev
 */

#ifndef ARM_FPU_H_
#define ARM_FPU_H_

#define ARM_FPU_VFP

/* One word for coprocessor state and 32 words for VPF registers */
#define FPU_DATA_LEN (32 + 1)

#define VFP_FPEXC_EN (1 << 30)

#ifndef __ASSEMBLER__
#include <string.h>
#include <stdint.h>

typedef struct fpu_context {
	uint32_t fpexc;
	union {
		float s[32];
		double d[16];
	} vfp_regs;
} __attribute__((packed, aligned(4))) fpu_context_t;

static inline void arm_fpu_context_init(void *opaque) {
	memset(opaque, 0, sizeof(uint32_t) * FPU_DATA_LEN);
}

extern int try_vfp_instructions(fpu_context_t *vfp);
#else
#include <arm/fpu_macro.s>
#endif /* __ASSEMBLER__ */

#endif /* ARM_FPU_H_ */
