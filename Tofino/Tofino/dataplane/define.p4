
/*************************************************************************
 ************* C O N S T A N T S    A N D   T Y P E S  *******************
*************************************************************************/
const bit<32> LAYER_SIZE = 1<<12;

/*************************************************************************
 ****************** D E F I N E   R E G I S T E R  ***********************
*************************************************************************/

#define DIAMOND_EMBEDDDER(index, stg1, stg2, stg3, output_addr, tmp_in, tmp_out1, tmp_out2) \
    Hash<bit<16>>(HashAlgorithm_t.IDENTITY) hash_##index##_1;\
    Hash<bit<16>>(HashAlgorithm_t.CRC16) hash_##index##_2;\
    action calculate_key_##index##_srcip(bit<32> mask) {\
        ##tmp_in## = hdr.flag.src_ip & mask;\
    }\
    \
    action calculate_key_##index##_dstip(bit<32> mask) {\
        ##tmp_in## = hdr.flag.dst_ip & mask;\
    }\
    \
    action calculate_key_##index##_mac() {\
        ##tmp_in## = hdr.flag.mac_addr[31:0];\
    }\
    \
    @stage(##stg1##)\
    table calculate_key_table_##index## {\
        key = {}\
        actions = {\
            calculate_key_##index##_srcip;\
            calculate_key_##index##_dstip;\
            calculate_key_##index##_mac;\
            NoAction;\
        }\
        size = 256;\
        default_action = NoAction;\
    }\
    \
    Register<bit<8>, _>(LAYER_SIZE) diamond_embedder_##index##_1;\
    RegisterAction<bit<8>, _, bit<8>>(diamond_embedder_##index##_1) read_de_##index##_1_salu = {\
        void apply(inout bit<8> reg_data, out bit<8> out_data) {\
            out_data = reg_data;\
        }\
    };\
    \
    action read_de_##index##_1_action() {\
        ##tmp_out1## = read_de_##index##_1_salu.execute(hash_##index##_1.get(##tmp_in##));\
    }\
    \
    @stage(##stg2##)\
    table read_de_##index##_1 {\
        key = {}\
        actions = {\
            read_de_##index##_1_action;\
        }\
        size = 1;\
        default_action = read_de_##index##_1_action;\
    }\
    \
    Register<bit<8>, _>(LAYER_SIZE) diamond_embedder_##index##_2;\
    RegisterAction<bit<8>, _, bit<8>>(diamond_embedder_##index##_2) read_de_##index##_2_salu = {\
        void apply(inout bit<8> reg_data, out bit<8> out_data) {\
            out_data = reg_data;\
        }\
    };\
    \
    action read_de_##index##_2_action() {\
        ##tmp_out2## = read_de_##index##_2_salu.execute(hash_##index##_2.get(##tmp_in##));\
    }\
    \
    @stage(##stg2##)\
    table read_de_##index##_2 {\
        key = {}\
        actions = {\
            read_de_##index##_2_action;\
        }\
        size = 1;\
        default_action = read_de_##index##_2_action;\
    }\
    \
    action calculate_diff_##index##() {\
        ##output_addr## = ##tmp_out2## - ##tmp_out1##;\
    }\
    \
    @stage(##stg3##)\
    table calculate_diff_table_##index## {\
        key = {}\
        actions = {\
            calculate_diff_##index##;\
            NoAction;\
        }\
        size = 1;\
        default_action = calculate_diff_##index##;\
    }\
    


#define APPLY_DIAMOND_EMBEDDER(index) \
    calculate_key_table_##index##.apply();\
    read_de_##index##_1.apply();\
    read_de_##index##_2.apply();\
    calculate_diff_table_##index##.apply();\



#define POST_PROCESS_RESULT(index, result) \
    action set_result_##index##(bit<8> res) {\
        ##result## = res;\
    }\
    \
    table set_result_table_##index## {\
        key = {\
            ##result##: ternary;\
        }\
        actions = {\
            set_result_##index##;\
        }\
        size = 2;\
        const entries = {\
            0 &&& 0x2: set_result_##index##(0);\
            2 &&& 0x2: set_result_##index##(1);\
        }\
    }\



#define APPLY_POST_PROCESS_RESULT(index) set_result_table_##index##.apply();