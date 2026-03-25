#ifndef INC_SCENE_ROOT_H_
#define INC_SCENE_ROOT_H_

void OnInit(void);
void OnButtonPress();
void OnJoystickInput(uint32_t* _buffer, float _delta);
void OnRender(void);
void OnTick(float _delta);

#endif /* INC_SCENE_ROOT_H_ */
