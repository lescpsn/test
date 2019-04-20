#include<stdlib.h>
#include<stdio.h>
#include<libusb-1.0/libusb.h>
#include <sys/time.h>
//#define VID	0x1ea7
//#define PID	0x0064

#define VID	0x1234
#define PID	0xabcd

//#define VID	0x058f
//#define PID	0x6387

long long ustime(void) {
    struct timeval tv;
    long long ust;

	gettimeofday(&tv, NULL);
	ust = ((long long)tv.tv_sec)*1000000;
	ust += tv.tv_usec;
	return ust;
}

long long mstime(void) {
    return ustime()/1000;
}

static int device_status(libusb_device_handle *hd)
{

	int interface = 0;
	unsigned char byte;
	libusb_control_transfer(hd, LIBUSB_ENDPOINT_IN | LIBUSB_REQUEST_TYPE_CLASS | LIBUSB_RECIPIENT_INTERFACE,
			LIBUSB_REQUEST_CLEAR_FEATURE,
			0,
			interface,
			&byte, 1, 5000);

	printf("status:0x%x\n", byte);
/**
 * byte
 * normal:0x18
 * other :0x10
 */
	return 0;
}


int fun (libusb_context *ctx,libusb_device *device,libusb_hotplug_event event,void *user_data)
{
	printf("-----HOTPLUG     Device   IN or  OUT!-----\n");

	return 0;
}
void print_endpoint(const struct libusb_endpoint_descriptor *endpoint)
{
	    int i, ret;

	    printf("      Endpoint descriptor:\n");
	    printf("        bEndpointAddress: %02xh\n", endpoint->bEndpointAddress);
	    printf("        bmAttributes:     %02xh\n", endpoint->bmAttributes);
	    printf("        wMaxPacketSize:   %d\n", endpoint->wMaxPacketSize);
	    printf("        bInterval:        %d\n", endpoint->bInterval);
	    printf("        bRefresh:         %d\n", endpoint->bRefresh);
	    printf("        bSynchAddress:    %d\n", endpoint->bSynchAddress);

}

void print_altsetting(const struct libusb_interface_descriptor *interface)
{
	    int i;

	    printf("    Interface descriptor:\n");
	    printf("      bInterfaceNumber:   %d\n", interface->bInterfaceNumber);
	    printf("      bAlternateSetting:  %d\n", interface->bAlternateSetting);
	    printf("      bNumEndpoints:      %d\n", interface->bNumEndpoints);
	    printf("      bInterfaceClass:    %d\n", interface->bInterfaceClass);
	    printf("      bInterfaceSubClass: %d\n", interface->bInterfaceSubClass);
	    printf("      bInterfaceProtocol: %d\n", interface->bInterfaceProtocol);
	    printf("      iInterface:         %d\n", interface->iInterface);

	    for (i = 0; i < interface->bNumEndpoints; i++)
		    print_endpoint(&interface->endpoint[i]);
}


void print_interface(const struct libusb_interface *interface)
{
	    int i;

	    for (i = 0; i < interface->num_altsetting; i++)
		    print_altsetting(&interface->altsetting[i]);
}

void print_configuration(struct libusb_config_descriptor *config)
{
	    int i;

	    printf("  Configuration descriptor:\n");
	    printf("    wTotalLength:         %d\n", config->wTotalLength);
	    printf("    bNumInterfaces:       %d\n", config->bNumInterfaces);
	    printf("    bConfigurationValue:  %d\n", config->bConfigurationValue);
	    printf("    iConfiguration:       %d\n", config->iConfiguration);
	    printf("    bmAttributes:         %02xh\n", config->bmAttributes);
	    printf("    MaxPower:             %d\n", config->MaxPower);

	    for (i = 0; i < config->bNumInterfaces; i++)
		    print_interface(&config->interface[i]);
}

void print_device(libusb_device *dev, struct libusb_device_descriptor *desc)
{
	    int i;

	    printf("Device descriptor:\n");
	    printf("  bDescriptorType:         %d\n", desc->bDescriptorType);
	    printf("  bcdUSB:                  %#06x\n", desc->bcdUSB);
	    printf("  bDeviceClass:            %d\n", desc->bDeviceClass);
	    printf("  bDeviceSubClass:         %d\n", desc->bDeviceSubClass);
	    printf("  bDeviceProtocol:         %d\n", desc->bDeviceProtocol);
	    printf("  bMaxPacketSize0:         %d\n", desc->bMaxPacketSize0);
	    printf("  idVendor:                %#06x\n", desc->idVendor);
	    printf("  idProduct:               %#06x\n", desc->idProduct);
	    printf("  bNumConfigurations:      %d\n", desc->bNumConfigurations);


	    for (i = 0; i < desc->bNumConfigurations; i++)
	    {
		        struct libusb_config_descriptor *config;
		        int ret = libusb_get_config_descriptor(dev, i, &config);
		        if (LIBUSB_SUCCESS != ret)
		        {
			printf("Couldn't retrieve descriptors\n");
			continue;
		        }

		        print_configuration(config);
		        libusb_free_config_descriptor(config);
	    }
}


int print_descriptor(libusb_device *dev)
{
	    struct libusb_device_descriptor desc;
	    int ret, i;

	    ret = libusb_get_device_descriptor(dev, &desc);
	    if (ret < 0) {
	        printf("failed to get device descriptor\n");
	        return -1;
	    }

	    print_device(dev, &desc);

}

int main()
{
	printf("test start!\n");
	libusb_device **devs;
	libusb_device *device;
	libusb_device_handle *dev_handle;
	libusb_context *ctx = NULL;
	int ret;
	ssize_t count;
	ret = libusb_init(&ctx);
	if(ret<0)
	{
		perror("Init Error\n");
		return 1;
	}
	libusb_set_debug(ctx,LIBUSB_LOG_LEVEL_INFO);
	count = libusb_get_device_list(ctx,&devs);
	if(count<0)
	{
		perror("Get Device Error\n");
		return 1;
	}
	printf("%d Devices in list.\n",count);
	//libusb_free_device_list(devs, 1);
	dev_handle = libusb_open_device_with_vid_pid(ctx,VID,PID);
		//libusb_device *dev = devs[dev_num];
		//libusb_open(dev,&dev_handle);
	if(dev_handle == NULL)
	{
		perror("Cannot open device  \n");
		return 1;
	}
	else
	{
		printf("Device Opened   \n");
		//ret = device_status(dev_handle);
		ret = libusb_kernel_driver_active(dev_handle, 0); //ret 1 表示 active
		 if (ret)
		{
			ret = libusb_detach_kernel_driver(dev_handle, 0);// return 0 表示success
			if (ret)
			{
				printf("libusb_detach_kernel_driver  err!\n");
			}
		}
		ret = libusb_claim_interface(dev_handle, 0);            //claim interface 0 (the first) of device (mine had jsut 1)
		if(ret != 0)
		{
			printf("libusb_claim_interface  err!\n");
		}
		device = libusb_get_device(dev_handle);//通过dev_handle获取设备句柄
		print_descriptor(device);//获取描述符
	}

	//goto goto_end;

	libusb_hotplug_callback_handle  handle;//回调函数返回的句柄

	 libusb_hotplug_register_callback(ctx,
			 LIBUSB_HOTPLUG_EVENT_DEVICE_ARRIVED|LIBUSB_HOTPLUG_EVENT_DEVICE_LEFT,
			 LIBUSB_HOTPLUG_ENUMERATE,
							VID, PID,
							LIBUSB_HOTPLUG_MATCH_ANY,
							&fun,
							NULL,
							&handle);


	while(0)//响应热插拔
	{

		libusb_handle_events_completed(ctx, NULL);//调用热插拔事件时必须在循环中调用一次该函数，否则只会识别一次热插拔
		//libusb_hotplug_deregister_callback(ctx,handle);  //注销libusb_hotplug_register_callback注册的回调函数，handle为注册时返回的句柄

	}


	char buf[4096];
	memset(buf,1,4096);
	int i;
	int len;
	unsigned  int times = 10*1024*1024/2048;
	printf("times = %d\n",times);

	long long llStart = mstime();
	while(times-->0)
	{
		ret = libusb_bulk_transfer (dev_handle,
				0x01,
				buf,
				2048,
				&len,
				10
			) ;
		//printf("times = %d\n",times);
		//for(i=0;i<1000;i++);
	}
	long long llEnd=mstime();
	printf("use time: %lld ms\n",llEnd - llStart);


	while(1);
	ret = libusb_bulk_transfer (dev_handle,
			0x01,
			buf,
			512,
			&len,
			0
		) ;

	if(ret == 0)
	{
		printf("interrupt send sucess, length: %d bytes\n", len);
		for(i=0;i<len;i++)
		{
			printf("%d ",buf[i]);
		}
		printf("\n");
	}
	else
	{
		printf("interrupt send faild, err: %s\n", libusb_error_name(ret));
	}



	goto goto_end;

	while(1)
	{
		ret = libusb_interrupt_transfer(dev_handle,0x81, buf, 64,&len, 12);
		if(ret == 0) {
		        printf("interrupt send sucess, length: %d bytes\n", len);
		        for(i=0;i<len;i++)
			{
				printf("%d ",buf[i]);
			}
		        printf("\n");
		    }
		    else{
		        printf("interrupt send faild, err: %s\n", libusb_error_name(ret));
		}


		ret = libusb_bulk_transfer( dev_handle,
				0x01,
				buf,
				64,
				&len,
				5000
			);

	}
	goto goto_end;



	libusb_free_device_list(devs,1);

	//device_status(dev_handle);
	libusb_hotplug_deregister_callback(ctx,handle);

goto_end :
	libusb_close(dev_handle);
	libusb_exit(ctx);


	return 1;
}
