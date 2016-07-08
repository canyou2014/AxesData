#include "imutransformation.h"
#include <math.h>
#include "mat_types.h"
//#include <iostream>
//#include <typeinfo>
imutransformation::imutransformation(){}
imutransformation::imutransformation(const double altitude_, const double latitude_, const double dt_){
    PI = 3.1415;
    altitude = altitude_;
    latitude = latitude_;
    dt = dt_;
    gravity();
}
void imutransformation::set_init_state(mat3 Cbn, vec3 pos)
{
    for(size_t i = 0; i < 9; i ++)
    {
        Rb2t[i] = Cbn[i];

    }
    rotation2quat(quaternions, Rb2t);
     for(size_t i = 0; i < 3; i ++)
    {
        position[i] = pos[i];

    }

}
void imutransformation::getresult(mat3 Cbn, vec3 pos){
    for(size_t i = 0; i < 9; i ++)
        {
            Cbn[i] = Rb2t[i];

        }
        rotation2quat(quaternions, Rb2t);
         for(size_t i = 0; i < 3; i ++)
        {
            pos[i] = position[i];

        }

}

inline precision imutransformation::sqrt_hf(precision arg){


	if(sizeof(precision)==4)
	{
	float tmp1, tmp2, tmp3;
	__asm__ __volatile__ ( "frsqrta.s %0, %1" : "=r" (tmp3) : "r" (arg));
	tmp1 = tmp3*tmp3;
	tmp2 = 3.0f - tmp1*arg;
	tmp3 = 0.5f * (tmp2 * tmp3);
	tmp1 = tmp3*tmp3;
	tmp2 = 3.0f - tmp1*arg;
	tmp3 = 0.5f * (tmp2 * tmp3);
	tmp1 = tmp3*tmp3;
	tmp2 = 3.0f - tmp1*arg;
	tmp3 = 0.5f * (tmp2 * tmp3);
	return tmp3*arg;
	}
	else
	{
	return sqrt(arg);
	}

}
inline precision imutransformation::vecnorm2(precision *arg_vec, int len)
{
precision norm2=0;
int ctr;

	for (ctr=0; ctr<len; ctr++){
	norm2=norm2+arg_vec[ctr]*arg_vec[ctr];
	}

return norm2;
}
inline void imutransformation::euler2rotation(mat3 rotmat, const vec3 euler){


precision sin_phi = sin(euler[0]);
precision cos_phi = cos(euler[0]);
precision sin_theta =sin(euler[1]);
precision cos_theta =cos(euler[1]);
precision sin_psi = sin(euler[2]);
precision cos_psi = cos(euler[2]);



rotmat[0]=cos_psi*cos_theta;
rotmat[3]=sin_psi*cos_theta;
rotmat[6]=-sin_theta;
rotmat[1]=(-sin_psi*cos_phi) + cos_psi*(sin_theta*sin_phi);
rotmat[4]=(cos_psi*cos_phi) + sin_psi*(sin_theta*sin_phi);
rotmat[7]=cos_theta*sin_phi;
rotmat[2]=(sin_psi*sin_phi) + cos_psi*(sin_theta*cos_phi);
rotmat[5]=(-cos_psi*sin_phi) + sin_psi*(sin_theta*cos_phi);
rotmat[8]=cos_theta*cos_phi;
}
inline void imutransformation::rotation2quat(quat_vec q,const mat3 rotmat){


	precision T = 1 + rotmat[0] + rotmat[4]+rotmat[8];


	if(T > (0.00000001)){
		precision S = 0.5 / sqrt_hf(T);
		q[3] = 0.25 / S;
		q[0] =(rotmat[7]-rotmat[5]) * S;
		q[1] =(rotmat[2]-rotmat[6]) * S;
		q[2] =(rotmat[3]-rotmat[1]) * S;
		}
	else if( (rotmat[0] > rotmat[4] ) && (rotmat[0] > rotmat[8]) )
	{
		precision S = sqrt_hf(1 + rotmat[0]-rotmat[4]-rotmat[8]) * 2;
		q[3] =(rotmat[6]-rotmat[5])/S;
		q[0] = 0.25 * S;
		q[1] = (rotmat[1]+rotmat[3])/S;
		q[2] = (rotmat[2]+rotmat[6])/S;
		}
	else if (rotmat[4]>rotmat[8])
	{
		precision S = sqrt_hf(1+rotmat[4]-rotmat[0]-rotmat[8])*2;
		q[3] = (rotmat[2]-rotmat[6])/S;
		q[0] =	(rotmat[1]+rotmat[3])/S;
		q[1] = 0.25 * S;
		q[2] = (rotmat[5]+rotmat[7])/S;
		}
	else{
		precision S=sqrt_hf(1+rotmat[8]-rotmat[0]-rotmat[4])*2;
		q[3] = (rotmat[3]-rotmat[1])/S;
		q[0] = (rotmat[2]+rotmat[6])/S;
		q[1] = (rotmat[1]+rotmat[3])/S;
		q[2] = 0.25 * S;
		}



	T=sqrt_hf(vecnorm2(q,4));

	q[0]=q[0]/T;
	q[1]=q[1]/T;
	q[2]=q[2]/T;
	q[3]=q[3]/T;
}
inline void imutransformation::quat2rotation(mat3 rotmat,const quat_vec q){

precision p[6];

p[0]=q[0]*q[0];
p[1]=q[1]*q[1];
p[2]=q[2]*q[2];
p[3]=q[3]*q[3];

p[4]=p[1]+p[2];

if (p[0]+p[3]+p[4] != 0)
{
	p[5]=2/(p[0]+p[3]+p[4]);
}
else
{
	p[5]=0;
}

rotmat[0]=1-p[5]*p[4];
rotmat[4]=1-p[5]*(p[0]+p[2]);
rotmat[8]=1-p[5]*(p[0]+p[1]);

p[0]=p[5]*q[0];
p[1]=p[5]*q[1];
p[4]=p[5]*q[2]*q[3];
p[5]=p[0]*q[1];

rotmat[1]=p[5]-p[4];
rotmat[3]=p[5]+p[4];

p[4]=p[1]*q[3];
p[5]=p[0]*q[2];

rotmat[2]=p[5]+p[4];
rotmat[6]=p[5]-p[4];

p[4]=p[0]*q[3];
p[5]=p[1]*q[2];

rotmat[5]=p[5]-p[4];
rotmat[7]=p[5]+p[4];


}
inline void  imutransformation::rotation2euler(vec3 euler, const mat3 rotmat){



	euler[0] = atan2(rotmat[7],rotmat[8]);
	euler[1] = asin(-rotmat[6]);
	euler[2] = -atan2(rotmat[3],rotmat[0]);
}


inline void imutransformation::gravity(void){

precision lambda = PI/180.0*latitude;

	g=9.780327*(1+0.0053024*(sin(lambda)*sin(lambda))
             -0.0000058*(sin(2*lambda)*sin(2*lambda)))
	-(0.0000030877-0.000000004*(sin(lambda)*sin(lambda)))*altitude+0.000000000000072*(altitude*altitude);

}

void imutransformation::strapdown_mechanisation_equations(const vec3 accelerations_out, const vec3 angular_rates_out){
    vec3 an_hat;
    vec3 angular_rates_dt;
	quat_vec quat_tmp;
	precision cos_v;
	precision sin_v;

    if((angular_rates_out[0]!=0)|(angular_rates_out[1]!=0)|(angular_rates_out[2]!=0))
	{

	angular_rates_dt[0]=angular_rates_out[0]*dt;
	angular_rates_dt[1]=angular_rates_out[1]*dt;
	angular_rates_dt[2]=angular_rates_out[2]*dt;

	precision v=( sqrt_hf( vecnorm2(angular_rates_dt, 3) ) );
	cos_v=cos(v/2);
	sin_v=(sin(v/2)/v);

	quat_tmp[0]=cos_v*quaternions[0]+sin_v*(angular_rates_dt[2]*quaternions[1]-angular_rates_dt[1]*quaternions[2]+angular_rates_dt[0]*quaternions[3]);	// w_tb(2)*quaternions(1)-w_tb(1)*quaternions(2)+w_tb(0)*quaternions(3)
	quat_tmp[1]=cos_v*quaternions[1]+sin_v*(-angular_rates_dt[2]*quaternions[0]+angular_rates_dt[0]*quaternions[2]+angular_rates_dt[1]*quaternions[3]);  //-w_tb(2)*quaternions(0)+w_tb(0)*quaternions(2)+w_tb(1)*quaternions(3)
	quat_tmp[2]=cos_v*quaternions[2]+sin_v*(angular_rates_dt[1]*quaternions[0]-angular_rates_dt[0]*quaternions[1]+angular_rates_dt[2]*quaternions[3]);   //w_tb(1)*quaternions(0)-w_tb(0)*quaternions(1)+w_tb(2)*quaternions(3)
	quat_tmp[3]=cos_v*quaternions[3]+sin_v*(-angular_rates_dt[0]*quaternions[0]-angular_rates_dt[1]*quaternions[1]-angular_rates_dt[2]*quaternions[2]);  //-w_tb(0)*quaternions(0)-w_tb(1)*quaternions(1)-w_tb(2)*quaternions(2)

	v=sqrt_hf(vecnorm2(quat_tmp, 4));
	quaternions[0]=quat_tmp[0]/v;
	quaternions[1]=quat_tmp[1]/v;
	quaternions[2]=quat_tmp[2]/v;
	quaternions[3]=quat_tmp[3]/v;
	}

	quat2rotation(Rb2t,quaternions);

	an_hat[0]=Rb2t[0]*accelerations_out[0]+Rb2t[1]*accelerations_out[1]+Rb2t[2]*accelerations_out[2];
	an_hat[1]=Rb2t[3]*accelerations_out[0]+Rb2t[4]*accelerations_out[1]+Rb2t[5]*accelerations_out[2];
	an_hat[2]=Rb2t[6]*accelerations_out[0]+Rb2t[7]*accelerations_out[1]+Rb2t[8]*accelerations_out[2] - g;

	velocity[0]=velocity[0]+an_hat[0]*dt;
	velocity[1]=velocity[1]+an_hat[1]*dt;
	velocity[2]=velocity[2]+an_hat[2]*dt;

	position[0]=position[0]+velocity[0]*dt;
	position[1]=position[1]+velocity[1]*dt;
	position[2]=position[2]+velocity[2]*dt;


}

imutransformation::~imutransformation()
{

}

