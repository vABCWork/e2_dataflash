
#define DATA_FLASH_READ_BASE_ADRS	( 0x00100000 )
#define DATA_FLASH_WRITE_BASE_ADRS	( 0xFE000000 )

void  data_flash_wr_rd_test(void);
void  set_flash_write_buf(void);
void  set_flash_write_buf_1024byte(void);

void data_flash_erase_all_test(void);

void data_flash_enable(void);

uint32_t  data_flash_erase(uint32_t block_index );

uint32_t  data_flash_erase_all( void );

uint32_t  data_flash_write(uint32_t block_index, uint8_t *wr_buf, uint32_t wr_num);

void data_flash_enter_pe_mode(void);

void data_flash_exit_pe_mode(void);
