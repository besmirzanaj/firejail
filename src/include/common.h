/*
 * Copyright (C) 2014, 2015 netblue30 (netblue30@yahoo.com)
 *
 * This file is part of firejail project
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

#ifndef COMMON_H
#define COMMON_H
#define _GNU_SOURCE
#include "../include/exechelper-logger.h"
#include <errno.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

#define DEFAULT_USER_PROFILE	"generic"
#define DEFAULT_ROOT_PROFILE	"server"

#define errExit(msg)    do { char msgout[500]; snprintf(msgout, 500, "Error %s:%s(%d)", msg, __FUNCTION__, __LINE__); exechelp_logerrv("firejail", "Error: %s: %s\n", msgout, strerror(errno)); perror(msgout); exit(1);} while (0)

// macro to print ip addresses in a printf statement
#define PRINT_IP(A) \
((int) (((A) >> 24) & 0xFF)),  ((int) (((A) >> 16) & 0xFF)), ((int) (((A) >> 8) & 0xFF)), ((int) ( (A) & 0xFF))

// macro to print a mac addresses in a printf statement
#define PRINT_MAC(A) \
((unsigned) (*(A)) & 0xff), ((unsigned) (*((A) + 1) & 0xff)), ((unsigned) (*((A) + 2) & 0xff)), \
((unsigned) (*((A) + 3)) & 0xff), ((unsigned) (*((A) + 4) & 0xff)), ((unsigned) (*((A) + 5)) & 0xff)

// the number of bits in a network mask
static inline uint8_t mask2bits(uint32_t mask) {
	uint32_t tmp = 0x80000000;
	int i;
	uint8_t rv = 0;

	for (i = 0; i < 32; i++, tmp >>= 1) {
		if (tmp & mask)
			rv++;
		else
			break;
	}
	return rv;
}

// read an IPv4 address and convert it to uint32_t
static inline int atoip(const char *str, uint32_t *ip) {
	unsigned a, b, c, d;

	if (sscanf(str, "%u.%u.%u.%u", &a, &b, &c, &d) != 4 || a > 255 || b > 255 || c > 255 || d > 255)
		return 1;
		
	*ip = a * 0x1000000 + b * 0x10000 + c * 0x100 + d;
	return 0;
}

// verify an ip address is in the network range given by ifip and mask
static inline char *in_netrange(uint32_t ip, uint32_t ifip, uint32_t ifmask) {
	if ((ip & ifmask) != (ifip & ifmask))
		return "Error: the IP address is not in the interface range\n";
	else if ((ip & ifmask) == ip)
		return "Error: the IP address is a network address\n";
	else if ((ip | ~ifmask) == ip)
		return "Error: the IP address is a network address\n";
	return NULL;
}

// read a mac address
static inline int atomac(char *str, unsigned char macAddr[6]) {
	unsigned mac[6];

	if (sscanf(str, "%2x:%2x:%2x:%2x:%2x:%2x", &mac[0], &mac[1], &mac[2], &mac[3], &mac[4], &mac[5]) != 6)
		return 1;

	int i;
	for (i = 0; i < 6; i++) {
		if (mac[i] > 0xff)
			return 1;
			
		macAddr[i] = (unsigned char) mac[i];
	}

	return 0;
}

// check a mac address is configured
static inline int mac_not_zero(const unsigned char mac[6]) {
	int i;
	for (i = 0; i < 6; i++) {
		if (mac[i] != 0)
			return 1;
	}
	
	return 0;
}

int join_namespace(pid_t pid, char *type);
int name2pid(const char *name, pid_t *pid);
char *pid_proc_comm(const pid_t pid);
char *pid_proc_cmdline(const pid_t pid);
int string_in_list_comma(const char* list, const char* string);
int string_in_list(const char* list, const char* string);
char *string_list_flatten(char **list, char *sep);
int string_list_is_empty(char **list);
void string_list_append(char ***list, char *item);
char **string_list_copy(char **other);
void string_list_free(char ***list);
int strcmp_comma(const char* s1, const char* s2);
char *split_comma(char *str);
int firejail_strcmp(const char *a, const char *b);
#endif
