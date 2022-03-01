#include	<machine.h>
#include	 "iodefine.h"
#include	 "misratypes.h"
#include	 "debug_port.h"
#include	 "delay.h"

static void debug_port_ini(void);


void main(void)
{

	data_flash_enable();	  //  E2�f�[�^�t���b�V���ւ̃A�N�Z�X����
	
	data_flash_wr_rd_test();  //  E2�f�[�^�t���b�V���ւ̏����݂ƓǏo���e�X�g�i�u���b�N0�֏�������)
	
	//data_flash_erase_all_test();  // E2�f�[�^�t���b�V���S�̈�C���[�Y�e�X�g
	
	debug_port_ini();
	
	while(1) {		// �f�B���C�֐��̊m�F
		delay_5usec();	// 5usec�҂�
		
		DEBUG_0_PODR = 1; // ON
		
		delay_5usec();	// 5usec�҂�
		delay_5usec();  // 5usec�҂�
		
		DEBUG_0_PODR = 0; // OFF
	}
	
	
}


//   �f�o�b�N�|�[�g�̐ݒ� 
//   (debug_port.h)

static void debug_port_ini(void)
{	
        DEBUG_0_PMR = 0;    //  P31 �ėp���o�̓|�[�g
	DEBUG_0_PDR = 1;    //  �o�̓|�[�g�Ɏw��
	
}
