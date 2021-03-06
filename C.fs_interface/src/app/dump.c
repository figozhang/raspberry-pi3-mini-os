#include "fs.h"
#include "fat16.h"
#include "fat32.h"
#include "mini_uart.h"
#include "printf.h"
#include "mm.h"
extern unsigned char _end;
extern unsigned char _start_;
extern int cd_rem;
void data_dump(void *ptr, int size)
{
    unsigned long a,b,d,stop;
    unsigned char c;
    for(a=(unsigned long)ptr;a<(unsigned long)ptr+size;a+=16) {
				uart_hex(a); uart_puts(": ");
				for(b=0;b<16;b++) {        
				    c=*((unsigned char*)(a+b));
			
                                    d=(unsigned int)c;d>>=4;d&=0xF;d+=d>9?0x37:0x30;
	
				    uart_send(d);
                   
				    d=(unsigned int)c;d&=0xF;d+=d>9?0x37:0x30;
				    uart_send(d);
				    uart_send(' ');
				    if(b%4==3)
				    uart_send(' ');
				}
				for(b=0;b<16;b++) {
				    c=*((unsigned char*)(a+b));
				    uart_send(c<32||c>=127?'.':c);
				}
				uart_send('\r');
				uart_send('\n');
			
				if(stop==512){
					stop = 0;
					while(!uart_recv()){}
				}
   			}
}
 
int dump(char* file_name){
	unsigned long addr = 0;
	for(int k = 0;file_dir[k].name[0]!='\0';k++){
	   if(!memcmp(file_dir[k].name,file_name,8)){

		/*get cluster for*/
		unsigned int clust = ((unsigned int)file_dir[k].ch)<<16|(unsigned int)file_dir[k].cl;
		if(clust){
                        printf("\n\r");
			struct fs_unit* return_fs = fs_type_support(partition[cd_rem].type);
               		if(return_fs){
				addr = bl_init( &_start_+(return_fs->addr_readfile -(unsigned int)&_start_), clust, &partition[cd_rem]);
			}else{
				printf("Not support %x type in File system",partition[cd_rem].type);
				return 0;
			}
			
                        data_dump((char*)&_start_ + addr,file_dir[k].size);
			//sdTransferBlocks (sect, 1, &_end, 1);

		}
		else{
			printf("\n\rNot file");		
		}
		return 0;
	   }
	   

        }
	printf("\n\rNot file");

}

