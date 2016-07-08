#ifndef IMUTRANSFORMATION_H
#define IMUTRANSFORMATION_H
#include "mat_types.h"
#include <math.h>
class imutransformation
{
    public:
        imutransformation();
        imutransformation(const double altitude_, const double latitude_, const double dt_);
        virtual ~imutransformation();
        void set_init_state(mat3 Cbn, vec3 pos);
        void getresult(mat3 Cbn, vec3 pos);
        void strapdown_mechanisation_equations(const vec3 accelerations_out, const vec3 angular_rates_out);
    protected:
    private:
        precision altitude;
        precision latitude;
        precision dt;
        precision PI;
        vec3 accelerations_out;
        vec3 angular_rates_out;
        inline precision sqrt_hf(precision arg);
        inline precision vecnorm2(precision *arg_vec, int len);
        inline void euler2rotation(mat3 rotmat, const vec3 euler);
        inline void rotation2quat(quat_vec q, const mat3 rotmat);
        inline void quat2rotation(mat3 rotmat, const quat_vec q);
        inline void rotation2euler(vec3 euler, const mat3 rotmat);
        inline void gravity(void);
        precision g;
        vec3 attitude;
       // precision* position = new vec3;
        vec3 position;
        mat3 Rb2t;
        vec3 velocity;
        quat_vec quaternions;
        //precision* Rb2t = new mat3;

};

#endif // IMUTRANSFORMATION_H
