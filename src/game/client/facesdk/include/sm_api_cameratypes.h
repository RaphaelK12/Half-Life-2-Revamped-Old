/*
	Copyright (C) 2008 Seeing Machines Ltd. All rights reserved.

	This file is part of the FaceTrackingAPI, also referred to as "faceAPI".

	This file may be distributed under the terms of the Seeing Machines 
	FaceTrackingAPI Non-Commercial License Agreement.

	This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
	WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.

	Further information about faceAPI licensing is available at:
	http://www.seeingmachines.com/faceapi/licensing.html
*/
#ifndef SM_API_CAMERATYPES_H
#define SM_API_CAMERATYPES_H

/*! @file 
    Defines types for cameras.
    @ingroup sm_api_group_camera */
#ifdef __cplusplus
extern "C"
{
#endif

#define SM_API_CAMERA_DEFAULT_FIFO_LEN 2147483647 /*!< Defined as INT_MAX */

/*!@struct smCameraHandle
   @brief Passed to any API routines that use cameras.
    
    @see
    - @ref smCameraCreate()
    - @ref smCameraCreateImagePush()
    - @ref smCameraCreateFile()
    - @ref sm_api_handles
    @ingroup sm_api_group_camera */
SM_API_DECLARE_HANDLE(smCameraHandle);

/*! @brief Types of cameras that the API supports */
typedef enum smCameraType
{
    SM_API_CAMERA_TYPE_WDM = 0,        /*!< Windows Driver Model compatible cameras (webcams). */
    SM_API_CAMERA_TYPE_PTGREY = 1,     /*!< PointGrey firewire cameras. Please note only 1 PtGrey camera can be created at a time. This is a temporary limitation. */
    SM_API_CAMERA_TYPE_IMAGE_PUSH = 2, /*!< "Push" camera, allowing user to supply image memory data. */
    SM_API_CAMERA_TYPE_FILE = 3,       /*!< File camera, reading images from a suitable movie files. */
    SM_API_CAMERA_NUM_TYPES            /*!< The number of supported camera types */
} smCameraType;

/*! @brief Represents the format that the camera can produce that is also compatible with the API.
    @see
    - smCameraInfo
    - smCameraCreateInfoList()
    
    @ingroup sm_api_group_camera */
typedef struct smCameraVideoFormat
{
    smSize2i res;           /*!< Resolution of the image that will be produced for this format. */
    smImageCode format;     /*!< Image format (type) that is produced (e.g. RGB) */
    float framerate;        /*!< Framerate of camera for this format. */
    smBool is_upside_down;  /*!< Whether the image should be interpreted as bottom-up or top-down. */
} smCameraVideoFormat;

/*! @brief Information that has been detected about a specific camera.

    A list of smCameraInfo is generated by smCameraCreateInfoList().
    
    A single chosen smCameraInfo is passed to smCameraCreate().

    @see
    - smCameraInfoList
    - smCameraCreateInfoList() 
    - smCameraCreate()
    @ingroup sm_api_group_camera */
typedef struct smCameraInfo
{
    smCameraType type;              /*!< General type of camera. */
    smStringHandle model;            /*!< Name of the camera model, e.g. "Logitech Quickcam Pro 5000". */
    int instance_index;             /*!< The Nth camera in this category of cameras. */
    int num_formats;                /*!< Number of video formats supported by the camera that are compatible with the API */
    int preferred_format_index;     /*!< The preferred format to use for tracking, as suggested by the API. A value from 0 to num_formats-1. */
    smCameraVideoFormat *formats;   /*!< Array of video formats, or 0 if num_formats == 0. */
    void *__vec;                    /*!< Internal use only. */
} smCameraInfo;

/*! @brief List of cameras that is detected on the system
    
    Reference a particular smCameraInfo using smCameraInfoList.info[i], where i is in [0,smCameraInfoList.num_cameras-1]
    @see
    - smCameraCreateInfoList()
    - smCameraCreate() */
typedef struct smCameraInfoList
{
    int num_cameras;        /*!< The number of cameras detected on the system */
    smCameraInfo *info;     /*!< Array of smCameraInfo, one for each detected camera, or 0 if num_cameras == 0 */
    void *__vec;            /*!< Internal use only. */
} smCameraInfoList;

/*! @brief Contains coefficients describing the camera lens properties, including the field-of-view and distortion.

    These coefficients can be determined for a particular camera by using the API tool "CamCal.exe" which
    generates a file that can be loaded using smCameraLoadLensParamsFile.
    
    @see
    - http://www.vision.caltech.edu/bouguetj/calib_doc/htmls/parameters.html */
typedef struct smCameraLensParams
{    
    smCoord2d focal_len;        /*!< Focal length (in pixels) for each axis X,Y. */
    smCoord2d principal_point;  /*!< Principal point of the lens (in pixels). */
    double skew;                /*!< Skew (defines the angle between X and Y axes), dimensionless. */
    double k1;                  /*!< Lens distortion coefficient k1, dimensionless. */
    double k2;                  /*!< Lens distortion coefficient k2, dimensionless. */
    double k3;                  /*!< Lens distortion coefficient k3, dimensionless. */
    double k4;                  /*!< Lens distortion coefficient k4, dimensionless. */
    double k5;                  /*!< Lens distortion coefficient k5, dimensionless. */
    smSize2i calib_res;         /*!< Image resolution at which the lens-parameters were calibrated. */
} smCameraLensParams;

/*! @brief Defines the settings of the camera to use when creating it.

    When creating a camera you need to provide a smCameraInfo to define the particular camera to use, and smCameraSettings
    to define how the camera is setup. */
typedef struct smCameraSettings
{
    /*! Optional lens model for the camera, can be 0.
        - If set, it must point to a valid smCameraLensParams struct that has been initialized using smCameraLoadLensParamsFile() or smCameraLensParamsFromHFOV() */
    smCameraLensParams *lens_params;    
    /*! Optional approximate field-of-view of the camera in degrees, can be 0.
        - If set, it will be used only if no lens_params are provided. The value must be between [1,179] degrees.
        - If 0 and lens_params is also 0 then a default field-of-view will be used. */
    int *approx_fov_deg;
    /*! Optional format index, can be 0.
        - If 0, the preferred smCameraVideoFormat will be used as specified by @a preferred_format_index in smCameraInfo.
        - If set, it must point to an integer which has a value from 0 to smCameraInfo.num_formats-1. */
    int *format_index;
} smCameraSettings;

/*! @brief Defines a frame in a sequence of video-frames. */
typedef struct smCameraVideoFrame
{
    smImageHandle image_handle; /*!< The video-frame image. */
    unsigned int frame_num;     /*!< Specifies the position in the video-sequence. Always increases with time. */
    smTime time;                /*!< UTC time of image exposure, accurate to 1ms. */
} smCameraVideoFrame;

#ifdef __cplusplus
}
#endif
#endif
