#include <gui/model/Model.hpp>
#include <gui/model/ModelListener.hpp>

#include "main.h"

Model::Model() : modelListener(0) , ButtonState(false)
{

}

void Model::tick()
{
  if(HAL_GPIO_ReadPin(InputBotton_GPIO_Port, InputBotton_Pin) == GPIO_PIN_SET){
	  ButtonState = false ;
  }else {
	  ButtonState= true ;
  }
  modelListener->SetLight(ButtonState) ;
}
