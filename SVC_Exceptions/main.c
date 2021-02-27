

int __svc(0x00) svc_services_add(int x, int y);
int __svc(0x01) svc_services_sub(int x, int y);
int __svc(0x02) svc_services_mul(int x, int y);
int __svc(0x03) svc_services_div(int x, int y);
void SVC_Handler_C(unsigned int * svc_args);
int main(void)
{
	int x,y,z;

	x=5;
	y=5;
	z= svc_services_add(x,y);
	
	x = 9;
	y = 2;
	z = svc_services_sub(x,y);
	x = 3;
	y = 4;
	z = svc_services_mul(x , y);
	x=12;
	y=6;
	z = svc_services_div(x , y);
	
}	

void  __asm  SVC_Handler()
{
		TST LR, #4 //Ob 0100     reading Link register second bit to get to return code ie stack is MS/PS as TST performs AND operation
		ITE EQ
		MRSEQ R0,MSP      //If equal zero store MSP in R0
		MRSNE R0,PSP			//If non zero store PSP in R0
		B 			 __cpp(SVC_Handler_C)
	
}	

/*The Stack Frame is directly passed to the below function.
svc_args[0]-R0
svc_args[1]-R1
svc_args[2]-R3
.
.
.
svc_args[6]-PC
*/



void SVC_Handler_C(unsigned int * svc_args)
{
		unsigned int svc_number = ((char *)svc_args[6])[-2];
		switch (svc_number)
		{
			case 0:
				//First 2 Arguments are in R0 and R1
				//Return value is also stored in R0;
				svc_args[0] = svc_args[0] + svc_args[1];
				
			
			break;
		case 1:
			//First 2 Arguments are in R0 and R1
			//Retrun value is also stored in R0;
			svc_args[0]=svc_args[0]-svc_args[1];
		break;
		case 2:
			//First 2 Arguments are in R0 and R1
			//Retrun value is also stored in R0;
			svc_args[0]=svc_args[0]*svc_args[1];
		break;
		case 3:
			//First 2 Arguments are in R0 and R1
			//Retrun value is also stored in R0;
			svc_args[0]=svc_args[0]/svc_args[1];
		break;
		default:
			break;
		

			
			
		}	


}	







