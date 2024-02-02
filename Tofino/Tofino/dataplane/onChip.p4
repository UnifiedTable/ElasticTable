/*
* 
*/
#include<core.p4>
#if __TARGET_TOFINO__ == 2
#include<t2na.p4>
#else
#include<tna.p4>
#endif

#include "header.p4"
#include "define.p4"

#define SENDER_PORT 144
#define OFFCHIP_PORT 152
#define RECVER_PORT 156

struct ingress_metadata_t {
	bit<32> tmp_in1;
	bit<32> tmp_in2;
	bit<32> tmp_in3;
	bit<32> tmp_in4;
	bit<32> tmp_in5;
	bit<32> tmp_in6;

	bit<8> tmp_out1;
	bit<8> tmp_out2;
	bit<8> tmp_out3;
	bit<8> tmp_out4;
	bit<8> tmp_out5;
	bit<8> tmp_out6;
	bit<8> tmp_out7;
	bit<8> tmp_out8;
	bit<8> tmp_out9;
	bit<8> tmp_out10;
	bit<8> tmp_out11;
	bit<8> tmp_out12;
}
struct egress_metadata_t {}

/*************************************************************************
 **************I N G R E S S  P A R S E R  *******************************
*************************************************************************/
parser IngressParser(
        packet_in pkt,
        out ingress_header_t hdr,
        out ingress_metadata_t meta,
        out ingress_intrinsic_metadata_t ig_intr_md)
{
	state start {
		pkt.extract(ig_intr_md);
		pkt.advance(PORT_METADATA_SIZE);
		transition parse_ethernet;
	}

	state parse_ethernet {
		pkt.extract(hdr.ethernet);
		transition select(hdr.ethernet.ether_type) {
			0x0800: parse_ipv4;
			default: reject;
		}		
	}

	state parse_ipv4 {
		pkt.extract(hdr.ipv4);
		transition select(hdr.ipv4.protocol) {
			17: parse_udp;
			default: accept;
		}
	}

	state parse_udp {
		pkt.extract(hdr.udp);
		pkt.extract(hdr.flag);
		transition select(hdr.flag.pkt_flag) {
			0xffff: parse_send;
			0xfafa: parse_offchip;
			default: accept;
		}
	}

	state parse_send {
		hdr.info.setValid();
		transition accept;
	}

	state parse_offchip {
		pkt.extract(hdr.info);
		transition accept;
	}

}

/*************************************************************************
 ***********************  I N G R E S S  *********************************
*************************************************************************/
control Ingress(inout ingress_header_t hdr,
		inout ingress_metadata_t meta,
		in ingress_intrinsic_metadata_t ig_intr_md,
		in ingress_intrinsic_metadata_from_parser_t ig_prsr_md,
		inout ingress_intrinsic_metadata_for_deparser_t ig_dprsr_md,
		inout ingress_intrinsic_metadata_for_tm_t ig_tm_md)
{

	//stage 0, store flag used to set if open the dataplane query
	Register<bit<8>, _>(1) flag_reg;
	RegisterAction<bit<8>, _, bit<8>>(flag_reg) read_flag_salu = {
		void apply(inout bit<8> reg_data, out bit<8> out_data) {
			out_data = reg_data;
		}
	};
	action read_flag() {
		hdr.flag.table_flag = read_flag_salu.execute(0);
	}
	@stage(0)
	table read_flag_table {
		key = {}
		actions = {
			read_flag;
		}
		size = 1;
		const default_action = read_flag;
	}

	//onChip mac match
	action pre_mac_match() {
		hdr.info.diff_1 = hdr.info.diff_1 & 0x2;
		hdr.info.diff_2 = hdr.info.diff_2 & 0x2;
		hdr.info.diff_3 = hdr.info.diff_3 & 0x2;
		hdr.info.diff_4 = hdr.info.diff_4 & 0x2;
		hdr.info.diff_5 = hdr.info.diff_5 & 0x2;
	}
	@stage(5) 
	table pre_mac_table {
		key = {
			hdr.flag.table_flag: ternary;
		}
		actions = {
			pre_mac_match;
			NoAction;
		}
		size = 16;
		const default_action = NoAction;
		const entries = {
			0x1 & 0x1: pre_mac_match();
		}
	}

	action set_mac_result(bit<32> result) {
		hdr.flag.result = result;
	}
	@stage(6)
	table mac_table{
		key = {
			hdr.info.diff_1: exact;
			hdr.info.diff_2: exact;
			hdr.info.diff_3: exact;
			hdr.info.diff_4: exact;
			hdr.info.diff_5: exact;
		}
		actions = {
			set_mac_result;
			NoAction;
		}
		size = 64;
		const default_action = NoAction;
		const entries = {
			(0, 0, 0, 0, 0): set_mac_result(0);
			(0, 0, 0, 0, 2): set_mac_result(1);
			(0, 0, 0, 2, 0): set_mac_result(2);
			(0, 0, 0, 2, 2): set_mac_result(3);
			(0, 0, 2, 0, 0): set_mac_result(4);
			(0, 0, 2, 0, 2): set_mac_result(5);
			(0, 0, 2, 2, 0): set_mac_result(6);
			(0, 0, 2, 2, 2): set_mac_result(7);
			(0, 2, 0, 0, 0): set_mac_result(8);
			(0, 2, 0, 0, 2): set_mac_result(9);
			(0, 2, 0, 2, 0): set_mac_result(10);
			(0, 2, 0, 2, 2): set_mac_result(11);
			(0, 2, 2, 0, 0): set_mac_result(12);
			(0, 2, 2, 0, 2): set_mac_result(13);
			(0, 2, 2, 2, 0): set_mac_result(14);
			(0, 2, 2, 2, 2): set_mac_result(15);
			(2, 0, 0, 0, 0): set_mac_result(16);
			(2, 0, 0, 0, 2): set_mac_result(17);
			(2, 0, 0, 2, 0): set_mac_result(18);
			(2, 0, 0, 2, 2): set_mac_result(19);
			(2, 0, 2, 0, 0): set_mac_result(20);
			(2, 0, 2, 0, 2): set_mac_result(21);
			(2, 0, 2, 2, 0): set_mac_result(22);
			(2, 0, 2, 2, 2): set_mac_result(23);
			(2, 2, 0, 0, 0): set_mac_result(24);
			(2, 2, 0, 0, 2): set_mac_result(25);
			(2, 2, 0, 2, 0): set_mac_result(26);
			(2, 2, 0, 2, 2): set_mac_result(27);
			(2, 2, 2, 0, 0): set_mac_result(28);
			(2, 2, 2, 0, 2): set_mac_result(29);
			(2, 2, 2, 2, 0): set_mac_result(30);
			(2, 2, 2, 2, 2): set_mac_result(31);
		}
	}

	// forward table
	action forward(PortId_t port) {
        ig_tm_md.ucast_egress_port = port;
	}
	@stage(11)
    table forward_table {
        key = {
            hdr.flag.pkt_flag :exact;
        }
        actions = {
            forward;
        }
        size = 16;
		const default_action = forward(RECVER_PORT);
		const entries = {
			0xffff : forward(OFFCHIP_PORT);
			0xfafa : forward(RECVER_PORT);
		}
    }
	// table used to generate 
	
	// a array of 20 pipelined Diamond Embedders
    DIAMOND_EMBEDDDER(1, 0, 1, 2, hdr.info.diff_1, meta.tmp_in1, meta.tmp_out1, meta.tmp_out2)
    DIAMOND_EMBEDDDER(2, 0, 1, 2, hdr.info.diff_2, meta.tmp_in2, meta.tmp_out3, meta.tmp_out4)
    DIAMOND_EMBEDDDER(3, 1, 2, 3, hdr.info.diff_3, meta.tmp_in3, meta.tmp_out5, meta.tmp_out6)
    DIAMOND_EMBEDDDER(4, 1, 2, 3, hdr.info.diff_4, meta.tmp_in4, meta.tmp_out7, meta.tmp_out8)
    DIAMOND_EMBEDDDER(5, 2, 3, 4, hdr.info.diff_5, meta.tmp_in5, meta.tmp_out9, meta.tmp_out10)
    DIAMOND_EMBEDDDER(6, 2, 3, 4, hdr.info.diff_6, meta.tmp_in6, meta.tmp_out11, meta.tmp_out12)
    DIAMOND_EMBEDDDER(7, 3, 4, 5, hdr.info.diff_7, meta.tmp_in1, meta.tmp_out1, meta.tmp_out2)
    DIAMOND_EMBEDDDER(8, 3, 4, 5, hdr.info.diff_8, meta.tmp_in2, meta.tmp_out3, meta.tmp_out4)
    DIAMOND_EMBEDDDER(9, 4, 5, 6, hdr.info.diff_9, meta.tmp_in3, meta.tmp_out5, meta.tmp_out6)
    DIAMOND_EMBEDDDER(10, 4, 5, 6, hdr.info.diff_10, meta.tmp_in4, meta.tmp_out7, meta.tmp_out8)
    DIAMOND_EMBEDDDER(11, 5, 6, 7, hdr.info.diff_11, meta.tmp_in5, meta.tmp_out9, meta.tmp_out10)
    DIAMOND_EMBEDDDER(12, 5, 6, 7, hdr.info.diff_12, meta.tmp_in6, meta.tmp_out11, meta.tmp_out12)
    DIAMOND_EMBEDDDER(13, 6, 7, 8, hdr.info.diff_13, meta.tmp_in1, meta.tmp_out1, meta.tmp_out2)
    DIAMOND_EMBEDDDER(14, 6, 7, 8, hdr.info.diff_14, meta.tmp_in2, meta.tmp_out3, meta.tmp_out4)
    DIAMOND_EMBEDDDER(15, 7, 8, 9, hdr.info.diff_15, meta.tmp_in3, meta.tmp_out5, meta.tmp_out6)
    DIAMOND_EMBEDDDER(16, 7, 8, 9, hdr.info.diff_16, meta.tmp_in4, meta.tmp_out7, meta.tmp_out8)
    DIAMOND_EMBEDDDER(17, 8, 9, 10, hdr.info.diff_17, meta.tmp_in5, meta.tmp_out9, meta.tmp_out10)
    DIAMOND_EMBEDDDER(18, 8, 9, 10, hdr.info.diff_18, meta.tmp_in6, meta.tmp_out11, meta.tmp_out12)
    DIAMOND_EMBEDDDER(19, 9, 10, 11, hdr.info.diff_19, meta.tmp_in1, meta.tmp_out1, meta.tmp_out2)
    DIAMOND_EMBEDDDER(20, 9, 10, 11, hdr.info.diff_20, meta.tmp_in2, meta.tmp_out3, meta.tmp_out4)



    apply{
		if(hdr.flag.isValid() && hdr.flag.pkt_flag == 0xffff) {
			read_flag_table.apply();
			APPLY_DIAMOND_EMBEDDER(1)
			APPLY_DIAMOND_EMBEDDER(2)
			APPLY_DIAMOND_EMBEDDER(3)
			APPLY_DIAMOND_EMBEDDER(4)
			APPLY_DIAMOND_EMBEDDER(5)
			APPLY_DIAMOND_EMBEDDER(6)
			APPLY_DIAMOND_EMBEDDER(7)
			APPLY_DIAMOND_EMBEDDER(8)
			APPLY_DIAMOND_EMBEDDER(9)
			APPLY_DIAMOND_EMBEDDER(10)
			APPLY_DIAMOND_EMBEDDER(11)
			APPLY_DIAMOND_EMBEDDER(12)
			APPLY_DIAMOND_EMBEDDER(13)
			APPLY_DIAMOND_EMBEDDER(14)
			APPLY_DIAMOND_EMBEDDER(15)
			APPLY_DIAMOND_EMBEDDER(16)
			APPLY_DIAMOND_EMBEDDER(17)
			APPLY_DIAMOND_EMBEDDER(18)
			APPLY_DIAMOND_EMBEDDER(19)
			APPLY_DIAMOND_EMBEDDER(20)
			pre_mac_table.apply();
			mac_table.apply();
			if(hdr.flag.table_flag <= 0x1) {
				hdr.flag.pkt_flag = 0xfafa;
			}
			else {
				hdr.ipv4.total_len = hdr.ipv4.total_len + 20;
				hdr.udp.total_len = hdr.udp.total_len + 20;
			}
		}
		forward_table.apply();
    }
}

/*************************************************************************
 ***************  I N G R E S S  D E P A R S E R *************************
*************************************************************************/
control IngressDeparser(packet_out pkt,
	inout ingress_header_t hdr,
	in ingress_metadata_t meta,
	in ingress_intrinsic_metadata_for_deparser_t ig_dprtr_md)
{
	apply{
		pkt.emit(hdr);
	}
}


/*************************************************************************
 *********************  E G R E S S  P A R S E R *************************
*************************************************************************/
parser EgressParser(packet_in pkt,
	out egress_header_t hdr,
	out egress_metadata_t meta,
	out egress_intrinsic_metadata_t eg_intr_md)
{
	state start{
		pkt.extract(eg_intr_md);
		transition parse_ethernet;
	}

	state parse_ethernet {
		pkt.extract(hdr.ethernet);
		transition select(hdr.ethernet.ether_type) {
			0x0800: parse_ipv4;
			default: reject;
		}		
	}

	state parse_ipv4 {
		pkt.extract(hdr.ipv4);
		transition select(hdr.ipv4.protocol) {
			17: parse_udp;
			default: accept;
		}
	}

	state parse_udp {
		pkt.extract(hdr.udp);
		pkt.extract(hdr.flag);
		pkt.extract(hdr.info);
		transition accept;
	}
}

/*************************************************************************
 *****************************  E G R E S S ******************************
*************************************************************************/
control Egress(inout egress_header_t hdr,
	inout egress_metadata_t meta,
	in egress_intrinsic_metadata_t eg_intr_md,
	in egress_intrinsic_metadata_from_parser_t eg_prsr_md,
	inout egress_intrinsic_metadata_for_deparser_t eg_dprsr_md,
	inout egress_intrinsic_metadata_for_output_port_t eg_oport_md)
{
	POST_PROCESS_RESULT(1, hdr.info.diff_1)
	POST_PROCESS_RESULT(2, hdr.info.diff_2)
	POST_PROCESS_RESULT(3, hdr.info.diff_3)
	POST_PROCESS_RESULT(4, hdr.info.diff_4)
	POST_PROCESS_RESULT(5, hdr.info.diff_5)
	POST_PROCESS_RESULT(6, hdr.info.diff_6)
	POST_PROCESS_RESULT(7, hdr.info.diff_7)
	POST_PROCESS_RESULT(8, hdr.info.diff_8)
	POST_PROCESS_RESULT(9, hdr.info.diff_9)
	POST_PROCESS_RESULT(10, hdr.info.diff_10)
	POST_PROCESS_RESULT(11, hdr.info.diff_11)
	POST_PROCESS_RESULT(12, hdr.info.diff_12)
	POST_PROCESS_RESULT(13, hdr.info.diff_13)
	POST_PROCESS_RESULT(14, hdr.info.diff_14)
	POST_PROCESS_RESULT(15, hdr.info.diff_15)
	POST_PROCESS_RESULT(16, hdr.info.diff_16)
	POST_PROCESS_RESULT(17, hdr.info.diff_17)
	POST_PROCESS_RESULT(18, hdr.info.diff_18)
	POST_PROCESS_RESULT(19, hdr.info.diff_19)
	POST_PROCESS_RESULT(20, hdr.info.diff_20)
	apply{
		if(hdr.flag.pkt_flag == 0xffff) {
			APPLY_POST_PROCESS_RESULT(1)
			APPLY_POST_PROCESS_RESULT(2)
			APPLY_POST_PROCESS_RESULT(3)
			APPLY_POST_PROCESS_RESULT(4)
			APPLY_POST_PROCESS_RESULT(5)
			APPLY_POST_PROCESS_RESULT(6)
			APPLY_POST_PROCESS_RESULT(7)
			APPLY_POST_PROCESS_RESULT(8)
			APPLY_POST_PROCESS_RESULT(9)
			APPLY_POST_PROCESS_RESULT(10)
			APPLY_POST_PROCESS_RESULT(11)
			APPLY_POST_PROCESS_RESULT(12)
			APPLY_POST_PROCESS_RESULT(13)
			APPLY_POST_PROCESS_RESULT(14)
			APPLY_POST_PROCESS_RESULT(15)
			APPLY_POST_PROCESS_RESULT(16)
			APPLY_POST_PROCESS_RESULT(17)
			APPLY_POST_PROCESS_RESULT(18)
			APPLY_POST_PROCESS_RESULT(19)
			APPLY_POST_PROCESS_RESULT(20)
		}
		else {
			hdr.info.setInvalid();
		}
	}
}
/*************************************************************************
 *********************  E G R E S S  D E P A R S E R *********************
*************************************************************************/
control EgressDeparser(packet_out pkt,
	inout egress_header_t hdr,
	in egress_metadata_t meta,
	in egress_intrinsic_metadata_for_deparser_t eg_dprsr_md)
{
	apply{
        pkt.emit(hdr);
	}
}


/*************************************************************************
 ***************************  M A I N ************************************
*************************************************************************/
Pipeline(IngressParser(),Ingress(),IngressDeparser(),
EgressParser(),Egress(),EgressDeparser()) pipe;


Switch(pipe) main;

