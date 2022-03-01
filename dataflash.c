#include	 "iodefine.h"
#include	 "misratypes.h"
#include	 "delay.h"
#include	 "dataflash.h"


uint8_t 	flash_wr_data_buf[1024];	// データフラッシュ書込み時に使用
uint8_t		flash_rd_data_buf[1024];	// データフラッシュ読出し時に使用

int32_t		offset_ch0;
int32_t		offset_ch1;
int32_t		offset_ch2;
int32_t		offset_ch3;
int32_t		offset_ch4;
int32_t		offset_ch5;



//
//  E2データフラッシュへの書込みと読出しテスト
//
//  E2データフラッシュ領域: 0x0010 0000から0x0010 1FFFまで 8Kbyte
//                          1ブロック(1Kbyte)の8ブロックで構成。
//
// ブロックindex :   読出しアドレス,     　　 :　書込み(プログラム)、イレーズ用アドレス
//     0         : 0x0010 0000 - 0x0010 03FF  :  0xFE00 0000 - 0xFE00 03FF　 
//     1         : 0x0010 0400 - 0x0010 07FF  :  0xFE00 0400 - 0xFE00 07FF　 
//     2         : 0x0010 0800 - 0x0010 0BFF  :  0xFE00 0800 - 0xFE00 0BFF　 
//     3         : 0x0010 0C00 - 0x0010 0FFF  :  0xFE00 0C00 - 0xFE00 0FFF　 
//     4         : 0x0010 1000 - 0x0010 13FF　:  0xFE00 1000 - 0xFE00 13FF
//     5         : 0x0010 1400 - 0x0010 17FF  :  0xFE00 1400 - 0xFE00 17FF
//     6         : 0x0010 1800 - 0x0010 1BFF  :  0xFE00 1800 - 0xFE00 1BFF
//     7         : 0x0010 1C00 - 0x0010 1FFF  :  0xFE00 1C00 - 0xFE00 1FFF
//
//　注意:
//  書込み用アドレスと読出しアドレスが異なります。
//  データを書き込む前にイレーズが必要です。
//  イレーズは1ブロック(1 Kbyte)単位です。
//
void data_flash_wr_rd_test(void)
{
	uint32_t blk_index;
	uint32_t blk_start;
	
	uint32_t wr_num;
	uint32_t err_fg;
	
	
	set_flash_write_buf();		// 書き込みデータのセット
	wr_num = 24;			// 書き込みバイト数
	
	//set_flash_write_buf_1024byte(); // 書き込みデータのセット(1024byte)
	//wr_num = 1024;			// 書き込みバイト数
	
	data_flash_enter_pe_mode();	// E2データフラッシュをリードモードからP/E モードにする	
	blk_index = 0;			// 書き込み対象ブロック
	
	err_fg = data_flash_erase( blk_index );	// イレーズ
	if ( err_fg > 0 ) {			// エラー発生の場合
		data_flash_exit_pe_mode();	//  E2データフラッシュをP/E モードからリードモードにする
		return;
	}
	
	err_fg = data_flash_write(blk_index, &flash_wr_data_buf[0], wr_num);  // 書込み
	if ( err_fg > 0 ) {			// エラー発生の場合
		data_flash_exit_pe_mode();	//  E2データフラッシュをP/E モードからリードモードにする
		return;
	}
	
        data_flash_exit_pe_mode();		//  E2データフラッシュをP/E モードからリードモードにする
 
	blk_start = 0x400 * ( blk_index ) +  DATA_FLASH_READ_BASE_ADRS;   // ブロック開始アドレス（読出し用)
	   
	memcpy(&flash_rd_data_buf[0], blk_start, wr_num); 	      // 読み出し処理 flash_rd_data_buf[0]へ コピー
	
	
}


// フラッシュ書き込み用バッファへデータセット
void  set_flash_write_buf(void)
{
	offset_ch0 = 0;
	offset_ch1 = 1;
	offset_ch2 = 2;
	offset_ch3 = 3;
	offset_ch4 = 4;
	offset_ch5 = 5;

	memcpy( &flash_wr_data_buf[0], &offset_ch0, 4);	 	// offset_ch0 をflash_wr_data_buf[0]へ
	memcpy( &flash_wr_data_buf[4], &offset_ch1, 4);	 	// offset_ch1 をflash_wr_data_buf[4]へ
	memcpy( &flash_wr_data_buf[8], &offset_ch2, 4);	 	// offset_ch2 をflash_wr_data_buf[8]へ
	memcpy( &flash_wr_data_buf[12], &offset_ch3, 4);	// offset_ch3 をflash_wr_data_buf[12]へ
	memcpy( &flash_wr_data_buf[16], &offset_ch4, 4);	// offset_ch4 をflash_wr_data_buf[16]へ
	memcpy( &flash_wr_data_buf[20], &offset_ch5, 4);	// offset_ch5 をflash_wr_data_buf[20]へ
	
}



// フラッシュ書き込み用バッファへデータセット
void  set_flash_write_buf_1024byte(void)
{
	uint32_t i;
	
	for ( i = 0; i < 1024; i++ ) {
		flash_wr_data_buf[i]  = i;
	}
	
}


//
//  全ブロックのイレーズテスト
//

void data_flash_erase_all_test(void)
{
	uint32_t err_fg;
	
	data_flash_enter_pe_mode();	// E2データフラッシュをリードモードからP/E モードにする	
	
	err_fg = data_flash_erase_all();	// 全ブロックのイレーズ
	if ( err_fg > 0 ) {			// エラー発生の場合
		data_flash_exit_pe_mode();	//  E2データフラッシュをP/E モードからリードモードにする
		return;
	}
	
	data_flash_exit_pe_mode();	//  E2データフラッシュをP/E モードからリードモードにする
	
	
}


//
// E2データフラッシュへのアクセス許可
// (E2 データフラッシュアクセス禁止モードからリードモードへ) 
//
void data_flash_enable(void)
{
	FLASH.DFLCTL.BIT.DFLEN = 1;		// E2データフラッシュへのアクセス許可

	delay_5usec();	  // E2 データフラッシュSTOP 解除時間(tDSTOP)待ち　(5[usec]以上)
}


//
// E2データフラッシュ ブロック単位のイレーズ
// 入力: block_index:  ブロックインデックス (0～7) 
//
// 出力: err_fg = 0 :イレーズ成功
//              = 1 :イレーズ失敗
//              = 2 :ブロックインデックス範囲外
//

uint32_t  data_flash_erase(uint32_t block_index )
{
	uint32_t	block_start;	
	uint32_t	block_end;

	if  ( block_index > 7 )  {  		// ブロックインデックスが8以上の場合
		return 2;
	}
	
	block_start = 0x400 * ( block_index ) +  DATA_FLASH_WRITE_BASE_ADRS;   // ブロック開始アドレス (書込み用)
	block_end = block_start + 0x3ff;			           // ブロック終了アドレス 
	
	FLASH.FASR.BIT.EXS = 0;			// フラッシュ領域はデータ領域
	
	FLASH.FSARH = block_start >> 16;	// フラッシュ処理開始アドレスの設定
	FLASH.FSARL = block_start & 0xFFFF;
	
	FLASH.FEARH = block_end >> 16;		// フラッシュ処理終了アドレスの設定
	FLASH.FEARL = block_end  & 0xFFFF;
	
	FLASH.FCR.BYTE = 0x84;					// ブロックのイレーズ開始
	
	while( FLASH.FSTATR1.BIT.FRDY == 0 ) {			// 処理完了待ち　(処理完了でFRDY = 1)
	}
	
	FLASH.FCR.BYTE = 0x00;					// 処理停止
	while( FLASH.FSTATR1.BIT.FRDY == 1 ) {			// FRDY = 0の確認待ち (FRDY=0後、次の処理実行可能)
	}
	
	
	if (( FLASH.FSTATR0.BIT.ERERR == 1 ) || ( FLASH.FSTATR0.BIT.ILGLERR == 1 ))  // イレーズ中にエラー発生またはイリーガルなコマンド発行
	{
		FLASH.FRESETR.BIT.FRESET = 1;			// フラッシュのリセット
    		FLASH.FRESETR.BIT.FRESET = 0;			// リセット解除
		
		return	0x01;		// イレーズ失敗
	}
	
	return 0;			// イレーズ成功
	
}




//
// E2データフラッシュ 全領域のイレーズ 
//
// 出力: err_fg = 0 :イレーズ成功
//              = 1 :イレーズ失敗
//

uint32_t  data_flash_erase_all( void )
{
	uint32_t	block_start;	
	uint32_t	block_end;

	block_start = DATA_FLASH_WRITE_BASE_ADRS;    			// 先頭ブロック開始アドレス (書込み用)
	block_end =   0x400 *7 + DATA_FLASH_WRITE_BASE_ADRS +  0x3ff;   // 最終ブロック終了アドレス 
	
	FLASH.FASR.BIT.EXS = 0;			// フラッシュ領域はデータ領域
	
	FLASH.FSARH = block_start >> 16;	// フラッシュ処理開始アドレスの設定
	FLASH.FSARL = block_start & 0xFFFF;
	
	FLASH.FEARH = block_end >> 16;		// フラッシュ処理終了アドレスの設定
	FLASH.FEARL = block_end  & 0xFFFF;
	
	FLASH.FCR.BYTE = 0x86;					// 全ブロックのイレーズ開始
	
	while( FLASH.FSTATR1.BIT.FRDY == 0 ) {			// 処理完了待ち　(処理完了でFRDY = 1)
	}
	
	FLASH.FCR.BYTE = 0x00;					// 処理停止
	while( FLASH.FSTATR1.BIT.FRDY == 1 ) {			// FRDY = 0の確認待ち (FRDY=0後、次の処理実行可能)
	}
	
	
	if (( FLASH.FSTATR0.BIT.ERERR == 1 ) || ( FLASH.FSTATR0.BIT.ILGLERR == 1 ))  // イレーズ中にエラー発生またはイリーガルなコマンド発行
	{
		FLASH.FRESETR.BIT.FRESET = 1;			// フラッシュのリセット
    		FLASH.FRESETR.BIT.FRESET = 0;			// リセット解除
		
		return	0x01;		// イレーズ失敗
	}
	
	return 0;			// イレーズ成功
	
}








//
// E2データフラッシュ データ書込み
//
// 入力: block_index:  ブロックインデックス (0～7) 
// 出力: err_fg = 0 :書込み成功
//              = 1 :書き込み失敗
//  		= 2 :ブロック範囲外または、書込みバイト数が1Kbyteを超えている
//
//　注意:
//  データを書き込む前にイレーズが必要です。
//  イレーズは1ブロック(1 Kbyte)単位です。
//  データフラッシュへの書き込みは1バイト単位で行います。
//

uint32_t  data_flash_write(uint32_t block_index, uint8_t *wr_buf, uint32_t wr_num) 
{
	uint32_t 	i;
	uint32_t	block_start;	
	uint32_t	block_end;

	
	if ( ( block_index > 7 ) || ( wr_num > 1024 ))  {  // ブロック番号が8以上または書込みバイト数が1Kbyteを超えている場合
		return 2;
	}
	
	block_start = 0x400 * ( block_index ) +  DATA_FLASH_WRITE_BASE_ADRS;   // ブロック開始アドレス 書込み用)
	block_end = block_start + 0x3ff;				   // ブロック終了アドレス 
	
	FLASH.FASR.BIT.EXS = 0;				// フラッシュ領域はデータ領域
	
	FLASH.FSARH = block_start >> 16;		// フラッシュ処理開始アドレスの設定
	FLASH.FSARL = block_start & 0xFFFF;
	
	
	for ( i = 0; i < wr_num; i++ ) {		// 書き込み処理
	
		FLASH.FWB0 = wr_buf[i];		// 書き込みデータ(1byte)セット
	
		FLASH.FCR.BYTE = 0x81;			// 書き込み実行
	
		while( FLASH.FSTATR1.BIT.FRDY == 0 ) {	// 処理完了待ち　(処理完了でFRDY = 1)
		}
	
		FLASH.FCR.BYTE = 0x00;			// 処理停止
		while( FLASH.FSTATR1.BIT.FRDY == 1 ) {	// FRDY = 0の確認待ち (FRDY=0後、次の処理実行可能)
		}
	
	
		if (( FLASH.FSTATR0.BIT.PRGERR == 1 ) || ( FLASH.FSTATR0.BIT.ILGLERR == 1 ))  // プログラム中にエラー発生またはイリーガルなコマンド発行
		{
			FLASH.FRESETR.BIT.FRESET = 1;	// フラッシュのリセット
    			FLASH.FRESETR.BIT.FRESET = 0;	// リセット解除
		
			return	0x01;			// 書き込み失敗
		}
	}
	
	return 0;			// 書き込み成功	
	
}



//
// E2データフラッシュをリードモードからP/E モードにする
//
void data_flash_enter_pe_mode(void)
{
		
	FLASH.FENTRYR.WORD =0xAA80;	// E2データフラッシュP/Eモードに設定
	delay_5usec();	  		// E2 データフラッシュSTOP 解除時間(tDSTOP)待ち　(5[usec]以上)
	
					// 高速動作モードで動作(SYSTEM.OPCCR.BIT.OPCM = 0)
	FLASH.FPR = 0xA5;		// プロテクト解除
	FLASH.FPMCR.BYTE = 0x10;	// リードモードからE2 データフラッシュP/E モードに遷移させる
        FLASH.FPMCR.BYTE = 0xEF;
	FLASH.FPMCR.BYTE = 0x10;
	
	FLASH.FISR.BIT.PCKA = 0x1F;	// FCLK=32 MHz
	
}


//
// E2データフラッシュをP/E モードからリードモードにする
//
void data_flash_exit_pe_mode(void)
{
	FLASH.FPR = 0xA5;		// プロテクト解除
	FLASH.FPMCR.BYTE = 0x08;	// P/E モードからリードモードに遷移させる
        FLASH.FPMCR.BYTE = 0xF7;
	FLASH.FPMCR.BYTE = 0x08;
	
	delay_5usec();	  		// E2 データフラッシュSTOP 解除時間(tDSTOP)待ち　(5[usec]以上)
	
	FLASH.FENTRYR.WORD =0xAA00;	// リードモードに設定
	
	while(0x0000 != FLASH.FENTRYR.WORD) // 設定の確認
	{
	}
	
}