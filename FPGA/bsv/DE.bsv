// Copyright (c) 2023 NUDT, dave zhong.  All Rights Reserved.
package DE; //can omit
//====================================================
//elaboration: implement MCE
//====================================================
//BSV lib imports
import Vector::*;
import DReg::*;
import FIFO::*;

//====================================================
//Project imports
import MYCRC::*;

//====================================================
//Interface definition

interface DE_IFC;
    //perform look operation
    method Action lookup (Bit#(48) value); //取最大位宽，小于该位宽可填0 
    //result output
    method Bit#(1) get_res;
    method ActionValue #(Tuple2#(Bit#(14), Bit#(14))) get_read_addr;
    //get ram read data
    method Action recv_ram_read(Vector#(1024, Bit#(2)) rd_data_vec_a, Vector#(1024, Bit#(2)) rd_data_vec_b);
       //configure bram base addr
    method Action config_base_bram(Bit#(10) base_addr);
endinterface

//====================================================
//Module def
(* synthesize *)
module mkDE#(parameter Bit#(48) mask)(DE_IFC);

    //internal structure
    Reg#(Bit#(10))          bram_base_addr  <-  mkReg(0);
    Reg#(Maybe#(Bit#(48)))  masked_value    <-  mkDReg(tagged Invalid);
    Reg#(Maybe#(Bit#(64)))  hash_value      <-  mkDReg(tagged Invalid);
    Reg#(Maybe#(Bit#(24)))  read_addr1      <-  mkDReg(tagged Invalid);
    Reg#(Maybe#(Bit#(24)))  read_addr2      <-  mkDReg(tagged Invalid);
    Reg#(Bit#(2))           rg_ram_rd_a_res <-  mkReg(0);
    Reg#(Bit#(2))           rg_ram_rd_b_res <-  mkReg(0);
    Reg#(Bool)              final_ram_valid <-  mkDReg(False);
    
    Reg#(Vector#(1024, Bit#(2))) rd_data_vec_rga <- mkReg(replicate(0));
    Reg#(Vector#(1024, Bit#(2))) rd_data_vec_rgb <- mkReg(replicate(0));
    Reg#(Bool)              ram_read_valid  <-  mkDReg(False);

    FIFO#(Bit#(10))         fifo_rd_ram_ida <-  mkSizedFIFO(5);
    FIFO#(Bit#(10))         fifo_rd_ram_idb <-  mkSizedFIFO(5);

    //tab ram

    //rules (behaviors)
    rule calc_crc (isValid(masked_value));
        Bit#(64) crcIn = 64'hffffffff99999999;
        let value = fromMaybe(?, masked_value);
        hash_value <= tagged Valid calc_crc(crcIn, value);
    endrule

    rule addr_add_base (isValid(hash_value));
        Bit#(48) hash_high48 = fromMaybe(?, hash_value)[63:16];
        Bit#(24) addr1 = hash_high48[47:24];// + bram_base_addr;
        Bit#(24) addr2 = hash_high48[23:0];// + bram_base_addr;
        read_addr1 <= tagged Valid addr1;
        read_addr2 <= tagged Valid addr2;
    endrule

    rule mux_choose_rd_data (ram_read_valid);
        let ida = fifo_rd_ram_ida.first;
        let idb = fifo_rd_ram_idb.first;
        fifo_rd_ram_ida.deq;
        fifo_rd_ram_idb.deq;
        //mux implementation
        rg_ram_rd_a_res <= rd_data_vec_rga[ida];
        rg_ram_rd_b_res <= rd_data_vec_rgb[ida];
        final_ram_valid <= True;
    endrule

    //interface implementation
    method Action lookup (Bit#(48) value);
        masked_value <= tagged Valid (value & mask);
    endmethod

    method Action config_base_bram(Bit#(10) base_addr);
        bram_base_addr  <=  base_addr;
    endmethod

    method ActionValue #(Tuple2#(Bit#(14), Bit#(14))) get_read_addr if(isValid(read_addr1));
        fifo_rd_ram_ida.enq(fromMaybe(?, read_addr1)[23:14]);
        fifo_rd_ram_idb.enq(fromMaybe(?, read_addr2)[23:14]);
        return tuple2(fromMaybe(?, read_addr1)[13:0], fromMaybe(?, read_addr2)[13:0]);
    endmethod

    method Bit#(1) get_res if(final_ram_valid);
        Bit#(1) res = (((rg_ram_rd_a_res - rg_ram_rd_b_res) & 2'b10) == 2'b10) ? 1'b1 : 1'b0;
        return res;
    endmethod 

    method Action recv_ram_read(Vector#(1024, Bit#(2)) rd_data_vec_a, Vector#(1024, Bit#(2)) rd_data_vec_b);
        rd_data_vec_rga <= rd_data_vec_a;
        rd_data_vec_rgb <= rd_data_vec_b;
        ram_read_valid <= True;
    endmethod
endmodule

endpackage