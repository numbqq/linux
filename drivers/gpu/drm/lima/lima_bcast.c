// SPDX-License-Identifier: GPL-2.0 OR MIT
/* Copyright 2018-2019 Qiang Yu <yuq825@gmail.com> */

#include <linux/io.h>
#include <linux/device.h>

#include "lima_device.h"
#include "lima_bcast.h"
#include "lima_regs.h"

#define bcast_write(reg, data) writel(data, ip->iomem + reg)
#define bcast_read(reg) readl(ip->iomem + reg)

void lima_bcast_enable(struct lima_device *dev, int num_pp)
{
	struct lima_sched_pipe *pipe = dev->pipe + lima_pipe_pp;
	struct lima_ip *ip = dev->ip + lima_ip_bcast;
	int i, mask = bcast_read(LIMA_BCAST_BROADCAST_MASK) & 0xffff0000;

	for (i = 0; i < num_pp; i++) {
		struct lima_ip *pp = pipe->processor[i];
		mask |= 1 << (pp->id - lima_ip_pp0);
	}

	bcast_write(LIMA_BCAST_BROADCAST_MASK, mask);
}

int lima_bcast_init(struct lima_ip *ip)
{
	int i, mask = 0;

	for (i = lima_ip_pp0; i <= lima_ip_pp7; i++) {
		if (ip->dev->ip[i].present)
			mask |= 1 << (i - lima_ip_pp0);
	}

	bcast_write(LIMA_BCAST_BROADCAST_MASK, mask << 16);
	bcast_write(LIMA_BCAST_INTERRUPT_MASK, mask);
	return 0;
}

void lima_bcast_fini(struct lima_ip *ip)
{
	
}

