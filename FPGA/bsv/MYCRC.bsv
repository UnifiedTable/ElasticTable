//simplified version: for quickly compile/sim with bsv
package MYCRC;

// CRC polynomial coefficients: x^64 + x^4 + x^3 + x + 1
//                              0xD800000000000000 (hex)
// CRC width:                   64 bits
// CRC shift direction:         right (little endian)
// Input word width:            48 bits
(* noinline *)
function Bit#(64) calc_crc(
    Bit#(64) crcIn,
    Bit#(48) data
);
    Bit#(64) crc;
    crc[0] = crcIn[48];
    crc[1] = crcIn[49];
    crc[2] = crcIn[50];
    crc[3] = crcIn[51];
    crc[4] = crcIn[52];
    crc[5] = crcIn[53];
    crc[6] = crcIn[54];
    crc[7] = crcIn[55];
    crc[8] = crcIn[56];
    crc[9] = crcIn[57];
    crc[10] = crcIn[58];
    crc[11] = crcIn[59];
    crc[12] = crcIn[0] ^ crcIn[60] ^ data[0];
    crc[13] = crcIn[0] ^ crcIn[1] ^ crcIn[61] ^ data[0] ^ data[1];
    crc[14] = crcIn[1] ^ crcIn[2] ^ crcIn[62] ^ data[1] ^ data[2];
    crc[15] = crcIn[0] ^ crcIn[2] ^ crcIn[3] ^ crcIn[63] ^ data[0] ^ data[2] ^ data[3];
    crc[16] = crcIn[0] ^ crcIn[1] ^ crcIn[3] ^ crcIn[4] ^ data[0] ^ data[1] ^ data[3] ^ data[4];
    crc[17] = crcIn[1] ^ crcIn[2] ^ crcIn[4] ^ crcIn[5] ^ data[1] ^ data[2] ^ data[4] ^ data[5];
    crc[18] = crcIn[2] ^ crcIn[3] ^ crcIn[5] ^ crcIn[6] ^ data[2] ^ data[3] ^ data[5] ^ data[6];
    crc[19] = crcIn[3] ^ crcIn[4] ^ crcIn[6] ^ crcIn[7] ^ data[3] ^ data[4] ^ data[6] ^ data[7];
    crc[20] = crcIn[4] ^ crcIn[5] ^ crcIn[7] ^ crcIn[8] ^ data[4] ^ data[5] ^ data[7] ^ data[8];
    crc[21] = crcIn[5] ^ crcIn[6] ^ crcIn[8] ^ crcIn[9] ^ data[5] ^ data[6] ^ data[8] ^ data[9];
    crc[22] = crcIn[6] ^ crcIn[7] ^ crcIn[9] ^ crcIn[10] ^ data[6] ^ data[7] ^ data[9] ^ data[10];
    crc[23] = crcIn[7] ^ crcIn[8] ^ crcIn[10] ^ crcIn[11] ^ data[7] ^ data[8] ^ data[10] ^ data[11];
    crc[24] = crcIn[8] ^ crcIn[9] ^ crcIn[11] ^ crcIn[12] ^ data[8] ^ data[9] ^ data[11] ^ data[12];
    crc[25] = crcIn[9] ^ crcIn[10] ^ crcIn[12] ^ crcIn[13] ^ data[9] ^ data[10] ^ data[12] ^ data[13];
    crc[26] = crcIn[10] ^ crcIn[11] ^ crcIn[13] ^ crcIn[14] ^ data[10] ^ data[11] ^ data[13] ^ data[14];
    crc[27] = crcIn[11] ^ crcIn[12] ^ crcIn[14] ^ crcIn[15] ^ data[11] ^ data[12] ^ data[14] ^ data[15];
    crc[28] = crcIn[12] ^ crcIn[13] ^ crcIn[15] ^ crcIn[16] ^ data[12] ^ data[13] ^ data[15] ^ data[16];
    crc[29] = crcIn[13] ^ crcIn[14] ^ crcIn[16] ^ crcIn[17] ^ data[13] ^ data[14] ^ data[16] ^ data[17];
    crc[30] = crcIn[14] ^ crcIn[15] ^ crcIn[17] ^ crcIn[18] ^ data[14] ^ data[15] ^ data[17] ^ data[18];
    crc[31] = crcIn[15] ^ crcIn[16] ^ crcIn[18] ^ crcIn[19] ^ data[15] ^ data[16] ^ data[18] ^ data[19];
    crc[32] = crcIn[16] ^ crcIn[17] ^ crcIn[19] ^ crcIn[20] ^ data[16] ^ data[17] ^ data[19] ^ data[20];
    crc[33] = crcIn[17] ^ crcIn[18] ^ crcIn[20] ^ crcIn[21] ^ data[17] ^ data[18] ^ data[20] ^ data[21];
    crc[34] = crcIn[18] ^ crcIn[19] ^ crcIn[21] ^ crcIn[22] ^ data[18] ^ data[19] ^ data[21] ^ data[22];
    crc[35] = crcIn[19] ^ crcIn[20] ^ crcIn[22] ^ crcIn[23] ^ data[19] ^ data[20] ^ data[22] ^ data[23];
    crc[36] = crcIn[20] ^ crcIn[21] ^ crcIn[23] ^ crcIn[24] ^ data[20] ^ data[21] ^ data[23] ^ data[24];
    crc[37] = crcIn[21] ^ crcIn[22] ^ crcIn[24] ^ crcIn[25] ^ data[21] ^ data[22] ^ data[24] ^ data[25];
    crc[38] = crcIn[22] ^ crcIn[23] ^ crcIn[25] ^ crcIn[26] ^ data[22] ^ data[23] ^ data[25] ^ data[26];
    crc[39] = crcIn[23] ^ crcIn[24] ^ crcIn[26] ^ crcIn[27] ^ data[23] ^ data[24] ^ data[26] ^ data[27];
    crc[40] = crcIn[24] ^ crcIn[25] ^ crcIn[27] ^ crcIn[28] ^ data[24] ^ data[25] ^ data[27] ^ data[28];
    crc[41] = crcIn[25] ^ crcIn[26] ^ crcIn[28] ^ crcIn[29] ^ data[25] ^ data[26] ^ data[28] ^ data[29];
    crc[42] = crcIn[26] ^ crcIn[27] ^ crcIn[29] ^ crcIn[30] ^ data[26] ^ data[27] ^ data[29] ^ data[30];
    crc[43] = crcIn[27] ^ crcIn[28] ^ crcIn[30] ^ crcIn[31] ^ data[27] ^ data[28] ^ data[30] ^ data[31];
    crc[44] = crcIn[28] ^ crcIn[29] ^ crcIn[31] ^ crcIn[32] ^ data[28] ^ data[29] ^ data[31] ^ data[32];
    crc[45] = crcIn[29] ^ crcIn[30] ^ crcIn[32] ^ crcIn[33] ^ data[29] ^ data[30] ^ data[32] ^ data[33];
    crc[46] = crcIn[30] ^ crcIn[31] ^ crcIn[33] ^ crcIn[34] ^ data[30] ^ data[31] ^ data[33] ^ data[34];
    crc[47] = crcIn[31] ^ crcIn[32] ^ crcIn[34] ^ crcIn[35] ^ data[31] ^ data[32] ^ data[34] ^ data[35];
    crc[48] = crcIn[32] ^ crcIn[33] ^ crcIn[35] ^ crcIn[36] ^ data[32] ^ data[33] ^ data[35] ^ data[36];
    crc[49] = crcIn[33] ^ crcIn[34] ^ crcIn[36] ^ crcIn[37] ^ data[33] ^ data[34] ^ data[36] ^ data[37];
    crc[50] = crcIn[34] ^ crcIn[35] ^ crcIn[37] ^ crcIn[38] ^ data[34] ^ data[35] ^ data[37] ^ data[38];
    crc[51] = crcIn[35] ^ crcIn[36] ^ crcIn[38] ^ crcIn[39] ^ data[35] ^ data[36] ^ data[38] ^ data[39];
    crc[52] = crcIn[36] ^ crcIn[37] ^ crcIn[39] ^ crcIn[40] ^ data[36] ^ data[37] ^ data[39] ^ data[40];
    crc[53] = crcIn[37] ^ crcIn[38] ^ crcIn[40] ^ crcIn[41] ^ data[37] ^ data[38] ^ data[40] ^ data[41];
    crc[54] = crcIn[38] ^ crcIn[39] ^ crcIn[41] ^ crcIn[42] ^ data[38] ^ data[39] ^ data[41] ^ data[42];
    crc[55] = crcIn[39] ^ crcIn[40] ^ crcIn[42] ^ crcIn[43] ^ data[39] ^ data[40] ^ data[42] ^ data[43];
    crc[56] = crcIn[40] ^ crcIn[41] ^ crcIn[43] ^ crcIn[44] ^ data[40] ^ data[41] ^ data[43] ^ data[44];
    crc[57] = crcIn[41] ^ crcIn[42] ^ crcIn[44] ^ crcIn[45] ^ data[41] ^ data[42] ^ data[44] ^ data[45];
    crc[58] = crcIn[42] ^ crcIn[43] ^ crcIn[45] ^ crcIn[46] ^ data[42] ^ data[43] ^ data[45] ^ data[46];
    crc[59] = crcIn[43] ^ crcIn[44] ^ crcIn[46] ^ crcIn[47] ^ data[43] ^ data[44] ^ data[46] ^ data[47];
    crc[60] = crcIn[44] ^ crcIn[45] ^ crcIn[47] ^ data[44] ^ data[45] ^ data[47];
    crc[61] = crcIn[45] ^ crcIn[46] ^ data[45] ^ data[46];
    crc[62] = crcIn[46] ^ crcIn[47] ^ data[46] ^ data[47];
    crc[63] = crcIn[47] ^ data[47];

    return crc;
endfunction

endpackage : MYCRC