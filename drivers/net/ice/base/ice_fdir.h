/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) 2001-2019
 */

#ifndef _ICE_FDIR_H_
#define _ICE_FDIR_H_

#include "ice_common.h"

/* Flow Director (FD) Filter Programming descriptor */
struct ice_fd_fltr_desc_ctx {
	u32 fdid;
	u16 qindex;
	u16 cnt_index;
	u16 fd_vsi;
	u16 flex_val;
	u8 comp_q;
	u8 comp_report;
	u8 fd_space;
	u8 cnt_ena;
	u8 evict_ena;
	u8 toq;
	u8 toq_prio;
	u8 dpu_recipe;
	u8 drop;
	u8 flex_prio;
	u8 flex_mdid;
	u8 dtype;
	u8 pcmd;
	u8 desc_prof_prio;
	u8 desc_prof;
	u8 swap;
	u8 fdid_prio;
	u8 fdid_mdid;
};

enum ice_status ice_alloc_fd_res_cntr(struct ice_hw *hw, u16 *cntr_id);
enum ice_status ice_free_fd_res_cntr(struct ice_hw *hw, u16 cntr_id);
void
ice_set_fd_desc_val(struct ice_fd_fltr_desc_ctx *fd_fltr_ctx,
		    struct ice_fltr_desc *fdir_desc);
void ice_set_dflt_val_fd_desc(struct ice_fd_fltr_desc_ctx *fd_fltr_ctx);
enum ice_status
ice_alloc_fd_guar_item(struct ice_hw *hw, u16 *cntr_id, u16 num_fltr);
enum ice_status
ice_free_fd_guar_item(struct ice_hw *hw, u16 cntr_id, u16 num_fltr);
enum ice_status
ice_alloc_fd_shrd_item(struct ice_hw *hw, u16 *cntr_id, u16 num_fltr);
enum ice_status
ice_free_fd_shrd_item(struct ice_hw *hw, u16 cntr_id, u16 num_fltr);
enum ice_status ice_clear_vsi_fd_table(struct ice_hw *hw, u16 vsi_num);
enum ice_status ice_clear_pf_fd_table(struct ice_hw *hw);

#define ICE_FDIR_IP_PROTOCOLS
#define ICE_IP_PROTO_TCP		6
#define ICE_IP_PROTO_UDP		17
#define ICE_IP_PROTO_SCTP		132
#define ICE_IP_PROTO_IP			0
#define ICE_IP_PROTO_ESP		50

#define ICE_FDIR_MAX_RAW_PKT_SIZE	512
#define ICE_FDIR_BUF_FULL_MARGIN	10
#define ICE_FDIR_BUF_HEAD_ROOM		32

/* macros for offsets into packets for flow director programming */
#define ICE_IPV4_SRC_ADDR_OFFSET	26
#define ICE_IPV4_DST_ADDR_OFFSET	30
#define ICE_IPV4_TCP_SRC_PORT_OFFSET	34
#define ICE_IPV4_TCP_DST_PORT_OFFSET	36
#define ICE_IPV4_UDP_SRC_PORT_OFFSET	34
#define ICE_IPV4_UDP_DST_PORT_OFFSET	36
#define ICE_IPV4_SCTP_SRC_PORT_OFFSET	34
#define ICE_IPV4_SCTP_DST_PORT_OFFSET	36
#define ICE_IPV4_PROTO_OFFSET		23
#define ICE_IPV6_SRC_ADDR_OFFSET	22
#define ICE_IPV6_DST_ADDR_OFFSET	38
#define ICE_IPV6_TCP_SRC_PORT_OFFSET	54
#define ICE_IPV6_TCP_DST_PORT_OFFSET	56
#define ICE_IPV6_UDP_SRC_PORT_OFFSET	54
#define ICE_IPV6_UDP_DST_PORT_OFFSET	56
#define ICE_IPV6_SCTP_SRC_PORT_OFFSET	54
#define ICE_IPV6_SCTP_DST_PORT_OFFSET	56

#define ICE_FDIR_MAX_FLTRS		16384

/* IP v4 has 2 flag bits that enable fragment processing: DF and MF. DF
 * requests that the packet not be fragmented. MF indicates that a packet has
 * been fragmented.
 */
#define ICE_FDIR_IPV4_PKT_FLAG_DF		0x20
#define ICE_FDIR_IPV4_PKT_FLAG_MF		0x40

enum ice_fltr_prgm_desc_dest {
	ICE_FLTR_PRGM_DESC_DEST_DROP_PKT,
	ICE_FLTR_PRGM_DESC_DEST_DIRECT_PKT_QINDEX,
	ICE_FLTR_PRGM_DESC_DEST_DIRECT_PKT_QGROUP,
	ICE_FLTR_PRGM_DESC_DEST_DIRECT_PKT_OTHER,
};

enum ice_fltr_prgm_desc_fd_status {
	ICE_FLTR_PRGM_DESC_FD_STATUS_NONE,
	ICE_FLTR_PRGM_DESC_FD_STATUS_FD_ID,
	ICE_FLTR_PRGM_DESC_FD_STATUS_FD_ID_4FLEX_BYTES,
	ICE_FLTR_PRGM_DESC_FD_STATUS_8FLEX_BYTES,
};

#define ICE_FLTR_PRGM_FLEX_WORD_SIZE	sizeof(__be16)

struct ice_rx_flow_userdef {
	u16 flex_word;
	u16 flex_offset;
	u16 flex_fltr;
};

struct ice_fdir_v4 {
	__be32 dst_ip;
	__be32 src_ip;
	__be16 dst_port;
	__be16 src_port;
	__be32 l4_header;
	__be32 sec_parm_idx;	/* security parameter index */
	u8 tos;
	u8 ip_ver;
	u8 proto;
};

#define ICE_IPV6_ADDR_LEN_AS_U32		4

struct ice_fdir_v6 {
	__be32 dst_ip[ICE_IPV6_ADDR_LEN_AS_U32];
	__be32 src_ip[ICE_IPV6_ADDR_LEN_AS_U32];
	__be16 dst_port;
	__be16 src_port;
	__be32 l4_header; /* next header */
	__be32 sec_parm_idx; /* security parameter index */
	u8 tc;
	u8 proto;
};

struct ice_fdir_extra {
	u8 dst_mac[ETH_ALEN];	/* dest MAC address */
	u32 usr_def[2];		/* user data */
	__be16 vlan_type;	/* VLAN ethertype */
	__be16 vlan_tag;	/* VLAN tag info */
};

struct ice_fdir_fltr {
	struct LIST_ENTRY_TYPE fltr_node;
	enum ice_fltr_ptype flow_type;

	union {
		struct ice_fdir_v4 v4;
		struct ice_fdir_v6 v6;
	} ip, mask;

	struct ice_fdir_extra ext_data;
	struct ice_fdir_extra ext_mask;

	/* flex byte filter data */
	__be16 flex_word;
	u16 flex_offset;
	u16 flex_fltr;

	/* filter control */
	u16 q_index;
	u16 dest_vsi;
	u8 dest_ctl;
	u8 fltr_status;
	u16 cnt_index;
	u32 fltr_id;
};

/* Dummy packet filter definition structure. */
struct ice_fdir_base_pkt {
	enum ice_fltr_ptype flow;
	u16 pkt_len;
	const u8 *pkt;
};

void
ice_fdir_get_prgm_desc(struct ice_hw *hw, struct ice_fdir_fltr *input,
		       struct ice_fltr_desc *fdesc, bool add);
enum ice_status
ice_fdir_get_prgm_pkt(struct ice_fdir_fltr *input, u8 *pkt, bool frag);
enum ice_status
ice_add_del_fdir(struct ice_hw *hw, struct ice_fdir_fltr *input, bool add);
int ice_get_fdir_cnt_all(struct ice_hw *hw);
bool ice_fdir_is_dup_fltr(struct ice_hw *hw, struct ice_fdir_fltr *input);
enum ice_status
ice_update_fdir_list_entry(struct ice_hw *hw, struct ice_fdir_fltr *input,
			   u16 sw_idx);
bool ice_fdir_has_frag(enum ice_fltr_ptype flow);
struct ice_fdir_fltr *
ice_fdir_find_fltr_by_idx(struct ice_hw *hw, u32 fltr_idx);
void
ice_fdir_update_cntrs(struct ice_hw *hw, enum ice_fltr_ptype flow, bool add);
void ice_fdir_list_add_fltr(struct ice_hw *hw, struct ice_fdir_fltr *input);
#endif /* _ICE_FDIR_H_ */
