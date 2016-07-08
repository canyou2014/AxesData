/*
 * libsensors.h
 *
 *  Created on: 2016��2��19��
 *      Author: Yuan
 */

#ifndef LIBSENSORS_H_
#define LIBSENSORS_H_

//#define NULL (void*)0
#define MAX_ULTRASONIC_SENSORS	4
typedef float float32;
typedef unsigned int uint32;
typedef int int32;

struct axes_data {
	uint32	seq;	/* Sequence number */
	float32 acc_x;	/* Accelerator data in m/s2 */
	float32 acc_y;
	float32 acc_z;
	float32 gyro_x;	/* Gyroscope data in deg/s */
	float32 gyro_y;
	float32 gyro_z;
	float32 comp_x;	/* Compass data in microTesla */
	float32 comp_y;
	float32 comp_z;
};

struct ultrasonic_data {
	uint32	seq;		/*Sequence number*/
	float32	distance[MAX_ULTRASONIC_SENSORS];	/* In m */
};

enum battery_status {
	NOT_IN_CHARGING = 0,
	IN_CHARGING = 1,
};

struct battery_data {
	uint32	seq;
	float32	capacity;	/* In AH */
	float32	residue_capacity;
	float32 voltage;	/* In V */
	float32 current;	/* In A */
	enum battery_status	status;
};

struct io_data {
	uint32	seq;
	uint32	io_status;	/* Each bit for 1 IO */
};

struct version {
	uint32	major;
	uint32	minor;
};

/* Return -ERR_NUMBER */
enum ret_val {
	RET_OK = 0,
	RET_NO_DEV = 1,
	RET_DEV_BUSY = 2,
	RET_NOT_INIT = 3,
	RET_INV_ARGS = 4,
	RET_ERR = 5,
};

int32 sensors_init(char * dev);
void sensors_exit(void);

int32 get_version(struct version *ver);
int32 get_axes_data(struct axes_data * data);
int32 get_ultrasonic_data(struct ultrasonic_data * data);
int32 get_io_data(struct io_data * data);
int32 get_battery_data(struct battery_data * data);

int32 set_axes_hz(float32 hz);
int32 set_ultrasonic_hz(float32 hz);
int32 set_io_hz(float32 hz);

float32 get_axes_hz(void);
float32 get_ultrasonic_hz(void);
float32 get_io_hz(void);

int32 set_navi_orientation(uint32 ori);
int32 get_navi_orientation(void);

#endif /* LIBSENSORS_H_ */
