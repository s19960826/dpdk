# SPDX-License-Identifier: BSD-3-Clause
# Copyright 2018 NXP

version = 2

build = dpdk_conf.has('RTE_LIBRTE_DPAA2_MEMPOOL')
reason = 'missing dependency, DPDK DPAA2 mempool driver'
deps += ['rawdev', 'mempool_dpaa2', 'bus_vdev']
sources = files('dpaa2_cmdif.c')

allow_experimental_apis = true

install_headers('rte_pmd_dpaa2_cmdif.h')
