

#include "main.h"
	
//MOVE
uint8_t Move_Cmd (void)
{
	Position Pos_Obj;
	float valf;
	if (Get_Param_Float(&valf))
		return 1;
	Pos_Obj.x = valf;
	if (Get_Param_Float(&valf))
		return 1;
	Pos_Obj.y = valf;
	Pos_Obj.t = 0;
	
	motion_pos(Pos_Obj);
	return 0;
}	

//MOVESEG
uint8_t Move_Seg_Cmd (void)
{
	Position Pos_Obj;
	float valf;
	if (Get_Param_Float(&valf))
		return 1;
	Pos_Obj.x = valf;
	if (Get_Param_Float(&valf))
		return 1;
	Pos_Obj.y = valf;
	Pos_Obj.t = 0;
	
	motion_pos_segment(Pos_Obj);
	return 0;
}	

// PUSH
uint8_t PUSH_Cmd (void)
{
	Position Pos_Obj;
	float valf;
	if (Get_Param_Float(&valf))
		return 1;
	Pos_Obj.x = valf;
	if (Get_Param_Float(&valf))
		return 1;
	Pos_Obj.y = valf;
	Pos_Obj.t = 0;
    
	if (Get_Param_Float(&valf)) // stop distance
		return 1;
	
	motion_push(Pos_Obj, valf);
	return 0;
}

// SPEED
uint8_t SPEED_Cmd (void)
{
	Speed Vitesse_Obj;
	float valf;
	if (Get_Param_Float(&valf))
		return 1;
	Vitesse_Obj.v = valf;
	if (Get_Param_Float(&valf))
		return 1;
	Vitesse_Obj.vt = valf;
	motion_linear_speed(Vitesse_Obj);
	return 0;
}

// ANGLE
uint8_t ANGLE_Cmd (void)
{
	float valf;
	if (Get_Param_Float(&valf))
		return 1;
	motion_angle(valf);
	
	return 0;
}

// FREE
uint8_t FREE_Cmd (void)
{
	motion_free();
	return 0;
}

// DONE
uint8_t Asserv_Done_Cmd (void)
{
    printf("%d", Get_Motion_Done());
    return 0;
}

uint8_t Get_Pos_Cmd (void)
{
    Position temp;
    temp = get_position();
    printf("X:%.4f,", (double)(temp.x));
    printf("Y:%.4f,", (double)(temp.y));
    printf("T:%.4f", (double)(temp.t));
    return 0;
}

uint8_t Get_Odo_Cmd (void)
{
    Position pos;
    Speed speed;
    pos = get_position();
    speed = get_speed();
    printf("X:%.4f,", (double)(pos.x));
    printf("Y:%.4f,", (double)(pos.y));
    printf("T:%.4f,", (double)(pos.t));
    printf("V:%.4f,", (double)(speed.v));
    printf("Vt:%.4f", (double)(speed.vt));
    return 0;
}

// SETX
uint8_t SETX_Cmd (void)
{
	float valf;
	if (Get_Param_Float(&valf))
		return 1;
	set_position_x(valf);
	return 0;
}

// SETY
uint8_t SETY_Cmd (void)
{
	float valf;
	if (Get_Param_Float(&valf))
		return 1;
	set_position_y(valf);
	return 0;
}

// SETT
uint8_t SETT_Cmd (void)
{
	float valf;
	if (Get_Param_Float(&valf))
		return 1;
	set_position_t(valf);
	return 0;
}

// VMAX
uint8_t VMAX_Cmd (void)
{
	float valf;
	if (Get_Param_Float(&valf))
		return 1;
	set_Constraint_vitesse_max(valf);
	return 0;
}

// VTMAX
uint8_t VTMAX_Cmd (void)
{
	float valf;
	if (Get_Param_Float(&valf))
		return 1;
	set_Constraint_vt_max(valf);
	return 0;
}

// AMAX
uint8_t AMAX_Cmd (void)
{
	float valfa, valfb, valfc;
	if (Get_Param_Float(&valfa))	//almax
		return 1;
	if (Get_Param_Float(&valfb))	// atmax
		return 1;
	if (Get_Param_Float(&valfc))	// amax
		return 1;
	set_Constraint_acceleration_max(valfa, valfb, valfc);
	return 0;
}
