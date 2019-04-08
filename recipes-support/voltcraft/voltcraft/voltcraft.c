/*
 * (C) Copyright 2010
 * Stefano Babic, stefano.babic@babic.homelinux.org
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#include <termios.h>		
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <syslog.h>
#include <getopt.h>
#include <sys/signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/select.h>
#include <sys/ioctl.h>

#ifdef DEBUG
#define debug printf
#else
#define debug(...) {}
#endif
#define BUF_SIZE	10500

#define SFLG_REM	(1 << 1)
#define SFLG_ONOFF	(1 << 2)
#define SFLG_OVERTEMP	(1 << 3)
#define SFLG_PCONST	(1 << 4)
#define SFLG_ICONST	(1 << 5)
#define SFLG_UCONST	(1 << 6)
#define SFLG_FINE	(1 << 7)

#define FLAG_ACTIVE	1
#define FLAG_VOLTAGE	2

/* Code delivered pressing a key */
#define KEY_U	0x00
#define KEY_u	0x01
#define KEY_N	0x02
#define KEY_I	0x04
#define KEYENT	0x05
#define KEY_F	0x06
#define KEY_P	0x08
#define KEY_CE	0x09
#define KEY_IO	0x0C

#define get_val(b) (((b.H & 0xFF) << 8) | (b.L & 0xFF))

struct bcd_value {
	char H;
	char L;
};

struct recv_frame {
	char flag_1;
	char flag_2;
	struct bcd_value U;
	struct bcd_value I;
	struct bcd_value P;
	struct bcd_value LMV;
	struct bcd_value LMI;
	struct bcd_value LMP;
	char SFLG;
} __attribute__((packed));

enum {
	WAIT_FOR_FRAME = 0,
	FIRST_FLAG,
	RECV_DATA
};

void usage (char *program_name)
{
	printf ( (
		"Voltcraft DPS-4005 remote control\n\n"
		"Usage %s [OPTION]\n"
		" -p, --port\t\t\t: serial interface to be used for serial connection\n"
		" -a, --active[1|0]\t\t: enable / disable output\n"
		" -v, --voltage <millivolts>\t: set output voltage \n"
		" -m, --vmax <millivolts>\t: set upper limit for voltage\n"
		" -h, --help\t\t\t: this help\n" 
		"\n\n"),
	program_name);
}

void setparms (int fd)
{
	int bit = '8';
	int spd = 0;
	struct termios tty;

	tcgetattr (fd, &tty);
	bit = '8';
	spd = B1200;

	cfsetospeed (&tty, (speed_t) spd);
	cfsetispeed (&tty, (speed_t) spd);

	tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;
	tty.c_iflag = IGNBRK & IGNPAR;
	tty.c_lflag = 0;
	tty.c_oflag = 0;
	tty.c_cflag |= CREAD;
	tty.c_cc[VMIN] = 1;
	tty.c_cc[VTIME] = 5;
	tty.c_iflag &= ~(IXON | IXOFF | IXANY);
	tty.c_cflag &= ~(PARENB | PARODD);
	
	tcsetattr (fd, TCSANOW, &tty);
	
}

int receive(int fd, char *rxbuf, int rxlen)
{
	int recvlen, n;
	int rxchars = 0;
	int c;
	fd_set fds;
	struct timeval timeout;
	int status = WAIT_FOR_FRAME;
	char buf[10];

	memset(rxbuf, 0, rxlen);
	do {
		FD_ZERO(&fds);
		FD_SET(fd, &fds);
		timeout.tv_sec = 4;
		timeout.tv_usec = 0;

		n = select(fd + 1, &fds, NULL, NULL, &timeout);
		if (n < 0) {
			printf("Select returns error: %s\n", strerror(errno));
			return -1;
		}
		if (n == 0) {
			rxchars = 0;
			break;
		}
		if (!FD_ISSET(fd, &fds))
			return -1;

		recvlen = read(fd, buf, 1);

		if (recvlen != 1) {
			printf("Error reading UART: %d %s\n", errno, strerror(errno));
			return -1;
		}

		c = buf[0] & 0xFF;
		switch (status) {
		case WAIT_FOR_FRAME:
			if (c == 0xEB)
				status = FIRST_FLAG;
			debug("status = %d received %x", status, c);
			break;
		case FIRST_FLAG:
			if (c == 0x90) {
				rxchars = 2;
				status = RECV_DATA;
			}
			debug("status = %d\n", status);
			break;
		case RECV_DATA:
			rxbuf[rxchars] = c;
			rxchars++;
			break;
		}
	} while (rxchars < 15);

	return (rxchars);
}

void dump_status (struct recv_frame *f)
{
	printf("\nVoltcraft DPS-40 settings:\n");
	printf("--------------------------\n");
	printf("%02u.%02u V\t\t V-Const %02u.%02u\n",
		get_val(f->U) / 100,
		get_val(f->U) % 100,
		get_val(f->LMV) / 100,
		get_val(f->LMV) % 100);
	printf("%01u.%03u A\t\t I-Const %01u.%03u\n",
		get_val(f->I) / 1000,
		get_val(f->I) % 1000,
	 	get_val(f->LMI) / 1000,
	 	get_val(f->LMI) % 1000);
	printf("%02x%01x.%01x W\t\t P-Const %02x%01x.%01x\n",
		f->P.H & 0xFF,
		(f->P.L & 0xF0) >> 4,
		f->P.L & 0x0F,
		f->LMP.H,
		f->LMP.L >> 4,
		f->LMP.L & 0x0F);
	printf ("%s %s %s %s %s %s %s\n",
		f->SFLG & SFLG_REM ? "REM" : " ",
		f->SFLG & SFLG_ONOFF ? "ON" : "OFF",
		f->SFLG & SFLG_OVERTEMP ? "OVERTEMP" : " ",
		f->SFLG & SFLG_PCONST ? "Power" : " ",
		!f->SFLG & SFLG_ICONST ? "I" : " ",
		!f->SFLG & SFLG_UCONST ? "U" : " ",
		f->SFLG & SFLG_FINE ? "Fine" : " ");
	printf("\n\n");
}

int send_data(int fd, int up, int steps)
{
	char cmd[4];

	cmd[0] = 0xEB;
	cmd[1] = 0x90;

	if (up)
		cmd[2] = 0x55;
	else
		cmd[2] = 0xCC;

	cmd[3] = steps & 0xFF;
	return write(fd, cmd, 4); 
}

#define increase_data(fd, steps) send_data(fd, 1, steps)
#define decrease_data(fd, steps) send_data(fd, 0, steps)

int send_cmd_key(int fd, char key)
{
	char cmd[4];

	cmd[0] = 0xEB;
	cmd[1] = 0x90;
	cmd[2] = 0xAA;
	cmd[3] = key;
	return write(fd, cmd, 4); 
}

unsigned long wait_for_stabilized(int fd)
{
	char buf[40];
	unsigned long old_value, value, steps = 0, integral = 0;
	int stabilized = 0;
	int res;
	struct recv_frame *pstatus= (struct recv_frame *)buf;
#define THRESHOLD 20
#define ACCEPTABLE_DIFF	2

	res = receive(fd, buf, 20);
	value = get_val(pstatus->U);

	while (!stabilized) {
		old_value = value;

		/* Let the Voltcraft to stabilize */
		tcflush(fd, TCIFLUSH);
		res = receive(fd, buf, 20);
		value = get_val(pstatus->U);

		if (abs(value - old_value) < ACCEPTABLE_DIFF)
			integral++;
		else
			integral = 0;
		steps++;
		if (integral > THRESHOLD || steps > 100)
			stabilized = 1;
	}

	return value;

}

int main (int argc, char **argv)
{
	int on, steps;
	int steps_per_frame, sign;
	unsigned long flags = 0;
	unsigned long voltage, measured_volt;
	struct recv_frame *pstatus;

	static struct option long_options[] = {
		{"port", required_argument, NULL, 'p'},
		{"active", required_argument, NULL, 'a'},
		{"voltage", required_argument, NULL, 'v'},
		{"vmax", required_argument, NULL, 'm'},
		{"help", no_argument, NULL, 'h'},
		{NULL, 0, NULL, 0}
	};
	int fd, res;
	int c;
	char portdev[256];
	struct termios oldtio;
	char *buf;

	memset(portdev, 0, sizeof(portdev));
	
	buf = (char *)malloc(BUF_SIZE);
	if (!buf) {
		printf("Error: memory not available\n");
		exit(1);
	}

	while ((c = getopt_long (argc, argv, "p:a:hv:",
				 long_options, NULL)) != EOF)
	{

		switch (c) {
		case 'p':
			strncpy (portdev, optarg, sizeof(portdev));
			break;
		case 'a':
			flags |= FLAG_ACTIVE;
			on=strtoul(optarg,NULL,10);
			break;
		case 'v':
			flags |= FLAG_VOLTAGE;
			voltage=strtoul(optarg,NULL,10);
			break;
		case 'h':
		default:
			usage(argv[0]);
			exit (0);
			break;
		}
	}

	/* Check parameters */

	if (flags & FLAG_VOLTAGE) {
		if (voltage > 4000) {
			printf("Error: max voltage for the device is 40 V\n");
			exit(1);
		}
	}

	if (!strlen(portdev)) {
		printf("There is no default for the serial port.\n\n");
		usage(argv[0]);
		exit (1);
	}

	/* open serial port to which instrument is connected */
	fd = open (portdev, O_RDWR | O_NDELAY);
	if (fd < 0) {
		perror (portdev);
		exit (-1);
	}

	tcgetattr (fd, &oldtio);  /* save old serial port settings */
	setparms (fd);  /* initialize serial port settings to read stv */

	res = receive(fd, buf, 20);
	if (res != 15) {
		printf ("No communication with device, exiting...\n");
		exit(1);
	}
	pstatus = (struct recv_frame *)buf;

	/* Check for REM enabled */
	if (!(pstatus->SFLG & SFLG_REM)) {
		printf("Remote access not enabled. Press REM button on device\n");
		exit(1);
	}

	if (flags & FLAG_VOLTAGE) {
		if (!(pstatus->SFLG & SFLG_FINE)) {
			send_cmd_key(fd, KEY_F);
		}
		send_cmd_key(fd, KEY_u);
		steps = voltage - get_val(pstatus->U);
		sign = steps > 0;
		steps = abs(steps);
		while (steps > 0) {
			if (steps > 255 )
				steps_per_frame = 255;
			else
				steps_per_frame = steps;
			send_data(fd, sign, steps_per_frame);
			steps -= steps_per_frame;
			if (steps)
				sleep(2);
		}

		/* Let the Voltcraft to stabilize */
		tcflush(fd, TCIFLUSH);
	
		/* We need some fine tuning now */
		
		measured_volt = wait_for_stabilized(fd);
		steps_per_frame = voltage - measured_volt;
		send_data(fd, (steps_per_frame > 0), abs(steps_per_frame));
	}

	if (flags & FLAG_ACTIVE) {
		if (((pstatus->SFLG & SFLG_ONOFF) && !on) ||
			(!(pstatus->SFLG & SFLG_ONOFF) && on)) {
			send_cmd_key(fd, KEY_IO);
		}
	}

	tcflush(fd, TCIFLUSH);
	res = receive(fd, buf, 20);
	dump_status(pstatus);

	close(fd);

	return 0;
}
