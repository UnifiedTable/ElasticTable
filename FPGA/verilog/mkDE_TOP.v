//
// Generated by Bluespec Compiler, version 2023.07 (build 0eb551dc)
//
// On Thu Jan 11 17:01:19 CST 2024
//
//
// Ports:
// Name                         I/O  size props
// RDY_lookup                     O     1
// get_res_fst                    O     5 reg
// RDY_get_res_fst                O     1 reg
// get_res_snd_fst                O    32 reg
// RDY_get_res_snd_fst            O     1 reg
// get_res_snd_snd_fst            O    32 reg
// RDY_get_res_snd_snd_fst        O     1 reg
// get_res_snd_snd_snd            O    32 reg
// RDY_get_res_snd_snd_snd        O     1 reg
// RDY_configure_DE_ram           O     1 const
// RDY_write_ram                  O     1
// CLK                            I     1 clock
// RST_N                          I     1 reset
// lookup_mac                     I    48
// lookup_srcIP                   I    32
// lookup_dstIP                   I    32
// configure_DE_ram_id_of_DE      I     7
// configure_DE_ram_ram_id_start  I    10
// configure_DE_ram_ram_id_end    I    10
// write_ram_ram_id               I    10
// write_ram_addr_wr              I    14
// write_ram_data_wr              I     2
// EN_lookup                      I     1
// EN_configure_DE_ram            I     1
// EN_write_ram                   I     1
//
// No combinational paths from inputs to outputs
//
//

`ifdef BSV_ASSIGNMENT_DELAY
`else
  `define BSV_ASSIGNMENT_DELAY
`endif

`ifdef BSV_POSITIVE_RESET
  `define BSV_RESET_VALUE 1'b1
  `define BSV_RESET_EDGE posedge
`else
  `define BSV_RESET_VALUE 1'b0
  `define BSV_RESET_EDGE negedge
`endif

module mkDE_TOP(CLK,
		RST_N,

		lookup_mac,
		lookup_srcIP,
		lookup_dstIP,
		EN_lookup,
		RDY_lookup,

		get_res_fst,
		RDY_get_res_fst,

		get_res_snd_fst,
		RDY_get_res_snd_fst,

		get_res_snd_snd_fst,
		RDY_get_res_snd_snd_fst,

		get_res_snd_snd_snd,
		RDY_get_res_snd_snd_snd,

		configure_DE_ram_id_of_DE,
		configure_DE_ram_ram_id_start,
		configure_DE_ram_ram_id_end,
		EN_configure_DE_ram,
		RDY_configure_DE_ram,

		write_ram_ram_id,
		write_ram_addr_wr,
		write_ram_data_wr,
		EN_write_ram,
		RDY_write_ram);
  input  CLK;
  input  RST_N;

  // action method lookup
  input  [47 : 0] lookup_mac;
  input  [31 : 0] lookup_srcIP;
  input  [31 : 0] lookup_dstIP;
  input  EN_lookup;
  output RDY_lookup;

  // value method get_res_fst
  output [4 : 0] get_res_fst;
  output RDY_get_res_fst;

  // value method get_res_snd_fst
  output [31 : 0] get_res_snd_fst;
  output RDY_get_res_snd_fst;

  // value method get_res_snd_snd_fst
  output [31 : 0] get_res_snd_snd_fst;
  output RDY_get_res_snd_snd_fst;

  // value method get_res_snd_snd_snd
  output [31 : 0] get_res_snd_snd_snd;
  output RDY_get_res_snd_snd_snd;

  // action method configure_DE_ram
  input  [6 : 0] configure_DE_ram_id_of_DE;
  input  [9 : 0] configure_DE_ram_ram_id_start;
  input  [9 : 0] configure_DE_ram_ram_id_end;
  input  EN_configure_DE_ram;
  output RDY_configure_DE_ram;

  // action method write_ram
  input  [9 : 0] write_ram_ram_id;
  input  [13 : 0] write_ram_addr_wr;
  input  [1 : 0] write_ram_data_wr;
  input  EN_write_ram;
  output RDY_write_ram;

  // signals for module outputs
  wire [31 : 0] get_res_snd_fst, get_res_snd_snd_fst, get_res_snd_snd_snd;
  wire [4 : 0] get_res_fst;
  wire RDY_configure_DE_ram,
       RDY_get_res_fst,
       RDY_get_res_snd_fst,
       RDY_get_res_snd_snd_fst,
       RDY_get_res_snd_snd_snd,
       RDY_lookup,
       RDY_write_ram;

  // inlined wires
  wire [101 : 0] de_bit_res_tup_rg_1$wget, real_res_rg_1$wget;
  wire de_bit_res_tup_rg_1$whas, real_res_rg_1$whas;

  // register de_bit_res_tup_rg
  reg [101 : 0] de_bit_res_tup_rg;
  wire [101 : 0] de_bit_res_tup_rg$D_IN;
  wire de_bit_res_tup_rg$EN;

  // register real_res_rg
  reg [101 : 0] real_res_rg;
  wire [101 : 0] real_res_rg$D_IN;
  wire real_res_rg$EN;

  // ports of submodule de_connect
  wire [47 : 0] de_connect$lookup_mac;
  wire [31 : 0] de_connect$get_res_snd_fst,
		de_connect$get_res_snd_snd_fst,
		de_connect$get_res_snd_snd_snd,
		de_connect$lookup_dstIP,
		de_connect$lookup_srcIP;
  wire [13 : 0] de_connect$write_ram_addr_wr;
  wire [9 : 0] de_connect$configure_DE_ram_ram_id_end,
	       de_connect$configure_DE_ram_ram_id_start,
	       de_connect$write_ram_ram_id;
  wire [6 : 0] de_connect$configure_DE_ram_id_of_DE;
  wire [4 : 0] de_connect$get_res_fst;
  wire [1 : 0] de_connect$write_ram_data_wr;
  wire de_connect$EN_configure_DE_ram,
       de_connect$EN_lookup,
       de_connect$EN_write_ram,
       de_connect$RDY_get_res_fst,
       de_connect$RDY_get_res_snd_fst,
       de_connect$RDY_get_res_snd_snd_fst,
       de_connect$RDY_get_res_snd_snd_snd,
       de_connect$RDY_write_ram;

  // ports of submodule fifo_dstip
  wire [31 : 0] fifo_dstip$D_IN, fifo_dstip$D_OUT;
  wire fifo_dstip$CLR,
       fifo_dstip$DEQ,
       fifo_dstip$EMPTY_N,
       fifo_dstip$ENQ,
       fifo_dstip$FULL_N;

  // ports of submodule fifo_srcip
  wire [31 : 0] fifo_srcip$D_IN, fifo_srcip$D_OUT;
  wire fifo_srcip$CLR,
       fifo_srcip$DEQ,
       fifo_srcip$EMPTY_N,
       fifo_srcip$ENQ,
       fifo_srcip$FULL_N;

  // remaining internal signals
  wire [31 : 0] de_bit_res_tup_rg_BITS_31_TO_0__q2,
		de_bit_res_tup_rg_BITS_63_TO_32__q3,
		de_bit_res_tup_rg_BITS_95_TO_64__q1,
		x__h1469,
		x__h3034,
		x__h4235;

  // action method lookup
  assign RDY_lookup = fifo_srcip$FULL_N && fifo_dstip$FULL_N ;

  // value method get_res_fst
  assign get_res_fst = real_res_rg[100:96] ;
  assign RDY_get_res_fst = real_res_rg[101] ;

  // value method get_res_snd_fst
  assign get_res_snd_fst = real_res_rg[95:64] ;
  assign RDY_get_res_snd_fst = real_res_rg[101] ;

  // value method get_res_snd_snd_fst
  assign get_res_snd_snd_fst = real_res_rg[63:32] ;
  assign RDY_get_res_snd_snd_fst = real_res_rg[101] ;

  // value method get_res_snd_snd_snd
  assign get_res_snd_snd_snd = real_res_rg[31:0] ;
  assign RDY_get_res_snd_snd_snd = real_res_rg[101] ;

  // action method configure_DE_ram
  assign RDY_configure_DE_ram = 1'd1 ;

  // action method write_ram
  assign RDY_write_ram = de_connect$RDY_write_ram ;

  // submodule de_connect
  mkDE_CONNECT de_connect(.CLK(CLK),
			  .RST_N(RST_N),
			  .configure_DE_ram_id_of_DE(de_connect$configure_DE_ram_id_of_DE),
			  .configure_DE_ram_ram_id_end(de_connect$configure_DE_ram_ram_id_end),
			  .configure_DE_ram_ram_id_start(de_connect$configure_DE_ram_ram_id_start),
			  .lookup_dstIP(de_connect$lookup_dstIP),
			  .lookup_mac(de_connect$lookup_mac),
			  .lookup_srcIP(de_connect$lookup_srcIP),
			  .write_ram_addr_wr(de_connect$write_ram_addr_wr),
			  .write_ram_data_wr(de_connect$write_ram_data_wr),
			  .write_ram_ram_id(de_connect$write_ram_ram_id),
			  .EN_lookup(de_connect$EN_lookup),
			  .EN_configure_DE_ram(de_connect$EN_configure_DE_ram),
			  .EN_write_ram(de_connect$EN_write_ram),
			  .RDY_lookup(),
			  .get_res_fst(de_connect$get_res_fst),
			  .RDY_get_res_fst(de_connect$RDY_get_res_fst),
			  .get_res_snd_fst(de_connect$get_res_snd_fst),
			  .RDY_get_res_snd_fst(de_connect$RDY_get_res_snd_fst),
			  .get_res_snd_snd_fst(de_connect$get_res_snd_snd_fst),
			  .RDY_get_res_snd_snd_fst(de_connect$RDY_get_res_snd_snd_fst),
			  .get_res_snd_snd_snd(de_connect$get_res_snd_snd_snd),
			  .RDY_get_res_snd_snd_snd(de_connect$RDY_get_res_snd_snd_snd),
			  .RDY_configure_DE_ram(),
			  .RDY_write_ram(de_connect$RDY_write_ram));

  // submodule fifo_dstip
  SizedFIFO #(.p1width(32'd32),
	      .p2depth(32'd20),
	      .p3cntr_width(32'd5),
	      .guarded(1'd1)) fifo_dstip(.RST(RST_N),
					 .CLK(CLK),
					 .D_IN(fifo_dstip$D_IN),
					 .ENQ(fifo_dstip$ENQ),
					 .DEQ(fifo_dstip$DEQ),
					 .CLR(fifo_dstip$CLR),
					 .D_OUT(fifo_dstip$D_OUT),
					 .FULL_N(fifo_dstip$FULL_N),
					 .EMPTY_N(fifo_dstip$EMPTY_N));

  // submodule fifo_srcip
  SizedFIFO #(.p1width(32'd32),
	      .p2depth(32'd20),
	      .p3cntr_width(32'd5),
	      .guarded(1'd1)) fifo_srcip(.RST(RST_N),
					 .CLK(CLK),
					 .D_IN(fifo_srcip$D_IN),
					 .ENQ(fifo_srcip$ENQ),
					 .DEQ(fifo_srcip$DEQ),
					 .CLR(fifo_srcip$CLR),
					 .D_OUT(fifo_srcip$D_OUT),
					 .FULL_N(fifo_srcip$FULL_N),
					 .EMPTY_N(fifo_srcip$EMPTY_N));

  // inlined wires
  assign de_bit_res_tup_rg_1$wget =
	     { 1'd1,
	       de_connect$get_res_fst,
	       de_connect$get_res_snd_fst,
	       de_connect$get_res_snd_snd_fst,
	       de_connect$get_res_snd_snd_snd } ;
  assign de_bit_res_tup_rg_1$whas =
	     de_connect$RDY_get_res_fst && de_connect$RDY_get_res_snd_fst &&
	     de_connect$RDY_get_res_snd_snd_fst &&
	     de_connect$RDY_get_res_snd_snd_snd ;
  assign real_res_rg_1$wget =
	     { 1'd1,
	       de_bit_res_tup_rg[100:96],
	       x__h1469,
	       x__h3034,
	       x__h4235 } ;
  assign real_res_rg_1$whas =
	     fifo_dstip$EMPTY_N && fifo_srcip$EMPTY_N &&
	     de_bit_res_tup_rg[101] ;

  // register de_bit_res_tup_rg
  assign de_bit_res_tup_rg$D_IN =
	     de_bit_res_tup_rg_1$whas ?
	       de_bit_res_tup_rg_1$wget :
	       102'h0AAAAAAAAAAAAAAAAAAAAAAAAA ;
  assign de_bit_res_tup_rg$EN = 1'd1 ;

  // register real_res_rg
  assign real_res_rg$D_IN =
	     real_res_rg_1$whas ?
	       real_res_rg_1$wget :
	       102'h0AAAAAAAAAAAAAAAAAAAAAAAAA ;
  assign real_res_rg$EN = 1'd1 ;

  // submodule de_connect
  assign de_connect$configure_DE_ram_id_of_DE = configure_DE_ram_id_of_DE ;
  assign de_connect$configure_DE_ram_ram_id_end =
	     configure_DE_ram_ram_id_end ;
  assign de_connect$configure_DE_ram_ram_id_start =
	     configure_DE_ram_ram_id_start ;
  assign de_connect$lookup_dstIP = lookup_dstIP ;
  assign de_connect$lookup_mac = lookup_mac ;
  assign de_connect$lookup_srcIP = lookup_srcIP ;
  assign de_connect$write_ram_addr_wr = write_ram_addr_wr ;
  assign de_connect$write_ram_data_wr = write_ram_data_wr ;
  assign de_connect$write_ram_ram_id = write_ram_ram_id ;
  assign de_connect$EN_lookup = EN_lookup ;
  assign de_connect$EN_configure_DE_ram = EN_configure_DE_ram ;
  assign de_connect$EN_write_ram = EN_write_ram ;

  // submodule fifo_dstip
  assign fifo_dstip$D_IN = lookup_dstIP ;
  assign fifo_dstip$ENQ = EN_lookup ;
  assign fifo_dstip$DEQ = real_res_rg_1$whas ;
  assign fifo_dstip$CLR = 1'b0 ;

  // submodule fifo_srcip
  assign fifo_srcip$D_IN = lookup_srcIP ;
  assign fifo_srcip$ENQ = EN_lookup ;
  assign fifo_srcip$DEQ = real_res_rg_1$whas ;
  assign fifo_srcip$CLR = 1'b0 ;

  // remaining internal signals
  assign de_bit_res_tup_rg_BITS_31_TO_0__q2 = de_bit_res_tup_rg[31:0] ;
  assign de_bit_res_tup_rg_BITS_63_TO_32__q3 = de_bit_res_tup_rg[63:32] ;
  assign de_bit_res_tup_rg_BITS_95_TO_64__q1 = de_bit_res_tup_rg[95:64] ;
  assign x__h1469 =
	     de_bit_res_tup_rg_BITS_95_TO_64__q1[31] ?
	       { fifo_dstip$D_OUT[31], 31'd0 } :
	       ((de_bit_res_tup_rg_BITS_95_TO_64__q1[31:30] == 2'b01) ?
		  { fifo_dstip$D_OUT[31:30], 30'd0 } :
		  ((de_bit_res_tup_rg_BITS_95_TO_64__q1[31:29] == 3'b001) ?
		     { fifo_dstip$D_OUT[31:29], 29'd0 } :
		     ((de_bit_res_tup_rg_BITS_95_TO_64__q1[31:28] ==
		       4'b0001) ?
			{ fifo_dstip$D_OUT[31:28], 28'd0 } :
			((de_bit_res_tup_rg_BITS_95_TO_64__q1[31:27] ==
			  5'b00001) ?
			   { fifo_dstip$D_OUT[31:27], 27'd0 } :
			   ((de_bit_res_tup_rg_BITS_95_TO_64__q1[31:26] ==
			     6'b000001) ?
			      { fifo_dstip$D_OUT[31:26], 26'd0 } :
			      ((de_bit_res_tup_rg_BITS_95_TO_64__q1[31:25] ==
				7'b0000001) ?
				 { fifo_dstip$D_OUT[31:25], 25'd0 } :
				 ((de_bit_res_tup_rg_BITS_95_TO_64__q1[31:24] ==
				   8'b00000001) ?
				    { fifo_dstip$D_OUT[31:24], 24'd0 } :
				    ((de_bit_res_tup_rg_BITS_95_TO_64__q1[31:23] ==
				      9'b000000001) ?
				       { fifo_dstip$D_OUT[31:23], 23'd0 } :
				       ((de_bit_res_tup_rg_BITS_95_TO_64__q1[31:22] ==
					 10'b0000000001) ?
					  { fifo_dstip$D_OUT[31:22], 22'd0 } :
					  ((de_bit_res_tup_rg_BITS_95_TO_64__q1[31:21] ==
					    11'b00000000001) ?
					     { fifo_dstip$D_OUT[31:21],
					       21'd0 } :
					     ((de_bit_res_tup_rg_BITS_95_TO_64__q1[31:20] ==
					       12'b000000000001) ?
						{ fifo_dstip$D_OUT[31:20],
						  20'd0 } :
						((de_bit_res_tup_rg_BITS_95_TO_64__q1[31:19] ==
						  13'b0000000000001) ?
						   { fifo_dstip$D_OUT[31:19],
						     19'd0 } :
						   ((de_bit_res_tup_rg_BITS_95_TO_64__q1[31:18] ==
						     14'b00000000000001) ?
						      { fifo_dstip$D_OUT[31:18],
							18'd0 } :
						      ((de_bit_res_tup_rg_BITS_95_TO_64__q1[31:17] ==
							15'b000000000000001) ?
							 { fifo_dstip$D_OUT[31:17],
							   17'd0 } :
							 ((de_bit_res_tup_rg_BITS_95_TO_64__q1[31:16] ==
							   16'b0000000000000001) ?
							    { fifo_dstip$D_OUT[31:16],
							      16'd0 } :
							    ((de_bit_res_tup_rg_BITS_95_TO_64__q1[31:15] ==
							      17'b00000000000000001) ?
							       { fifo_dstip$D_OUT[31:15],
								 15'd0 } :
							       ((de_bit_res_tup_rg_BITS_95_TO_64__q1[31:14] ==
								 18'b000000000000000001) ?
								  { fifo_dstip$D_OUT[31:14],
								    14'd0 } :
								  ((de_bit_res_tup_rg_BITS_95_TO_64__q1[31:13] ==
								    19'b0000000000000000001) ?
								     { fifo_dstip$D_OUT[31:13],
								       13'd0 } :
								     ((de_bit_res_tup_rg_BITS_95_TO_64__q1[31:12] ==
								       20'b00000000000000000001) ?
									{ fifo_dstip$D_OUT[31:12],
									  12'd0 } :
									((de_bit_res_tup_rg_BITS_95_TO_64__q1[31:11] ==
									  21'b000000000000000000001) ?
									   { fifo_dstip$D_OUT[31:11],
									     11'd0 } :
									   ((de_bit_res_tup_rg_BITS_95_TO_64__q1[31:10] ==
									     22'b0000000000000000000001) ?
									      { fifo_dstip$D_OUT[31:10],
										10'd0 } :
									      ((de_bit_res_tup_rg_BITS_95_TO_64__q1[31:9] ==
										23'b00000000000000000000001) ?
										 { fifo_dstip$D_OUT[31:9],
										   9'd0 } :
										 ((de_bit_res_tup_rg_BITS_95_TO_64__q1[31:8] ==
										   24'b000000000000000000000001) ?
										    { fifo_dstip$D_OUT[31:8],
										      8'd0 } :
										    ((de_bit_res_tup_rg_BITS_95_TO_64__q1[31:7] ==
										      25'b0000000000000000000000001) ?
										       { fifo_dstip$D_OUT[31:7],
											 7'd0 } :
										       ((de_bit_res_tup_rg_BITS_95_TO_64__q1[31:6] ==
											 26'b00000000000000000000000001) ?
											  { fifo_dstip$D_OUT[31:6],
											    6'd0 } :
											  ((de_bit_res_tup_rg_BITS_95_TO_64__q1[31:5] ==
											    27'b000000000000000000000000001) ?
											     { fifo_dstip$D_OUT[31:5],
											       5'd0 } :
											     ((de_bit_res_tup_rg_BITS_95_TO_64__q1[31:4] ==
											       28'b0000000000000000000000000001) ?
												{ fifo_dstip$D_OUT[31:4],
												  4'd0 } :
												((de_bit_res_tup_rg_BITS_95_TO_64__q1[31:3] ==
												  29'b00000000000000000000000000001) ?
												   { fifo_dstip$D_OUT[31:3],
												     3'd0 } :
												   ((de_bit_res_tup_rg_BITS_95_TO_64__q1[31:2] ==
												     30'b000000000000000000000000000001) ?
												      { fifo_dstip$D_OUT[31:2],
													2'd0 } :
												      ((de_bit_res_tup_rg_BITS_95_TO_64__q1[31:1] ==
													31'b0000000000000000000000000000001) ?
													 { fifo_dstip$D_OUT[31:1],
													   1'd0 } :
													 fifo_dstip$D_OUT)))))))))))))))))))))))))))))) ;
  assign x__h3034 =
	     de_bit_res_tup_rg_BITS_63_TO_32__q3[31] ?
	       { fifo_srcip$D_OUT[31], 31'd0 } :
	       ((de_bit_res_tup_rg_BITS_63_TO_32__q3[31:30] == 2'b01) ?
		  { fifo_srcip$D_OUT[31:30], 30'd0 } :
		  ((de_bit_res_tup_rg_BITS_63_TO_32__q3[31:29] == 3'b001) ?
		     { fifo_srcip$D_OUT[31:29], 29'd0 } :
		     ((de_bit_res_tup_rg_BITS_63_TO_32__q3[31:28] ==
		       4'b0001) ?
			{ fifo_srcip$D_OUT[31:28], 28'd0 } :
			((de_bit_res_tup_rg_BITS_63_TO_32__q3[31:27] ==
			  5'b00001) ?
			   { fifo_srcip$D_OUT[31:27], 27'd0 } :
			   ((de_bit_res_tup_rg_BITS_63_TO_32__q3[31:26] ==
			     6'b000001) ?
			      { fifo_srcip$D_OUT[31:26], 26'd0 } :
			      ((de_bit_res_tup_rg_BITS_63_TO_32__q3[31:25] ==
				7'b0000001) ?
				 { fifo_srcip$D_OUT[31:25], 25'd0 } :
				 ((de_bit_res_tup_rg_BITS_63_TO_32__q3[31:24] ==
				   8'b00000001) ?
				    { fifo_srcip$D_OUT[31:24], 24'd0 } :
				    ((de_bit_res_tup_rg_BITS_63_TO_32__q3[31:23] ==
				      9'b000000001) ?
				       { fifo_srcip$D_OUT[31:23], 23'd0 } :
				       ((de_bit_res_tup_rg_BITS_63_TO_32__q3[31:22] ==
					 10'b0000000001) ?
					  { fifo_srcip$D_OUT[31:22], 22'd0 } :
					  ((de_bit_res_tup_rg_BITS_63_TO_32__q3[31:21] ==
					    11'b00000000001) ?
					     { fifo_srcip$D_OUT[31:21],
					       21'd0 } :
					     ((de_bit_res_tup_rg_BITS_63_TO_32__q3[31:20] ==
					       12'b000000000001) ?
						{ fifo_srcip$D_OUT[31:20],
						  20'd0 } :
						((de_bit_res_tup_rg_BITS_63_TO_32__q3[31:19] ==
						  13'b0000000000001) ?
						   { fifo_srcip$D_OUT[31:19],
						     19'd0 } :
						   ((de_bit_res_tup_rg_BITS_63_TO_32__q3[31:18] ==
						     14'b00000000000001) ?
						      { fifo_srcip$D_OUT[31:18],
							18'd0 } :
						      ((de_bit_res_tup_rg_BITS_63_TO_32__q3[31:17] ==
							15'b000000000000001) ?
							 { fifo_srcip$D_OUT[31:17],
							   17'd0 } :
							 ((de_bit_res_tup_rg_BITS_63_TO_32__q3[31:16] ==
							   16'b0000000000000001) ?
							    { fifo_srcip$D_OUT[31:16],
							      16'd0 } :
							    ((de_bit_res_tup_rg_BITS_63_TO_32__q3[31:15] ==
							      17'b00000000000000001) ?
							       { fifo_srcip$D_OUT[31:15],
								 15'd0 } :
							       ((de_bit_res_tup_rg_BITS_63_TO_32__q3[31:14] ==
								 18'b000000000000000001) ?
								  { fifo_srcip$D_OUT[31:14],
								    14'd0 } :
								  ((de_bit_res_tup_rg_BITS_63_TO_32__q3[31:13] ==
								    19'b0000000000000000001) ?
								     { fifo_srcip$D_OUT[31:13],
								       13'd0 } :
								     ((de_bit_res_tup_rg_BITS_63_TO_32__q3[31:12] ==
								       20'b00000000000000000001) ?
									{ fifo_srcip$D_OUT[31:12],
									  12'd0 } :
									((de_bit_res_tup_rg_BITS_63_TO_32__q3[31:11] ==
									  21'b000000000000000000001) ?
									   { fifo_srcip$D_OUT[31:11],
									     11'd0 } :
									   ((de_bit_res_tup_rg_BITS_63_TO_32__q3[31:10] ==
									     22'b0000000000000000000001) ?
									      { fifo_srcip$D_OUT[31:10],
										10'd0 } :
									      ((de_bit_res_tup_rg_BITS_63_TO_32__q3[31:9] ==
										23'b00000000000000000000001) ?
										 { fifo_srcip$D_OUT[31:9],
										   9'd0 } :
										 ((de_bit_res_tup_rg_BITS_63_TO_32__q3[31:8] ==
										   24'b000000000000000000000001) ?
										    { fifo_srcip$D_OUT[31:8],
										      8'd0 } :
										    ((de_bit_res_tup_rg_BITS_63_TO_32__q3[31:7] ==
										      25'b0000000000000000000000001) ?
										       { fifo_srcip$D_OUT[31:7],
											 7'd0 } :
										       ((de_bit_res_tup_rg_BITS_63_TO_32__q3[31:6] ==
											 26'b00000000000000000000000001) ?
											  { fifo_srcip$D_OUT[31:6],
											    6'd0 } :
											  ((de_bit_res_tup_rg_BITS_63_TO_32__q3[31:5] ==
											    27'b000000000000000000000000001) ?
											     { fifo_srcip$D_OUT[31:5],
											       5'd0 } :
											     ((de_bit_res_tup_rg_BITS_63_TO_32__q3[31:4] ==
											       28'b0000000000000000000000000001) ?
												{ fifo_srcip$D_OUT[31:4],
												  4'd0 } :
												((de_bit_res_tup_rg_BITS_63_TO_32__q3[31:3] ==
												  29'b00000000000000000000000000001) ?
												   { fifo_srcip$D_OUT[31:3],
												     3'd0 } :
												   ((de_bit_res_tup_rg_BITS_63_TO_32__q3[31:2] ==
												     30'b000000000000000000000000000001) ?
												      { fifo_srcip$D_OUT[31:2],
													2'd0 } :
												      ((de_bit_res_tup_rg_BITS_63_TO_32__q3[31:1] ==
													31'b0000000000000000000000000000001) ?
													 { fifo_srcip$D_OUT[31:1],
													   1'd0 } :
													 fifo_srcip$D_OUT)))))))))))))))))))))))))))))) ;
  assign x__h4235 =
	     de_bit_res_tup_rg_BITS_31_TO_0__q2[31] ?
	       { fifo_dstip$D_OUT[31], 31'd0 } :
	       ((de_bit_res_tup_rg_BITS_31_TO_0__q2[31:30] == 2'b01) ?
		  { fifo_dstip$D_OUT[31:30], 30'd0 } :
		  ((de_bit_res_tup_rg_BITS_31_TO_0__q2[31:29] == 3'b001) ?
		     { fifo_dstip$D_OUT[31:29], 29'd0 } :
		     ((de_bit_res_tup_rg_BITS_31_TO_0__q2[31:28] == 4'b0001) ?
			{ fifo_dstip$D_OUT[31:28], 28'd0 } :
			((de_bit_res_tup_rg_BITS_31_TO_0__q2[31:27] ==
			  5'b00001) ?
			   { fifo_dstip$D_OUT[31:27], 27'd0 } :
			   ((de_bit_res_tup_rg_BITS_31_TO_0__q2[31:26] ==
			     6'b000001) ?
			      { fifo_dstip$D_OUT[31:26], 26'd0 } :
			      ((de_bit_res_tup_rg_BITS_31_TO_0__q2[31:25] ==
				7'b0000001) ?
				 { fifo_dstip$D_OUT[31:25], 25'd0 } :
				 ((de_bit_res_tup_rg_BITS_31_TO_0__q2[31:24] ==
				   8'b00000001) ?
				    { fifo_dstip$D_OUT[31:24], 24'd0 } :
				    ((de_bit_res_tup_rg_BITS_31_TO_0__q2[31:23] ==
				      9'b000000001) ?
				       { fifo_dstip$D_OUT[31:23], 23'd0 } :
				       ((de_bit_res_tup_rg_BITS_31_TO_0__q2[31:22] ==
					 10'b0000000001) ?
					  { fifo_dstip$D_OUT[31:22], 22'd0 } :
					  ((de_bit_res_tup_rg_BITS_31_TO_0__q2[31:21] ==
					    11'b00000000001) ?
					     { fifo_dstip$D_OUT[31:21],
					       21'd0 } :
					     ((de_bit_res_tup_rg_BITS_31_TO_0__q2[31:20] ==
					       12'b000000000001) ?
						{ fifo_dstip$D_OUT[31:20],
						  20'd0 } :
						((de_bit_res_tup_rg_BITS_31_TO_0__q2[31:19] ==
						  13'b0000000000001) ?
						   { fifo_dstip$D_OUT[31:19],
						     19'd0 } :
						   ((de_bit_res_tup_rg_BITS_31_TO_0__q2[31:18] ==
						     14'b00000000000001) ?
						      { fifo_dstip$D_OUT[31:18],
							18'd0 } :
						      ((de_bit_res_tup_rg_BITS_31_TO_0__q2[31:17] ==
							15'b000000000000001) ?
							 { fifo_dstip$D_OUT[31:17],
							   17'd0 } :
							 ((de_bit_res_tup_rg_BITS_31_TO_0__q2[31:16] ==
							   16'b0000000000000001) ?
							    { fifo_dstip$D_OUT[31:16],
							      16'd0 } :
							    ((de_bit_res_tup_rg_BITS_31_TO_0__q2[31:15] ==
							      17'b00000000000000001) ?
							       { fifo_dstip$D_OUT[31:15],
								 15'd0 } :
							       ((de_bit_res_tup_rg_BITS_31_TO_0__q2[31:14] ==
								 18'b000000000000000001) ?
								  { fifo_dstip$D_OUT[31:14],
								    14'd0 } :
								  ((de_bit_res_tup_rg_BITS_31_TO_0__q2[31:13] ==
								    19'b0000000000000000001) ?
								     { fifo_dstip$D_OUT[31:13],
								       13'd0 } :
								     ((de_bit_res_tup_rg_BITS_31_TO_0__q2[31:12] ==
								       20'b00000000000000000001) ?
									{ fifo_dstip$D_OUT[31:12],
									  12'd0 } :
									((de_bit_res_tup_rg_BITS_31_TO_0__q2[31:11] ==
									  21'b000000000000000000001) ?
									   { fifo_dstip$D_OUT[31:11],
									     11'd0 } :
									   ((de_bit_res_tup_rg_BITS_31_TO_0__q2[31:10] ==
									     22'b0000000000000000000001) ?
									      { fifo_dstip$D_OUT[31:10],
										10'd0 } :
									      ((de_bit_res_tup_rg_BITS_31_TO_0__q2[31:9] ==
										23'b00000000000000000000001) ?
										 { fifo_dstip$D_OUT[31:9],
										   9'd0 } :
										 ((de_bit_res_tup_rg_BITS_31_TO_0__q2[31:8] ==
										   24'b000000000000000000000001) ?
										    { fifo_dstip$D_OUT[31:8],
										      8'd0 } :
										    ((de_bit_res_tup_rg_BITS_31_TO_0__q2[31:7] ==
										      25'b0000000000000000000000001) ?
										       { fifo_dstip$D_OUT[31:7],
											 7'd0 } :
										       ((de_bit_res_tup_rg_BITS_31_TO_0__q2[31:6] ==
											 26'b00000000000000000000000001) ?
											  { fifo_dstip$D_OUT[31:6],
											    6'd0 } :
											  ((de_bit_res_tup_rg_BITS_31_TO_0__q2[31:5] ==
											    27'b000000000000000000000000001) ?
											     { fifo_dstip$D_OUT[31:5],
											       5'd0 } :
											     ((de_bit_res_tup_rg_BITS_31_TO_0__q2[31:4] ==
											       28'b0000000000000000000000000001) ?
												{ fifo_dstip$D_OUT[31:4],
												  4'd0 } :
												((de_bit_res_tup_rg_BITS_31_TO_0__q2[31:3] ==
												  29'b00000000000000000000000000001) ?
												   { fifo_dstip$D_OUT[31:3],
												     3'd0 } :
												   ((de_bit_res_tup_rg_BITS_31_TO_0__q2[31:2] ==
												     30'b000000000000000000000000000001) ?
												      { fifo_dstip$D_OUT[31:2],
													2'd0 } :
												      ((de_bit_res_tup_rg_BITS_31_TO_0__q2[31:1] ==
													31'b0000000000000000000000000000001) ?
													 { fifo_dstip$D_OUT[31:1],
													   1'd0 } :
													 fifo_dstip$D_OUT)))))))))))))))))))))))))))))) ;

  // handling of inlined registers

  always@(posedge CLK)
  begin
    if (RST_N == `BSV_RESET_VALUE)
      begin
        de_bit_res_tup_rg <= `BSV_ASSIGNMENT_DELAY
	    102'h0AAAAAAAAAAAAAAAAAAAAAAAAA;
	real_res_rg <= `BSV_ASSIGNMENT_DELAY 102'h0AAAAAAAAAAAAAAAAAAAAAAAAA;
      end
    else
      begin
        if (de_bit_res_tup_rg$EN)
	  de_bit_res_tup_rg <= `BSV_ASSIGNMENT_DELAY de_bit_res_tup_rg$D_IN;
	if (real_res_rg$EN)
	  real_res_rg <= `BSV_ASSIGNMENT_DELAY real_res_rg$D_IN;
      end
  end

  // synopsys translate_off
  `ifdef BSV_NO_INITIAL_BLOCKS
  `else // not BSV_NO_INITIAL_BLOCKS
  initial
  begin
    de_bit_res_tup_rg = 102'h2AAAAAAAAAAAAAAAAAAAAAAAAA;
    real_res_rg = 102'h2AAAAAAAAAAAAAAAAAAAAAAAAA;
  end
  `endif // BSV_NO_INITIAL_BLOCKS
  // synopsys translate_on
endmodule  // mkDE_TOP
