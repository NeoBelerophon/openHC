#ifndef _CRC_H


unsigned short crc16(unsigned char* buf, int n); // CRC across a buffer
int assemble_packet_phc(unsigned char* buf, int* n); // add checksum + send, PHC bus
int assemble_packet_rs232(unsigned char* buf, int* n); // add checksum + send, rs232 hookup

#endif // #ifndef _CRC_H
