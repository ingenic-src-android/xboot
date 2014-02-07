#include <config.h>
#include <common.h>
#include <serial.h>

#if defined(CONFIG_JZ4780)
#include <jz4780.h>
#endif

#undef DEBUG
//#define DEBUG

#ifdef DEBUG
#define dprintf(fmt,args...)	printf(fmt, ##args)
#else
#define dprintf
#endif

#define MASK(byte) (0xff << (byte * 8))
#define EXPECT0 0x11223344	
#define EXPECT1 0x55667788	
#define EXPECT2 0xaabbccdd	
#define EXPECT3 0xeeeeffff	

static struct command {
	char tag;
	char dtp;
	char rpt;
	char cmd;
	char rank;
	char bank;
	int  addr;
	char mask;
	int data1; 
	int data2; 
};

static void write_dcu_cache(unsigned int *data, unsigned int len)
{
	int i;

	for (i = len; i > 0; i--) {
		dprintf("send: 0x%08x\n", data[i - 1]);
		REG_DDRP_DCUDR = data[i - 1];
	}
	dprintf("\n");
}

static void to_cmd_format(struct command cmd, unsigned int *cmd_word)
{
	cmd_word[0] = (cmd.rpt & 0x1f) << 7 | (cmd.dtp & 0x1f) << 2 | (cmd.tag & 0x3);
	cmd_word[1] = (cmd.cmd & 0xf) << 28 | (cmd.rank & 0x1) << 27 | (cmd.bank & 0x7) << 24 | 
			(cmd.addr & 0xffff) << 8 | (cmd.mask & 0xff);
	cmd_word[2] = cmd.data1;
	cmd_word[3] = cmd.data2;
}

#if 0
void ddr3_reset_high(void)
{
	struct command cmd = {0};
	unsigned int word[4] = {0};

	dprintf("ddr3_reset_high()\n");
	
	REG_DDRP_DCUAR = 0x400;

	cmd.rpt = cmd.bank = cmd.mask = cmd.data1 = cmd.data2 = 0;
	cmd.dtp = 0x0;	
	cmd.cmd = 0x7; 	//SPEC_CMD;
	cmd.addr = 0x1;	// driver ddr3 reset high
	cmd.rank = 0;
	cmd.tag = 1;
	to_cmd_format(cmd, word);
	dprintf("SPECIAL CMD DRIVER DDR3 RESET HIGH:\n");
	write_dcu_cache(word, 4);

	cmd.dtp = cmd.cmd = cmd.addr = cmd.rank = cmd.tag = cmd.rpt = cmd.bank = cmd.mask = cmd.data1 = cmd.data2 = 0;
	to_cmd_format(cmd, word);
	dprintf("NOP:\n");
	write_dcu_cache(word, 4);

	cmd.rpt = cmd.bank = cmd.mask = cmd.data1 = cmd.data2 = 0;
	cmd.dtp = 0xf;	
	cmd.cmd = 0x7; 	//SPEC_CMD;
	cmd.addr = 0x0;	// driver ddr3 reset low
	cmd.rank = 0;
	cmd.tag = 1;
	to_cmd_format(cmd, word);
	dprintf("SPECIAL CMD DRIVER DDR3 RESET LOW:\n");
	write_dcu_cache(word, 4);

	cmd.rpt = cmd.bank = cmd.mask = cmd.data1 = cmd.data2 = 0;
	cmd.dtp = 0x0;	
	cmd.cmd = 0x7; 	//SPEC_CMD;
	cmd.addr = 0x8000;	// driver ddr3 reset high
	cmd.rank = 0;
	cmd.tag = 1;
	to_cmd_format(cmd, word);
	dprintf("SPECIAL CMD DRIVER DDR3 RESET HIGH:\n");
	write_dcu_cache(word, 4);

	REG_DDRP_DCURR = 0x301;

	while (!(REG_DDRP_DCUSR0 & 0x1))
		;

	dprintf("driver ddr reset high over!\n");
	dprintf("====================================================================\n");
	dprintf("DCUAR: 0x%08x\n", REG_DDRP_DCUAR);
	dprintf("DCURR: 0x%08x\n", REG_DDRP_DCURR);
	dprintf("DCUSR0: 0x%08x\n", REG_DDRP_DCUSR0);
	dprintf("DCUSR1: 0x%08x\n", REG_DDRP_DCUSR1);
	dprintf("====================================================================\n");
}
#endif

void ddr3_init(void)
{
	struct command cmd = {0};
	unsigned int word[4] = {0};

	dprintf("ddr3_init()\n");

	REG_DDRP_DCUAR = 0x400;

	cmd.rpt = cmd.mask = cmd.data1 = cmd.data2 = 0;
	cmd.tag = 1;
	cmd.rank = 0;

	cmd.bank = 0;
	cmd.addr = 0; // RESET_LO 
	cmd.dtp = 15;	//tDINITRST;
	cmd.cmd = 0x7; 	//SPECIAL_CMD;
	to_cmd_format(cmd, word);
	dprintf("RESET LO:\n");
	write_dcu_cache(word, 4);

	cmd.bank = 0;
	cmd.addr = 1; // RESET_HI 
	cmd.dtp = 0;	//0
	cmd.cmd = 0x7; 	//SPECIAL_CMD;
	to_cmd_format(cmd, word);
	dprintf("RESET HI:\n");
	write_dcu_cache(word, 4);

	cmd.bank = 0;
	cmd.addr = 2; // CKE_LO 
	cmd.dtp = 16;	// tDINITCKE LO
	cmd.cmd = 0x7; 	//SPECIAL_CMD;
	to_cmd_format(cmd, word);
	dprintf("CKE LO:\n");
	write_dcu_cache(word, 4);

	cmd.bank = 0;
	cmd.addr = 3; // CKE_HI 
	cmd.dtp = 17;	// tDINITCKE HI
	cmd.cmd = 0x7; 	//SPECIAL_CMD;
	to_cmd_format(cmd, word);
	dprintf("CKE HI:\n");
	write_dcu_cache(word, 4);

	cmd.bank = 2;
	cmd.addr = 0x8; // data, CWL=6 dll-off
		//cmd.addr = 0x408; // data, CWL=6, dll-off request
	cmd.dtp = 5;	// tMRD
	cmd.cmd = 0x1; 	// LOAD_MODE emr2
	to_cmd_format(cmd, word);
	dprintf("LOAD MODE MR2:\n");
	write_dcu_cache(word, 4);

	cmd.bank = 3;
	cmd.addr = 0x0; // data 
	cmd.dtp = 5;	// tMRD
	cmd.cmd = 0x1; 	// LOAD_MODE emr3
	to_cmd_format(cmd, word);
	dprintf("LOAD MODE MR3:\n");
	write_dcu_cache(word, 4);

	cmd.bank = 1;
	//cmd.addr = 0x6; // data 
		cmd.addr = 0x3; // dll-off mode 
	cmd.dtp = 5;	// tMRD
	cmd.cmd = 0x1; 	// LOAD_MODE emr1
	to_cmd_format(cmd, word);
	dprintf("LOAD MODE MR1:\n");
	write_dcu_cache(word, 4);

	cmd.bank = 0;
	//cmd.addr = 0x930; // data 
		cmd.addr = 0x220; // dll-off request, CL=6 
	cmd.dtp = 6;	// tMOD
	cmd.cmd = 0x1; 	// LOAD_MODE mr0
	to_cmd_format(cmd, word);
	dprintf("LOAD MODE MR0:\n");
	write_dcu_cache(word, 4);

	cmd.bank = 0;
	cmd.addr = 0;
	cmd.dtp = 18;	// tDINITZQ
	cmd.cmd = 0xd; 	// ZQCAL_LONG
	to_cmd_format(cmd, word);
	dprintf("ZQCAL LONG:\n");
	write_dcu_cache(word, 4);

	cmd.bank = 0;
	cmd.addr = 0; 
	cmd.dtp = 0;
	cmd.cmd = 0xf; 	// SDRAM_NOP
	to_cmd_format(cmd, word);
	dprintf("SDRAM NOP:\n");
	write_dcu_cache(word, 4);

	REG_DDRP_DCURR = 0x901;
	//REG_DDRP_DCURR = 0x301;

	while (!(REG_DDRP_DCUSR0 & 0x1))
		;

	serial_puts("DDR3 software init finish!\n");
	dprintf("====================================================================\n");
	dprintf("DCUAR: 0x%08x\n", REG_DDRP_DCUAR);
	dprintf("DCURR: 0x%08x\n", REG_DDRP_DCURR);
	dprintf("DCUSR0: 0x%08x\n", REG_DDRP_DCUSR0);
	dprintf("DCUSR1: 0x%08x\n", REG_DDRP_DCUSR1);
	dprintf("====================================================================\n");
}

static int ddr_test_write(int rank)
{
	struct command cmd = {0};
	unsigned int word[4] = {0};

	dprintf("ddr_test_write()\n");

	REG_DDRP_DCUTPR = 0x3;
	REG_DDRP_DCUAR = 0x400;

	cmd.rank = rank;

	cmd.rpt = cmd.bank = cmd.mask = cmd.data1 = cmd.data2 = cmd.addr = 0;
	cmd.dtp = 0x13;	//tRPA;
	cmd.cmd = 0x5; 	//RP_ALL;
	cmd.rank = rank;
	cmd.tag = 1;
	to_cmd_format(cmd, word);
	dprintf("PRECHARGE ALL:\n");
	write_dcu_cache(word, 4);

	cmd.rpt = cmd.tag = cmd.bank = cmd.mask = cmd.data1 = cmd.data2 = cmd.addr = 0;
	cmd.dtp = 0x15;	//TACT2RW;
	cmd.cmd = 0x6; 	//ACT;
	to_cmd_format(cmd, word);
	dprintf("ACT:\n");
	write_dcu_cache(word, 4);

	cmd.rpt = 0x4;	//tBL;	
	cmd.dtp = 0x19;	//TWR2RD;
	cmd.cmd = 0x8;	//WRITE;
	cmd.addr = 0;
	cmd.bank = 0;
	cmd.data1 = EXPECT0;
	cmd.data2 = EXPECT1;
	to_cmd_format(cmd, word);
	dprintf("WRITE:\n");
	write_dcu_cache(word, 4);

	cmd.addr = 0x20;
	cmd.data1 = EXPECT2;
	cmd.data2 = EXPECT3;
	to_cmd_format(cmd, word);
	dprintf("WRITE:\n");
	write_dcu_cache(word, 4);

	cmd.rpt = 0x4;	//tBL;	
	cmd.dtp = 0x18;	//tRD2WR;
	cmd.cmd = 0xa;	//READ;
	cmd.addr = 0;
	cmd.bank = 0;
	cmd.data1 = cmd.data2 = 0;
	to_cmd_format(cmd, word);
	dprintf("READ:\n");
	write_dcu_cache(word, 4);

	cmd.addr = 0x20;
	to_cmd_format(cmd, word);
	dprintf("READ:\n");
	write_dcu_cache(word, 4);

#if 0
	cmd.dtp = 0x1;	//tRP; MUST NO
	cmd.cmd = 0x4; 	//RP;
	to_cmd_format(cmd, word);
	dprintf("PRECHARGE:\n");
	write_dcu_cache(word, 4);

	REG_DDRP_DCUAR = 0x500;

	word[0] = 0x11223344;	
	word[1] = 0x55667788;	
	write_dcu_cache(word, 2);

	word[0] = 0x11223344;	
	word[1] = 0x55667788;	
	write_dcu_cache(word, 2);

	word[0] = 0xaabbccdd;
	word[1] = 0xeeeeffff;	
	write_dcu_cache(word, 2);

	word[0] = 0xaabbccdd;
	word[1] = 0xeeeeffff;	
	write_dcu_cache(word, 2);
#endif

	REG_DDRP_DCURR = 0x301;

	while (!(REG_DDRP_DCUSR0 & 0x1))
		;

	dprintf("dcu command stag1 over!\n");
	dprintf("====================================================================\n");
	dprintf("DCUAR: 0x%08x\n", REG_DDRP_DCUAR);
	dprintf("DCURR: 0x%08x\n", REG_DDRP_DCURR);
	dprintf("DCUSR0: 0x%08x\n", REG_DDRP_DCUSR0);
	dprintf("DCUSR1: 0x%08x\n", REG_DDRP_DCUSR1);
	dprintf("====================================================================\n");

	return 0;
}

static int ddr_test_read(int byte)
{
	dprintf("ddr_test_read()\n");
	unsigned int i, data, expect, res = 1;

	REG_DDRP_DCURR = 0x00400541;
	//REG_DDRP_DCURR = 0x00400501;

	while (!(REG_DDRP_DCUSR0 & 0x1))
		;

	dprintf("dcu command stag2 over!\n");
	dprintf("====================================================================\n");
	dprintf("DCUAR: 0x%08x\n", REG_DDRP_DCUAR);
	dprintf("DCURR: 0x%08x\n", REG_DDRP_DCURR);
	dprintf("DCUSR0: 0x%08x\n", REG_DDRP_DCUSR0);
	dprintf("DCUSR1: 0x%08x\n", REG_DDRP_DCUSR1);
	dprintf("====================================================================\n");

	REG_DDRP_DCUAR = 0xe00;

	for (i = 0; i < 16; i++) {
		data = REG_DDRP_DCUDR;
		if (i < 8) {
			if (i % 2)
				expect = EXPECT0;
			else
				expect = EXPECT1;
		} else {
			if (i % 2)
				expect = EXPECT2;
			else
				expect = EXPECT3;
		}
		if ((data & MASK(byte)) != (expect & MASK(byte))) {
			dprintf("\tERROR DCUDR: 0x%08x\n", data);
			dprintf("\tEXPECT DCUDR: 0x%08x\n", expect);
			dprintf("\tMASK: 0x%08x\n", MASK(byte));
			res = 0;
			break;
		} else {
			dprintf("\tOK DCUDR: 0x%08x\n", data & MASK(byte));
		}
	}

	REG_DDRP_PIR = 0x20011;
	while (REG_DDRP_PGSR != 0xf)
		;
	dprintf("ITM Reset over\n");

	return res;
}

static int tunning_dqs_parameter(int rank, int byte)
{
	int dgps, dgsl;
	unsigned int start, end;

	start = end = 0xffff;
	dprintf("\ntunning_dqs_parameter()\n");
	for (dgsl = 0; dgsl < 8; dgsl++) {
		for (dgps = 0; dgps < 4; dgps++) {	
			dprintf("\tdgsl: %d\n", dgsl);
			dprintf("\tdgps: %d\n", dgps);
			REG_DDRP_DXDQSTR(byte) &= ~((0x7 << (rank * 3)) | (0x3 << (rank * 2 + 12)));
			REG_DDRP_DXDQSTR(byte) |= (dgsl << (rank * 3) | (dgps << (rank * 2 + 12)));
			dprintf("ddr_test_read REG_DDRP_DXDQSTR(%d): 0x%08x\n", byte, REG_DDRP_DXDQSTR(byte));
			if (ddr_test_read(byte)) {
				dprintf("ddr_test_read ok!!!\n");
				if (start == 0xffff)
					start = dgps + dgsl * 4;
				end = dgps + dgsl * 4;
				dprintf("start: %d\n", start);
				dprintf("end: %d\n", end);
			} else {
				dprintf("ddr_test_read error!!!\n");
				if (end - start > 0) {
					dprintf("!!!!!!found!\n");
					return (start + end + 1) / 2;
				} else 
					end = start = 0xffff;
			}
		}
	}

	if (end - start > 0) {
		dprintf("!!!!!!found!\n");
		return (start + end + 1) / 2;
	} else {
		dprintf("!!!!!! NO found!\n");
		return 0xffffffff;
	}
}

int dqs_gate_train(int rank_cnt, int byte_cnt)
{
	unsigned int rank, byte, middle = 0, res = 0;

	dprintf("dqs_gate_train()\n");
	REG_DDRP_PGCR &= ~DDRP_PGCR_DFTCMP;
	for (rank = 0; rank < rank_cnt; rank++ ) {
		dprintf("########RANK: %d\n", rank);
		ddr_test_write(rank);
		for (byte = 0; byte < byte_cnt; byte++) {
			dprintf("########BYTE: %d\n", byte);
			REG_DDRP_DXGCR(0) &= ~DDRP_DXGCR_DXEN;
			REG_DDRP_DXGCR(1) &= ~DDRP_DXGCR_DXEN;
			REG_DDRP_DXGCR(2) &= ~DDRP_DXGCR_DXEN;
			REG_DDRP_DXGCR(3) &= ~DDRP_DXGCR_DXEN;
			REG_DDRP_DXGCR(byte) |= DDRP_DXGCR_DXEN;
			middle = tunning_dqs_parameter(rank, byte);
			if (middle != 0xffffffff) {
				REG_DDRP_DXDQSTR(byte) &= ~((0x7 << (rank * 3)) | (0x3 << (rank * 2 + 12)));
				REG_DDRP_DXDQSTR(byte) |= ((middle / 4) << (rank * 3) | ((middle % 4) << (rank * 2 + 12)));
				dprintf("===================================\n");
				dprintf("Middle: %d\n", middle);
				dprintf("Rank: %d\n", rank);
				dprintf("Byte: %d\n", byte);
				dprintf("@@@@@@passing configuration: DXDQSTR(%d) = 0x%08x\n", byte, REG_DDRP_DXDQSTR(byte));
				dprintf("===================================\n");
			} else {
				dprintf("=====================================================================\n");
				dprintf("Rank: %d\n", rank);
				dprintf("No passing configuration is possbile for this rank at byte: %d\n", byte);
				dprintf("=====================================================================\n");
				res = 1;
			}
		}
	}

	dprintf("Dummy read: ");
	ddr_test_read(0);
	REG_DDRP_DXGCR(0) |= DDRP_DXGCR_DXEN;
	REG_DDRP_DXGCR(1) |= DDRP_DXGCR_DXEN;
	REG_DDRP_DXGCR(2) |= DDRP_DXGCR_DXEN;
	REG_DDRP_DXGCR(3) |= DDRP_DXGCR_DXEN;
	REG_DDRP_PGCR |= DDRP_PGCR_DFTCMP;

	return res;
}
