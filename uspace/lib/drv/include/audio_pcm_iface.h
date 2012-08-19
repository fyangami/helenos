/*
 * Copyright (c) 2012 Jan Vesely
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * - Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 * - Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in the
 *   documentation and/or other materials provided with the distribution.
 * - The name of the author may not be used to endorse or promote products
 *   derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
/** @addtogroup libdrv
 * @addtogroup usb
 * @{
 */
/** @file
 * @brief Audio PCM buffer interface.
 */

#ifndef LIBDRV_AUDIO_PCM_IFACE_H_
#define LIBDRV_AUDIO_PCM_IFACE_H_

#include <async.h>
#include <bool.h>
#include <loc.h>
#include <pcm/sample_format.h>

#include "ddf/driver.h"

typedef enum {
	AUDIO_CAP_CAPTURE,
	AUDIO_CAP_PLAYBACK,
	AUDIO_CAP_MAX_BUFFER,
	AUDIO_CAP_BUFFER_POS,
	AUDIO_CAP_INTERRUPT,
	AUDIO_CAP_INTERRUPT_MIN_FRAMES,
	AUDIO_CAP_INTERRUPT_MAX_FRAMES,
} audio_cap_t;

enum {
	PCM_EVENT_FRAMES_PLAYED = IPC_FIRST_USER_METHOD,
	PCM_EVENT_FRAMES_CAPTURED,
	PCM_EVENT_PLAYBACK_TERMINATED,
	PCM_EVENT_CAPTURE_TERMINATED
};

typedef async_sess_t audio_pcm_sess_t;

audio_pcm_sess_t *audio_pcm_open(const char *);
audio_pcm_sess_t *audio_pcm_open_service(service_id_t service);
void audio_pcm_close(audio_pcm_sess_t *);

int audio_pcm_get_info_str(audio_pcm_sess_t *, const char **);
int audio_pcm_test_format(audio_pcm_sess_t *, unsigned *, unsigned *,
    pcm_sample_format_t *);
int audio_pcm_query_cap(audio_pcm_sess_t *, audio_cap_t, unsigned *);

int audio_pcm_get_buffer(audio_pcm_sess_t *, void **, size_t *,
    async_client_conn_t, void *);
int audio_pcm_release_buffer(audio_pcm_sess_t *);

int audio_pcm_start_playback(audio_pcm_sess_t *, unsigned,
    unsigned, unsigned, pcm_sample_format_t);
int audio_pcm_stop_playback(audio_pcm_sess_t *);

int audio_pcm_start_capture(audio_pcm_sess_t *, unsigned,
    unsigned, unsigned, pcm_sample_format_t);
int audio_pcm_stop_capture(audio_pcm_sess_t *);

/** Audio pcm communication interface. */
typedef struct {
	int (*get_info_str)(ddf_fun_t *, const char **);
	int (*test_format)(ddf_fun_t *, unsigned *, unsigned *,
	    pcm_sample_format_t *);
	unsigned (*query_cap)(ddf_fun_t *, audio_cap_t);
	int (*get_buffer)(ddf_fun_t *, void **, size_t *);
	int (*release_buffer)(ddf_fun_t *);
	int (*set_event_session)(ddf_fun_t *, async_sess_t *);
	int (*start_playback)(ddf_fun_t *, unsigned,
	    unsigned, unsigned, pcm_sample_format_t);
	int (*stop_playback)(ddf_fun_t *);
	int (*start_capture)(ddf_fun_t *, unsigned,
	    unsigned, unsigned, pcm_sample_format_t);
	int (*stop_capture)(ddf_fun_t *);
} audio_pcm_iface_t;

#endif
/**
 * @}
 */
