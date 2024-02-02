// Copyright (c) 2023 NUDT, dave zhong.  All Rights Reserved.
//====================================================
//elaboration
//testbench through stmt implementation
//====================================================
//BSV lib imports
import StmtFSM::*;
import Vector::*;
import RegFile::*;
//====================================================
//Project imports
import DE_TOP::*;
import Utils::*;

//====================================================
//Module def
(* synthesize *)
module mkTb(Empty);
    //internal structure
    DE_TOP_IFC de_top <- mkDE_TOP;

    //rule

    //stmt motivation
    Stmt test = seq
        $display("cur cycle: %d, stmt starting...", cur_cycle);
        de_top.lookup(48'h123456789abc, 32'h12345678, 32'h87654321);
        //$display("cur cycle: %d, res:", de_top.get_res);
    endseq;
    mkAutoFSM(test);
endmodule
