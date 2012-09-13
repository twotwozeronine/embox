/**
 * @file
 *
 * @date Sep 6, 2012
 * @author: Anton Bondarev
 */

#include <drivers/pci.h>
#include <framework/mod/api.h>

ARRAY_SPREAD_DEF(const struct pci_driver *,
		__pci_driver_registry);


static const struct pci_driver * pci_driver_find(uint16_t ven_id, uint16_t dev_id) {
	const struct pci_driver *pci_drv;
	array_foreach(pci_drv, __pci_driver_registry, ARRAY_SPREAD_SIZE(__pci_driver_registry)) {
		if(ven_id == pci_drv->ven_id &&  dev_id == pci_drv->dev_id) {
			return pci_drv;
		}
	}
	return NULL;
}

int pci_driver_load(struct pci_slot_dev *dev) {
	const struct pci_driver *drv;
	const struct mod *dep;

	assert(dev != NULL);

	drv = pci_driver_find(dev->vendor, dev->device);

	if(NULL == drv) {
		return -1;
	}

	/* Enable mod (and dependencies) without cyclic detection error generating.
	 * This introduced since some driver can be inited before pci_driver_load, then next lines
	 * just ensure that other dependecies of driver are satisfied */
	mod_foreach_requires(dep, drv->mod) {
		mod_enable_rec_safe(dep, true);
	}

	drv->init(dev);

	return 0;
}


