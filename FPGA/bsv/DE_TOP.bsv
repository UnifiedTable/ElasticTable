// Copyright (c) 2023 NUDT, dave zhong.  All Rights Reserved.
package DE_TOP; //can omit

//====================================================
//elaboration: transform DE bit res to ip prefix
//====================================================
//BSV lib imports
import Vector::*;
import DReg::*;
import FIFO::*;

//====================================================
//Project imports
import DE_CONNECT::*;

//====================================================
//Interface definition

interface DE_TOP_IFC;
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
module mkDE_TOP(DE_TOP_IFC);

//internal structure
    DE_CONNECT_IFC de_connect <- mkDE_CONNECT;
    Reg#(Maybe#(Tuple4#(Bit#(5), Bit#(32), Bit#(32), Bit#(32)))) de_bit_res_tup_rg <- mkDReg(tagged Invalid);
    Reg#(Maybe#(Tuple4#(Bit#(5), Bit#(32), Bit#(32), Bit#(32)))) real_res_rg <- mkDReg(tagged Invalid);
    FIFO#(Bit#(32)) fifo_srcip <- mkSizedFIFO(20);
    FIFO#(Bit#(32)) fifo_dstip <- mkSizedFIFO(20);



//rules (behaviours)
    rule get_de_res;
        de_bit_res_tup_rg <= tagged Valid de_connect.get_res;
    endrule

    rule gen_real_res if(isValid(de_bit_res_tup_rg));
        Vector#(3, Bit#(32)) ip_vec;
        let de_bit_res_tup = fromMaybe(?, de_bit_res_tup_rg);
        match{.mac_port, .fib_res, .acl_src_res, .acl_dst_res} = de_bit_res_tup;
        ip_vec[0] = fifo_dstip.first;
        ip_vec[1] = fifo_srcip.first;
        ip_vec[2] = fifo_dstip.first;
        fifo_dstip.deq;
        fifo_srcip.deq;

        Vector#(3, Bit#(32)) mask_ip_vec;
        Vector#(3, Bit#(32)) de_ip_res_vec;// = append(append(fib_res, acl_src_res), acl_dst_res);
        de_ip_res_vec[0] = fib_res;
        de_ip_res_vec[1] = acl_src_res;
        de_ip_res_vec[2] = acl_dst_res;
        for(Integer i = 0; i < 3; i = i + 1)begin
            mask_ip_vec[i] = case(de_ip_res_vec[i]) matches
                32'b1???????????????????????????????: return ip_vec[i] & 32'h80000000;
                32'b01??????????????????????????????: return ip_vec[i] & 32'hc0000000;
                32'b001?????????????????????????????: return ip_vec[i] & 32'he0000000;
                32'b0001????????????????????????????: return ip_vec[i] & 32'hf0000000;
                32'b00001???????????????????????????: return ip_vec[i] & 32'hf8000000;
                32'b000001??????????????????????????: return ip_vec[i] & 32'hfc000000;
                32'b0000001?????????????????????????: return ip_vec[i] & 32'hfe000000;
                32'b00000001????????????????????????: return ip_vec[i] & 32'hff000000;
                32'b000000001???????????????????????: return ip_vec[i] & 32'hff800000;
                32'b0000000001??????????????????????: return ip_vec[i] & 32'hffc00000;
                32'b00000000001?????????????????????: return ip_vec[i] & 32'hffe00000;
                32'b000000000001????????????????????: return ip_vec[i] & 32'hfff00000;
                32'b0000000000001???????????????????: return ip_vec[i] & 32'hfff80000;
                32'b00000000000001??????????????????: return ip_vec[i] & 32'hfffc0000;
                32'b000000000000001?????????????????: return ip_vec[i] & 32'hfffe0000;
                32'b0000000000000001????????????????: return ip_vec[i] & 32'hffff0000;
                32'b00000000000000001???????????????: return ip_vec[i] & 32'hffff8000;
                32'b000000000000000001??????????????: return ip_vec[i] & 32'hffffc000;
                32'b0000000000000000001?????????????: return ip_vec[i] & 32'hffffe000;
                32'b00000000000000000001????????????: return ip_vec[i] & 32'hfffff000;
                32'b000000000000000000001???????????: return ip_vec[i] & 32'hfffff800;
                32'b0000000000000000000001??????????: return ip_vec[i] & 32'hfffffc00;
                32'b00000000000000000000001?????????: return ip_vec[i] & 32'hfffffe00;
                32'b000000000000000000000001????????: return ip_vec[i] & 32'hffffff00;
                32'b0000000000000000000000001???????: return ip_vec[i] & 32'hffffff80;
                32'b00000000000000000000000001??????: return ip_vec[i] & 32'hffffffc0;
                32'b000000000000000000000000001?????: return ip_vec[i] & 32'hffffffe0;
                32'b0000000000000000000000000001????: return ip_vec[i] & 32'hfffffff0;
                32'b00000000000000000000000000001???: return ip_vec[i] & 32'hfffffff8;
                32'b000000000000000000000000000001??: return ip_vec[i] & 32'hfffffffc;
                32'b0000000000000000000000000000001?: return ip_vec[i] & 32'hfffffffe;
                32'b00000000000000000000000000000001: return ip_vec[i];
            endcase;
        end
        real_res_rg <= tagged Valid tuple4(mac_port, mask_ip_vec[0], mask_ip_vec[1], mask_ip_vec[2]);
    endrule

//interface implementation
//perform look operation
    method Action lookup (Bit#(48) mac, Bit#(32) srcIP, Bit#(32) dstIP);
        fifo_srcip.enq(srcIP);
        fifo_dstip.enq(dstIP);
        de_connect.lookup(mac, srcIP, dstIP); 
    endmethod

    //result output: mac outport, srcIP/dstIP longest match prefix
    method Tuple4#(Bit#(5), Bit#(32), Bit#(32), Bit#(32)) get_res if(isValid(real_res_rg));
        return fromMaybe(?, real_res_rg);
    endmethod

    method Action configure_DE_ram(Bit#(7) id_of_DE, Bit#(10) ram_id_start, Bit#(10) ram_id_end);
        de_connect.configure_DE_ram(id_of_DE, ram_id_start, ram_id_end);
    endmethod

    method Action write_ram(Bit#(10) ram_id, Bit#(14) addr_wr, Bit#(2) data_wr);
        de_connect.write_ram(ram_id, addr_wr, data_wr);
    endmethod

endmodule

endpackage