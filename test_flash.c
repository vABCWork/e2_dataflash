#include	<machine.h>
#include	 "iodefine.h"
#include	 "misratypes.h"
#include	 "debug_port.h"
#include	 "delay.h"

static void debug_port_ini(void);


void main(void)
{

	data_flash_enable();	  //  E2データフラッシュへのアクセス許可
	
	data_flash_wr_rd_test();  //  E2データフラッシュへの書込みと読出しテスト（ブロック0へ書き込み)
	
	//data_flash_erase_all_test();  // E2データフラッシュ全領域イレーズテスト
	
	debug_port_ini();
	
	while(1) {		// ディレイ関数の確認
		delay_5usec();	// 5usec待ち
		
		DEBUG_0_PODR = 1; // ON
		
		delay_5usec();	// 5usec待ち
		delay_5usec();  // 5usec待ち
		
		DEBUG_0_PODR = 0; // OFF
	}
	
	
}


//   デバックポートの設定 
//   (debug_port.h)

static void debug_port_ini(void)
{	
        DEBUG_0_PMR = 0;    //  P31 汎用入出力ポート
	DEBUG_0_PDR = 1;    //  出力ポートに指定
	
}
