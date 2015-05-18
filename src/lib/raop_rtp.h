/**
 *  Copyright (C) 2011-2012  Juho Vähä-Herttua
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 */

#ifndef RAOP_RTP_H
#define RAOP_RTP_H

/* For raop_callbacks_t */
#include "raop.h"
#include "raop_decoder.h"
#include "raop_output.h"
#include "logger.h"

#define RAOP_PACKET_LEN 32768

typedef struct raop_rtp_s raop_rtp_t;

raop_rtp_t *raop_rtp_init(logger_t *logger, raop_decoder_t *decoder,
                          raop_output_t *output, const char *remote);
void raop_rtp_start(raop_rtp_t *raop_rtp, int use_udp,
                    unsigned short control_rport, unsigned short timing_rport,
                    unsigned short *control_lport, unsigned short *timing_lport,
                    unsigned short *data_lport);
void raop_rtp_stop(raop_rtp_t *raop_rtp);
void raop_rtp_destroy(raop_rtp_t *raop_rtp);

#endif
