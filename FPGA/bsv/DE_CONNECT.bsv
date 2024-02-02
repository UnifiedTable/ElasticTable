// Copyright (c) 2023 NUDT, dave zhong.  All Rights Reserved.
package DE_CONNECT; //can omit

//====================================================
//elaboration: connect multiple DE with BRAM access
//====================================================
//BSV lib imports
import Vector::*;
import DReg::*;

//====================================================
//Project imports
import DE::*;
import DYNAMIC_RAM::*;

//====================================================
//Interface definition

interface DE_CONNECT_IFC;
    //perform look operation
    method Action lookup (Bit#(48) mac, Bit#(32) srcIP, Bit#(32) dstIP); //取最大位宽，小于该位宽可填0 
    //result output: mac outport, srcIP/dstIP longest match prefix
    method Tuple4#(Bit#(5), Bit#(32), Bit#(32), Bit#(32)) get_res;
    method Action configure_DE_ram(Bit#(7) id_of_DE, Bit#(10) ram_id_start, Bit#(10) ram_id_end);
    method Action write_ram(Bit#(10) ram_id, Bit#(14) addr_wr, Bit#(2) data_wr);
endinterface

//====================================================
//Module def
(* synthesize *)
module mkDE_CONNECT(DE_CONNECT_IFC);

    //internal structure
    Vector#(1024, DYNAMIC_RAM_IFC) dynamic_ram_vec;
    for(Integer i = 0; i < 1024; i = i+1) begin
        dynamic_ram_vec[i] <- mkDYNAMIC_RAM;
    end

    Vector#(5, DE_IFC) mac_de_vec;
    //prefix length from 0 to 31
    Vector#(32, DE_IFC) fib_de_vec;
    Vector#(32, DE_IFC) acl_src_de_vec;
    Vector#(32, DE_IFC) acl_dst_de_vec;
    

    for(Integer i = 0; i < 5; i = i + 1) begin
        Bit#(48) mask = 48'hffffffffffff;
        mac_de_vec[i] <- mkDE(mask);
    end
    for(Integer i = 0; i < 32; i = i + 1) begin
        Bit#(48) mask;
        case(i) 
            0       :   mask = 48'h800000000000;
            1       :   mask = 48'hc00000000000;
            2       :   mask = 48'he00000000000;
            3       :   mask = 48'hf00000000000;
            4       :   mask = 48'hf80000000000;
            5       :   mask = 48'hfc0000000000;
            6       :   mask = 48'hfe0000000000;
            7       :   mask = 48'hff0000000000;
            8       :   mask = 48'hff8000000000;
            9       :   mask = 48'hffc000000000;
            10      :   mask = 48'hffe000000000;
            11      :   mask = 48'hfff000000000;
            12      :   mask = 48'hfff800000000;
            13      :   mask = 48'hfffc00000000;
            14      :   mask = 48'hfffe00000000;
            15      :   mask = 48'hffff00000000;
            16      :   mask = 48'hffff80000000;
            17      :   mask = 48'hffffc0000000;
            18      :   mask = 48'hffffe0000000;
            19      :   mask = 48'hfffff0000000;
            20      :   mask = 48'hfffff8000000;
            21      :   mask = 48'hfffffc000000;
            22      :   mask = 48'hfffffe000000;
            23      :   mask = 48'hffffff000000;
            24      :   mask = 48'hffffff800000;
            25      :   mask = 48'hffffffc00000;
            26      :   mask = 48'hffffffe00000;
            27      :   mask = 48'hfffffff00000;
            28      :   mask = 48'hfffffff80000;
            29      :   mask = 48'hfffffffc0000;
            30      :   mask = 48'hfffffffe0000;
            31      :   mask = 48'hffffffff0000;
        endcase
        fib_de_vec[i] <- mkDE(mask);
        acl_src_de_vec[i] <- mkDE(mask);
        acl_dst_de_vec[i] <- mkDE(mask);
    end

    Vector#(101, DE_IFC) de_vec = append(append(mac_de_vec, fib_de_vec), append(acl_src_de_vec, acl_dst_de_vec));

    //tab ram

    //rules (behaviors)
    rule send_DEread_to_bram;
        //each DE has two read request: a and b 
        Vector#(5,  Bit#(14)) mac_read_addr_vec_a;
        Vector#(5,  Bit#(14)) mac_read_addr_vec_b;
        Vector#(32, Bit#(14)) fib_read_addr_vec_a;
        Vector#(32, Bit#(14)) fib_read_addr_vec_b;
        Vector#(32, Bit#(14)) acl_src_read_addr_vec_a;
        Vector#(32, Bit#(14)) acl_dst_read_addr_vec_a;
        Vector#(32, Bit#(14)) acl_src_read_addr_vec_b;
        Vector#(32, Bit#(14)) acl_dst_read_addr_vec_b;
        //possible bug: match{.a, .b} = DE.get_res;
        for(Integer i = 0; i < 5; i = i + 1) begin
            match{.tmpa, .tmpb} <- mac_de_vec[i].get_read_addr;
            mac_read_addr_vec_a[i] = tmpa;
            mac_read_addr_vec_b[i] = tmpb;
        end
        for(Integer i = 0; i < 32; i = i + 1) begin
            match{.tmpa, .tmpb} <- fib_de_vec[i].get_read_addr;
            fib_read_addr_vec_a[i] = tmpa;
            fib_read_addr_vec_b[i] = tmpb;
            match{.tmpc, .tmpd} <- acl_src_de_vec[i].get_read_addr;
            acl_src_read_addr_vec_a[i] = tmpc;
            acl_src_read_addr_vec_b[i] = tmpd;
            match{.tmpe, .tmpf} <- acl_dst_de_vec[i].get_read_addr;
            acl_dst_read_addr_vec_a[i] = tmpe;
            acl_dst_read_addr_vec_b[i] = tmpf;
        end

        //connect all read addr signals to each RAM unit module
        Vector#(101, Bit#(14)) read_addr_vec_a = append(append(mac_read_addr_vec_a, fib_read_addr_vec_a), append(acl_src_read_addr_vec_a, acl_dst_read_addr_vec_a));
        Vector#(101, Bit#(14)) read_addr_vec_b = append(append(mac_read_addr_vec_b, fib_read_addr_vec_b), append(acl_src_read_addr_vec_b, acl_dst_read_addr_vec_b));
        //large fan out!!
        for(Integer i = 0; i < 1024; i = i+1)begin
            dynamic_ram_vec[i].read_ram(read_addr_vec_a, read_addr_vec_b);
        end
    endrule

    rule send_ram_res_toDE;
        Vector#(1024, Bit#(2)) ram_res_a;
        Vector#(1024, Bit#(2)) ram_res_b;
        for(Integer i = 0; i < 1024; i = i + 1)begin
            match{.tmpa, .tmpb} <- dynamic_ram_vec[i].get_read_res;
            ram_res_a[i] = tmpa;
            ram_res_b[i] = tmpb;
        end
        //todo: connect with each DE
        //large fan in!!
        for(Integer i = 0; i < 101; i = i + 1)begin
            de_vec[i].recv_ram_read(ram_res_a, ram_res_b);
        end
    endrule
    

    //interface implementation
    method Action lookup (Bit#(48) mac, Bit#(32) srcIP, Bit#(32) dstIP);
        for(Integer i = 0; i < 5; i = i + 1)begin
            mac_de_vec[i].lookup(mac);
        end
        for(Integer i = 0; i < 32; i = i + 1)begin
            fib_de_vec[i].lookup(extend(dstIP));
        end
        for(Integer i = 0; i < 32; i = i + 1)begin
            acl_src_de_vec[i].lookup(extend(srcIP));
        end
        for(Integer i = 0; i < 32; i = i + 1)begin
            acl_dst_de_vec[i].lookup(extend(dstIP));
        end
    endmethod

    method Tuple4#(Bit#(5), Bit#(32), Bit#(32), Bit#(32)) get_res;
        Bit#(5) mac_lookup_res;
        Bit#(32) fib_lookup_res;
        Bit#(32) acl_src_lookup_res;
        Bit#(32) acl_dst_lookup_res;
        for(Integer i = 0; i < 5; i = i + 1)begin
            mac_lookup_res[i] = mac_de_vec[i].get_res;
        end
        for(Integer i = 0; i < 32; i = i + 1)begin
            fib_lookup_res[i] = fib_de_vec[i].get_res;
        end
        for(Integer i = 0; i < 32; i = i + 1)begin
            acl_src_lookup_res[i] = acl_src_de_vec[i].get_res;
        end
        for(Integer i = 0; i < 32; i = i + 1)begin
            acl_dst_lookup_res[i] = acl_dst_de_vec[i].get_res;
        end
        return tuple4(mac_lookup_res, fib_lookup_res, acl_src_lookup_res, acl_dst_lookup_res);
    endmethod

    method Action configure_DE_ram(Bit#(7) id_of_DE, Bit#(10) ram_id_start, Bit#(10) ram_id_end);
        de_vec[id_of_DE].config_base_bram(ram_id_start);
        for(Integer i = 0; i < 1024; i = i + 1)begin
            Bit#(10) id_i = fromInteger(i);
            if(id_i >= ram_id_start && id_i <= ram_id_end)
                dynamic_ram_vec[i].configure_DE_belonging(id_of_DE);
        end
    endmethod

    method Action write_ram(Bit#(10) ram_id, Bit#(14) addr_wr, Bit#(2) data_wr);
        dynamic_ram_vec[ram_id].write_data(addr_wr, data_wr);
    endmethod
endmodule

endpackage