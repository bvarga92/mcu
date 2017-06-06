#ifndef _MCP2515_H_
#define _MCP2515_H_

	/* idozites 20 MHz-es oszcillatorhoz */
	#define R_CNF1_125kbps	(0x03)
	#define R_CNF2_125kbps	(0xB6)
	#define R_CNF3_125kbps	(0x04)
	#define R_CNF1_250kbps	(0x01)
	#define R_CNF2_250kbps	(0xB6)
	#define R_CNF3_250kbps	(0x04)
	#define R_CNF1_500kbps	(0x00)
	#define R_CNF2_500kbps	(0xB6)
	#define R_CNF3_500kbps	(0x04)

	/* idozites 8 MHz-es oszcillatorhoz *//*
	#define R_CNF1_125kbps	(0x03)
	#define R_CNF2_125kbps	(0x90)
	#define R_CNF3_125kbps	(0x02)
	#define R_CNF1_250kbps	(0x01)
	#define R_CNF2_250kbps	(0x90)
	#define R_CNF3_250kbps	(0x02)
	#define R_CNF1_500kbps	(0x00)
	#define R_CNF2_500kbps	(0x90)
	#define R_CNF3_500kbps	(0x02)
	*/

	/* CAN uzenet struktura */
	typedef struct{
		uint16_t id;
		uint8_t  rtr;
		uint8_t  length;
		uint8_t  data[8];
	} CanMsg_t;

	/* CAN vezerlo inicializalasa */
	void mcp2515Init(void);

	/* CAN uzenet vetele (blokkol, amig nem jon ervenyes uzenet!) */
	void mcp2515Recv(CanMsg_t *msgPtr);

	/* standard CAN uzenet elkuldese a buszra */
	void mcp2515SendStd(CanMsg_t *msgPtr);

	/* veteli szurok maszkjainak beallitasa (ahol 1 van, ott vizsgal a szuro) */
	void mcp2515SetStdMaskRx0(uint16_t mask);
	void mcp2515SetStdMaskRx1(uint16_t mask);

	/* veteli szurok beallitasa */
	void mcp2515SetStdFilterRxF0(uint16_t filter);
	void mcp2515SetStdFilterRxF1(uint16_t filter);
	void mcp2515SetStdFilterRxF2(uint16_t filter);
	void mcp2515SetStdFilterRxF3(uint16_t filter);
	void mcp2515SetStdFilterRxF4(uint16_t filter);
	void mcp2515SetStdFilterRxF5(uint16_t filter);

#endif
