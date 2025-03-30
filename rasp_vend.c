#include "rasp_vend.h"

pthread_t p_i2c; 			//опрос датчиков по i2c
pthread_t step_mot_p;		//поток шагового двигателя
pthread_t col_mot_p;		//поток колекторного дкигателя
pthread_t prog_p;			//проток программы 

uint8_t param_photo = 0;

int main (void) {

/*
//потоки создаются при запуске программы печати
 pthread_create(step_mot_p, NULL, run_step_mot, NULL);
 pthread_create(col_mot_p, NULL, run_col_mot, NULL);
 pthread_create(prog_p, NULL, run_prog, NULL);
 */
 for ( ; ; ) {
 //постоянная проверка поступления команды
 	if ( param_photo != 0 ) {
 		pthread_create(prog_p, NULL, run_prog, param_photo);
 		param_photo = 0;
 	}
 }
 
 
return 0;
}



void *request_i2c(void *period) {
  for ( ; ; ) {
	
	
  }
}


void *run_step_mot( void *arg ) {
int steps = *(uint32_t*)&arg[0];
uint8_t speed = 	*arg[4];
uint32_t accel = 	*arg[5]; 
int8_t num_opt = 	*arg[6];
uint8_t status = 	*arg[7]; 
uint16_t timeout = 	*arg[8];
uint8_t *flag; 

if ( DEBUG == 1 ) {
	Msg("Start Step mot");
}

	//WriteMtk(mt);
	if ( steps > 0 ) 
		HAL_GPIO_WritePin( DIR_STEP_MOT, GPIO_PIN_RESET ); 			//если положительное число, то вперед
	else 
		HAL_GPIO_WritePin(DIR_STEP_MOT,GPIO_PIN_SET);				//иначе, назад

	count_100ms = 0; 
	step = fabs( steps );
	//старт ШД

	HAL_GPIO_WritePin( EN_STEP_MOT,GPIO_PIN_SET);				//включить ШД
/*
 * ожидание открытия оптодатчика для обнуления счетчика шагов
 */
	for( ; ; ) {
		//Строб ШД.
////////////////////////////////////////////////////////////////
		HAL_GPIO_WritePin( STEP_STEP_MOT,GPIO_PIN_RESET);
		HAL_GPIO_WritePin( STEP_STEP_MOT,GPIO_PIN_SET);
		usleep( speed );
		HAL_GPIO_WritePin( STEP_STEP_MOT,GPIO_PIN_RESET);
		usleep( speed );
/////////////////////////////////////////////////////////////////
		PortRead(&hi2c1, adr_ur_sens,&input_UR);
		if ( bitRead( input_UR, opto_print_in ) == 0 ) {
		//count_step = 0;
		break;
		}
		else if(count_100ms > 1000 ){
			if (DEBUG == 1) {
				Msg("MT_TMT1_SM");
			}
			//fl_er = 1;
			//break;
			*flag = 1;
			return;
		}
	}

	if (DEBUG == 1) {
		Msg("count_step = 0");
	}

	count_step = 0;
	for(;;){

		//Строб ШД
////////////////////////////////////////////////////////////////
		HAL_GPIO_WritePin( STEP_STEP_MOT,GPIO_PIN_RESET);
		HAL_GPIO_WritePin( STEP_STEP_MOT,GPIO_PIN_SET);
		delay_micros(speed);
		HAL_GPIO_WritePin( STEP_STEP_MOT,GPIO_PIN_RESET);
		delay_micros(speed);
		count_step++;
/////////////////////////////////////////////////////////////////

		PortRead(&hi2c1, adr_ur_sens,&input_UR);					//опрос оптодатчиков

		if(num_opt >= 0 ){											//если оспользуется оптодатчик
			PortRead(&hi2c1, adr_ur_sens,&input_UR);
			if(bitRead(input_UR, num_opt) == status){

				HAL_GPIO_WritePin(EN_STEP_MOT,GPIO_PIN_RESET);		//выключить ШД
				if (DEBUG == 1) {
					Msgint(count_step);
				}
				return;
			}
		}
			if(count_step >= step){
			HAL_GPIO_WritePin(EN_STEP_MOT,GPIO_PIN_RESET);			//выключить ШД
			if (DEBUG == 1) {
				Msg("count_step >= step");
				Msgint(count_step);
			}
			return;
		}
			else if(count_100ms > timeout ){						//если превышен таймаут
			HAL_GPIO_WritePin(EN_STEP_MOT,GPIO_PIN_RESET);			//выключить ШД

			if (DEBUG == 1) {
				Msg("MT_TMT2_ST");
			}
			Pause(10);
			*flag = 1;
			return ;
		}

	}//for

}

void *run_col_mot(void *arg) {




}

void *run_prog(void *arg) {
 pthread_create(p_i2c, NULL, request_i2c, 100);
	
	

 pthread_cancel(p_i2c);
} 

