/*
 * Copyright (c) 2017 Petr Manek
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

/** @addtogroup drvusbdiag
 * @{
 */
/**
 * @file
 * Main routines of USB diagnostic device driver.
 */
#include <errno.h>
#include <usb/debug.h>
#include <usb/dev/driver.h>
#include <usb/diag/diag.h>

#include "usbdiag.h"
#include "device.h"

#define NAME "usbdiag"

static void usb_diag_test_impl(ipc_callid_t rid, ipc_call_t *request)
{
	int x = IPC_GET_ARG1(*request);
	int ret = 4200 + x;
	async_answer_0(rid, ret);
}

static int device_add(usb_device_t *dev)
{
	usb_log_info("Adding device '%s'", usb_device_get_name(dev));

	int err;

	usb_diag_dev_t *diag_dev;
	if ((err = usb_diag_dev_create(dev, &diag_dev)))
		return err;

	/* TODO: Register device in some list. */
	/* TODO: Register device DDF function. */

	return EOK;
}

static int device_remove(usb_device_t *dev)
{
	usb_log_info("Removing device '%s'", usb_device_get_name(dev));

	usb_diag_dev_t *diag_dev = usb_diag_dev_get(dev);

	/* TODO: Make sure nothing is going on with the device. */
	/* TODO: Unregister device DDF function. */
	/* TODO: Remove device from list */

	usb_diag_dev_destroy(diag_dev);

	return EOK;
}

static int device_gone(usb_device_t *dev)
{
	usb_log_info("Device '%s' gone.", usb_device_get_name(dev));

	usb_diag_dev_t *diag_dev = usb_diag_dev_get(dev);

	/* TODO: Make sure nothing is going on with the device. */
	/* TODO: Unregister device DDF function. */
	/* TODO: Remove device from list */

	usb_diag_dev_destroy(diag_dev);

	return EOK;
}

static int function_online(ddf_fun_t *fun)
{
	return ddf_fun_online(fun);
}

static int function_offline(ddf_fun_t *fun)
{
	return ddf_fun_offline(fun);
}

static void connection(ipc_callid_t iid, ipc_call_t *icall, void *arg)
{
	bool cont = true;

	async_answer_0(iid, EOK);

	while (cont) {
		ipc_call_t call;
		ipc_callid_t callid = async_get_call(&call);

		if (!IPC_GET_IMETHOD(call))
			break;

		switch (IPC_GET_IMETHOD(call)) {
		case USB_DIAG_IN_TEST:
			usb_diag_test_impl(callid, &call);
			break;
		default:
			async_answer_0(callid, ENOTSUP);
			break;
		}
	}
}

static int server_fibril(void *arg)
{
	// async_set_client_data_constructor(NULL);
	// async_set_client_data_destructor(NULL);
	async_set_fallback_port_handler(connection, NULL);
	// async_event_task_subscribe();
	// service_register();
	async_manager();

	/* Never reached. */
	return EOK;
}

/** USB diagnostic driver ops. */
static const usb_driver_ops_t diag_driver_ops = {
	.device_add = device_add,
	.device_rem = device_remove,
	.device_gone = device_gone,
	.function_online = function_online,
	.function_offline = function_offline
};

/** USB diagnostic driver. */
static const usb_driver_t diag_driver = {
	.name = NAME,
	.ops = &diag_driver_ops,
	.endpoints = NULL
};

int main(int argc, char *argv[])
{
	printf(NAME ": USB diagnostic device driver.\n");

	log_init(NAME);

	/* Start usbdiag service. */
	fid_t srv = fibril_create(server_fibril, NULL);
	if (!srv)
		return ENOMEM;
	fibril_add_ready(srv);

	return usb_driver_main(&diag_driver);
}

/**
 * @}
 */
