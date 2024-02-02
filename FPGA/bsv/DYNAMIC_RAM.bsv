// Copyright (c) 2023 NUDT, dave zhong.  All Rights Reserved.
package DYNAMIC_RAM; //can omit

//====================================================
//elaboration: minimum dynamic BRAM unit
//====================================================
//BSV lib imports
import BRAM::*;
import Vector::*;
import DReg::*;

//====================================================
//Project imports

//====================================================
//Interface definition

interface DYNAMIC_RAM_IFC;
    method Action read_ram(Vector#(101, Bit#(14)) read_addr_vec_a, Vector#(101, Bit#(14)) read_addr_vec_b);
    method ActionValue #(Tuple2#(Bit#(2), Bit#(2))) get_read_res;
    method Action configure_DE_belonging(Bit#(7) id_DE);
    method Action write_data(Bit#(14) addr, Bit#(2) data);
endinterface

//====================================================
//Module def
(* synthesize *)
module mkDYNAMIC_RAM(DYNAMIC_RAM_IFC);

    //internal structure
    Reg#(Maybe#(Vector#(101, Bit#(14)))) read_addr_vec_a_reg <- mkDReg(tagged Invalid);
    Reg#(Maybe#(Vector#(101, Bit#(14)))) read_addr_vec_b_reg <- mkDReg(tagged Invalid);
    Reg#(Bit#(7)) id_DE_reg <- mkReg(0);
    BRAM2Port#(Bit#(14), Bit#(2)) ram <- mkBRAM2Server(defaultValue);

    //rules (behaviors)
    rule mux_read_addr (isValid(read_addr_vec_a_reg));
        Vector#(101, Bit#(14)) read_addr_vec_a = fromMaybe(?, read_addr_vec_a_reg);
        Vector#(101, Bit#(14)) read_addr_vec_b = fromMaybe(?, read_addr_vec_b_reg);
        //vector index: mux implementation
        let read_addr_a = read_addr_vec_a[id_DE_reg];
        let read_addr_b = read_addr_vec_b[id_DE_reg];
        ram.portA.request.put(
            BRAMRequest{
                write: False,
                responseOnWrite: False,
                address: read_addr_a,
                datain: 2'b00 //no matter
            }
        );
        ram.portB.request.put(
            BRAMRequest{
                write: False,
                responseOnWrite: False,
                address: read_addr_b,
                datain: 2'b00 //no matter
            }
        );
    endrule

    //interface implementation
    method Action read_ram(Vector#(101, Bit#(14)) read_addr_vec_a, Vector#(101, Bit#(14)) read_addr_vec_b);
        read_addr_vec_a_reg <= tagged Valid read_addr_vec_a;
        read_addr_vec_b_reg <= tagged Valid read_addr_vec_b;
        /*
        for(Integer i = 0; i < 101; i = i + 1)begin
            read_addr_vec_a_reg[i] <= tagged Valid read_addr_vec_a[i];
            read_addr_vec_b_reg[i] <= tagged Valid read_addr_vec_b[i];
        end*/
    endmethod

    method ActionValue #(Tuple2#(Bit#(2), Bit#(2))) get_read_res;
        Bit#(2) ares <- ram.portA.response.get;
        Bit#(2) bres <- ram.portB.response.get;
        return tuple2(ares, bres);
    endmethod

    method Action configure_DE_belonging(Bit#(7) id_DE);
        id_DE_reg <= id_DE;
    endmethod

    method Action write_data(Bit#(14) addr, Bit#(2) data);
        ram.portA.request.put(
            BRAMRequest{
                write: True,
                responseOnWrite: False,
                address: addr,
                datain: data
            }
        );
    endmethod
endmodule

endpackage