#include <jni.h>
#include <OpenCLWrapper.h>
#include <cstdlib>



//#define LIB_OPENCL "/system/vendor/lib/libOpenCL.so"
//
//#define TAG OpenCL
////#define LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,"OPENCL",__VA_ARGS__, __FILE__, __FUNCTION__, __LINE__)


#define LOGD(x...) do { \
  char buf[512]; \
  sprintf(buf, x); \
  __android_log_print(ANDROID_LOG_ERROR,"MMCVOPENCL", "%s:%i| %s",  __FILE__, __LINE__,  buf); \
} while (0)



//const int ARRAY_SIZE = 100000;
//
//
//static void *getCLHandle(){
//
//    LOGD("get_handle");
//    void *res = NULL;
//    char* so_name="Unknown Shared library for OpenCL";
//    res = dlopen(LIB_OPENCL,RTLD_LAZY);
//    if(res != NULL){
//        so_name = LIB_OPENCL;
//    }else{
//        LOGD("Could not open library :(\n");
//    }
//    LOGD("Loaded library name:%s\n",so_name);
//    return res;
//}
//
////一、 选择OpenCL平台并创建一个上下文
//cl_context CreateContext()
//{
//    cl_int errNum;
//    cl_uint numPlatforms;
//    cl_platform_id firstPlatformId;
//    cl_context context = NULL;
//
//    //选择可用的平台中的第一个
//    errNum = clGetPlatformIDs(1, &firstPlatformId, &numPlatforms);
//    if (errNum != CL_SUCCESS || numPlatforms <= 0)
//    {
//        std::cerr << "Failed to find any OpenCL platforms." << std::endl;
//        return NULL;
//    }
//
//    //创建一个OpenCL上下文环境
//    cl_context_properties contextProperties[] =
//            {
//                    CL_CONTEXT_PLATFORM,
//                    (cl_context_properties)firstPlatformId,
//                    0
//            };
//    context = clCreateContextFromType(contextProperties, CL_DEVICE_TYPE_GPU,
//                                      NULL, NULL, &errNum);
//
//    return context;
//}
//
//
////二、 创建设备并创建命令队列
//cl_command_queue CreateCommandQueue(cl_context context, cl_device_id *device)
//{
//    cl_int errNum;
//    cl_device_id *devices;
//    cl_command_queue commandQueue = NULL;
//    size_t deviceBufferSize = -1;
//
//    // 获取设备缓冲区大小
//    errNum = clGetContextInfo(context, CL_CONTEXT_DEVICES, 0, NULL, &deviceBufferSize);
//
//    if (deviceBufferSize <= 0)
//    {
//        std::cerr << "No devices available.";
//        return NULL;
//    }
//
//    // 为设备分配缓存空间
//    devices = new cl_device_id[deviceBufferSize / sizeof(cl_device_id)];
//    errNum = clGetContextInfo(context, CL_CONTEXT_DEVICES, deviceBufferSize, devices, NULL);
//
//    //选取可用设备中的第一个
//    commandQueue = clCreateCommandQueue(context, devices[0], 0, NULL);
//
//    *device = devices[0];
//    delete[] devices;
//    return commandQueue;
//}
//
//
//// 三、创建和构建程序对象
//cl_program CreateProgram(cl_context context, cl_device_id device, const char* fileName)
//{
//    cl_int errNum;
//    cl_program program;
//
//    std::ifstream kernelFile(fileName, std::ios::in);
//    if (!kernelFile.is_open())
//    {
//        LOGD("Failed to open file for reading: %s\n" , fileName );
//        return NULL;
//    }
//
//    std::ostringstream oss;
//    oss << kernelFile.rdbuf();
//
//    std::string srcStdStr = oss.str();
//    const char *srcStr = srcStdStr.c_str();
//    program = clCreateProgramWithSource(context, 1,
//                                        (const char**)&srcStr,
//                                        NULL, NULL);
//
//    errNum = clBuildProgram(program, 0, NULL, NULL, NULL, NULL);
//
//    return program;
//}
//
////创建和构建程序对象
//bool CreateMemObjects(cl_context context, cl_mem memObjects[3],
//                      float *a, float *b)
//{
//    memObjects[0] = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
//                                   sizeof(float) * ARRAY_SIZE, a, NULL);
//    memObjects[1] = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
//                                   sizeof(float) * ARRAY_SIZE, b, NULL);
//    memObjects[2] = clCreateBuffer(context, CL_MEM_READ_WRITE,
//                                   sizeof(float) * ARRAY_SIZE, NULL, NULL);
//    return true;
//}
//
//
//// 释放OpenCL资源
//void Cleanup(cl_context context, cl_command_queue commandQueue,
//             cl_program program, cl_kernel kernel, cl_mem memObjects[3])
//{
//    for (int i = 0; i < 3; i++)
//    {
//        if (memObjects[i] != 0)
//            clReleaseMemObject(memObjects[i]);
//    }
//    if (commandQueue != 0)
//        clReleaseCommandQueue(commandQueue);
//
//    if (kernel != 0)
//        clReleaseKernel(kernel);
//
//    if (program != 0)
//        clReleaseProgram(program);
//
//    if (context != 0)
//        clReleaseContext(context);
//}
//
//
//int test()
//{
//    cl_context context = 0;
//    cl_command_queue commandQueue = 0;
//    cl_program program = 0;
//    cl_device_id device = 0;
//    cl_kernel kernel = 0;
//    cl_mem memObjects[3] = { 0, 0, 0 };
//    cl_int errNum;
//    // uint64_t t1,t2,t3;
//    clock_t t1,t2,t3;
//
//
//    const char* filename = "HelloWorld.cl";
//    // 一、选择OpenCL平台并创建一个上下文
//    context = CreateContext();
//
//    // 二、 创建设备并创建命令队列
//    commandQueue = CreateCommandQueue(context, &device);
//
//    //创建和构建程序对象
//    program = CreateProgram(context, device, filename);//"HelloWorld.cl");
//
//    // 四、 创建OpenCL内核并分配内存空间
//    kernel = clCreateKernel(program, "hello_kernel", NULL);
//
//    //创建要处理的数据
//    float result[ARRAY_SIZE];
//    float a[ARRAY_SIZE];
//    float b[ARRAY_SIZE];
//    for (int i = 0; i < ARRAY_SIZE; i++)
//    {
//        a[i] = (float)i;
//        b[i] = (float)(ARRAY_SIZE - i);
//    }
//
//    t1 = clock();  //mach_absolute_time();
//    LOGD("t1 = %.8f\n",(double)t1);
//    for(int j = 0;j <  ARRAY_SIZE;j++){
//        result[j] = a[j]+b[j];
//
//    }
//
//    t2 = clock(); //mach_absolute_time();
//    LOGD("t2 = %.8f\n",(double)t2);
//
//    //创建内存对象
//    if (!CreateMemObjects(context, memObjects, a, b))
//    {
//        Cleanup(context, commandQueue, program, kernel, memObjects);
//        return 1;
//    }
//
//    // 五、 设置内核数据并执行内核
//    errNum = clSetKernelArg(kernel, 0, sizeof(cl_mem), &memObjects[0]);
//    errNum |= clSetKernelArg(kernel, 1, sizeof(cl_mem), &memObjects[1]);
//    errNum |= clSetKernelArg(kernel, 2, sizeof(cl_mem), &memObjects[2]);
//
//    size_t globalWorkSize[1] = { ARRAY_SIZE };
//    size_t localWorkSize[1] = { 1 };
//
//    errNum = clEnqueueNDRangeKernel(commandQueue, kernel, 1, NULL,
//                                    globalWorkSize, localWorkSize,
//                                    0, NULL, NULL);
//
//    // 六、 读取执行结果并释放OpenCL资源
//    errNum = clEnqueueReadBuffer(commandQueue, memObjects[2], CL_TRUE,
//                                 0, ARRAY_SIZE * sizeof(float), result,
//                                 0, NULL, NULL);
//
//    t3 = clock();  //mach_absolute_time();
//
//
//
//
//    LOGD("cpu t = %.8f\n",(float)(t2-t1)/CLOCKS_PER_SEC);
//    LOGD("gpu t = %.8f \n",(double)(t3-t2)/CLOCKS_PER_SEC);
//
//
//    LOGD("Executed program succesfully.");
//    getchar();
//    Cleanup(context, commandQueue, program, kernel, memObjects);
//
//    return 0;
//}


int test_openc_demo()
{
    MNN::OpenCLSymbolsOperator::createOpenCLSymbolsOperatorSingleInstance();
    if (nullptr ==  MNN::OpenCLSymbolsOperator::getOpenclSymbolsPtr()) {
        LOGD("OpenCL init error , callback ...");
        return 0;
    } else
    {
        LOGD("OpenCL init ok");
    }


    int i, j;
    char info[1024];
    cl_int status;

    cl_uint nPlatform;
    cl_platform_id *listPlatform;

    cl_uint nDevice;
    cl_device_id *listDevice;

    clGetPlatformIDs(0, NULL, &nPlatform);

    listPlatform = (cl_platform_id*)malloc(nPlatform * sizeof(cl_platform_id));
    clGetPlatformIDs(nPlatform, listPlatform, NULL);


    for (i = 0; i < nPlatform; i++)
    {
        clGetPlatformInfo(listPlatform[i], CL_PLATFORM_NAME, 1024, info, NULL);
        LOGD("Platfom[%d]:\n\tName\t\t%s", i, info);
        clGetPlatformInfo(listPlatform[i], CL_PLATFORM_VERSION, 1024, info, NULL);
        LOGD("\n\tVersion\t\t%s", info);
        //clGetPlatformInfo(listPlatform[i], CL_PLATFORM_VENDOR, 1024, info, NULL);
        //printf("\n\tVendor\t\t%s", info);
        //clGetPlatformInfo(listPlatform[i], CL_PLATFORM_PROFILE, 1024, info, NULL);
        //printf("\n\tProfile\t\t%s", info);
        clGetPlatformInfo(listPlatform[i], CL_PLATFORM_EXTENSIONS, 1024, info, NULL);
        LOGD("\n\tExtension\t%s", info);

        clGetDeviceIDs(listPlatform[i], CL_DEVICE_TYPE_ALL, 0, NULL, &nDevice);
        listDevice = (cl_device_id*)malloc(nDevice * sizeof(cl_device_id));
        clGetDeviceIDs(listPlatform[i], CL_DEVICE_TYPE_ALL, nDevice, listDevice, NULL);

        for (j = 0; j < nDevice; j++)
        {
            LOGD("\n");
            clGetDeviceInfo(listDevice[j], CL_DEVICE_NAME, 1024, info, NULL);
            LOGD("\n\tDevice[%d]:\n\tName\t\t%s", j, info);
            clGetDeviceInfo(listDevice[j], CL_DEVICE_VERSION, 1024, info, NULL);
            LOGD("\n\tVersion\t\t%s", info);
            clGetDeviceInfo(listDevice[j], CL_DEVICE_TYPE, 1024, info, NULL);
//            switch (info[0])
//            {
//                case CL_DEVICE_TYPE_DEFAULT:strcpy_s(info, "DEFAULT"); break;
//                case CL_DEVICE_TYPE_CPU:strcpy_s(info, "CPU"); break;
//                case CL_DEVICE_TYPE_GPU:strcpy_s(info, "GPU"); break;
//                case CL_DEVICE_TYPE_ACCELERATOR:strcpy_s(info, "ACCELERATOR"); break;
//                case CL_DEVICE_TYPE_CUSTOM:strcpy_s(info, "CUSTOM"); break;
//                case CL_DEVICE_TYPE_ALL:strcpy_s(info, "ALL"); break;
//            }
            LOGD("\n\tType\t\t%s", info);

            cl_device_svm_capabilities svm;
            clGetDeviceInfo(listDevice[j], CL_DEVICE_VERSION, sizeof(cl_device_svm_capabilities), &svm, NULL);
            info[0] = '\0';
//            if (svm & CL_DEVICE_SVM_COARSE_GRAIN_BUFFER)
//                strcat_s(info, "COARSE_GRAIN_BUFFER ");
//            if (svm & CL_DEVICE_SVM_FINE_GRAIN_BUFFER)
//                strcat_s(info, "FINE_GRAIN_BUFFER ");
//            if (svm & CL_DEVICE_SVM_FINE_GRAIN_SYSTEM)
//                strcat_s(info, "FINE_GRAIN_SYSTEM ");
//            if (svm & CL_DEVICE_SVM_ATOMICS)
//                strcat_s(info, "ATOMICS");
            LOGD("\n\tSVM\t\t%s", info);
        }
        printf("\n\n");
        free(listDevice);
    }
    free(listPlatform);
    getchar();
    return 0;



}

extern "C" JNIEXPORT jstring JNICALL
Java_com_ai_momocv_opencldemo_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";

    test_openc_demo();
    //test();
    return env->NewStringUTF(hello.c_str());
}
