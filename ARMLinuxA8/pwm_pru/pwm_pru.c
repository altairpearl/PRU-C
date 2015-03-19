/*Author : Muthu Kumaran 
/* Description : Use the PRUSS to control the PWMSS and run the ePWM submodule to generate a pwm wave */

#include <stdio.h>
#include <stdlib.h>
#include <soc_AM335x.h>
#include <hw_types.h>
#include <hw_pwmss.h>
#include <ehrpwm.h>
#include <epwm.h>
#include <hw_ehrpwm.h>
#include <hw_cm_per.h>
#include <gpio_v2.h>

#define PRU0_ARM_INTERRUPT 	19

#define AM33XX
#define GPIO1 			(*(volatile unsigned long *)(0x4804c000))		// The address of the GPIO1 
#define GPIO_INSTANCE_ADDRESS           (SOC_GPIO_1_REGS)

int C4 __attribute__((cregister("MEM",near),peripheral));	//only compatible with v1.1.0B1 +
								//add following lines to MEMORY{} in lnk.cmd
								//PAGE 2:
								//	MEM : o = 0x00026000 l = 0x00002000 CREGISTER=4

#define PRU0_ARM_INTERRUPT              19
#define SYSCFG (*(&C4+0x01))
//int C4 __attribute__((cregister("MEM",near),peripheral));       //only compatible with v1.1.0B1 +
                                                                //add following lines to MEMORY{} in lnk.cmd
                                                                //PAGE 2:
                                                                //      MEM : o = 0x00026000 l = 0x00002000 CREGISTER=4

static void pwm_Init(void);
static void pwmss_Init(void);
static void blink1(void);
static void blink2(void);
static void blink3(void);
static void blink4(void);
volatile register unsigned int __R31;

int main()
{
	
	/*Intialise OCP Master port for accessing external memories*/
        SYSCFG &=0xFFFFFFEF;       // Clear SYSCFG[STANDBY_INIT] to enable OCP master port

	blink2();
	/*Start Main Code*/
//	pwmss_Init();
	printf("OK1");
	blink1();
//	pwm_Init();
	blink3();
	printf("OK2");
	blink4();
	}
        /*Exiting procedure*/
	__R31 = 35;			// Send notification to Host for program completion
	__halt();			//only compatible with v2.0.0B1 + for lower verions of Compiler use
					//asm(" HALT");
}

/*NOTE : ePWM modules use 16-bit register*/

/* Initialize ePWM module */
void pwm_Init(void)
{
	/* Timer Base(TB) submodule config */
	
	/* TB control register; Refer 15.2.4.1 */
	//Free running mode + TBHRCLK = SYSCLK + TBCLK = SYSCLK +  Up counter mode
	HWREGH(SOC_EPWM_0_REGS + EHRPWM_TBCTL) =  (EHRPWM_TBCTL_FREE_SOFT & (0x2 << EHRPWM_TBCTL_FREE_SOFT_SHIFT))|(EHRPWM_TBCTL_SYNCOSEL & (EHRPWM_TBCTL_SYNCOSEL_DISABLE << EHRPWM_TBCTL_SYNCOSEL_SHIFT)) | (EHRPWM_TBCTL_CLKDIV & (EHRPWM_TBCTL_CLKDIV_DIVBY1 << EHRPWM_TBCTL_CLKDIV_SHIFT)) | (EHRPWM_TBCTL_HSPCLKDIV & (EHRPWM_TBCTL_HSPCLKDIV_DIVBY1 << EHRPWM_TBCTL_HSPCLKDIV_SHIFT)) | (EHRPWM_TBCTL_CTRMODE & (EHRPWM_TBCTL_CTRMODE_UP << EHRPWM_TBCTL_CTRMODE_SHIFT)); 
	blink2();
	
	/* TB HRPWM extension phase register; Refer 15.2.4.3 */
//	HWREG(SOC_EPWM_0_REGS + EHRPWM_TBPHS ) = (EHRPWM_TBPHSHR_TBPHSHR & (0x0u << EHRPWM_TBPHSHR_TBPHSHR_SHIFT));	//Ignore sync event

	/* TB PWM phase register; Refer 15.2.4.4 */
	HWREGH(SOC_EPWM_0_REGS + EHRPWM_TBPHS) = (EHRPWM_TBPHS_TBPHS & (0x0 << EHRPWM_TBPHS_TBPHS_SHIFT));	//Ignore sync event
        blink2();
	/* TB HRPWM period register; Refer 15.2.4.6 */
        HWREGH(SOC_EPWM_0_REGS + EHRPWM_TBPRD) = (EHRPWM_TBPRD_TBPRD & (0xfa0 << EHRPWM_TBPRD_TBPRD_SHIFT));	//Setting the time period of the pwm wave
	blink2();
	/*TB counter register;*/
	HWREGH(SOC_EPWM_0_REGS + EHRPWM_TBCTR) = (EHRPWM_TBCTR_TBCTR & (0x0 << EHRPWM_TBCTR_TBCTR_SHIFT));
	blink2();
	
	/* Counter Compare(CCMP)  submodule configuration */        

	/* CCMP control register; Refer 15.2.4.7 */
        HWREGH(SOC_EPWM_0_REGS + EHRPWM_CMPCTL) = (EHRPWM_CMPCTL_SHDWBMODE & (0x0u << EHRPWM_CMPCTL_SHDWBMODE_SHIFT)) | (EHRPWM_CMPCTL_SHDWAMODE & (0x0u << EHRPWM_CMPCTL_SHDWAMODE_SHIFT)) | (EHRPWM_CMPCTL_LOADBMODE & (0x0u << EHRPWM_CMPCTL_LOADBMODE_SHIFT)) | (EHRPWM_CMPCTL_LOADAMODE & (0x0u << EHRPWM_CMPCTL_LOADAMODE_SHIFT)); 	
	blink2();
	/*CCMP HRPWM A reg; Refer 15.2.4.8 */
//	HWREG(SOC_EPWM_0_REGS + EHRPWM_CMPAHR) = ();

        /*CCMP PWM A reg; Refer 15.2.4.9 */
        HWREGH(SOC_EPWM_0_REGS + EHRPWM_CMPA) = (EHRPWM_CMPA_CMPA & (0x7d0u << EHRPWM_CMPA_CMPA_SHIFT));
	blink2();
        /*CCMP PWM B reg; Refer 15.2.4.10 */
//        HWREG(SOC_EPWM_0_REGS + EHRPWM_CMPB) = (EHRPWM_CMPB_CMPB & (0x1F3u << EHRPWM_CMPB_CMPB_SHIFT));

	/*Action-Qualifier(AQ) submodule config */
	
	/*AQ control register for output A; Refer 15.2.4.11 */
	HWREGH(SOC_EPWM_0_REGS + EHRPWM_AQCTLA) = (EHRPWM_AQCTLA_CAU & (EHRPWM_AQCTLA_CAU_EPWMXALOW << EHRPWM_AQCTLA_CAU_SHIFT)) | (EHRPWM_AQCTLA_PRD & (EHRPWM_AQCTLA_PRD_DONOTHING << EHRPWM_AQCTLA_PRD_SHIFT)) | (EHRPWM_AQCTLA_ZRO & (EHRPWM_AQCTLA_ZRO_EPWMXAHIGH << EHRPWM_AQCTLA_ZRO_SHIFT));
	blink2();
	/*AQ control register for output B; Refer 15.2.4.12 */
        HWREGH(SOC_EPWM_0_REGS + EHRPWM_AQCTLB) = (EHRPWM_AQCTLA_CBU & (EHRPWM_AQCTLA_CBU_DONOTHING << EHRPWM_AQCTLB_CBU_SHIFT)) | (EHRPWM_AQCTLB_PRD & (EHRPWM_AQCTLB_PRD_DONOTHING << EHRPWM_AQCTLB_PRD_SHIFT)) | (EHRPWM_AQCTLB_ZRO & (EHRPWM_AQCTLB_ZRO_DONOTHING << EHRPWM_AQCTLB_ZRO_SHIFT));
	blink2();
	HWREGH(SOC_EPWM_0_REGS + EHRPWM_TZSEL) = 0x00u;
	blink2();
	HWREGH(SOC_EPWM_0_REGS + EHRPWM_TZEINT) = 0x00u;
        blink2();
	HWREGH(SOC_EPWM_0_REGS + EHRPWM_ETSEL) = 0x00u;
	blink2();
        HWREGH(SOC_EPWM_0_REGS + EHRPWM_PCCTL) = 0x00u;
	blink2();

}

/*Initialize PWMSS  */
static void pwmss_Init(void)
{
	/*Enabling clock to PRUSS*/
	HWREG (SOC_PRCM_REGS + CM_PER_EPWMSS0_CLKCTRL) = (CM_PER_EPWMSS0_CLKCTRL_MODULEMODE & (CM_PER_EPWMSS0_CLKCTRL_MODULEMODE_ENABLE << CM_PER_EPWMSS0_CLKCTRL_MODULEMODE_SHIFT));
//	HWREG(SOC_PWMSS0_REGS + PWMSS_CLOCK_CONFIG) = PWMSS_EHRPWM_CLK_EN_ACK ;
	HWREG(SOC_PWMSS0_REGS + PWMSS_CLOCK_CONFIG) = 0x00u ;
	blink2();
	HWREG(SOC_PWMSS0_REGS + 0x4u ) =( 0x12u) ;
	blink2();
	HWREG (SOC_PRCM_REGS + CM_PER_EPWMSS0_CLKCTRL) = (CM_PER_EPWMSS0_CLKCTRL_MODULEMODE & (CM_PER_EPWMSS0_CLKCTRL_MODULEMODE_ENABLE << CM_PER_EPWMSS0_CLKCTRL_MODULEMODE_SHIFT)); 
	blink2();
}

static void blink1(void)
{	
	unsigned int i ;
 	GPIOPinWrite(GPIO_INSTANCE_ADDRESS, 21, GPIO_PIN_LOW);

       	for(i=0;i<0x00a00000;i++);
       	GPIOPinWrite(GPIO_INSTANCE_ADDRESS, 21, GPIO_PIN_HIGH);

       	for(i=0;i<0x00a00000;i++);
       	GPIOPinWrite(GPIO_INSTANCE_ADDRESS, 21, GPIO_PIN_LOW);

       	for(i=0;i<0x00a00000;i++);
       	GPIOPinWrite(GPIO_INSTANCE_ADDRESS, 21, GPIO_PIN_HIGH);

       	for(i=0;i<0x00a00000;i++);
	GPIOPinWrite(GPIO_INSTANCE_ADDRESS, 21, GPIO_PIN_LOW);

}


static void blink2(void)
{       
	unsigned int i;
        GPIOPinWrite(GPIO_INSTANCE_ADDRESS, 22, GPIO_PIN_LOW);

        for(i=0;i<0x00a00000;i++);
        GPIOPinWrite(GPIO_INSTANCE_ADDRESS, 22, GPIO_PIN_HIGH);

        for(i=0;i<0x00a00000;i++);
        GPIOPinWrite(GPIO_INSTANCE_ADDRESS, 22, GPIO_PIN_LOW);

        for(i=0;i<0x00a00000;i++);
        GPIOPinWrite(GPIO_INSTANCE_ADDRESS, 22, GPIO_PIN_HIGH);

        for(i=0;i<0x00a00000;i++);
        GPIOPinWrite(GPIO_INSTANCE_ADDRESS, 22, GPIO_PIN_LOW);

}

void blink3(void)
{       
	unsigned int i;
        GPIOPinWrite(GPIO_INSTANCE_ADDRESS, 23, GPIO_PIN_LOW);

        for(i=0;i<0x00a00000;i++);
        GPIOPinWrite(GPIO_INSTANCE_ADDRESS, 23, GPIO_PIN_HIGH);

        for(i=0;i<0x00a00000;i++);
        GPIOPinWrite(GPIO_INSTANCE_ADDRESS, 23, GPIO_PIN_LOW);

        for(i=0;i<0x00a00000;i++);
        GPIOPinWrite(GPIO_INSTANCE_ADDRESS, 23, GPIO_PIN_HIGH);

        for(i=0;i<0x00a00000;i++);
        GPIOPinWrite(GPIO_INSTANCE_ADDRESS, 23, GPIO_PIN_LOW);

}


void blink4(void)
{       
	unsigned int i;
        GPIOPinWrite(GPIO_INSTANCE_ADDRESS, 24, GPIO_PIN_LOW);

        for(i=0;i<0x00a00000;i++);
        GPIOPinWrite(GPIO_INSTANCE_ADDRESS, 24, GPIO_PIN_HIGH);

        for(i=0;i<0x00a00000;i++);
        GPIOPinWrite(GPIO_INSTANCE_ADDRESS, 24, GPIO_PIN_LOW);

        for(i=0;i<0x00a00000;i++);
        GPIOPinWrite(GPIO_INSTANCE_ADDRESS, 24, GPIO_PIN_HIGH);

        for(i=0;i<0x00a00000;i++);
        GPIOPinWrite(GPIO_INSTANCE_ADDRESS, 24, GPIO_PIN_LOW);

}


