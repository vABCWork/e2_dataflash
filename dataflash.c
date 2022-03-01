#include	 "iodefine.h"
#include	 "misratypes.h"
#include	 "delay.h"
#include	 "dataflash.h"


uint8_t 	flash_wr_data_buf[1024];	// �f�[�^�t���b�V�������ݎ��Ɏg�p
uint8_t		flash_rd_data_buf[1024];	// �f�[�^�t���b�V���Ǐo�����Ɏg�p

int32_t		offset_ch0;
int32_t		offset_ch1;
int32_t		offset_ch2;
int32_t		offset_ch3;
int32_t		offset_ch4;
int32_t		offset_ch5;



//
//  E2�f�[�^�t���b�V���ւ̏����݂ƓǏo���e�X�g
//
//  E2�f�[�^�t���b�V���̈�: 0x0010 0000����0x0010 1FFF�܂� 8Kbyte
//                          1�u���b�N(1Kbyte)��8�u���b�N�ō\���B
//
// �u���b�Nindex :   �Ǐo���A�h���X,     �@�@ :�@������(�v���O����)�A�C���[�Y�p�A�h���X
//     0         : 0x0010 0000 - 0x0010 03FF  :  0xFE00 0000 - 0xFE00 03FF�@ 
//     1         : 0x0010 0400 - 0x0010 07FF  :  0xFE00 0400 - 0xFE00 07FF�@ 
//     2         : 0x0010 0800 - 0x0010 0BFF  :  0xFE00 0800 - 0xFE00 0BFF�@ 
//     3         : 0x0010 0C00 - 0x0010 0FFF  :  0xFE00 0C00 - 0xFE00 0FFF�@ 
//     4         : 0x0010 1000 - 0x0010 13FF�@:  0xFE00 1000 - 0xFE00 13FF
//     5         : 0x0010 1400 - 0x0010 17FF  :  0xFE00 1400 - 0xFE00 17FF
//     6         : 0x0010 1800 - 0x0010 1BFF  :  0xFE00 1800 - 0xFE00 1BFF
//     7         : 0x0010 1C00 - 0x0010 1FFF  :  0xFE00 1C00 - 0xFE00 1FFF
//
//�@����:
//  �����ݗp�A�h���X�ƓǏo���A�h���X���قȂ�܂��B
//  �f�[�^���������ޑO�ɃC���[�Y���K�v�ł��B
//  �C���[�Y��1�u���b�N(1 Kbyte)�P�ʂł��B
//
void data_flash_wr_rd_test(void)
{
	uint32_t blk_index;
	uint32_t blk_start;
	
	uint32_t wr_num;
	uint32_t err_fg;
	
	
	set_flash_write_buf();		// �������݃f�[�^�̃Z�b�g
	wr_num = 24;			// �������݃o�C�g��
	
	//set_flash_write_buf_1024byte(); // �������݃f�[�^�̃Z�b�g(1024byte)
	//wr_num = 1024;			// �������݃o�C�g��
	
	data_flash_enter_pe_mode();	// E2�f�[�^�t���b�V�������[�h���[�h����P/E ���[�h�ɂ���	
	blk_index = 0;			// �������ݑΏۃu���b�N
	
	err_fg = data_flash_erase( blk_index );	// �C���[�Y
	if ( err_fg > 0 ) {			// �G���[�����̏ꍇ
		data_flash_exit_pe_mode();	//  E2�f�[�^�t���b�V����P/E ���[�h���烊�[�h���[�h�ɂ���
		return;
	}
	
	err_fg = data_flash_write(blk_index, &flash_wr_data_buf[0], wr_num);  // ������
	if ( err_fg > 0 ) {			// �G���[�����̏ꍇ
		data_flash_exit_pe_mode();	//  E2�f�[�^�t���b�V����P/E ���[�h���烊�[�h���[�h�ɂ���
		return;
	}
	
        data_flash_exit_pe_mode();		//  E2�f�[�^�t���b�V����P/E ���[�h���烊�[�h���[�h�ɂ���
 
	blk_start = 0x400 * ( blk_index ) +  DATA_FLASH_READ_BASE_ADRS;   // �u���b�N�J�n�A�h���X�i�Ǐo���p)
	   
	memcpy(&flash_rd_data_buf[0], blk_start, wr_num); 	      // �ǂݏo������ flash_rd_data_buf[0]�� �R�s�[
	
	
}


// �t���b�V���������ݗp�o�b�t�@�փf�[�^�Z�b�g
void  set_flash_write_buf(void)
{
	offset_ch0 = 0;
	offset_ch1 = 1;
	offset_ch2 = 2;
	offset_ch3 = 3;
	offset_ch4 = 4;
	offset_ch5 = 5;

	memcpy( &flash_wr_data_buf[0], &offset_ch0, 4);	 	// offset_ch0 ��flash_wr_data_buf[0]��
	memcpy( &flash_wr_data_buf[4], &offset_ch1, 4);	 	// offset_ch1 ��flash_wr_data_buf[4]��
	memcpy( &flash_wr_data_buf[8], &offset_ch2, 4);	 	// offset_ch2 ��flash_wr_data_buf[8]��
	memcpy( &flash_wr_data_buf[12], &offset_ch3, 4);	// offset_ch3 ��flash_wr_data_buf[12]��
	memcpy( &flash_wr_data_buf[16], &offset_ch4, 4);	// offset_ch4 ��flash_wr_data_buf[16]��
	memcpy( &flash_wr_data_buf[20], &offset_ch5, 4);	// offset_ch5 ��flash_wr_data_buf[20]��
	
}



// �t���b�V���������ݗp�o�b�t�@�փf�[�^�Z�b�g
void  set_flash_write_buf_1024byte(void)
{
	uint32_t i;
	
	for ( i = 0; i < 1024; i++ ) {
		flash_wr_data_buf[i]  = i;
	}
	
}


//
//  �S�u���b�N�̃C���[�Y�e�X�g
//

void data_flash_erase_all_test(void)
{
	uint32_t err_fg;
	
	data_flash_enter_pe_mode();	// E2�f�[�^�t���b�V�������[�h���[�h����P/E ���[�h�ɂ���	
	
	err_fg = data_flash_erase_all();	// �S�u���b�N�̃C���[�Y
	if ( err_fg > 0 ) {			// �G���[�����̏ꍇ
		data_flash_exit_pe_mode();	//  E2�f�[�^�t���b�V����P/E ���[�h���烊�[�h���[�h�ɂ���
		return;
	}
	
	data_flash_exit_pe_mode();	//  E2�f�[�^�t���b�V����P/E ���[�h���烊�[�h���[�h�ɂ���
	
	
}


//
// E2�f�[�^�t���b�V���ւ̃A�N�Z�X����
// (E2 �f�[�^�t���b�V���A�N�Z�X�֎~���[�h���烊�[�h���[�h��) 
//
void data_flash_enable(void)
{
	FLASH.DFLCTL.BIT.DFLEN = 1;		// E2�f�[�^�t���b�V���ւ̃A�N�Z�X����

	delay_5usec();	  // E2 �f�[�^�t���b�V��STOP ��������(tDSTOP)�҂��@(5[usec]�ȏ�)
}


//
// E2�f�[�^�t���b�V�� �u���b�N�P�ʂ̃C���[�Y
// ����: block_index:  �u���b�N�C���f�b�N�X (0�`7) 
//
// �o��: err_fg = 0 :�C���[�Y����
//              = 1 :�C���[�Y���s
//              = 2 :�u���b�N�C���f�b�N�X�͈͊O
//

uint32_t  data_flash_erase(uint32_t block_index )
{
	uint32_t	block_start;	
	uint32_t	block_end;

	if  ( block_index > 7 )  {  		// �u���b�N�C���f�b�N�X��8�ȏ�̏ꍇ
		return 2;
	}
	
	block_start = 0x400 * ( block_index ) +  DATA_FLASH_WRITE_BASE_ADRS;   // �u���b�N�J�n�A�h���X (�����ݗp)
	block_end = block_start + 0x3ff;			           // �u���b�N�I���A�h���X 
	
	FLASH.FASR.BIT.EXS = 0;			// �t���b�V���̈�̓f�[�^�̈�
	
	FLASH.FSARH = block_start >> 16;	// �t���b�V�������J�n�A�h���X�̐ݒ�
	FLASH.FSARL = block_start & 0xFFFF;
	
	FLASH.FEARH = block_end >> 16;		// �t���b�V�������I���A�h���X�̐ݒ�
	FLASH.FEARL = block_end  & 0xFFFF;
	
	FLASH.FCR.BYTE = 0x84;					// �u���b�N�̃C���[�Y�J�n
	
	while( FLASH.FSTATR1.BIT.FRDY == 0 ) {			// ���������҂��@(����������FRDY = 1)
	}
	
	FLASH.FCR.BYTE = 0x00;					// ������~
	while( FLASH.FSTATR1.BIT.FRDY == 1 ) {			// FRDY = 0�̊m�F�҂� (FRDY=0��A���̏������s�\)
	}
	
	
	if (( FLASH.FSTATR0.BIT.ERERR == 1 ) || ( FLASH.FSTATR0.BIT.ILGLERR == 1 ))  // �C���[�Y���ɃG���[�����܂��̓C���[�K���ȃR�}���h���s
	{
		FLASH.FRESETR.BIT.FRESET = 1;			// �t���b�V���̃��Z�b�g
    		FLASH.FRESETR.BIT.FRESET = 0;			// ���Z�b�g����
		
		return	0x01;		// �C���[�Y���s
	}
	
	return 0;			// �C���[�Y����
	
}




//
// E2�f�[�^�t���b�V�� �S�̈�̃C���[�Y 
//
// �o��: err_fg = 0 :�C���[�Y����
//              = 1 :�C���[�Y���s
//

uint32_t  data_flash_erase_all( void )
{
	uint32_t	block_start;	
	uint32_t	block_end;

	block_start = DATA_FLASH_WRITE_BASE_ADRS;    			// �擪�u���b�N�J�n�A�h���X (�����ݗp)
	block_end =   0x400 *7 + DATA_FLASH_WRITE_BASE_ADRS +  0x3ff;   // �ŏI�u���b�N�I���A�h���X 
	
	FLASH.FASR.BIT.EXS = 0;			// �t���b�V���̈�̓f�[�^�̈�
	
	FLASH.FSARH = block_start >> 16;	// �t���b�V�������J�n�A�h���X�̐ݒ�
	FLASH.FSARL = block_start & 0xFFFF;
	
	FLASH.FEARH = block_end >> 16;		// �t���b�V�������I���A�h���X�̐ݒ�
	FLASH.FEARL = block_end  & 0xFFFF;
	
	FLASH.FCR.BYTE = 0x86;					// �S�u���b�N�̃C���[�Y�J�n
	
	while( FLASH.FSTATR1.BIT.FRDY == 0 ) {			// ���������҂��@(����������FRDY = 1)
	}
	
	FLASH.FCR.BYTE = 0x00;					// ������~
	while( FLASH.FSTATR1.BIT.FRDY == 1 ) {			// FRDY = 0�̊m�F�҂� (FRDY=0��A���̏������s�\)
	}
	
	
	if (( FLASH.FSTATR0.BIT.ERERR == 1 ) || ( FLASH.FSTATR0.BIT.ILGLERR == 1 ))  // �C���[�Y���ɃG���[�����܂��̓C���[�K���ȃR�}���h���s
	{
		FLASH.FRESETR.BIT.FRESET = 1;			// �t���b�V���̃��Z�b�g
    		FLASH.FRESETR.BIT.FRESET = 0;			// ���Z�b�g����
		
		return	0x01;		// �C���[�Y���s
	}
	
	return 0;			// �C���[�Y����
	
}








//
// E2�f�[�^�t���b�V�� �f�[�^������
//
// ����: block_index:  �u���b�N�C���f�b�N�X (0�`7) 
// �o��: err_fg = 0 :�����ݐ���
//              = 1 :�������ݎ��s
//  		= 2 :�u���b�N�͈͊O�܂��́A�����݃o�C�g����1Kbyte�𒴂��Ă���
//
//�@����:
//  �f�[�^���������ޑO�ɃC���[�Y���K�v�ł��B
//  �C���[�Y��1�u���b�N(1 Kbyte)�P�ʂł��B
//  �f�[�^�t���b�V���ւ̏������݂�1�o�C�g�P�ʂōs���܂��B
//

uint32_t  data_flash_write(uint32_t block_index, uint8_t *wr_buf, uint32_t wr_num) 
{
	uint32_t 	i;
	uint32_t	block_start;	
	uint32_t	block_end;

	
	if ( ( block_index > 7 ) || ( wr_num > 1024 ))  {  // �u���b�N�ԍ���8�ȏ�܂��͏����݃o�C�g����1Kbyte�𒴂��Ă���ꍇ
		return 2;
	}
	
	block_start = 0x400 * ( block_index ) +  DATA_FLASH_WRITE_BASE_ADRS;   // �u���b�N�J�n�A�h���X �����ݗp)
	block_end = block_start + 0x3ff;				   // �u���b�N�I���A�h���X 
	
	FLASH.FASR.BIT.EXS = 0;				// �t���b�V���̈�̓f�[�^�̈�
	
	FLASH.FSARH = block_start >> 16;		// �t���b�V�������J�n�A�h���X�̐ݒ�
	FLASH.FSARL = block_start & 0xFFFF;
	
	
	for ( i = 0; i < wr_num; i++ ) {		// �������ݏ���
	
		FLASH.FWB0 = wr_buf[i];		// �������݃f�[�^(1byte)�Z�b�g
	
		FLASH.FCR.BYTE = 0x81;			// �������ݎ��s
	
		while( FLASH.FSTATR1.BIT.FRDY == 0 ) {	// ���������҂��@(����������FRDY = 1)
		}
	
		FLASH.FCR.BYTE = 0x00;			// ������~
		while( FLASH.FSTATR1.BIT.FRDY == 1 ) {	// FRDY = 0�̊m�F�҂� (FRDY=0��A���̏������s�\)
		}
	
	
		if (( FLASH.FSTATR0.BIT.PRGERR == 1 ) || ( FLASH.FSTATR0.BIT.ILGLERR == 1 ))  // �v���O�������ɃG���[�����܂��̓C���[�K���ȃR�}���h���s
		{
			FLASH.FRESETR.BIT.FRESET = 1;	// �t���b�V���̃��Z�b�g
    			FLASH.FRESETR.BIT.FRESET = 0;	// ���Z�b�g����
		
			return	0x01;			// �������ݎ��s
		}
	}
	
	return 0;			// �������ݐ���	
	
}



//
// E2�f�[�^�t���b�V�������[�h���[�h����P/E ���[�h�ɂ���
//
void data_flash_enter_pe_mode(void)
{
		
	FLASH.FENTRYR.WORD =0xAA80;	// E2�f�[�^�t���b�V��P/E���[�h�ɐݒ�
	delay_5usec();	  		// E2 �f�[�^�t���b�V��STOP ��������(tDSTOP)�҂��@(5[usec]�ȏ�)
	
					// �������샂�[�h�œ���(SYSTEM.OPCCR.BIT.OPCM = 0)
	FLASH.FPR = 0xA5;		// �v���e�N�g����
	FLASH.FPMCR.BYTE = 0x10;	// ���[�h���[�h����E2 �f�[�^�t���b�V��P/E ���[�h�ɑJ�ڂ�����
        FLASH.FPMCR.BYTE = 0xEF;
	FLASH.FPMCR.BYTE = 0x10;
	
	FLASH.FISR.BIT.PCKA = 0x1F;	// FCLK=32 MHz
	
}


//
// E2�f�[�^�t���b�V����P/E ���[�h���烊�[�h���[�h�ɂ���
//
void data_flash_exit_pe_mode(void)
{
	FLASH.FPR = 0xA5;		// �v���e�N�g����
	FLASH.FPMCR.BYTE = 0x08;	// P/E ���[�h���烊�[�h���[�h�ɑJ�ڂ�����
        FLASH.FPMCR.BYTE = 0xF7;
	FLASH.FPMCR.BYTE = 0x08;
	
	delay_5usec();	  		// E2 �f�[�^�t���b�V��STOP ��������(tDSTOP)�҂��@(5[usec]�ȏ�)
	
	FLASH.FENTRYR.WORD =0xAA00;	// ���[�h���[�h�ɐݒ�
	
	while(0x0000 != FLASH.FENTRYR.WORD) // �ݒ�̊m�F
	{
	}
	
}